/*
 * Instrument.cpp
 *
 *  Created on: 3 Dec 2014
 *      Author: phillipbentley
 */

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

#include "Instrument.h"
#include "globals.h"

Instrument::Instrument() {
	int i;

	//Matrix is automatically assigned to unity

	num_choppers = 0;
	num_guideBlocks =0;
	beamCentre = 0.0;
	backgroundSensitivity = 0;
	noisiness = 0;

	deployed = false;

	terminalPoint = Point(0.0, 0.0);

	curvatureDirection = CURVE_LEFT;

	id = 0;

	tranche = 0;

	for(i=0; i<10 ;i++)
	{
		chopper_min[i] = 20.0;
		chopper_max[i] = 20.0;
		chopper_rad[i] = 0.0;
		chopper_x_offset[i] = 0.0;
	}
}


Instrument::Instrument(int create_id) {
	int i;

	num_choppers = 0;
	num_guideBlocks = 0;
	beamCentre = 0.0;
	backgroundSensitivity = 0;
	noisiness = 0;

	deployed = false;

	curvatureDirection = CURVE_LEFT;

	id = create_id;

	terminalPoint = Point(0.0, 0.0);

	tranche = 0;

	for(i=0; i<10 ;i++)
	{
		chopper_min[i] = 20.0;
		chopper_max[i] = 20.0;
		chopper_rad[i] = 0.0;
		chopper_x_offset[i] = 0.0;
	}
}


Instrument::~Instrument() {
	// TODO Auto-generated destructor stub
}



void Instrument::set_num_choppers(int num)
{
	num_choppers = num;
}



int Instrument::get_num_choppers()
{
	return(num_choppers );
}

int Instrument::get_num_guideBlocks()
{
	return(num_guideBlocks);
}


double Instrument::get_beam_centre()
{
	return(beamCentre);
}


void Instrument::set_beam_centre(double set)
{
	beamCentre = set;

}

void Instrument::setCurvatureDirection(int set)
{
	curvatureDirection = set;
}


void Instrument::rotateFrame(const double ang)
{
	mat.rotate(ang);
}

void Instrument::translateFrame(const Point& pt)
{
	mat.translate(pt);
}

void Instrument::bispectralExtraction(void)
{
  //tan theta = theta in radians in small angle limit
  const double angle=(4.0/400.0)*(180.0/M_PI);
  
  if(sectorToStudy == WEST_SECTOR || sectorToStudy == EAST_SECTOR)
    {
      //Translate 4 cm to thermal and rotate to left
      mat.translate(Point(0.04,0));
      mat.rotate(-angle);
    }
  else
    {
      //Translate 4 cm to thermal and rotate to left
      mat.translate(Point(-0.04,0));
      mat.rotate(angle);
    }
}

void Instrument::invBispectralExtraction(void)
{
  //tan theta = theta in radians in small angle limit
  const double angle=(4.0/400.0)*(180.0/M_PI);
  
  if(sectorToStudy == WEST_SECTOR || sectorToStudy == EAST_SECTOR)
    {
      //Translate 4 cm to thermal and rotate to left
      mat.translate(Point(-0.04,0));
      mat.rotate(angle);
    }
  else
    {
      //Translate 4 cm to thermal and rotate to left
      mat.translate(Point(0.04,0));
      mat.rotate(-angle);
    }
}

void Instrument::thermalExtraction(void)
{
  bispectralExtraction();
}

void Instrument::invThermalExtraction(void)
{
  invBispectralExtraction();
}

void Instrument::coldExtraction(void)
{
  //arctan = theta in radians in small angle limit
  const double angle=(4.0/400.0)*(180.0/M_PI);;
  
  if(sectorToStudy == WEST_SECTOR || sectorToStudy == EAST_SECTOR)
    {
      //Translate 4 cm to thermal and rotate to left
      mat.translate(Point(-0.04,0));
      mat.rotate(angle);
    }
  else
    {
      //Translate 4 cm to thermal and rotate to left
      mat.translate(Point(0.04,0));
      mat.rotate(-angle);
    }
}

void Instrument::invColdExtraction(void)
{
  //arctan = theta in radians in small angle limit
  const double angle=(4.0/400.0)*(180.0/M_PI);;
  
  if(sectorToStudy == WEST_SECTOR || sectorToStudy == EAST_SECTOR)
    {
      //Translate 4 cm to thermal and rotate to left
      mat.translate(Point(0.04,0));
      mat.rotate(-angle);
    }
  else
    {
      //Translate 4 cm to thermal and rotate to left
      mat.translate(Point(-0.04,0));
      mat.rotate(angle);
    }
}

void Instrument::resetCoordinates(void)
{
  //Full reset to initialise
  mat = unitMatrix;
  num_guideBlocks = 0;
  num_choppers = 0;
  deployed = false;
}

void Instrument::resetPort(void)
{
  //Soft reset to deploy second guide (heimdal!)
  mat = unitMatrix;
  //rotateToPort();
}

void Instrument::rotateToPort(void)
{
  rotateFrame(beamCentre);
}

void Instrument::terminate(void)
{
  terminalPoint = mat*Point(0.0, 0.0);
  deployed = true;
}





void Instrument::set_chopper_rad(int num, double rad)
{
	if(num < 10)
	{
		chopper_rad[num] = rad;
	}
	else
	{
		std::cerr << "Attempt to set parameters for chopper beyond #10, number passed " << num << std::endl;
		return;
	}
}

void Instrument::set_chopper_min(int num, double min)
{
	if(num < 10)
	{
		chopper_min[num] = min;
	}
	else
	{
		std::cerr << "Attempt to set chopper min for chopper beyond #10, number passed " << num << std::endl;
		return;
	}
}

void Instrument::set_chopper_max(int num, double max)
{
	if(num < 10)
	{
		chopper_max[num] = max;
	}
	else
	{
		std::cerr << "Attempt to set chopper max for chopper beyond #10, number passed " << num << std::endl;
		return;
	}
}




double Instrument::get_chopper_min(int num)
{
	if(num < 10)
	{
		return(chopper_min[num]);
	}
	else
	{
		std::cerr << "Attempt to get chopper min for chopper beyond #10, number passed " << num << std::endl;
		return(0.0);
	}
}

double Instrument::get_chopper_max(int num)
{
	if(num < 10)
	{
		return(chopper_max[num]);
	}
	else
	{
		std::cerr << "Attempt to get chopper max for chopper beyond #10, number passed " << num << std::endl;
		return(0.0);
	}
}


double Instrument::get_chopper_rad(int num)
{
	if(num < 10)
	{
		return(chopper_rad[num]);
	}
	else
	{
		std::cerr << "Attempt to get chopper rad for chopper beyond #10, number passed " << num << std::endl;
		return(0.0);
	}
}

double Instrument::get_chopper_x_offset(int num)
{
	if(num < 10)
	{
		return(chopper_x_offset[num]);
	}
	else
	{
		std::cerr << "Attempt to get chopper x offset for chopper beyond #10, number passed " << num << std::endl;
		return(0.0);
	}
}


void Instrument::addChopper(double radialDistance, double upstreamDiskClearance, double downstreamDiskClearance, double totalHousingWidth, double xOffset)
{
	Point pts[4];

	//Old code
	chopper_rad[num_choppers] = totalHousingWidth/2.0;
	chopper_min[num_choppers] = radialDistance-upstreamDiskClearance;
	chopper_max[num_choppers] = radialDistance+downstreamDiskClearance;
	chopper_x_offset[num_choppers] = xOffset;

	//object oriented code
	pts[0].set(-chopper_rad[num_choppers],chopper_min[num_choppers]);
	pts[1].set(chopper_rad[num_choppers],chopper_min[num_choppers]);
	pts[2].set(chopper_rad[num_choppers],chopper_max[num_choppers]);
	pts[3].set(-chopper_rad[num_choppers],chopper_max[num_choppers]);

	cblock[num_choppers].setBlock(pts);
	cblock[num_choppers].deploy(mat);

	num_choppers++;
}




void Instrument::addChopper(double pitWidth, double pitLength, double xOffset)
{
	Point pts[4];

	//Old code
//	chopper_rad[num_choppers] = totalHousingWidth/2.0;
//	chopper_min[num_choppers] = radialDistance-upstreamDiskClearance;
//	chopper_max[num_choppers] = radialDistance+downstreamDiskClearance;
//	chopper_x_offset[num_choppers] = xOffset;

	//object oriented code
	pts[0].set(-pitWidth/2.0 + xOffset, 0.0);
	pts[1].set(pitWidth/2.0 + xOffset, 0.0);
	pts[2].set(pitWidth/2.0 + xOffset, pitLength);
	pts[3].set(-pitWidth/2.0 + xOffset, pitLength);

	cblock[num_choppers].setBlock(pts);
	cblock[num_choppers].deploy(mat);
	cblock[num_choppers].resetCollisionFlag();

	//Translate the matrix so next block is in the right place
	translateFrame(Point(xOffset, pitLength));

	num_choppers++;
}


void Instrument::addGuideBlock(double width, double length)
{
	Point pts[4];

	pts[0].set(-width/2.0, 0.0);
	pts[1].set(width/2.0, 0.0);

	pts[2].set(width/2.0, length);
	pts[3].set(-width/2.0, length);

	gblock[num_guideBlocks].setBlock(pts);
	gblock[num_guideBlocks].deploy(mat);
	gblock[num_guideBlocks].resetCollisionFlag();

	//Translate the matrix so next block is in the right place
	translateFrame(Point(0.0, length));

	num_guideBlocks++;
}


void Instrument::addCurvedGuide(double width, double length, double radius, int pieces)
{
	int i;

	double pieceLength = length / (double) pieces;
	double angle = length / radius;	//in radians
	angle = angle / (double) pieces;

	if(curvatureDirection == CURVE_LEFT)
	{
		angle = -angle;
	}

	angle = angle*180.0/M_PI;

	for(i=0; i<pieces; i++)
	{
		addGuideBlock(width, pieceLength);
		rotateFrame(angle);
	}

}


void Instrument::addStraightGuide(double width, double length, int pieces)
{
	int i;

	double pieceLength = length / (double) pieces;

	for(i=0; i<pieces; i++)
	{
		addGuideBlock(width, pieceLength);
	}

}



Instrument& Instrument::operator=(const Instrument& src)
{
	int i;

	this->num_choppers = src.num_choppers;
	this->num_guideBlocks = src.num_guideBlocks;
	this->beamCentre = src.beamCentre;
	this->tranche = src.tranche;

	this->id = src.id;

	this->deployed = src.deployed;

	this->terminalPoint = src.terminalPoint;

	this->curvatureDirection = src.curvatureDirection;

	for(i=0; i<10; i++)
	{
		this->chopper_rad[i] = src.chopper_rad[i];
		this->chopper_min[i] = src.chopper_min[i];
		this->chopper_max[i] = src.chopper_max[i];
		this->chopper_x_offset[i] = src.chopper_x_offset[i];
	}

	for(i=0; i<this->num_choppers; i++)
	{
		this->cblock[i] = src.cblock[i];
	}

	for(i=0; i<this->num_guideBlocks; i++)
	{
		this->gblock[i] = src.gblock[i];
	}

	this->mat = src.mat;

	this->c1shift = src.c1shift;

	this->backgroundSensitivity = src.backgroundSensitivity;
	this->noisiness = src.noisiness;

	return(*this);
}

int Instrument::backgroundSensitivityRating(void)
{
	return(backgroundSensitivity);
}

int Instrument::noisinessRating(void)
{
	return(noisiness);
}

void Instrument::setBackgroundSensitivity(int setVal)
{
	backgroundSensitivity = setVal;
}

void Instrument::setNoisiness(int setVal)
{
	noisiness = setVal;
}

void Instrument::setC1shift(const double shift)
{
  c1shift = shift;
}

double Instrument::getC1shift(void)
{
  return(c1shift);
}

bool Instrument::checkChopperInterference(Block& bk)
{
	int i;

	for(i=0; i< num_choppers; i++)
	{
		if(bk.checkInterference(cblock[i]))
		{
			cblock[i].setCollisionFlag(true);
			bk.setCollisionFlag(true);
			return(true);
		}
	}

	return(false);
}


bool Instrument::checkGuideInterference(Block& bk)
{
	int i;

	bool ret = false;

	for(i=0; i< num_guideBlocks; i++)
	{
		if(bk.checkInterference(gblock[i]))
		{
			gblock[i].setCollisionFlag(true);
			bk.setCollisionFlag(true);
			ret = true;
		}
	}

	return(ret);
}



double Instrument::getMinimumSeparation(Block& bk)
{
	double result=10.0;
	double sep=0.0;

	int i;

	for(i=0; i< num_choppers; i++)
	{
		sep = bk.measureDistanceBoost(cblock[i]);

		if(sep < result)
			result = sep;
	}

	for(i=0; i<num_guideBlocks; i++)
	{
		sep = bk.measureDistanceBoost(gblock[i]);

		if(sep < result)
			result = sep;
	}

	return(result);
}


void Instrument::declareEukleides(std::ofstream& euk)
{
	int g, p;

	int c;

	euk << std::setprecision(3);
	euk.setf(std::ios_base::fixed);

	for(g=0; g < num_guideBlocks; g++)
	{
		for(p=0; p<4; p++)
			euk << "i" << id << "gb" << g << "p" << p << " = point(" << gblock[g].getPoint(p).getx() << "," << gblock[g].getPoint(p).gety() << ")" << std::endl;

		euk << "i" << id << "gb" << g << "=" << "i" << id << "gb" << g << "p0." << "i" << id << "gb" << g << "p1." << "i" << id << "gb" << g << "p2."<< "i" << id << "gb" << g << "p3."<< "i" << id << "gb" << g << "p0" << std::endl;
	}

	if(genericInstruments == true)
	{
		euk << codeNames[id] << " = point(" << terminalPoint.getx() << ", " << terminalPoint.gety() << ")" << std::endl;

		euk << "label" << std::endl;
		euk << "  " << codeNames[id] << " 0 deg" << std::endl;
		euk << "end" << std::endl;
	}
	else
	{
		euk << instNames[id] << " = point(" << terminalPoint.getx() << ", " << terminalPoint.gety() << ")" << std::endl;

		euk << "label" << std::endl;
		euk << "  " << instNames[id] << " 0 deg" << std::endl;
		euk << "end" << std::endl;
	}

	for(c=0; c < num_choppers; c++)
		{
			for(p=0; p<4; p++)
				euk << "i" << id << "c" << c << "p" << p << " = point(" << cblock[c].getPoint(p).getx() << "," << cblock[c].getPoint(p).gety() << ")" << std::endl;

			euk << "i" << id << "c" << c << "=" << "i" << id << "c" << c << "p0." << "i" << id << "c" << c << "p1." << "i" << id << "c" << c << "p2."<< "i" << id << "c" << c << "p3."<< "i" << id << "c" << c << "p0" << std::endl;
		}

}


void Instrument::drawEukleides(std::ofstream& euk)
{
	int c, g;
	std::string linestyle;

	std::string colour;
	double sectorArc;

	switch(tranche)
	{
	case 1 :
		colour = "green";
		break;
	case 2 :
		colour = "blue";
		break;
	case 3 :
		colour = "black";
		break;
	default :
		colour = "gray";
		break;
	}

	switch(sectorToStudy)
	{
	case WEST_SECTOR:
		sectorArc = 43.0;
		break;
	default:
		sectorArc = 60.0;
		break;
	}

	std::cout << "Sector arc: " << sectorArc << std::endl;

	if(deployed ==false)
	{
		std::cerr << "Error: undeployed instrument " << instNames[id] << " being drawn." << std::endl;
	}

	if(beamCentre > sectorArc)
	{
		linestyle = " dashed, ";
	}
	else
	{
		linestyle = " ";
	}

	for(g=0; g < num_guideBlocks; g++)
	{
		if(!gblock[g].hasCollisions())
			euk << "  i" << id << "gb" << g << linestyle << colour << std::endl;
		else
			euk << "  i" << id << "gb" << g << linestyle << "red" << std::endl;
	}

	for(c=0; c < num_choppers; c++)
		{
		if(!cblock[c].hasCollisions())
			euk << "  i" << id << "c" << c << linestyle << colour << std::endl;
		else
			euk << "  i" << id << "c" << c << linestyle << "red" << std::endl;
		}

	if(genericInstruments == true)
		{
			euk << codeNames[id] << " disc" << std::endl;
		}
		else
		{
			euk << instNames[id] << " disc" << std::endl;
		}


}






void Instrument::drawPovray(std::ofstream& pov)
{
  int c, g, i;
  double tposx, tposy;
  double height=1;
	// std::string linestyle;

	std::string colour;
	double sectorArc;

	std::string red;



	switch(sectorToStudy)
	{
	case WEST_SECTOR:
		sectorArc = 43.0;
		break;
	default:
		sectorArc = 60.0;
		break;
	}


	red = "Red";

	colour = "rgb <0.1, 0.1, 0.1>";


	//Make guides semi-transparent if they are not within the sector
	if(beamCentre > sectorArc)
	{
	  red = "rgbt <1.0, 0.0, 0.0, 0.9> ";
	  colour = "rgbt <0.1, 0.1, 0.1, 0.9>";
	  
	}
	  

	
	height = 0.2;
	
	for(g=0; g < num_guideBlocks; g++)
	{
		if(!gblock[g].hasCollisions())
		  gblock[g].drawPovray(pov, colour,height);
		else
		  gblock[g].drawPovray(pov, red, height);
	}


	tposx = gblock[num_guideBlocks-1].centreX();
	tposy = gblock[num_guideBlocks-1].centreY();

	pov << " text { ttf \"timrom.ttf\" \"" << instNames[id] << "\" 0.1, 0    pigment { " << red << "}";
	//pov << "translate <-4,0,0> ";
	pov << "rotate <90, 0, 0> ";
	pov << "translate <" << tposx << ", 3, " << tposy <<"> no_shadow";
	pov << "}" << std::endl;


	
	colour = "rgb <0.5, 0.5, 0.5>";


	height = 1.5;
	for(g=0; g < num_choppers; g++)
	{
		if(!cblock[g].hasCollisions())
		  cblock[g].drawPovray(pov, colour, height);
		else
		  cblock[g].drawPovray(pov, red, height);
	}


	tposx = cblock[num_choppers-1].centreX();
	tposy = cblock[num_choppers-1].centreY();

	pov << " text { ttf \"timrom.ttf\" \"" << instNames[id] << "\" 0.1, 0    pigment { " << red << "}";
	//pov << "translate <-4,0,0> ";
	pov << "rotate <90, 0, 0> ";
	pov << "translate <" << tposx << ", 3, " << tposy <<"> no_shadow";
	pov << "}" << std::endl;

  
}
