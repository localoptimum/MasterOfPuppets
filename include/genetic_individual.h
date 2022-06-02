#include "algo.h"
#include "Sector.h"

class genetic_individual : public base_individual {

private:
	char position_chromosome[MAX_PARAMS][33];	
	void calc_fitness();
	
	
	//Calculates the position vectors from the chromosomes

	
	//Calculates the chromosomes from the position vectors
	//this is used after hitting the search space walls, for example
	
	void needs_evaluating();

	void sync_sector();


public:

	
	
	double get_position(int index);
		
	void calculate_position(void);		

	void calculate_chromosomes(void);	

	
	
	
	//Control Functions
	void trace_position(void);
	void load_limits(const char *fileName);
	void init_best(double position[MAX_PARAMS]);
	void set_id(int idnum);
	void set_trace(bool setval);
	void randomise(void);
	
	void display_diagnostics(void);


	void mutate(void);
	
	void setlimit(int i, double low, double high);
	void setposition(int i, double val);
	
	void do_chromosome_test(void);
	
	//Constructors
	genetic_individual();
	
	void first_run_calc_fitness();
	
	//Operators
	//simple copy
	genetic_individual& operator=(const genetic_individual& source);	
	//Blends genes with mum creature
	genetic_individual& operator+(const genetic_individual& other);	
};
