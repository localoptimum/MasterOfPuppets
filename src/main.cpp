/* 
"MoP - Master of Puppets"
 Program to automatically optimise instrument beam ports, beam allocations
 using genetic and particle swarm optmisation algorithms.
 
 Copyright (C) 2014  Phillip Bentley <phillip.bentley@esss.se>
  
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/ 

#include <iostream>
#include <fstream>
#include <climits>
#include <cstdlib>
#include <omp.h>

#include "algo.h"
#include "Sector.h"
#include "globals.h"
#include "base_individual.h"
#include "swarm_individual.h"
#include "genetic_individual.h"
#include "monte_carlo.h"
#include "graycodes.h"


using namespace std;

//Global parameter - Number of dimensions in search space
int num_params;

//Global parameter - random number generator
monte_carlo* mc;

//Useful for debugging geometry and making sure things are bending in the right way
const bool exaggerate_geometry = false;//true;

// Whether or not to fix instruments in their locations
const bool fix_nmx = true;
const bool fix_beer = false;
const bool fix_all_instruments = false;


//Global parameter - whether to use generic instruments or not
const bool genericInstruments = false;


//Global parameter - which sector we are using
//const int sectorToStudy = NORTH_SECTOR;
const int sectorToStudy = WEST_SECTOR;
//const int sectorToStudy = EAST_SECTOR;

int mode=SWARM_MODE;

const Matrix unitMatrix = Matrix();



void copypos(double src[MAX_PARAMS], double dst[MAX_PARAMS])
{
	int i;
	
	for(i=0; i<num_params; i++)
		dst[i] = src[i];
}


void save_best(double gBest, double gBestPos[])
{
	ofstream bestfile;
	int i;
	
	bestfile.open("best_instrument.txt");
	
	if(!bestfile.good())
	{
		cerr << "Error opening best_instrument.txt" << endl;
		exit(1);
	}
	
	bestfile << "Best instrument: " << gBest << endl;
	
	for(i=0; i<num_params; i++)
		bestfile << gBestPos[i] << endl;
	
	bestfile.close();
}




void visualise_best(genetic_individual best)
{

	//Bugs.  Go through and use instrument objects to clarify the code

	ofstream euk;
	int port;
	double arg;
	int chop;

	int sectArcStart=30; // Angle of green sector arc in eukledies

	arg = best.get_fitness();  //makes sure that the sect object is up to date, distributed beam ports etc

	Sector sect = best.sect;   // extract a copy of the sect object to reduce code verbosity

	Instrument inst;			// a copy of internal instrument object to reduce code verbosity




	euk.open("best_layout.euk");	//eukleides file for pretty diagram

	if(!euk.good())
	{
		cerr << "Error opening best_layout.euk" << endl;
		exit(1);
	}

	
	
	// Start of eukleides file
	euk << "box -20, -1, 20, 20" << endl;
	euk << "O = point(0, 0)" << endl;
	euk << "Og1 = point(-0.05, 6.5)" << endl;
	euk << "Og2 = point(0.05, 6.5)" << endl;
	euk << "Og11= point(-0.05, 20.0)" << endl;
	euk << "Og22= point(0.05, 20.0)" << endl;


	
	

	for(port=0; port<12; port++)
	{
		inst = sect.getInstrumentOnPort(port);
		euk << codeNames[inst.id] << port << " = point(0, 18)" << endl;
		cout << "viz nm " << codeNames[inst.id] << endl;
	}

	euk << "OC = circle(O, 6.5)" << endl;
	euk << "IC = circle(O, 2)" << endl;

	euk << "SECT = circle(O, 4.5)" << endl;



	for(port=0; port<12; port++)
	{
		inst = sect.getInstrumentOnPort(port);

		euk << "axis" << port << "=line(O," << codeNames[inst.id] << port << ")"<<endl;
		euk << "axis" << port << "g1 = Og1.Og11" << endl;
		euk << "axis" << port << "g2 = Og2.Og22" << endl;

		for(chop=0; chop < inst.get_num_choppers(); chop++)
		{
			euk << "c" << port << "c" << chop << "p1 = point(-" << inst.get_chopper_rad(chop) << ", " << inst.get_chopper_min(chop) << ")" << endl;
			euk << "c" << port << "c" << chop << "p2 = point( " << inst.get_chopper_rad(chop) << ", " << inst.get_chopper_min(chop) << ")" << endl;
			euk << "c" << port << "c" << chop << "p3 = point( " << inst.get_chopper_rad(chop) << ", " << inst.get_chopper_max(chop) << ")" << endl;
			euk << "c" << port << "c" << chop << "p4 = point(-" << inst.get_chopper_rad(chop) << ", " << inst.get_chopper_max(chop) << ")" << endl;

			euk << "c" << port << "c" << chop << " = " <<
					"c" << port << "c" << chop << "p1." <<
					"c" << port << "c" << chop << "p2." <<
					"c" << port << "c" << chop << "p3." <<
					"c" << port << "c" << chop << "p4." <<
					"c" << port << "c" << chop << "p1" << endl;
		}
	}

	//cout << "viz dump:" << endl;

	//sect.listPortAllocations();

	for(port=0; port<12; port++)
	{
		inst = sect.getInstrumentOnPort(port);

		arg = (90.0-30.0-inst.get_beam_centre());
		euk << "axis" << port << "= rotation(axis" << port << ", O, " << arg <<" deg)" << endl;
		euk << "axis" << port << "g1 = rotation(axis" << port << "g1, O, " << arg <<" deg)" << endl;
		euk << "axis" << port << "g2 = rotation(axis" << port << "g2, O, " << arg <<" deg)" << endl;

		//from sector.cpp
		//cout << "beam port " << i << " "  << instrumentAllocation[i] << "th Instrument " << instNames[instrumentAllocation[i]] << " centre " << instruments[i].get_beam_centre() << endl;

		//cout << "viz port " << port << " "  << sect.instAllocation(port) << "th Instrument " <<  instNames[sect.instAllocation(port)] << "  ang " << inst.get_beam_centre() << endl;


		for(chop=0; chop < inst.get_num_choppers(); chop++)
		{
			euk << "c" << port << "c" << chop << " = rotation(c" << port << "c" << chop << ", O, " << arg <<" deg)" << endl;
		}

		euk << codeNames[inst.id] << port << "= rotation(" << codeNames[inst.id] << port << ", O, " << arg <<" deg)" << endl;

		euk << "label" << endl;
		euk << "  " << codeNames[inst.id] << port << " 0 deg" <<endl;
		euk << "end" << endl;


	}

	euk << "draw" << endl;

	euk << "  OC black" << endl;
	euk << "  IC black" << endl;


	switch(sectorToStudy)
	  {
	  case NORTH_SECTOR:
	    sectArcStart = 0;
	    break;

	  default:
	    sectArcStart = 30;
	    break;
	  }
	
	euk << "  SECT " << 180-sectArcStart-sect.getArcLength()  << " deg " << 180-sectArcStart << " deg green" << endl;
	
	for(port=0; port<12; port++)
	{
		inst = sect.getInstrumentOnPort(port);

		if( (inst.id == EMPTY) || (inst.id == EMPTY2) || (inst.id == EMPTY3) || (inst.id == EMPTY4) || (inst.id == EMPTY5) || (inst.id == EMPTY6) || (inst.id == EMPTY7) || (inst.id == EMPTY8) || (inst.id == EMPTY9) )
		{
			euk << "  axis" << port << ", dashed, gray" << endl;
			euk << "  axis" << port << "g1, dotted, gray" << endl;
			euk << "  axis" << port << "g2, dotted, gray" << endl;
		}
		else
		{
			euk << "  axis" << port << ", black" << endl;
			switch(inst.tranche)
			{
			case 1 :
			{
				euk << "  axis" << port << "g1, dashed, red" << endl;
				euk << "  axis" << port << "g2, dashed, red" << endl;
				break;
			}
			case 2 :
			{
				euk << "  axis" << port << "g1, dashed, green" << endl;
				euk << "  axis" << port << "g2, dashed, green" << endl;
				break;
			}
			case 3 :
			{
				euk << "  axis" << port << "g1, dashed, blue" << endl;
				euk << "  axis" << port << "g2, dashed, blue" << endl;
				break;
			}
			default :
			{
				euk << "  axis" << port << "g1, dashed, black" << endl;
				euk << "  axis" << port << "g2, dashed, black" << endl;
				break;
			}

			}
		}
		euk << "  " << codeNames[inst.id] << port << " disc" << endl;

		for(chop=0; chop < inst.get_num_choppers(); chop++)
				{
					euk << "  c" << port << "c" << chop << " gray" << endl;
				}
	}

	euk << "end" << endl;

	euk.close();



}







void visualise_best_zoom_in_delegated(base_individual best)
{

	//Bugs.  Go through and use instrument objects to clarify the code

	ofstream euk;
	ofstream pov;
	int port;
	int rad;

	int sectArcStart;

	//best.get_fitness();  //makes sure that the sect object is up to date, distributed beam ports etc

	Sector sect = best.sect;   // extract a copy of the sect object to reduce code verbosity


	Instrument inst;			// a copy of internal instrument object to reduce code verbosity


	euk.open("best_layout_zoom_in.euk");	//eukleides file for pretty diagram

	if(!euk.good())
	{
		cerr << "Error opening best_layout.euk" << endl;
		exit(1);
	}


	
	pov.open("best_layout.pov");   //povray file

	if(!pov.good())
	{
		cerr << "Error opening best_layout.pov" << endl;
		exit(1);
	}
	





	// Start of povray file - this is now handled externally
	// pov << "#version 3.7;" << endl;
	// pov << "#include \"colors.inc\"" << endl;
	// pov << "global_settings { assumed_gamma 1 " << endl;
	// pov << "   ambient_light rgb<1,1,1> }" << endl;

	// pov << "camera {" << endl;
	
	// if(sectorToStudy == WEST_SECTOR || sectorToStudy == NORTH_SECTOR)
	//   pov << "   location  <-20,80,20>"  << endl;
	// else
	//   pov << "   location  <0,80,0>"  << endl;
	
	// if(sectorToStudy == WEST_SECTOR || sectorToStudy == NORTH_SECTOR)
	//   pov << "   look_at   <-20,0,20>" << endl;
	// else
	//   pov << "   look_at   <0,0,0>" << endl;

	// pov << "   angle 70.0 }" << endl;
	// pov << "background {color rgb <0.4, 0.4, 0.4>}"  << endl;

	// pov << "light_source {<20, 20, 20>, rgb <1,1,1>} " << endl;
  	
	

	euk << "box -20, -1, 20, 20" << endl;

	euk << "O = point(0, 0)" << endl;
	euk << "Og1 = point(-0.05, 6.5)" << endl;
	euk << "Og2 = point(0.05, 6.5)" << endl;
	euk << "Og11= point(-0.05, 20.0)" << endl;
	euk << "Og22= point(0.05, 20.0)" << endl;

	for(rad = 8; rad < 20; rad=rad+2)
	{
		euk << "radc" << rad << " = circle(O, " << rad << ")" << std::endl;
		euk << "radius" << rad << " = point(0, " << rad << ")" << std::endl;
	}

	euk << "label" << std::endl;

	for(rad = 8; rad < 20; rad=rad+2)
	{
		euk << "  radius" << rad << " 0 deg" << std::endl;

	}
	euk << "end" << std::endl; // label


	for(port=0; port<12; port++)
	{
		inst = sect.getInstrumentOnPort(port);
		if(genericInstruments == true)
		{
			cout << "viz nm " << codeNames[inst.id] << endl;
		}
		else
		{
			cout << "viz nm " << instNames[inst.id] << endl;
		}
	}

	euk << "OC = circle(O, 6.0)" << endl;
	euk << "IC = circle(O, 2)" << endl;

	euk << "SECT = circle(O, 4.5)" << endl;


	sect.declareEukleides(euk);




	euk << "draw" << endl;

	euk << "  OC black" << endl;
	euk << "  IC black" << endl;

	switch(sectorToStudy)
	  {
	  case NORTH_SECTOR:
	    sectArcStart = 0;
	    break;

	  default:
	    sectArcStart = 30;
	    break;
	  }
	
	euk << "  SECT " << 180-sectArcStart-sect.getArcLength()  << " deg " << 180-sectArcStart << " deg green" << endl;

	for(rad = 8; rad < 20; rad=rad+2)
	{
		euk << "radc" << rad << " dashed, gray" << std::endl;
		euk << "radius" << rad << " gray, dot" << std::endl;
	}

	sect.drawEukleides(euk);
	sect.drawPovray(pov);
	

	euk << "end" << endl;

	euk.close();

	pov.close();

}




void visualise_best_zoom_out_delegated(base_individual best)
{

	//Bugs.  Go through and use instrument objects to clarify the code

	ofstream euk;
	int port;
	int rad;

	int sectArcStart;

	//best.get_fitness();  //makes sure that the sect object is up to date, distributed beam ports etc

	Sector sect = best.sect;   // extract a copy of the sect object to reduce code verbosity


	Instrument inst;			// a copy of internal instrument object to reduce code verbosity


	euk.open("best_layout_zoom_out.euk");	//eukleides file for pretty diagram

	if(!euk.good())
	{
		cerr << "Error opening best_layout.euk" << endl;
		exit(1);
	}


	euk << "box -180, -1, 10, 180" << endl;

	euk << "O = point(0, 0)" << endl;
	euk << "Og1 = point(-0.05, 6.5)" << endl;
	euk << "Og2 = point(0.05, 6.5)" << endl;
	euk << "Og11= point(-0.05, 20.0)" << endl;
	euk << "Og22= point(0.05, 20.0)" << endl;

	for(rad = 5; rad < 170; rad=rad+5)
	{
		euk << "radc" << rad << " = circle(O, " << rad << ")" << std::endl;
		euk << "radius" << rad << " = point(0, " << rad << ")" << std::endl;
	}

	euk << "label" << std::endl;

	for(rad = 5; rad < 170; rad=rad+5)
	{
		euk << "  radius" << rad << " 0 deg" << std::endl;

	}
	euk << "end" << std::endl; // label


	for(port=0; port<12; port++)
	{
		inst = sect.getInstrumentOnPort(port);
		if(genericInstruments == true)
		{
			cout << "viz nm " << codeNames[inst.id] << endl;
		}
		else
		{
			cout << "viz nm " << instNames[inst.id] << endl;
		}
	}

	euk << "OC = circle(O, 6.0)" << endl;
	euk << "IC = circle(O, 2)" << endl;

	euk << "SECT = circle(O, 4.5)" << endl;



	sect.declareEukleides(euk);


	euk << "draw" << endl;

	euk << "  OC black" << endl;
	euk << "  IC black" << endl;

	
	switch(sectorToStudy)
	  {
	  case NORTH_SECTOR:
	    sectArcStart = 0;
	    break;

	  default:
	    sectArcStart = 30;
	    break;
	  }
	
	euk << "  SECT " << 180-sectArcStart-sect.getArcLength()  << " deg " << 180-sectArcStart << " deg green" << endl;
	
	for(rad = 5; rad < 170; rad=rad+5)
	{
		euk << "radc" << rad << " dashed, gray" << std::endl;
		euk << "radius" << rad << " gray, dot" << std::endl;
	}

	sect.drawEukleides(euk);

	euk << "end" << endl;

	euk.close();

}




void visualise_best_delegated(base_individual best)
{
	visualise_best_zoom_in_delegated(best);
	visualise_best_zoom_out_delegated(best);
}




void log_best(int stepnum, double gBest, double gBestPos[])
{
	ofstream logfile;
	int i;
	
	logfile.open("best_log.txt", ofstream::out | ofstream::app);
	
	if(!logfile.good())
	{
		cerr << "Error opening best_log.txt" << endl;
		exit(1);
	}
	
	logfile << stepnum << "\t" << gBest;
	for(i=0; i<num_params; i++)
	{
		logfile << "\t" << gBestPos[i];
	}
	logfile << endl;
	
	logfile.close();
}


int tournament_choose_individual(genetic_individual *gAgentArray[NUM_AGENTS], genetic_individual& generationBest)
{
	//Use tournament selection to choose one of two instruments from the pool
	int r1, r2, ans;
	double fitness1, fitness2;
	
	r1 = (int) mc -> random_double(0.0, (double)(NUM_AGENTS-1));
	r2 = (int) mc -> random_double(0.0, (double)(NUM_AGENTS-1));

	//cout << "rand " <<r1 <<  endl;
	//cout << "rand " <<r2 << endl;
	fitness1 = gAgentArray[r1]->get_fitness();
	fitness2 = gAgentArray[r2]->get_fitness();
	
	if(fitness1 < fitness2)
		ans = r1;
	else
		ans = r2;
	
	if( gAgentArray[ans]->get_fitness() < generationBest.get_fitness())
		generationBest = *(gAgentArray[ans]);
	return(ans);
}


int rank_choose_individual(genetic_individual *gAgentArray[NUM_AGENTS], genetic_individual& generationBest)
{ 
	// Select a random member of population.  Probability of selection
	// is proportional to position in gene pool
	// population must be sorted best-worst
	
	
	double sum;
	double accum;
	int rand_val;
	int i;
	int choice;
	// calculate sum;
	
	sum = 0.0;
	for(i=0; i<NUM_AGENTS; i++)
    {
		sum = sum + (double)(NUM_AGENTS-i);
    }
	
	rand_val = mc -> random_double(0.0, sum);
	
	//Find the instrument by going through the accumulation until it exceeds r
	
	accum = 0.0;
	i=0;
	
	while(accum < rand_val)
    {
		accum = accum + (double) (double)(NUM_AGENTS-i);
		i++;
    }
	
	if(i > 0)
		choice =i-1;
	else
		choice = 0;
	//cout << "Rank choice = " << choice << endl;

	return(choice);
}


void sort_population(genetic_individual *gAgentArray[NUM_AGENTS], genetic_individual& generationBest)
{
	int i, j;
	genetic_individual copyTemp;
	
	double fitJ, fitJp1;
	
	//For debugging
	//double fitBefore[NUM_AGENTS];
	//double fitAfter[NUM_AGENTS];
	//for(i=0; i< NUM_AGENTS; i++)
	//	fitBefore[i] = gAgentArray[i]->get_fitness();
	
	//First bubble sort the whole lot
	//Bubble sort is slow, but the monte carlo is probably slower
	//so speed here is not such a worry as getting the right answer
	for(i=0; i<NUM_AGENTS; i++)
	{
		for(j=0; j<NUM_AGENTS-1; j++)
		{
			fitJ   = gAgentArray[j]->get_fitness();
			fitJp1 = gAgentArray[j+1]->get_fitness();
			
			if(fitJp1 < fitJ)
			{
				//Swap them round to accumulate better ones towards index 0
				copyTemp = *gAgentArray[j];
				*gAgentArray[j] = *gAgentArray[j+1];
				*gAgentArray[j+1] = copyTemp;
				
				//Just for debugging
				//fitJ   = gAgentArray[j]->get_fitness();
				//fitJp1 = gAgentArray[j+1]->get_fitness();
			}
		}
	}
	
	//For debugging
	//for(i=0; i< NUM_AGENTS; i++)
	//	fitAfter[i] = gAgentArray[i]->get_fitness();
	
	//cout << "Bubble sort results:" << endl;
	//for(i=0; i< NUM_AGENTS; i++)
	//	cout << fitBefore[i] << "   " << fitAfter[i] << endl;
	
	//After sorting, we know position 0 has the best individual
	generationBest = *gAgentArray[0];
}




void graycode_test(void)
{
	//Gray code test
	
	unsigned int tnum;
	char tnumenc[33];
	
	int i;
	unsigned int result;

	for(i=0; i<100; i++)
	{
		tnum = (unsigned int) mc -> random_double(0.0, (double)(ULONG_MAX/2));
		
		dec_to_gray(tnum, tnumenc);
		
		result = gray_to_dec(tnumenc);
		
		cout << "Encoding of " << tnum << " is:" << endl;
		cout << tnumenc << endl;
		cout << "which gives " << result << endl;
		
		if(result != tnum)
			cerr << "ERROR: " << result << " does not equal " << tnum << endl;
		cout << endl;
	}
	
	tnum = 5;
	
	dec_to_gray(tnum, tnumenc);
	
	result = gray_to_dec(tnumenc);
	
	cout << "Encoding of " << tnum << " is:" << endl;
	cout << tnumenc << endl;
	cout << "which gives " << result << endl;
	
	if(result != tnum)
		cerr << "ERROR: " << result << " does not equal " << tnum << endl;
	cout << endl;
	
	cout << endl;
	exit(0);
}


void chromosome_test(void)
{
	genetic_individual testcase;
	
	testcase.do_chromosome_test();	
}





void testGeometry(void)
{

	ofstream euk;
	Instrument test(LOKI);

	test.setCurvatureDirection(CURVE_RIGHT);

	test.translateFrame(Point(0.0, 4.0));

	test.addCurvedGuide(0.1, 5.0, 10.0, 10);

	test.translateFrame(Point(0.0, 1.0));
	test.terminate();



	euk.open("testGeom.euk");	//eukleides file for pretty diagram

	if(!euk.good())
	{
		cerr << "Error opening testGeom.euk" << endl;
		exit(1);
	}

	euk << "O = point(0, 0)" << endl;
	euk << "box -20, -1, 20, 20" << endl;

	euk << "OC = circle(O, 6.5)" << endl;
	euk << "IC = circle(O, 2)" << endl;

	test.declareEukleides(euk);


	euk << "draw" << endl;

	euk << "  OC black" << endl;
	euk << "  IC black" << endl;

	test.drawEukleides(euk);

	euk << "end" << endl;

	euk.close();
}






int main(int argc, char *argv[])
{	

//	testGeometry();
//	exit(0);

	mc = new monte_carlo();
	
	int n,j;
	
	//Set initial modes to default values
	// int mode=GENETIC_MODE;

	int selection=TOURNAMENT_MODE;
	int elitism=OFF;
	
	
	
	double solution_pos[MAX_PARAMS];
	double random_number;

	string filearg;
	
	double gBest;	//the global best
	double gBestPos[MAX_PARAMS]; //the position of global best	
	
	int steps=0;
	
	double agents_best;
	int num_agents;
	
	int iFather, iMother;	//indexes in arrays
	
	swarm_individual *sAgentArray[NUM_AGENTS];
	swarm_individual sBestAgent;
	
	genetic_individual *gAgentArray[NUM_AGENTS];
	genetic_individual *nextPopulation[NUM_AGENTS];
	genetic_individual *finalBest;
	genetic_individual *generationBest;

		
	 num_agents = NUM_AGENTS;
	 
	 num_params = NUM_PARAMS;


	
	
	
	
	//First thing's first, erase all traced trajectories and prior data
	
#ifdef WINDOWS
	//Windows specific commands
	system("delete traj\*.*");
	system("delete traj");
	system("delete best_instrument.txt");
	system("delete best_log.txt");
#else 
	//All other unix os
	system("rm -rf traj");
	system("rm -f best_instrument.txt");
	system("rm -f best_log.txt");
#endif
	
	
	
	
	
	//Parse the command options
	if(argc > 1)
	{
		for(n=1; n < argc; n++)
		{
			//First check that it's an option
			if(argv[n][0] == '-')
			{
				//Some options have been specified
				for(j=1; argv[n][j] !='\0'; j++)
				{
					switch(argv[n][j])
					{
						case 'g' :
							mode = GENETIC_MODE;
							break;
						case 's' :
							mode = SWARM_MODE;
							break;
					case 'v' :
					  mode = SEED_EVAL_MODE;
					  break;
					case 'S' :
					  mode  = SEEDED_SWARM_MODE;
					  break;
					 
						case 't' :
							selection = TOURNAMENT_MODE;
							break;
						case 'r' :
							selection = RANK_MODE;
							break;
						case 'e' :
							elitism = ON;
							break;
						default :
							//Unknown option so do nothing;
							break;
					}
				}
			}
			//else it's a file name.
			filearg = argv[n];
		}
	}


	if(fix_all_instruments)
	  {
	    cout << "All instrument locations fixed" << endl;
	  }


	if(mode == SEED_EVAL_MODE)
	  {

	    cout << "SEED VISUALISATION MODE" << endl;
	    cout << "******************************" << endl << endl;
	
	    sBestAgent.init_best(filearg);

	    //Make sure it is up to date
	    sBestAgent.get_fitness();

	    //Save the geometry in eukleides format
	    visualise_best_delegated(sBestAgent);
	    
	    //Output diagnostics of the figure of merit
	    sBestAgent.display_diagnostics();

#ifndef WINDOWS
	//All other unix os
	system("eukleides best_layout_zoom_in.euk");
	system("eukleides best_layout_zoom_out.euk");

	system("epstopdf best_layout_zoom_in.eps");
	system("epstopdf best_layout_zoom_out.eps");

#endif
	    
	    
	    exit(0);
	  }
	

	 

	 

	 
	 
	 
	 if(mode == SWARM_MODE || mode == SEEDED_SWARM_MODE)
	 {
		 //THIS IS THE PARTICLE SWARM OPTIMISER

	   if(mode == SWARM_MODE)
	     {
	       cout << "SWARM MODE" << endl;
	     }
	   else
	     {
	       cout << "SEEDED SWARM MODE" << endl;
	     }
	   cout << "******************************" << endl << endl;
		 
		 //Allocate memory
		 for(n=0; n<NUM_AGENTS; n++)
		 {
			 sAgentArray[n] = new swarm_individual;
		 }
		 
		 
		 //Load the limits into memory
		 for(n=0; n<num_agents; n++)
		 {
			 sAgentArray[n]->load_limits("limits.dat");
		 }
		 
		 //Initialise the agents into their random positions	
		 for(n=0; n<num_agents; n++)
		 {
			 sAgentArray[n]->randomise();

			 //Make the first one the best so far until we find better ones
			 gBest = sAgentArray[0]->get_best_fitness_pos(gBestPos);
		 }

		 
		 if(mode == SEEDED_SWARM_MODE)
		   {
		     //Overwrite the first in the array with seed data from the given file
		     sBestAgent.init_best(filearg);

		     //Update the agent so the figure of merit is stored
		     sBestAgent.get_fitness();

		     //Copy this into the population
		     *sAgentArray[0] = sBestAgent;

		     
		     //Track the seed as the starting point for the whole swarm
		     agents_best = sAgentArray[0]->get_best_fitness_pos(solution_pos);
		     gBest = agents_best;
		     copypos(solution_pos, gBestPos);
		   }
		 
		  
		 
		 //Set agents to trace the first 10 trajectories
		 for(n=0; n<num_agents; n++)
		 {
			 sAgentArray[n]->set_id(n);
			 if(n<10)
				 sAgentArray[n]->set_trace(true);
			 else
				 sAgentArray[n]->set_trace(false);
		 }
		 
		 cout << "Processing " << MAX_STEPS << " iterations..." << endl;
		 
		 do
		 {
			 log_best(steps, gBest, gBestPos);
			 save_best(gBest, gBestPos);

			 #pragma omp parallel for
			 for(int i=0; i<num_agents; i++)
			   {
			     sAgentArray[i]->update(gBest, gBestPos);     
			   }
			 
			 for(n=0; n<num_agents; n++)
			 {
			   agents_best = sAgentArray[n]->get_best_fitness_pos(solution_pos);
				 if(agents_best < gBest)
				 {
				   sBestAgent = *sAgentArray[n];
				   gBest = agents_best;
				   copypos(solution_pos, gBestPos);
				 }
				 //cout << steps << "." << n << "   " << agents_best << "   " << gBest << endl;
				 
			 }
			 cout << "   " << 100*steps/MAX_STEPS << "% complete : " << gBest << "\r" ;
			 cout << std::flush;
			 steps++;
		 }
		 while(steps < MAX_STEPS);

		 cout << endl;

		 //Output the final design.  All this pointer stuff "First create swarm copy is now obsolete
		 // sBestAgent = new swarm_individual;

		 // //Seed it with best position
		 // sBestAgent -> seed_manual_code(gBestPos);
		 // //Make sure it is up to date
		 // cout << "seeded " << sBestAgent -> get_fitness() <<  " damn it " << endl;
		 // cout << "gbest  " << gBest << endl;

		 

		 //Save the geometry in eukleides format
		 visualise_best_delegated(sBestAgent);
		 
		 //Output diagnostics of the figure of merit
		 sBestAgent.display_diagnostics();
	 }
	 else
	 {
		 //THIS IS THE GENETIC OPTIMISER
		 
		 cout << "GENETIC MODE" << endl;
		 cout << "******************************" << endl;
		 cout << "   ...using ";
		 
		 switch(selection)
		 {
			 case RANK_MODE :
				 cout << "rank selection" << endl;
				 break;
			 case TOURNAMENT_MODE :
				 cout << "tournament selection" << endl;
				 break;
			 default :
				 cout << "other selection" << endl;
				 break;
		 }
		 
		 cout << "   ...elitism is ";
		 if(elitism == ON)
			 cout << "on" << endl;
		 else
			 cout << "off" << endl;
		 cout << endl;
		 
		 
		 //Allocate memory
		 for(n=0; n<NUM_AGENTS; n++)
		 {
			 //We need to allocate memory for 2 populations
			 //The current and the next
			 gAgentArray[n] = new genetic_individual;
			 nextPopulation[n] = new genetic_individual;
		 }
		 
		 finalBest = new genetic_individual;
		 generationBest = new genetic_individual;

		 
		 //Load the limits into memory
		 for(j=0; j<num_agents; j++)
		 {
			 gAgentArray[j]->load_limits("limits.dat");
		 }
		 
		 //Initialise the agents into their random positions	
		 for(j=0; j<num_agents; j++)
		 {
			 gAgentArray[j]->randomise();
		 }
		 
		 //For tournament selection, evaluate first agent and copy it as best
		 gAgentArray[0]->get_fitness();
		 *finalBest = *gAgentArray[0];
		 gBest = finalBest->get_fitness();

		 
		 do
		 {
			 //This generation needs to keep track of which is best individual
			 gAgentArray[0]->get_fitness();
			 *generationBest = *(gAgentArray[0]);
			 
			 //If using rank_selection, bubblesort the population.  Tournament selection does not need this
			 if(selection == RANK_MODE)
				 sort_population(gAgentArray, *generationBest);
			 
			 //Now go through the update process and overwrite best if necessary
			 for(int i=0; i < NUM_AGENTS; i++) 
			 {
				 if(i<10)
				 {
					 gAgentArray[i]-> set_id(i);
					 gAgentArray[i]-> set_trace(true);
				 }
				 else
				 {
					 gAgentArray[i]-> set_trace(false);
				 }
				 
				 
				 //If elitism is on, copy the last generation best
				 //into the first position, otherwise do normal algorithm
				 if(elitism == ON && i==0)
				 {
					 *nextPopulation[0] = *generationBest;
				 }
				 else
				 {
					 
					 //Pick a random number
					 //If number is within sexual region, then do sexual combination
					 //Otherwise, clone a parent
					 random_number = mc -> random_double(0.0, 1.0);
					 
					 if(selection == RANK_MODE)
						 iMother = rank_choose_individual(gAgentArray, *generationBest);
					 else
						 iMother = tournament_choose_individual(gAgentArray, *generationBest);
					 
					 if(random_number <= SEXUAL_RATE)
					 {	
						 //Pick a different father and generate child using sexual reproduction
						 do
						 {
							 if(selection == RANK_MODE)
								 iFather = rank_choose_individual(gAgentArray,
																  *generationBest);
							 else
								 iFather = tournament_choose_individual(gAgentArray,
																		*generationBest);
						 }
						 while(iFather == iMother);
						 
						 *nextPopulation[i] = *gAgentArray[iMother] + *gAgentArray[iFather];
					 }
					 else
					 {
						 //Mother stays in gene pool for another generation
						 *nextPopulation[i] = *gAgentArray[iMother];
					 }
					 
					 
					 //Lastly, apply mutation
					 nextPopulation[i]->mutate();
				}
			 }
			 steps++;

			 #pragma omp parallel for
			 for(int i=0; i < NUM_AGENTS; i++)
			   {
			     cout << steps << "." << i 
				  << "   " << nextPopulation[i]->get_fitness() // That calculates the new fitness of everyone in generation
				  << "   " << gBest << endl;
			   }
			     
			 //Overwrite the old population with the new population
			 for(n=0; n< NUM_AGENTS; n++)
			 {
				 *gAgentArray[n] = *nextPopulation[n];
				 gAgentArray[n] -> trace_position();
			 }
				 
			 //Now we must keep track of the best individual
			 //Copy over the agent from the current 
			 //generation if it is better than the overall best
			 if(generationBest->get_fitness() < finalBest->get_fitness())
				 *finalBest = *generationBest;
			 
			 //Copy position into final solution space
			 gBest = finalBest->get_fitness();
			 for(n=0; n<num_params; n++)
			 {
				 gBestPos[n] = finalBest->get_position(n);
			 }
			 
			 //Update logs
			 log_best(steps, gBest, gBestPos);
			 save_best(gBest, gBestPos);

		 }
		 while(steps < MAX_STEPS);

		 //Genetic algorithm specific output

		 
		 cout << "Best fitness " << gBest << endl << endl;
		 
		 cout << "Found at: " << endl;
		 
		 
		 for(n=0; n<num_params; n++)
		   {
		     cout << "\t" << gBestPos[n];
		     
		     if(n < 2)
		       cout << endl;
		     else
		       {
			 cout << "        " << finalBest -> sect.instAllocation(n-2) << endl;
		       }
		   }
		 
		 
		 //Save the geometry in eukleides format
		 visualise_best_delegated(*finalBest);
		 
		 //Output diagnostics of the figure of merit
		 finalBest->display_diagnostics();
		 
	 }
	 
	 //Now we are back to general program - write out results found by whatever
	 //algorithm was used
	 
	 

#ifndef WINDOWS
	//All other unix os
	system("eukleides best_layout_zoom_in.euk");
	system("eukleides best_layout_zoom_out.euk");

	system("epstopdf best_layout_zoom_in.eps");
	system("epstopdf best_layout_zoom_out.eps");

#endif


	//Tidy up memory
	if(mode==GENETIC_MODE)
	  {
	    for(n=0; n<NUM_AGENTS; n++)
	      {
		delete gAgentArray[n];
		delete nextPopulation[n];
	      }
	  }
	else
	  {
	    //delete sBestAgent;
	    
	    
	    for(n=0; n<NUM_AGENTS; n++)
	      {
		delete sAgentArray[n];
	      }
	  }
	
	

	return(0);
}
