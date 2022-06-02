/*
 * Block.h
 *
 *  Created on: 22 Dec 2014
 *      Author: phillipbentley
 */



#ifndef MASTEROFPUPPETS_BLOCK_H_
#define MASTEROFPUPPETS_BLOCK_H_

#include "Point.h"
#include "Matrix.h"

class Block {
public:
	Block();
	Block(Point newpts[4]);
	virtual ~Block();

	Block& operator=(const Block& source);

	void setBlock(Point newpts[4]);

	//bool checkInterference(const Point& pt) const;  //bounding box - quick and dirty
	bool checkInterference(const Block& bk);	//polygon intersection using boost.

	//bool checkInterferenceManually(const Block& bk);	//proper line intersection algorithm, supposed to be more exact

	double measureDistance(const Block& bk);		//using my own fast method
	double measureDistanceBoost(const Block& bk);	//using boost::distance - has compile bugs right now, wait for boost to fix, probably faster method


	const Point& getPoint(int i) const;

	void deploy(const Matrix& mat);
	void deployInPlace(void);

	bool hasCollisions(void);
	void resetCollisionFlag(void);
	void setCollisionFlag(bool);

	void drawPovray(std::ofstream& pov, std::string& color, double height);  

	//void untick(void);		//avoid repetition
	//void tick(void);		//avoid repetition

	double centreX(); //returns the x value in the middle of the block
	double centreY(); //returns the y value in the middle of the block

private:
	Point pts[4];

	Point worldPts[4];

//	bool doIntersect(const Point& p1, const Point& q1, const Point& p2, const Point& q2) const;
//	int orientation(const Point& p, const Point& q, const Point& r) const;
//	bool onSegment(const Point& p, const Point& q, const Point& r) const;

//	double xmin;
//	double ymin;
//	double xmax;
//	double ymax;

	bool hasCollision;

	bool checked;

	friend std::ostream & operator<<(std::ostream &os, const Block& bk);


};

#endif /* MASTEROFPUPPETS_BLOCK_H_ */
