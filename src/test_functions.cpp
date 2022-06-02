#include <iostream>

using namespace std;


double rosenbrock(double xval, double yval)
{
	//Rosenbrock's curved valley ("banana")
	//Swarm MUCH better with this one
	
	double term1, term2;
	
	double answer;
	
	term1 = 100*(yval-xval*xval)*(yval-xval*xval);
	term2 = (1-xval)*(1-xval);
	
	answer = term1 + term2;
	
	return(answer);
}




double wood(double wval, double xval, double yval, double zval)
{
	//Wood's function of 4 parameters
	
	double term1, term2, term3, term4, term5, term6;
	double answer;
	
	term1 = 100*(xval - wval*wval)*(xval - wval*wval);
	term2 = (wval-1)*(wval-1);
	term3 = 90*(zval-yval*yval)*(zval-yval*yval);
	term4 = (1-yval)*(1-yval);
	term5 = 10.1*( (xval-1)*(xval-1) + (zval-1)*(zval-1) );
	term6 = 19.8*(xval-1)*(zval-1);
	
	answer = term1 + term2 + term3
		+ term4 + term5 + term6;
	return(answer);
}






double powell(double wval, double xval, double yval, double zval)
{
	//Powell's function of 4 parameters.

	//Input Domain:
	//The function is usually evaluated on the hypercube xi ∈ [-4, 5], for all i = 1, …, d.

	// Global Minimum:
	//f(x) = 0 at (0,0,...,0)
	
	double term1, term2, term3, term4;
	double answer;
	
	term1 = (wval+10*xval)*(wval+10*xval);
	term2 = 5*(xval-2*yval)*5*(xval-2*yval);
	term3 = (xval-2*yval)*(xval-2*yval)*(xval-2*yval)*(xval-2*yval);
	term4 = 10*(wval-zval)*(wval-zval)*(wval-zval)*(wval-zval);
	
	answer = term1 + term2 + term3 + term4;
	return(answer);
}

