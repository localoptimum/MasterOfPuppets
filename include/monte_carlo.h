#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/random/variate_generator.hpp>

#ifndef MONTE_CARLO_H_

#define MONTE_CARLO_H_


class monte_carlo {
public:
	monte_carlo();
	virtual ~monte_carlo();

	double random_double(double min, double max);
	int toss_coin(void);


private:

	boost::random::mt19937 mersenne_twister;
	boost::random::uniform_real_distribution<> flat_distribution;
	boost::random::variate_generator<boost::mt19937, boost::random::uniform_real_distribution<> > generator;

};




#endif /* MONTE_CARLO_H */
