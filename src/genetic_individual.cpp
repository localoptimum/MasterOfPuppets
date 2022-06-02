#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cmath>
#include <float.h>
#include <climits>

#include "globals.h"
#include "base_individual.h"
#include "genetic_individual.h"
#include "algo.h"
#include "graycodes.h"
#include "monte_carlo.h"
#include "Sector.h"
#include "Instrument.h"

#include "test_functions.h"

using namespace std;










genetic_individual::genetic_individual(void)
{
	//Initialises a new genetic_individual
	int i;

	


	//Set position and velocity to zero - randomise called later
	for(i=0; i<num_params; i++)
	{
		this->position[i] = 0.0;
	}

	fitness = 10000.0;	//This is updated at end of randomise()
	is_evaluated = false;


}



// double genetic_individual::get_fitness(void)
// {
// 	if(is_evaluated == false)
// 	{
// 		calc_fitness();
// 	}
// 	return(fitness);
// }

double genetic_individual::get_position(int index)
{
	return(position[index]);
}




void genetic_individual::set_id(int idnum)
{
	id = idnum;
}




void genetic_individual::set_trace(bool setval)
{
	trace = setval;
	if(trace==true)
	{
		trace_position();	//Put the initial data in
	}
}


void genetic_individual::init_best(double position[MAX_PARAMS])
{
	//sets the initial position to a good one

	ifstream bestParams;
	char buffer[256];	//String buffer for reading a line
	int i;

	bestParams.open("best_params.dat");

	if(!bestParams.good())
	{
		cerr << "Shite best_params.dat file" << endl;
		exit(1);
	}

	bestParams.getline(buffer, 256); //That was the gain factor

	for(i=0; i<num_params; i++)
	{
		bestParams.getline(buffer, 256);
		position[i] = atof(buffer);
	}

	bestParams.close();
}






void genetic_individual::randomise(void)
{	//This function is called after initialisation of the swarm
	//It sets up random values after the random number generator has been seeded
	int i;

        //Set position and velocity within the search area desired
        for(i=0; i<num_params; i++)
        {
                position[i] = mc -> random_double(lolimit[i], hilimit[i]);
        }

        //This next line overwrites the initial params with a good starting point
        //Should explore around the local minimum
        //init_best(position);
		
		//Now set the chromosomes to represent this number
		calculate_chromosomes();
}





void genetic_individual::load_limits(const char *fileName)
{
	ifstream limit_file;
	string comment;
	
	int i;
	
	limit_file.open("limits.dat");
	if(!limit_file.good())
	{
		cerr << "Limit file \"limits.dat\" not found" << endl;
		exit(0);
	}
	
	for(i=0; i<num_params; i++)
	{
		limit_file >> lolimit[i];
		limit_file >> hilimit[i];
		//And now ignore the rest of the line (comments)
		getline(limit_file, comment);
	}
}


	





void genetic_individual::calculate_position(void)
{
	const unsigned int NumDivs = ULONG_MAX/2;
	unsigned int iCodeVal;
	double dCodeVal;
	double dNumDivs = (double) NumDivs;
	double range;
	double scaleFactor;
	
	int i;
	
	//The code number goes from lower_limit to upper_limit
	//in steps of range / NumDivs
	
	for(i=0; i<num_params; i++)
	{
		range = hilimit[i] - lolimit[i];
		scaleFactor = dNumDivs / range;
		
		//Get the code value
		iCodeVal = gray_to_dec(position_chromosome[i]);
		
		dCodeVal = (double) iCodeVal;
		
		position[i] = lolimit[i] + (dCodeVal / scaleFactor);
	}
}		






void genetic_individual::calculate_chromosomes(void)
{
	//This function maps the chromosome float values onto gray strings
	
	const unsigned int NumDivs = ULONG_MAX/2;
	unsigned int iCodeVal;
	double dCodeVal;
	double dNumDivs = (double) NumDivs;
	double range;
	double scaleFactor;
		
	int i;
	//The code number goes from lower_limit to upper_limit
	//in steps of range / NumDivs
	
	for(i=0; i<num_params; i++)
	{
		range = hilimit[i] - lolimit[i];
		scaleFactor = dNumDivs / range;
		
		dCodeVal = (position[i] - lolimit[i])*scaleFactor;
		
		iCodeVal = (unsigned int) dCodeVal;
		
		//Now store that value in chromosome string
		dec_to_gray(iCodeVal, position_chromosome[i]);
	}
}


void genetic_individual::mutate(void)
{
	int i;
	int j;
	
	double random_number;

	//For low mutation rates, it's possible nothing changes here.
	//If we make a change, then we need to set is_evaluated to false
	bool was_modified = false;

	
	
	for(i=0; i< num_params; i++)
	{

		for(j=0; j<32; j++) //Last character in string is '\0'
		{
			random_number = mc -> random_double(0.0, 1.0);
			if(random_number <= MUTATION_RATE)
			{
				//We changed something
				was_modified = true;
				
				//Toggle gray code bit
				if(position_chromosome[i][j] == '0')
					position_chromosome[i][j] = '1';
				else
					position_chromosome[i][j] = '0';
			}
		}
		position_chromosome[i][32]='\0';
	}

	//If we modified any of the strings, we need to evaluate the new individual
	if(was_modified == true)
	{
		is_evaluated = false;
	}
}

void genetic_individual::do_chromosome_test(void)
{	
	setlimit(0, 0.0, 20.0);
	setlimit(1, 0.0, 20.0);

	setposition(0, 19.84);
	setposition(1, 19.84);

	calculate_chromosomes();

	setposition(0, 0.0);
	setposition(1, 0.0);

	cout << "reset position " << position[0] << ", " << position[1] << endl;
	
	calculate_position();

	cout << "recovered position " << position[0] << ", " << position[1] << endl;
}





void genetic_individual::setlimit(int i, double low, double high)
{
	lolimit[i] = low;
	hilimit[i] = high;
}



void genetic_individual::setposition(int i, double val)
{
	position[i] = val;
}








void genetic_individual::needs_evaluating(void)
{
	is_evaluated = false;
}




void genetic_individual::trace_position(void)
{
	ofstream trajfile;
	char fnam[256];
	int i;
	
	if(trace==false) return;
	
	sprintf(fnam, "traj/traj%i.dat", id);
	
	trajfile.open(fnam, ofstream::out | ofstream::app);
	
	if(!trajfile.good())
	{
		//Could be that the directory doesn't exist - mkdir and try again
		system("mkdir traj");
		
		trajfile.open(fnam, ofstream::out | ofstream::app);
		if(!trajfile.good())
		{
			cerr << "Error opening " << fnam << " for tracing" << endl;
			exit(1);
		}
	}
	
	trajfile << fitness << " ";

	for(i=0; i<num_params; i++)
	{
		trajfile << position[i] << " " ;
	}
	trajfile << endl;
	
	trajfile.close();
}



genetic_individual& genetic_individual::operator=(const genetic_individual& source)
{
  base_individual::operator=(source);
	int i,j;
	//Copies entire individual over

	//In order, from base class
		
	this->fitness = source.fitness;
	this->id = source.id;
	this->trace = source.trace;
	
	

	
	for(i=0; i< num_params; i++)
	{		
		//vectors
		this->position[i] = source.position[i];
		this->hilimit[i] = source.hilimit[i];
		this->lolimit[i] = source.lolimit[i];
		
	}
	
	
	
	//Extra parameters defined in genetic Individual
	//evaluation is quick, force it to repeat to rebuild the sector
	//this->is_evaluated = source.is_evaluated;
	this->is_evaluated = false;
	
	for(i=0; i< num_params; i++)
	{
		//Start with strings
		for(j=0; j<33; j++)
			this->position_chromosome[i][j] = source.position_chromosome[i][j];
	}

	return(*this);
}



genetic_individual& genetic_individual::operator+(const genetic_individual& other)
{
	//Mixes genetic material with "other" creature using uniform crossover
	int i, j;

	for(i=0; i< num_params; i++)
	{
		for(j=0; j<32; j++)
		{
			//Uniform cross-over
			if(mc -> toss_coin() != 0)
			{
				this->position_chromosome[i][j] = other.position_chromosome[i][j];
			}
		}
	}

	
	//Now update positions using new chromosome codes
	this->calculate_position();
	
	//We've changed the chromosome so this individual needs to be evaulated
	this->is_evaluated = false;
	
	return(*this);
}






void genetic_individual::display_diagnostics(void)
{
  base_individual::display_diagnostics();  
}




void genetic_individual::calc_fitness(void)
{
  base_individual::calc_fitness();  
  is_evaluated = true;
}



