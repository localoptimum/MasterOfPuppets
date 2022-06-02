#include <string>

#include "base_individual.h"
#include "globals.h"
#include "Instrument.h"
#include "Sector.h"


using namespace std;

base_individual::base_individual()
{
  trace=false;
  sect = Sector();
}

base_individual::~base_individual()
{}

void base_individual::set_id(int idnum)
{
}

void base_individual::set_trace(bool setval)
{}

void base_individual::seed_manual_code(const double set_position[MAX_PARAMS])
{
  int i;

  for(i=0; i<num_params; i++)
    {
      position[i] = set_position[i];
    }
}

void base_individual::init_best(string filename)
{}


double base_individual::get_fitness(void)
{
	if(is_evaluated == false)
	{
		calc_fitness();
	}
	return(fitness);
}

double base_individual::rate_c1_distances(void)
{
  double badness = 0.0;
  double impact = 1.0;
  int i;
  

  /*
    K. Andersen, O. Kirstein - 8/12/2015
    Impact on bandwidth changes for each instrument
    MIRACLES       low
    MAGIC          medium
    BIFROST        medium
    HEIMDAL        high
    BEER           high
  */

    
  
  for(i=0; i<6; i++)
    {

      //The impact is defined by scientists
      switch(i)
	{
	case 0 :
	  //MIRACLES
	  impact = 1.0;
	  break;
	case 1:
	  // BEER
	  impact = 3.0;
	  break;
	case 2:
	  //CAMEA
	  impact = 2.0;
	  break;
	case 3:
	  //CSPEC
	  impact = 1.0;
	  break;
	case 4:
	  //MAGIC
	  impact = 2.0;
	  break;
	case 5:
	  //HEIMDAL
	  impact = 3.0;
	  break;
	default:
	  //Cannot happen
	  impact = 1.0;
	  break;
	}
      
      if(position[C1+i] > 0.1)
	{
	  badness = badness + impact*position[C1+i]/(6.0*3.0);
	  //Normalised to number of instruments (6) multiplied by max value
	}
    }
  return(badness);
}


void base_individual::calc_fitness(void)
{
	//Weighting factors / 5

  const double priority00= 100000.0;  //Used to fix instruments in place
  const double priority0 = 10000.0;   //Used to count mechanical interferences
  const double priority1 = 1000.0;    //Shane's definition below
  const double priority2 = 100.0;     //Shane's definition below
  const double priority3 = 10.0;      //Shane's definition below
  const double priority4 = 1.0;       //Lower level priority

  int sFI = 0; //score for fixed instruments
  int cWI = 0; //count wall interferences
  int cGI = 0; //count of guide interferences
  int cHI = 0; //count of hard interferences
  int p16 = 0; //count of possible 16th instruments
  int f15 = 0; //count of first 15 instruments deployed
  int cEI = 0; //count of intereferences with empty beam lines

	// const double wallImportance = 5.0;
	// const double guideImportance = 4.0;
	// const double chopperImportance = 4.0;
	// const double backgroundImportance = 3.0;
	// const double lateralShieldingImportance = 3.0;
	// const double noisyNeighbourImportance = 2.0;
	// const double instrumentPriorityImportance = 2.0;


	// const double t1= 3.0;
	// const double t2= 2.0;
	// const double t3= 1.0;
	// const double t4= 0.1;


	//Build sector
	switch(sectorToStudy)
	{
	case NORTH_SECTOR:
		buildNorthSector();
		break;
	case EAST_SECTOR:
		buildEastSector();
		break;
	case SOUTH_SECTOR:
		buildSouthSector();
		break;
	case WEST_SECTOR:
		buildWestSector();
		break;

	default :
	  std::cerr << "calc_fitness called without defining sectorToStudy." << std::endl;
	  exit(1);
	  break;
	}


/*


	Figure of merit function


	S. Kennedy, 11/10/2015

	1.0 to provide the best outcome in terms of performance and
	operability, for instruments 1-15 + the two options for
	instrument 16;
	
	2.0 to provide the best outcome for “guessed”
	instruments 17-22; and 
	
	3.0 to minimize blocking of unallocated
	beam-ports, such that 32 instruments (or more) could be
	installed.

*/

	sFI = sect.scoreFixedInstruments(false);
	if(sFI == 1)
	  fixedInstOK = false;
	else
	  fixedInstOK = true;

	cWI = sect.countWallInterferencesDelegated(false);
	if(cWI == 0)
	  wallsOK = true;
	else
	  wallsOK = false;

	cHI = sect.countHardInterferencesDelegated(false);
	if(cHI == 0)
	  interfOK = true;
	else
	  interfOK = false;

	cGI = sect.countGuideInterferencesDelegated(false);
	if(cGI == 0)
	  guideOK = true;
	else
	  guideOK = false;

	cEI = sect.countEmptyInterferencesDelegated(false);

	f15 = sect.countInstruments(1);
	p16 = sect.countInstruments(2);

	if(f15 == 15 && p16 > 1)
	  suiteOK = true;
	else
	  suiteOK = false;
	  
			    

	//PRIORITY 00 = fix instruments where told to.  This is a boundary condition above all else
	fitness = priority00 * sFI;
	


	
	//PRIORITY 0  = avoid mechanical interferences, only present workable solutions

	//Instrument can only be placed within a hall that fits - walls.  This forces only viable solutions
	fitness = fitness + priority0*cWI;

	//Base function is to minimise chopper interferences in all planned instruments - again forces viable solutions
	fitness = fitness + priority0*cHI;

	//Choppers interfering with guides is weighted the same as choppers - again forces viable solutions
	fitness = fitness + priority0*cGI;



	
	//Number of instruments in the sector needs to be maximised (contributes negatively to fitness)
	//Prioritise score based on tranche!
	fitness = fitness - priority1 * f15; 	//tranche 1 = first 15
	fitness = fitness - priority1 * p16; 	//tranche 2 = possible 16th instrument

	//Instrument Performance - Backgrounds
	fitness = fitness + priority1 * sect.scoreSpallationBackground();

	//More backgrounds - Try to space instruments away from noisy neighbours
	fitness = fitness + priority1 * sect.scoreNoisyNeighbours();

	
	//Lateral shielding on short sector
	if(sectorToStudy != WEST_SECTOR)
	  {
	    fitness = fitness + priority1 * sect.scoreLateralShielding(false);
	  }
	  
	//Score first chopper position - the closer to the source the better.
	if(sectorToStudy == WEST_SECTOR)
	  {
	    fitness = fitness + priority1 * rate_c1_distances();
	  }
	


	
	//Choppers interfering with empty guides is priority 3
	fitness = fitness + priority3*cEI;
	
	//Now score future instruments
	fitness = fitness - priority3 * sect.countInstruments(3);  //tranche 3 = possible future instrument
	fitness = fitness - priority3 * sect.countInstruments(4);	//tranche 4 = Empty beam port




	//Penalise duplicate instruments in same sector(!)
	//fitness = fitness + 10.0*sect.countDuplicates(false);
}

double base_individual::get_best_fitness_pos(double answer_pos[MAX_PARAMS])
{
  return(0.0);
}


void base_individual::randomise(void)
{}


void base_individual::load_limits(const char *fileName)
{}


void base_individual::display_diagnostics(void)
{
  int i;
  
  bool verbose=true;
  
  const int chopperInterf= sect.countHardInterferencesDelegated(verbose);
  const int guideInterf= sect.countGuideInterferencesDelegated(verbose);
  const int emptyInterf= sect.countEmptyInterferencesDelegated(verbose);
  const int duplicates = sect.countDuplicates(verbose);
  const int instruments = sect.countInstruments(1);
  const int instruments16 = sect.countInstruments(2);
  const int possibleInstruments = sect.maxPossibleInstruments();
  const int walls = sect.countWallInterferencesDelegated(verbose);
  
  const double spallation = sect.scoreSpallationBackground();
  const double noisyNeighbours = sect.scoreNoisyNeighbours();
  const double lateral = sect.scoreLateralShielding(verbose);
  
  const double figureOfMerit = get_fitness();
  
  
  cout << "  Solution beam port allocations  = " << endl;
  
  sect.listPortAllocations();
  
  cout << "Diagnostic Data: " << endl << endl;
  
  cout << "  Theta                 = " << this->position[0] << endl;
  cout << "  Alpha                 = " << this->position[1] << endl;
  
  cout << "  Duplicates            = " << duplicates << endl;
  cout << "  Wall interferences    = " << walls << endl;
  cout << "  Chopper Interferences = " << chopperInterf << endl;
  cout << "  Guide Interferences   = " << guideInterf << endl;
  cout << "  Empty Interferences   = " << emptyInterf << endl;
  cout << "  # Inst in sector = " << instruments << endl;
  cout << "  # 16th inst in sector = " << instruments16 << endl;
  cout << "  # Poss instruments    = " << possibleInstruments << endl;
  cout << "  Noisy neighbour score = " << noisyNeighbours << endl;
  cout << "  Lateral shielding scr = " << lateral << endl;
  cout << "  Direct Background Scr = " << spallation << endl;
  
  cout << endl;
  
  switch(sectorToStudy)
    {
    case WEST_SECTOR :
      cout << "Chopper offsets:" << endl;
      cout << "   MIRACLES: " << this->position[C1] << endl;
      cout << "   BEER    : " << this->position[C2] << endl;
      cout << "   CAMEA   : " << this->position[C3] << endl;
      cout << "   CSPEC   : " << this->position[C4] << endl;
      cout << "   MAGIC   : " << this->position[C5] << endl;
      cout << "   HEIMDAL : " << this->position[C6] << endl;
      cout << "   TREX    : " << this->position[C7] << endl;
      break;
      
    default:
      cout << "No chopper offsets" << endl;
      break;
    }
  
  
  if( (guideInterf != 0) || chopperInterf != 0 || (duplicates != 0) || (walls != 0))
    {
      cout << endl;
      cout << "Diagnostics ERROR: This layout is NOT compatible with existing requirements!" << endl;
    }
  else
    {
      cout << endl;
      cout << "Diagnostics OK  :)" << endl;
    }
  
  
  cout << endl << "FIGURE OF MERIT = " << figureOfMerit << endl;  
}

void base_individual::calculate_position(void)
{}
	

void base_individual::trace_position(void)
{}


void base_individual::update(double gBest, double gBestPos[MAX_PARAMS])
{}


base_individual& base_individual::operator=(const base_individual& source)
{
  int i;

  this->is_evaluated = source.is_evaluated;

  this->fixedInstOK = source.fixedInstOK;
  this->wallsOK = source.wallsOK;
  this->interfOK = source.interfOK;
  this->guideOK = source.guideOK;
  this->suiteOK = source.suiteOK;

  this -> fitness = source.fitness;
  this -> id = source.id;
  this -> trace = source.trace;


  for(i=0; i < num_params; i++)
    {
      this -> position[i] = source.position[i];
      this -> hilimit[i] = source.hilimit[i];
      this -> lolimit[i] = source.lolimit[i];
    }
  
  this->sect = source.sect;  
  
}


void base_individual::buildNorthSector(void)
{

	sect = Sector();

	
	//ESSNSE, SKADI, FREIA, ODIN, FUND, ESTIA, DIFF, VOR, LOKI};
	Instrument essense(ESSENSE);
	Instrument freia(FREIA);
	Instrument loki(LOKI);
	Instrument sleipnir(SLEIPNIR);
	//Instrument surface(SURFACE_SCATTERING);
	Instrument wanse(WANSE);

	Instrument empty(EMPTY);
	Instrument empty2(EMPTY2);
	Instrument empty3(EMPTY3);
	Instrument empty4(EMPTY4);
	Instrument empty5(EMPTY5);
	Instrument empty6(EMPTY6);
	Instrument empty7(EMPTY7);
	// Instrument empty8(EMPTY8);
	// Instrument empty9(EMPTY9);



	const double lateralPadding = 0.2;		//10 cm clearance on each side padding - Iain Sutton
	const double motorSpace = 0.5;			// Motor needs to have extra space - Ian Sutton
	const double bearingSpace = 0.1;		// Clearnace on the wheel where the bearing is - Iain Sutton

	//Make sure our parameter values are up to date
	calculate_position();


	loki.id = LOKI;
	loki.tranche=1;
	loki.setBackgroundSensitivity(VERY_SENSITIVE);
	loki.setNoisiness(QUIET);
	sect.addInstrument(loki);

	freia.id = FREIA;
	freia.tranche=1;
	freia.setBackgroundSensitivity(VERY_SENSITIVE);
	freia.setNoisiness(NOISY);
	sect.addInstrument(freia);

	sleipnir.id = SLEIPNIR;
	sleipnir.tranche=2;
	sleipnir.setBackgroundSensitivity(VERY_SENSITIVE);
	sleipnir.setNoisiness(QUIET);
	sect.addInstrument(sleipnir);

	essense.id = ESSENSE;
	essense.tranche = 2;
	essense.setBackgroundSensitivity(NOT_SENSITIVE);
	essense.setNoisiness(QUIET);
	sect.addInstrument(essense);

	wanse.id = WANSE;
	wanse.tranche = 3;
	wanse.setBackgroundSensitivity(NOT_SENSITIVE);
	wanse.setNoisiness(QUIET);
	sect.addInstrument(wanse);



	empty.tranche =4;
	sect.addInstrument(empty);
	empty2.tranche =4;
	sect.addInstrument(empty2);
	empty3.tranche =4;
	sect.addInstrument(empty3);
	empty4.tranche =4;
	sect.addInstrument(empty4);
	empty5.tranche =4;
	sect.addInstrument(empty5);
	empty6.tranche =4;
	sect.addInstrument(empty6);
	empty7.tranche =4;
	sect.addInstrument(empty7);
	// empty8.tranche =4;
	// sect.addInstrument(empty8);
	// empty9.tranche =4;
	// sect.addInstrument(empty9);

	sect.distributeInstruments(position);
}



void base_individual::buildEastSector(void)
{

  
	sect = Sector();

	//TODO - test beamline
	Instrument vor(VOR);
	Instrument estia(ESTIA);
	Instrument skadi(SKADI);
	Instrument vespa(VESPA);	
	Instrument anni(ANNI);
	Instrument surf(SURFACE_SCATTERING);

	//TODO - surface scattering
	

	//Instrument HOD(HOD);

	Instrument empty(EMPTY);
	Instrument empty2(EMPTY2);
	Instrument empty3(EMPTY3);
	Instrument empty4(EMPTY4);
	Instrument empty5(EMPTY5);
	Instrument empty6(EMPTY6);


//	Instrument empty6(EMPTY6);
//	Instrument empty7(EMPTY7);


//	const double lateralPadding = 0.2;		//10 cm clearance on each side padding - Iain Sutton
//	const double motorSpace = 0.5;			// Motor needs to have extra space - Ian Sutton
//	const double bearingSpace = 0.1;		// Clearnace on the wheel where the bearing is - Iain Sutton

	//Make sure our parameter values are up to date
	calculate_position();


	//	void addChopper(double radialDistance, double upstreamDiskClearance, double downstreamDiskClearance, double totalHousingWidth);



	skadi.id = SKADI;
	skadi.tranche =1;
	skadi.setBackgroundSensitivity(SENSITIVE);
	skadi.setNoisiness(QUIET);
	sect.addInstrument(skadi);


	estia.id = ESTIA;			
	estia.tranche = 1;
	estia.setBackgroundSensitivity(VERY_SENSITIVE);
	estia.setNoisiness(QUIET);
	sect.addInstrument(estia);
	
	anni.id = ANNI;	
	anni.tranche =2;
	anni.setBackgroundSensitivity(SENSITIVE);
	anni.setNoisiness(NOISY);
	sect.addInstrument(anni);

	vespa.id = VESPA;	
	vespa.tranche =2;
	vespa.setBackgroundSensitivity(VERY_SENSITIVE);
	vespa.setNoisiness(VERY_NOISY);
	sect.addInstrument(vespa);

	vor.id = VOR;	
	vor.tranche =1;
	vor.setBackgroundSensitivity(VERY_SENSITIVE);
	vor.setNoisiness(VERY_NOISY);
	sect.addInstrument(vor);

	surf.id = SURFACE_SCATTERING;	
	surf.tranche =2;
	surf.setBackgroundSensitivity(VERY_SENSITIVE);
	surf.setNoisiness(QUIET);
	sect.addInstrument(surf);
	

	empty.tranche =3;
	empty.setBackgroundSensitivity(0);
	empty.setNoisiness(0);
	sect.addInstrument(empty);

	empty2.tranche =3;
	empty2.setBackgroundSensitivity(0);
	empty2.setNoisiness(0);
	sect.addInstrument(empty2);

	empty3.tranche =3;
	empty3.setBackgroundSensitivity(0);
	empty3.setNoisiness(0);
	sect.addInstrument(empty3);

	empty4.tranche =3;
	empty4.setBackgroundSensitivity(0);
	empty4.setNoisiness(0);
	sect.addInstrument(empty4);

	empty5.tranche =3;
	empty5.setBackgroundSensitivity(0);
	empty5.setNoisiness(0);
	sect.addInstrument(empty5);

	empty6.tranche =4;
	empty6.setBackgroundSensitivity(0);
	empty6.setNoisiness(0);
	sect.addInstrument(empty6);

//	empty7.tranche =4;
//	empty7.setBackgroundSensitivity(0);
//	empty7.setNoisiness(0);
//	sect.addInstrument(empty7);


	sect.distributeInstruments(position);
}



void base_individual::buildSouthSector(void)
{

 
sect = Sector();

Instrument dream(DREAM);
Instrument odin(ODIN);


//const double lateralPadding = 0.2;		//10 cm clearance on each side padding - Iain Sutton
//	const double motorSpace = 0.5;			// Motor needs to have extra space - Ian Sutton
//	const double bearingSpace = 0.1;		// Clearnace on the wheel where the bearing is - Iain Sutton

	//Make sure our parameter values are up to date
	calculate_position();


	odin.id = ODIN;
	odin.tranche = 1;
	odin.setBackgroundSensitivity(NOT_SENSITIVE);
	odin.setNoisiness(VERY_NOISY);
	sect.addInstrument(odin);

	dream.id = DREAM;
	dream.tranche = 1;
	dream.setBackgroundSensitivity(NOT_SENSITIVE);
	dream.setNoisiness(NOISY);
	sect.addInstrument(dream);

	//TODO ESPRESSO

	sect.distributeInstruments(position);
}



void base_individual::buildWestSector(void)
{
  
	//Dream T0 assembly 1.04 metres long, diameter = 75 cm box

	sect = Sector();
	sect.setArcLength(45.0);
	
	// Long sector defined in this order, insertion
	// BEER, NMX, CAMEA, CSPEC, MAGIC, TREX, HEIMDAL, MIRACLES
	
	Instrument nmx(NMX);
	Instrument miracles(MIRACLES);
	Instrument beer(BEER);
	Instrument cspec(CSPEC);
	Instrument trex(TREX);	
	Instrument magic(MAGIC);
	Instrument camea(CAMEA);
	Instrument heimdal(HEIMDAL);	
		
	Instrument empty(EMPTY);
	Instrument empty2(EMPTY2);
	Instrument empty3(EMPTY3);
	Instrument empty4(EMPTY4);

	

//	const double lateralPadding = 0.2;		//10 cm clearance on each side padding - Iain Sutton
//	const double motorSpace = 0.5;			// Motor needs to have extra space - Ian Sutton
//	const double bearingSpace = 0.1;		// Clearnace on the wheel where the bearing is - Iain Sutton

	//Make sure our parameter values are up to date
	calculate_position();

	nmx.id = NMX;
	nmx.tranche =1;
	nmx.setBackgroundSensitivity(NOT_SENSITIVE);
	nmx.setNoisiness(QUIET);
	sect.addInstrument(nmx);

	miracles.id = MIRACLES;	
	miracles.tranche =1;
	miracles.setBackgroundSensitivity(SENSITIVE);
	miracles.setNoisiness(NOISY);
	miracles.setC1shift(position[C1]);
	sect.addInstrument(miracles);

	beer.id = BEER;
	beer.tranche = 1;
	beer.setBackgroundSensitivity(NOT_SENSITIVE);
	beer.setC1shift(position[C2]);
	beer.setNoisiness(NOISY);
	sect.addInstrument(beer);

	cspec.id = CSPEC;	// Werner doesn't think that this chopper should be so large
	cspec.tranche = 1;
	cspec.setBackgroundSensitivity(VERY_SENSITIVE);
	cspec.setNoisiness(QUIET);
	cspec.setC1shift(position[C4]);
	sect.addInstrument(cspec);

	trex.id = TREX;			
	trex.tranche = 1;
	trex.setBackgroundSensitivity(VERY_SENSITIVE);
	trex.setNoisiness(QUIET);
	trex.setC1shift(position[C7]);
	sect.addInstrument(trex);

	magic.id = MAGIC;  
	magic.tranche=1;
	magic.setBackgroundSensitivity(NOT_SENSITIVE);
	magic.setNoisiness(NOISY);
	magic.setC1shift(position[C5]);
	sect.addInstrument(magic);

	camea.id = CAMEA;	//Werner thinks that the chopper can be smaller here
	camea.tranche =1;
	camea.setBackgroundSensitivity(SENSITIVE);
	camea.setNoisiness(QUIET);
	camea.setC1shift(position[C3]);
	sect.addInstrument(camea);

	heimdal.id = HEIMDAL;	// From proposal
	heimdal.setBackgroundSensitivity(SENSITIVE);
	heimdal.tranche = 1;	// Werner thinks that the guide is big but the chopper could be smaller
	heimdal.setNoisiness(NOISY);
	heimdal.setC1shift(position[C6]);
	sect.addInstrument(heimdal);
	
	empty.id = EMPTY;
	empty.tranche =3;
	empty.setBackgroundSensitivity(0);
	empty.setNoisiness(0);
	sect.addInstrument(empty);

	empty2.id = EMPTY2;
	empty2.tranche =3;
	empty2.setBackgroundSensitivity(0);
	sect.addInstrument(empty2);

	empty3.id = EMPTY3;
	empty3.tranche =3;
	empty3.setBackgroundSensitivity(0);
	sect.addInstrument(empty3);

	empty4.id = EMPTY4;
	empty4.tranche =3;
	empty4.setBackgroundSensitivity(0);
	sect.addInstrument(empty4);


	sect.distributeInstruments(position);

}
