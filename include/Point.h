/*
 * Point.h
 *
 *  Created on: 22 Dec 2014
 *      Author: phillipbentley
 */

#include<iostream>

#ifndef POINT_H_
#define POINT_H_

class Point {
public:
	Point();
	Point(double xinit, double yinit);
	virtual ~Point();

	Point& operator=(const Point& source);

	void set(double xset, double yset);

	double getx(void) const;
	double gety(void) const;

private:
	double vals[3];

	friend class Matrix;
    friend std::ostream & operator<<(std::ostream &os, const Point& pt);

};

#endif /* POINT_H_ */
