#include "algo.h"

#include "Sector.h"

#include <string>

#ifndef BASE_INDIVIDUAL_H_
#define BASE_INDIVIDUAL_H_

//Individual base class
class base_individual {
	
protected:

  bool is_evaluated;	//Only evaluate if requested in tournament selection
  //Therefore much faster!!!

  //Monitoring of solution search
  bool fixedInstOK;
  bool wallsOK;
  bool interfOK;
  bool guideOK;
  bool suiteOK;
  
	
	//MEASUREMENTS OF QUALITY
	double fitness;		//The fitness	
	int id;			//ID number for trajectory tracing, if needed
	bool trace;		//Do we trace the trajectory?
	
	//CONTROL PARAMETERS
	//Each position is described by an unsigned long
	//An unsigned long is converted into gray code and stored in a char[]
	//unsigned int = 32 bits, plus termination character '\0' makes 33
	double position[MAX_PARAMS];	//Position of individual in search space
	double hilimit[MAX_PARAMS];		//maximum position
	double lolimit[MAX_PARAMS];		//minimum position

	double rate_c1_distances(void); // Scores first chopper positions, as near to the source as possible
	
	virtual void calc_fitness(void);
	

public:
	
	base_individual();
	virtual ~base_individual();

	Sector sect;

	virtual double get_fitness(void);
	
	virtual void trace_position(void);
	virtual void load_limits(const char *limitFileName);
	virtual void seed_manual_code(const double set_position[MAX_PARAMS]); //seed individual with code
	virtual void init_best(std::string filename); // seed individual from file
	virtual void set_id(int idnum);
	virtual void set_trace(bool setval);
	virtual void randomise(void);
	virtual void update(double global_best, double global_best_position[MAX_PARAMS]);	//updates the position of the individual
	virtual double get_best_fitness_pos(double pos[MAX_PARAMS]);

	virtual void display_diagnostics(void);

	virtual void calculate_position(void);

	
	//Operators
	//simple copy
	base_individual& operator=(const base_individual& source);	


	// Master of Puppets specific code
	void buildWestSector();
	void buildNorthSector();
	void buildSouthSector();
	void buildEastSector();
};

#endif
