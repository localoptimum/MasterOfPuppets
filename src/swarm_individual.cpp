#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cmath>
#include <float.h>

#include "globals.h"
#include "base_individual.h"
#include "swarm_individual.h"
#include "algo.h"
#include "monte_carlo.h"

#include "test_functions.h"


using namespace std;






void swarm_individual::set_id(int idnum)
{
	id = idnum;
}

void swarm_individual::set_trace(bool setval)
{
	trace = setval;
	if(trace==true)
	{
		trace_position();	//Put the initial data in
	}
}


void swarm_individual::init_best(std::string filename)
{
	//sets the initial position to a good one

	ifstream bestParams;
	char buffer[256];	//String buffer for reading a line
	int i;

	
	bestParams.open(filename.c_str());
	if(!bestParams.good())
	  {
	    cerr << "Unable to open " << filename << endl;
	    exit(1);
	  }
	cout << "Loading from seed file " << filename << endl;
	
	
	
	bestParams.getline(buffer, 256); //That was the gain factor
	
	for(i=0; i<num_params; i++)
	  {
	    bestParams.getline(buffer, 256);
	    position[i] = atof(buffer);
	  }
	
	bestParams.close();	
}
		

void swarm_individual::calc_fitness(void)
{
  base_individual::calc_fitness();  
}






double swarm_individual::get_best_fitness_pos(double answer_pos[MAX_PARAMS])
{
	//Returns the best seen fitness and its position
	int i;
	
	for(i=0; i<num_params; i++)
	{
		answer_pos[i] = best_seen_pos[i];
	}
	
	return(fitness);
}


void swarm_individual::randomise(void)
{	//This function is called after initialisation of the swarm
	//It sets up random values after the random number generator has been seeded
	int i;
	double vmax;

        //Set position and velocity within the search area desired
        for(i=0; i<num_params; i++)
        {
                position[i] = mc -> random_double(lolimit[i], hilimit[i]);

                vmax = 0.5*(hilimit[i]-lolimit[i]);
                velocity[i] = mc -> random_double(0, vmax);
        }

        //This next line overwrites the initial params with a good starting point
        //Should explore around the local minimum
        //init_best(position);
        
        //Update the fitness
        calc_fitness();
        
        //Copy the intial fitness and position to the best seen so far
        best_seen_fitness = fitness;
        for(i=0; i < num_params; i++)
        {
                best_seen_pos[i] = position[i];
        }	
}


void swarm_individual::load_limits(const char *fileName)
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


	
swarm_individual::swarm_individual(void)
{
	//Initialises a new swarm_individual
	int i;
	
	//Set position and velocity to zero - randomise called later
	for(i=0; i<num_params; i++)
	{
		this->position[i] = 0.0;
		this->velocity[i] = 0.0;
	}
	
	best_seen_fitness = 0.0;

	fitness = 0.0;	//This is updated at end of randomise()
	
}

void swarm_individual::trace_position(void)
{
	ofstream trajfile;
	char fnam[256];
	int i;
	
	sprintf(fnam, "traj/traj%i.dat", id);
	
	trajfile.open(fnam, ofstream::out | ofstream::app);
	
	if(!trajfile.good())
	{
		//Could be that the directory doesn't exist
		system("mkdir traj");
		
		trajfile.open(fnam, ofstream::out | ofstream::app);
		if(!trajfile.good())
		{
			cerr << "Error opening " << fnam << " for tracing" << endl;
			exit(1);
		}
	}
	
	for(i=0; i<num_params; i++)
	{
		trajfile << position[i] << "\t|s| " ;
	}
	trajfile << endl;
	
	trajfile.close();
}


void swarm_individual::update(double gBest, double gBestPos[MAX_PARAMS])
{
	
	int i;
	
	double r1, r2;
		
	
	//Move the swarm_individual through the search space
	//The controlling parameters are defined in algo.h
	
	for(i=0; i<num_params; i++)
	{
		r1 = mc -> random_double(0.0, 1.0);
		r2 = mc -> random_double(0.0, 1.0);
		position[i] = position[i] + velocity[i];
		velocity[i] = INERTIAL_CONSTANT*velocity[i] 
			+ LOCAL_SEARCH_WEIGHT*r1*(best_seen_pos[i] - position[i])
			+ COLLECTIVE_SEARCH_WEIGHT*r2*(gBestPos[i] - position[i]);
		
		//Check to see that we have not escaped the allowed search volume
		if(position[i] > hilimit[i])
			position[i] = hilimit[i];
		if(position[i] < lolimit[i])
			position[i] = lolimit[i];
	}
	
	


	
	//Now see if we have found a better position
	//and update the local best so far if necessary
	
	this -> calc_fitness();
	
	if(fitness < best_seen_fitness)
	{
		best_seen_fitness = fitness;
		for(i=0; i < num_params; i++)
		{
			best_seen_pos[i] = position[i];
		}
	}
	
	//If tracing is turned on, output the trajectory trace data
	if(trace) trace_position();
}
