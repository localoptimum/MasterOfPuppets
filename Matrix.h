/*
 * Matrix.h
 *
 *  Created on: 22 Dec 2014
 *      Author: phillipbentley
 */

#ifndef MASTEROFPUPPETS_MATRIX_H_
#define MASTEROFPUPPETS_MATRIX_H_

#include <iostream>
#include "Point.h"

class Matrix {
public:
	Matrix();
	Matrix(const double nvals[3][3]);

	virtual ~Matrix();

	Matrix& operator=(const Matrix& src);
	Matrix operator*(const Matrix& mat);
	Point operator*(const Point& pt) const;

	void translate(const Point& pt);
	void rotate(double angle);			//origin is going to track current position

private:
	double vals[3][3];

    friend std::ostream & operator<<(std::ostream &os, const Matrix& mat);

};

#endif /* MASTEROFPUPPETS_MATRIX_H_ */
