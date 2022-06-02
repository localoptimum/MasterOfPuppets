//________________________________________________________
//
//  Algorithm modes
//________________________________________________________

#ifndef ALGO_H
#define ALGO_H

#define SWARM_MODE	1
#define GENETIC_MODE	2
#define SEED_EVAL_MODE  3
#define SEEDED_SWARM_MODE 4

//________________________________________________________
//
//  States
//________________________________________________________


#define ON		1
#define OFF		0


//________________________________________________________
//
//  Parameters controlling both algorithms behaviour
//________________________________________________________

//Maximum number of varying parameters.  Note that the actual
//dimensionality of the search space is calculated on the fly,
//and that it cannot exceed this value because of memory requirements.
//If you need a larger number of parameters to vary, increase this number
#define MAX_PARAMS 25

//The number of parameters.  If you want to do this on the fly, make sure
//that it is uncommented in main.cpp
//#define NUM_PARAMS 14 //This is 12 for each sector position, plus 2 for the angles
#define NUM_PARAMS 21 //The 14 from above, plus the chopper shift for 7 beam lines per sector

//This is the number of agents.  A figure of around 50 should be sufficient.
//For the genetic algorithm, it certainly should not be lower than 40.
//Counter-intuitively, a very large number close to, or more than 100, does not
//really change the behaviour of the evolution (look around you in the real world!)
#define NUM_AGENTS 50

//This is the number of iterations to perform before quitting
//You should certainly wait more than 150 in any case
#define MAX_STEPS 200


//________________________________________________________
//
//  Parameters controlling genetic algorithm behaviour
//________________________________________________________

//Mutation rate should be low, < 1%.  High values make search less guided
#define MUTATION_RATE	0.07

//Sexual rate should be high, perhaps around 90%.  But, it's best to not
//have 100 percent because some successful individuals should survive
//for a long time
//if you make this too low the algorithm will get stuck very quickly
#define SEXUAL_RATE		0.80

//The two provided selection mechanisms
//Tournament selection is fast but converges fast, so it might stop too soon
#define TOURNAMENT_MODE		1
//Rank selection requires evaluation of every individual and sorting
//it converges slower, however, so makes for a more thorough search
#define RANK_MODE			2




//_______________________________________________________________
//
//  Parameters controlling particle swarm algorithm behaviour
//_______________________________________________________________

//Inertial constant damps out the velocity.  This should be around0.95
#define INERTIAL_CONSTANT			0.95

//Local search weight places emphasis on the particle's own experiences.
//It should be around 1
#define LOCAL_SEARCH_WEIGHT			1.0

//Global search weight places emphasis on the collective experience of
//the swarm.  It should be around 1
#define COLLECTIVE_SEARCH_WEIGHT	1.0

#endif
