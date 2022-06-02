
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/random/variate_generator.hpp>
#include <time.h>

#include "monte_carlo.h"

using namespace std;






monte_carlo::monte_carlo()
//For reproducable runs, seed with a constant:
//: mersenne_twister(42), flat_distribution(0.0, 100), generator(mersenne_twister, flat_distribution)
//For random runs each time, seed with time of execution
: mersenne_twister(time(NULL)), flat_distribution(0.0, 100), generator(mersenne_twister, flat_distribution)
{
	//More initialisation required?
	//Reason for the above: the variate_generator has no default constructor, so need to put it all into an initialisation list

}

monte_carlo::~monte_carlo()
{}



int monte_carlo::toss_coin(void)
{
	//returns 1 or 0 with equal probability
	double rand_val;
	
	rand_val = random_double(0.0, 1.0);
	
	if(rand_val <= 0.5) return(0);
	else return(1);
}


double monte_carlo::random_double(double mn, double mx)
{
	double rand_val;
	double ans;
	
	rand_val = generator();

	ans = mn + (mx-mn)*rand_val/ 100.0;
//	cout << ans << "\t" << mn << "\t" << mx << endl;
	return(ans);
}
