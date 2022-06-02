/*
 * Instrument.h
 *
 *  Created on: 3 Dec 2014
 *      Author: phillipbentley
 */

#include "Matrix.h"
#include "Block.h"

#ifndef INSTRUMENT_H_
#define INSTRUMENT_H_

#include <fstream>


class Instrument {
public:
	Instrument();
	Instrument(int id);

	virtual ~Instrument();

	int id;

	int tranche;

	void set_num_choppers(int num);
	int get_num_choppers(void);
	int get_num_guideBlocks(void);
	void set_chopper_rad(int num, double rad);
	void set_chopper_min(int num, double min);
	void set_chopper_max(int num, double min);
	double get_chopper_min(int num);
	double get_chopper_max(int num);
	double get_chopper_rad(int num);
	double get_chopper_x_offset(int num);   //cartesian coordinates +xoffset shifts the chopper to the right
											//note that the rotation direction is the opposite, positive rotation moves it to the left
	void set_beam_centre(double set);
	double get_beam_centre(void);

	//Beam extraction options
	void bispectralExtraction(void);
	void coldExtraction(void);
	void thermalExtraction(void);

	//Inverse extraction options for heimdal
	void invBispectralExtraction(void);
	void invColdExtraction(void);
	void invThermalExtraction(void);

	

	void setCurvatureDirection(int curv);

	Instrument& operator=(const Instrument &src);

	void addChopper(double radiaDistance, double upstreamDiskClearance, double downstreamDiskClearance, double totalHousingWidth, double xOffset); // old method
	void addChopper(double pitWidth, double pitLength, double xOffset);
	void addGuideBlock(double width, double length);
	void addCurvedGuide(double width, double length, double radius, int pieces);
	void addStraightGuide(double width, double length, int pieces);

	void setC1shift(const double shift);
	double getC1shift(void);
	
	bool checkChopperInterference(Block& bk);
	bool checkGuideInterference(Block& bk);
	double getMinimumSeparation(Block& bk);

	void resetCoordinates(void);
	void resetPort(void);
	void rotateToPort(void);
	void rotateFrame(const double ang);
	void translateFrame(const Point& pt);
	void terminate(void);


	void declareEukleides(std::ofstream& os);
	void drawEukleides(std::ofstream& os);
	void drawPovray(std::ofstream& os);

	int backgroundSensitivityRating(void);
	int noisinessRating(void);

	void setBackgroundSensitivity(int setVal);
	void setNoisiness(int setVal);

private:
	int num_choppers;
	int num_guideBlocks;

	bool deployed;

	int backgroundSensitivity;
	int noisiness;


	int curvatureDirection;

	double chopper_rad[10];
	double chopper_min[10];
	double chopper_max[10];
	double chopper_x_offset[10];

	double c1shift;


	Block cblock[10];
	Block gblock[100];

	Matrix mat;

	Point terminalPoint;

	double beamCentre;

	friend class Sector;
};

#endif /* INSTRUMENT_H_ */
