#include "algo.h"


class swarm_individual : public base_individual {
private:
	
	void calc_fitness(void);
	void trace_position(void);
	
	double best_seen_pos[MAX_PARAMS];
	double velocity[MAX_PARAMS];
	double best_seen_fitness;
	
public:
	char vdef[256];				//The definition of $V from batch file
	char pdef[256];				//The definition of $P from batch file
	char vitessString[8192];	//The command string to execute the vitess sim
	char cmdHead[MAX_PARAMS][512]; //The command header per variable parameter
		
		
	//Control Functions
	void load_limits(const char *fileName);
	void init_best(std::string filename);
	void set_id(int idnum);
	void set_trace(bool setval);
	void randomise(void);
	void update(double global_best, double global_best_position[MAX_PARAMS]);	//updates the position of the individual
	double get_best_fitness_pos(double pos[MAX_PARAMS]);		//returns the best-so-far figure of merit for the individual
	
	//Constructors
	swarm_individual();
	
	//Operators
	//individual& operator=(const individual& src);
};
