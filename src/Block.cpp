/*
 * Block.cpp
 *
 *  Created on: 22 Dec 2014
 *      Author: phillipbentley
 */

#include "Block.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

#include <cmath>
#include <deque>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#include <boost/foreach.hpp>

using namespace std;

Block::Block() {

	int i;
	for(i=0; i<4; i++)
	{
		pts[i].set(0.0, 0.0);
	}

	hasCollision = false;
	checked = false;

//	xmin=0.0;
//	ymin=0.0;
//	xmax=0.0;
//	ymax=0.0;
}

Block::Block(Point newpts[4])
{
	hasCollision = false;
	checked = false;

	this -> setBlock(newpts);
}

void Block::setBlock(Point newpts[4])
{
	int i;

	for(i=0; i<4; i++)
	{
		this->pts[i] = newpts[i];

//		if(pts[i].getx() < xmin)
//			xmin = pts[i].getx();
//
//		if(pts[i].getx() > xmax)
//			xmax = pts[i].getx();
//
//		if(pts[i].gety() < ymin)
//			ymin = pts[i].gety();
//
//		if(pts[i].gety() > ymax)
//			ymax = pts[i].gety();
	}

	hasCollision = false;
	checked = false;
}

Block& Block::operator=(const Block& source)
{
	int i;

	for(i=0; i<4; i++)
	{
		this->pts[i] = source.pts[i];
		this->worldPts[i] = source.worldPts[i];
	}
//	this->ymin = source.ymin;
//	this->ymax = source.ymax;
//	this->xmin = source.xmin;
//	this->xmax = source.xmax;

	this->hasCollision = source.hasCollision;
	this->checked = source.checked;

	return(*this);
}

Block::~Block() {
	// TODO Auto-generated destructor stub
}

void Block::deploy(const Matrix& mat)	//Sends the block to the world coordinate system using transform matrix
{
	int i;

	for(i=0; i<4; i++)
	{
		worldPts[i] = mat * pts[i];
	}

	//For debug only
	//std::cout << *this << std::endl;
}




void Block::deployInPlace(void)	//Sends the block to the world coordinate system assuming the local coordinates are world coordinates
{
	int i;

	for(i=0; i<4; i++)
	{
		worldPts[i] = pts[i];
	}

	//For debug only
	//std::cout << *this << std::endl;
}




//bool Block::checkInterference(const Point& pt) const
//{
//	//Simple bounding box collision.  This is probably accurate enough for our purposes
//
//	if( (pt.getx() > xmin) && (pt.getx() < xmax))
//		if( (pt.gety() > ymin) && (pt.gety() < ymax) )
//			return(true);
//
//	return(false);
//}


// The code below is taken from GeeksforGeeks
// http://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/

// Given three colinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
//bool Block::onSegment(const Point& p, const Point& q, const Point& r) const
//{
//    if (q.getx() <= fmax(p.getx(), r.getx()) && q.getx() >= fmin(p.getx(), r.getx()) &&
//        q.gety() <= fmax(p.gety(), r.gety()) && q.gety() >= fmin(p.gety(), r.gety()))
//       return true;
//
//    return false;
//}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise

//int Block::orientation(const Point& p, const Point& q, const Point& r) const
//{
//    // See 10th slides from following link for derivation of the formula
//    // http://www.dcs.gla.ac.uk/~pat/52233/slides/Geometry1x1.pdf
//    int val = (q.gety() - p.gety()) * (r.getx() - q.getx()) -
//              (q.getx() - p.getx()) * (r.gety() - q.gety());
//
//    if (val == 0) return 0;  // colinear
//
//    return (val > 0)? 1: 2; // clock or counterclock wise
//}

// The main function that returns true if line segment 'p1q1'
// and 'p2q2' intersect.

//bool Block::doIntersect(const Point& p1, const Point& q1, const Point& p2, const Point& q2) const
//{
//    // Find the four orientations needed for general and
//    // special cases
//    int o1 = orientation(p1, q1, p2);
//    int o2 = orientation(p1, q1, q2);
//    int o3 = orientation(p2, q2, p1);
//    int o4 = orientation(p2, q2, q1);
//
//    // General case
//    if (o1 != o2 && o3 != o4)
//        return true;
//
//    // Special Cases
//    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
//    if (o1 == 0 && onSegment(p1, p2, q1)) return true;
//
//    // p1, q1 and p2 are colinear and q2 lies on segment p1q1
//    if (o2 == 0 && onSegment(p1, q2, q1)) return true;
//
//    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
//    if (o3 == 0 && onSegment(p2, p1, q2)) return true;
//
//     // p2, q2 and q1 are colinear and q1 lies on segment p2q2
//    if (o4 == 0 && onSegment(p2, q1, q2)) return true;
//
//    return false; // Doesn't fall in any of the above cases
//}



void Block::drawPovray(std::ofstream& pov, std::string& colour, double height)
{
  int i;
  

    pov << "prism { ";
    pov << " linear_sweep ";
    pov << " linear_spline ";
    pov << " 0, "; // sweep the following shape from here ...
    pov << " " << height << ","; // ... up through here
    pov << "4,"; // the number of points making up the shape ...
    for(i=0; i<4; i++)
      {
	pov << "<" << worldPts[i].getx() << ", " << worldPts[i].gety() << ">";
	if(i!=3)
	  pov << ",";
      }
    pov << "pigment { " << colour << " }";
    pov << "translate <0,-" << height/2.0 << ",0> ";
    pov << "}" << endl;
}



bool Block::checkInterference(const Block& otherblock)
{
	//Using boost geometry

	int i;

	bool result=false;

	typedef boost::geometry::model::d2::point_xy<double> boostpoint;
	typedef boost::geometry::model::polygon<boostpoint> boostpoly;

	using boost::geometry::append;
	using boost::geometry::make;
	using boost::geometry::correct;

	boostpoly mypoly;
	boostpoly testpoly;
	std::deque<boostpoly> output; // discarded for now, maybe useful for visualisation later


	//Make a boost polygon out of my block points
	for(i=0; i<4; i++)
	{
		append( mypoly, make<boostpoint>(this->worldPts[i].getx(), this->worldPts[i].gety()) );
	}

	//Boost polygons have to form a complete loop, add the first point again
	append( mypoly, make<boostpoint>(this->worldPts[0].getx(), this->worldPts[0].gety()) );


	//Make a boost polygon out of the test block points
	for(i=0; i<4; i++)
	{
		append( testpoly, make<boostpoint>(otherblock.worldPts[i].getx(), otherblock.worldPts[i].gety()) );
	}

	//Like above, form a complete loop with first point
	append( testpoly, make<boostpoint>(otherblock.worldPts[0].getx(), otherblock.worldPts[0].gety()) );

	correct(testpoly);
	correct(mypoly);

	//Use boost to check if the rectangles overlap
	result = boost::geometry::intersects(mypoly, testpoly);

	return(result);
}






double Block::measureDistance(const Block& otherblock)
{
	int i;
	int j;

	double minsqr = 100000.0;

	double dx;
	double dy;
	double dsqr;


	//Measures the nearest point separation very quickly.  Does not check for overlaps!  That is a separate function
	//Works in square distance, so sqrt only has to be performed once at the end.

	for(i=0; i<4; i++)
	{

		for(j=0; j<4; j++)
		{

			//Get the square distance between the points, and find the smallest one

			dx = this->worldPts[i].getx() - otherblock.worldPts[j].getx();
			dy = this->worldPts[i].gety() - otherblock.worldPts[j].gety();


			dsqr = dx*dx + dy*dy;

			if (dsqr < minsqr)
				minsqr = dsqr;
		}
	}


	return(sqrt(minsqr));
}






double Block::measureDistanceBoost(const Block& otherblock)
{
	//Using boost geometry

	int i;

	double result=0.0;

	typedef boost::geometry::model::d2::point_xy<double> boostpoint;
	typedef boost::geometry::model::polygon<boostpoint> boostpoly;

	using boost::geometry::append;
	using boost::geometry::make;
	using boost::geometry::correct;

	boostpoly mypoly;
	boostpoly testpoly;
	std::deque<boostpoly> output; // discarded for now, maybe useful for visualisation later


	//Make a boost polygon out of my block points
	for(i=0; i<4; i++)
	{
		append( mypoly, make<boostpoint>(this->worldPts[i].getx(), this->worldPts[i].gety()) );
	}

	//Boost polygons have to form a complete loop, add the first point again
	append( mypoly, make<boostpoint>(this->worldPts[0].getx(), this->worldPts[0].gety()) );


	//Make a boost polygon out of the test block points
	for(i=0; i<4; i++)
	{
		append( testpoly, make<boostpoint>(otherblock.worldPts[i].getx(), otherblock.worldPts[i].gety()) );
	}

	//Like above, form a complete loop with first point
	append( testpoly, make<boostpoint>(otherblock.worldPts[0].getx(), otherblock.worldPts[0].gety()) );

	correct(testpoly);
	correct(mypoly);

	//Use boost::geometry::distance to measure the distance between the blocks
	result = boost::geometry::distance(mypoly, testpoly);

	return(result);
}






//
//
//bool Block::checkInterferenceManually(const Block& bk)
//{
//	bool check = false;
//
//	//Left edge with my left edge
//	check = doIntersect(this-> worldPts[0], this-> worldPts[3], bk.worldPts[0], bk.worldPts[3]);
//	if(check)
//	{
//		this-> hasCollision = true;
//		return(true);
//	}
//
//	//Right edge with my left edge
//	check = doIntersect(this-> worldPts[0], this-> worldPts[3], bk.worldPts[1], bk.worldPts[2]);
//	if(check)
//	{
//		this-> hasCollision = true;
//		return(true);
//	}
//
//	//Left edge with my right edge
//	check = doIntersect(this-> worldPts[1], this-> worldPts[2], bk.worldPts[0], bk.worldPts[3]);
//	if(check)
//	{
//		this-> hasCollision = true;
//		return(true);
//	}
//
//	//Right edge with my right edge
//	check = doIntersect(this-> worldPts[1], this-> worldPts[2], bk.worldPts[1], bk.worldPts[2]);
//	if(check)
//	{
//		this-> hasCollision = true;
//		return(true);
//	}
//
//	//Left edge with my bottom edge
//	check = doIntersect(this-> worldPts[0], this-> worldPts[1], bk.worldPts[0], bk.worldPts[3]);
//	if(check)
//	{
//		this-> hasCollision = true;
//		return(true);
//	}
//
//	//Right edge with my bottom edge
//	check = doIntersect(this-> worldPts[0], this-> worldPts[1], bk.worldPts[1], bk.worldPts[2]);
//	if(check)
//	{
//		this-> hasCollision = true;
//		return(true);
//	}
//
//	//Left edge with my top edge
//	check = doIntersect(this-> worldPts[2], this-> worldPts[3], bk.worldPts[0], bk.worldPts[3]);
//	if(check)
//	{
//		this-> hasCollision = true;
//		return(true);
//	}
//
//	//Right edge with my top edge
//	check = doIntersect(this-> worldPts[2], this-> worldPts[3], bk.worldPts[1], bk.worldPts[2]);
//	if(check)
//	{
//		this-> hasCollision = true;
//		return(true);
//	}
//
//	return(false);
//}


const Point& Block::getPoint(int i) const
{
	return(worldPts[i]);
}

double Block::centreX(void)
{
  int i;
  double ans;

  ans = worldPts[0].getx();

  for(i=1; i<4; i++)
    {
      ans = ans + worldPts[i].getx();
    }

  ans = ans / 4.0;
  return(ans);
}

double Block::centreY(void)
{
int i;
  double ans;

  ans = worldPts[0].gety();

  for(i=1; i<4; i++)
    {
      ans = ans + worldPts[i].gety();
    }

  ans = ans / 4.0;
  return(ans);

}

std::ostream & operator<<(std::ostream &os, const Block& bk)
{
	int i;

	os << "Block:" << std::endl;

	os << std::setprecision(3);
	
	for(i=0; i<4; i++)
	{

		os << "  Point [" << i << "] " << bk.worldPts[i] << std::endl;
	}
    return os;
}

bool Block::hasCollisions(void)
{
	return(hasCollision);
}

void Block::resetCollisionFlag(void)
{
	hasCollision=false;
}

void Block::setCollisionFlag(bool flag)
{
	hasCollision=flag;
}
