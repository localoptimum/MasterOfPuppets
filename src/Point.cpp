/*
 * Point.cpp
 *
 *  Created on: 22 Dec 2014
 *      Author: phillipbentley
 */

#include "Point.h"

Point::Point() {
	// TODO Auto-generated constructor stub
	int i;

	for(i=0; i<3; i++)
		vals[i] = 0.0;
}

Point::Point(double xinit, double yinit)
{
	vals[0] = xinit;
	vals[1] = yinit;
	vals[2] = 1.0;
}

void Point::set(double xset, double yset)
{
	vals[0] = xset;
	vals[1] = yset;
	vals[2] = 1.0;
}

double Point::getx(void) const
{
	return(vals[0]);
}

double Point::gety(void) const
{
	return(vals[1]);
}

Point& Point::operator=(const Point& source)
{
	int i;
	for(i=0; i<3; i++)
	{
		this->vals[i] = source.vals[i];
	}

	return(*this);
}

std::ostream & operator<<(std::ostream &os, const Point& pt)
{
    return os << "( " << pt.vals[0] << " , " << pt.vals[1] << " )";
}

Point::~Point() {
	// TODO Auto-generated destructor stub
}

