/*
 * Matrix.cpp
 *
 *  Created on: 22 Dec 2014
 *      Author: phillipbentley
 */
#include <cmath>
#include "Matrix.h"
#include "Point.h"

Matrix::Matrix() {

	//Initialise unit matrix;
	int i,j;

	for(i=0; i<3; i++)
		for(j=0; j<3; j++)
		{
			vals[i][j] = 0.0;
			if(i==j)
				vals[i][j]=1.0;
		}
}

Matrix::Matrix(const double nvals[3][3])
{
	int i,j;

	for(i=0; i<3; i++)
	{
		for(j=0; j<3; j++)
		{
			vals[i][j] = nvals[i][j];
		}
	}
}

Matrix::~Matrix() {
	// TODO Auto-generated destructor stub
}

Matrix& Matrix::operator=(const Matrix& src)
{
	int i,j;

	for(i=0; i<3; i++)
	{
		for(j=0; j<3; j++)
			this->vals[i][j] = src.vals[i][j];
	}

	return(*this);
}

Matrix Matrix::operator*(const Matrix& mat)
{
	//Computes MAT*THIS matrix product
	int row,col,j;

	Matrix ans;

	for(row=0; row < 3; row++)
	{
		for(col=0;col<3;col++)
		{
		            ans.vals[row][col] = 0.0;

		            for(j=0;j<3;j++)
		            {
		                ans.vals[row][col]+= this->vals[row][j] * mat.vals[j][col];
		            }
		}
	}

	return(ans);
}

Point Matrix::operator*(const Point& pt) const
{
	//computes mat*pt
	int row, col;
	Point ans;

	for(row=0; row<3; row++)
		{
			ans.vals[row] = 0.0;
		for(col=0; col<3; col++)
		{
			ans.vals[row] += this-> vals[row][col] * pt.vals[col];
		}
		}

	return(ans);
}

void Matrix::translate(const Point& trans)
{
	Matrix mult;	//start as unit matrix
	mult.vals[0][2] = trans.vals[0];
	mult.vals[1][2] = trans.vals[1];

	*this = *this * mult;
}

void Matrix::rotate(const double ang)
{
	//Angle in degrees, apply rotation to the matrix;
	Matrix mult; //start as unit matrix
	mult.vals[0][0] = cos(ang*M_PI/180.0);
	mult.vals[0][1] = sin(ang*M_PI/180.0);
	mult.vals[1][0] = -mult.vals[0][1];
	mult.vals[1][1] = mult.vals[0][0];

	*this = *this * mult;
}


std::ostream & operator<<(std::ostream &os, const Matrix& mat)
{
    return os << mat.vals[0] << " " << mat.vals[1] << std::endl << mat.vals[2] << " " << mat.vals[3];
}
