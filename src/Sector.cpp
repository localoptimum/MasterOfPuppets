/*
 * Sector.cpp
 *
 *  Created on: 3 Dec 2014
 *      Author: phillipbentley
 */

#include <iostream>
#include <cmath>
#include "Sector.h"

#include "monte_carlo.h"

#include "globals.h"

using namespace std;


static const double lateralPadding = 0.2;	// Clearance on each side ESS-0042098
static const double motorSpace = 0.4;		// Motor needs to have extra space ESS-0042098
static const double bearingSpace = 0.175;	// Clearance on the wheel on the bearing side ESS-0042098
static const double firstChopperPos = 6.5;      // First place you can put the rotor






//////////////////////////////////////////////////////////
//							//
//	Instrument Geometry Construction Functions	//
//							//
//////////////////////////////////////////////////////////






void Sector::buildEmpty(Instrument& inst)
{
	double length;
	const double start = 3.0;

	switch(sectorToStudy)
	{
		  
	case WEST_SECTOR:
	  length = 25.0 - start;
	  break;

	default:
	  length = 12.0 - start;
	  break;
}
	
	inst.translateFrame(Point(0.0, start));

	inst.addGuideBlock(0.1, length);
	inst.terminate();
}






void Sector::buildBEER(Instrument& inst)
{
	const double start = 2.0;

	double radialDistance = 0.0;
	double leftToGo = 0.0;
	double c1pit;

	//BISPECTRAL
	
	inst.translateFrame(Point(0.0, start));
	radialDistance += start;

	inst.bispectralExtraction();

	// First chopper at 6.5 m
	c1pit = firstChopperPos - bearingSpace + inst.getC1shift();

	leftToGo = c1pit - radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}

	inst.addChopper(0.75+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;


	//Second chopper at 6.9 m
	leftToGo = 6.9 + inst.getC1shift() - bearingSpace- radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}
	inst.addChopper(0.75+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;


	//Third chopper at 7.6
	leftToGo = 7.6 + inst.getC1shift() - bearingSpace - radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}
	inst.addChopper(0.75+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;

	
	//Fourth chopper at 8.3
	leftToGo = 8.3 + inst.getC1shift() - bearingSpace - radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}
	inst.addChopper(0.7 +lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;


	//Fifth chopper at 9.0
	leftToGo = 9.0 + inst.getC1shift() - bearingSpace - radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}
	inst.addChopper(0.7 +lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;


	//Sixth chopper at 9.5
	leftToGo = 9.5 + inst.getC1shift() - bearingSpace - radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}
	inst.addChopper(0.75+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;


	// Now lay out guide to 50 m radius
	leftToGo = 160.0 - radialDistance;

	if(leftToGo > 0.0)
	  {
	    inst.addGuideBlock(0.1, leftToGo);
	    radialDistance += leftToGo;
	  }
	
	
	inst.terminate();
}





void Sector::buildNMX(Instrument& inst)
{
	const double start = 2.0;
	double radialDistance = 0.0;
	double leftToGo = 0.0;
	
	double width = 0.03;		//From proposal
	double radius = 1220.0;		//from proposal
	double gclearance = 0.05;	//from NOSG group leader


	if(exaggerate_geometry==true)
	  {
	    radius = radius / 10.0;
	  }
	
	inst.translateFrame(Point(0.0, start));
	radialDistance = start;

	inst.coldExtraction();

	leftToGo = 6-radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}
	
	inst.setCurvatureDirection(CURVE_LEFT);
	inst.addCurvedGuide(width+2.0*gclearance, 24-start, radius, (24-start)/2.0);

	radialDistance += 24.0-start;

	

	leftToGo = 160-radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}

	
	inst.terminate();
}







void Sector::buildESSENSE(Instrument& inst)
{

	const double start = 2.0;
	double kinkAngle = -3.4;

	double radialDistance = 0.0;
	double leftToGo = 0.0;


	if(exaggerate_geometry==true)
	  {
	    //radius = radius / 10.0;
	    kinkAngle = kinkAngle * 10.0;
	  }

	inst.translateFrame(Point(0.0, start));
	radialDistance += start;

	inst.coldExtraction();
	
	
	inst.addGuideBlock(0.1, firstChopperPos - bearingSpace - start);
	radialDistance += firstChopperPos - bearingSpace - start;


	//First double chopper front back
	//essense.addChopper(6.5, motorSpace, bearingSpace, 0.7+lateralPadding, 0.0);
	//essense.addChopper(6.5+0.05, bearingSpace, motorSpace, 0.7+lateralPadding, 0.0);

	inst.addChopper(0.75+lateralPadding, 2.0+motorSpace+2.0*bearingSpace, 0.0);
	radialDistance += 2.0*motorSpace+2.0*bearingSpace;
	

	

	

	leftToGo = 8.0 -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}

	inst.rotateFrame(kinkAngle/2.0);

	leftToGo = 11.0 -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}

	inst.rotateFrame(kinkAngle/2.0);

	
	leftToGo = 14.0 - bearingSpace - radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}


	//Chopper at 14 metres
	inst.addChopper(0.75+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;



	//Chopper at 16.7 metres
	leftToGo = 16.7- bearingSpace -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}
	inst.addChopper(0.75+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;

	
	//Chopper at 20.12 metres
	leftToGo = 20.12 - bearingSpace-radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}
	inst.addChopper(0.75+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;


	//Chopper at 23.0 metres
	leftToGo = 23.0 - bearingSpace-radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}
	inst.addChopper(0.75+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;


	//Traversal to cave
	leftToGo = 27.0 -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}

		
	//Cave
	inst.addChopper(5.0, 35.0-27.0, 1.0);

	inst.terminate();
}







void Sector::buildWANSE(Instrument& inst)
{

	const double start = 2.0;
	double kinkAngle = 3.4;

	double radialDistance = 0.0;
	double leftToGo = 0.0;

	

	if(exaggerate_geometry==true)
	  {
	    //radius = radius / 10.0;
	    kinkAngle = kinkAngle * 10.0;
	  }
	

	inst.translateFrame(Point(0.0, start));
	radialDistance += start;

	//COLD
	inst.coldExtraction();

	inst.addGuideBlock(0.1, firstChopperPos- bearingSpace-start);
	radialDistance += firstChopperPos- bearingSpace-start;


	//First double chopper front back
	//essense.addChopper(6.5, motorSpace, bearingSpace, 0.7+lateralPadding, 0.0);
	//essense.addChopper(6.5+0.05, bearingSpace, motorSpace, 0.7+lateralPadding, 0.0);

	inst.addChopper(0.75+lateralPadding, 2.0+motorSpace+2.0*bearingSpace, 0.0);
	radialDistance += 2.0*motorSpace+2.0*bearingSpace;
	

	

	

	leftToGo = 8.0 -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}

	inst.rotateFrame(kinkAngle/2.0);

	leftToGo = 11.0 -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}

	inst.rotateFrame(kinkAngle/2.0);
	leftToGo = 14.0 -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}


	leftToGo = 17.0- bearingSpace -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}

	//Chopper at 17 metres
	inst.addChopper(0.75+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;

	
	//Chopper at 21.0 metres
	leftToGo = 21.0 - bearingSpace-radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}
	inst.addChopper(0.75+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;


	//Chopper at 27.0 metres
	leftToGo = 27.0- bearingSpace -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}
	inst.addChopper(0.75+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;


	//Traversal to cave
	leftToGo = 49.0 -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}

		
	//Cave
	inst.addChopper(6.0, 6.0, 0.0);

	inst.terminate();
}








void Sector::buildCAMEA(Instrument& inst)
{
	const double start = 2.0;
	double radialDistance = 0.0;
	double leftToGo = 0.0;
	double c1pit;

	//COLD

	inst.translateFrame(Point(0.0, start));
	radialDistance += start;

	inst.coldExtraction();
	
	// Chopper 1 - 6.5
	c1pit = firstChopperPos + inst.getC1shift();

	leftToGo = c1pit- bearingSpace -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}
	
	inst.addChopper(0.7+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;
	
	// Chopper 2 - 8 m
	leftToGo = 8.0 + inst.getC1shift() - bearingSpace -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}
	inst.addChopper(0.7+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;


	// Chopper 3 - 13 m
	leftToGo = 13.0- bearingSpace -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}
	inst.addChopper(0.7+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;

	leftToGo = 160.0 -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}
	
	inst.terminate();

}








void Sector::buildCSPEC(Instrument& inst)
{
	const double start = 2.0;
	double radius = 2019.0;
	double radialDistance = 0.0;
	double leftToGo = 0.0;
	double c1pit;
	double s1l1;
	double s1l2;
	double s2l;

	//COLD

	if(exaggerate_geometry==true)
	  {
	    radius = radius / 10.0;
	    //kinkAngle = kinkAngle * 10.0;
	  }

	
	inst.translateFrame(Point(0.0, start));
	radialDistance += start;

	inst.coldExtraction();

	// Chopper 1 at 18.7 metres after a straight guide
	c1pit = 18.6 + inst.getC1shift();

	leftToGo = c1pit- bearingSpace -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}	

	inst.addChopper(0.7+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;


	s1l1 = 32.75 - radialDistance;
	// Sbender part 1 to 46 metres.  We do this in two parts around the chopper
	inst.setCurvatureDirection(CURVE_LEFT);
	inst.addCurvedGuide(0.1, s1l1, radius, s1l1/2.0);
	radialDistance = radialDistance + s1l1;
	
	
	// Chopper 2 at 32.75 metres.

	leftToGo = 32.75 - bearingSpace -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}
	inst.addChopper(0.7+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;

	
	s1l2 = 46.0 - radialDistance;
	// Second part of first Sbender section to 46 metres.
	inst.setCurvatureDirection(CURVE_LEFT);
	inst.addCurvedGuide(0.1, s1l2, radius, s1l2/2.0);
	radialDistance = radialDistance + s1l2;


	// second part of second bender 
	s2l = 75.0 - radialDistance;
	// Second Sbender section to 50 metres.
	inst.setCurvatureDirection(CURVE_RIGHT);
	inst.addCurvedGuide(0.1, s2l, radius, s2l/2.0);
	radialDistance = radialDistance + s2l;


	
	// Chopper 3 at 100.0 metres.

	leftToGo = 100.0 - bearingSpace -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}
	inst.addChopper(0.7+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;


	leftToGo = 160.0 - bearingSpace -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}
	
	inst.terminate();

}






void Sector::buildMAGIC(Instrument& inst)
{
	const double start = 2.0;
	double radialDistance = 0.0;
	double leftToGo = 0.0;
	double c1pit;


	
	inst.translateFrame(Point(0.0, start));
	radialDistance += start;

	//BISPECTRAL
	inst.bispectralExtraction();

	c1pit = firstChopperPos + inst.getC1shift();

	leftToGo = c1pit- bearingSpace -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}

	//	magic.addChopper(6.5, motorSpace, bearingSpace, 0.7+lateralPadding, 0.0);
	inst.addChopper(0.7+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;

	//	magic.addChopper(12, motorSpace, bearingSpace, 0.7+lateralPadding, 0.0);
	leftToGo = 12.0- bearingSpace -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}
	inst.addChopper(0.7+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;

	//	magic.addChopper(12.8, motorSpace, bearingSpace, 0.7+lateralPadding, 0.0);
	leftToGo = 12.8- bearingSpace -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}
	inst.addChopper(0.7+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;

	leftToGo = 160.0 -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}

	inst.terminate();
}



void Sector::buildTREX(Instrument& inst)
{


	const double start = 2.0;
	double radius = 19500.0;
	double radialDistance = 0.0;
	double leftToGo = 0.0;
	const double ppsLength = 0.854;
	double c1pit;

	
	
	if(exaggerate_geometry==true)
	  {
	    radius = radius / 100.0;
	    //kinkAngle = kinkAngle * 10.0;
	  }

	//BISPECTRAL
	inst.bispectralExtraction();

	
	inst.translateFrame(Point(0.0, start));
	radialDistance += start;

	//Go out straight to 6 metres
	leftToGo = 6.0 -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}

	inst.setCurvatureDirection(CURVE_LEFT);


	
	//Curve goes out to 96.0 metres.  First part needs to curve to 20 for the first chopper

	// BW chopper at 20 m
	leftToGo = 20.0 - radialDistance-bearingSpace;
	if(leftToGo > 0.0)
	  {
	    inst.addCurvedGuide(0.1, leftToGo, radius, leftToGo/2.0);
	    radialDistance += leftToGo;
	  }

	inst.addChopper(1.0 + lateralPadding, motorSpace + bearingSpace, 0.0);
	radialDistance += motorSpace + bearingSpace;

	
	
	// BW chopper at 24 m
	leftToGo = 24 -radialDistance-bearingSpace;
	if(leftToGo > 0.0)
	  {
	    inst.addCurvedGuide(0.1, leftToGo, radius, leftToGo/2.0);
	    radialDistance += leftToGo; 
	  }
	inst.addChopper(1.0 + lateralPadding, motorSpace + bearingSpace, 0.0);
	radialDistance += motorSpace + bearingSpace;


	
	//Rest of curve to 96 metres
	leftToGo = 96.0 -radialDistance-bearingSpace;
	if(leftToGo > 0.0)
	  {
	    inst.addCurvedGuide(0.1, leftToGo, radius, leftToGo/2.0);
	    radialDistance += leftToGo;
	  }

	
	
	// PPS at 99 m  -- probably enough choppers
	leftToGo = 16.5- bearingSpace -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}
	inst.addChopper(0.6 + lateralPadding, ppsLength, 0.0);
	radialDistance += ppsLength;

	
	// Escape to sample position 
	leftToGo = 160.0 -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}

	
	inst.terminate();
}





void Sector::buildHEIMDAL(Instrument& inst)
{
	const double start = 2.0;
	double radialDistance = 0.0;
	double leftToGo = 0.0;
	double c1pit;

	//THERMAL
	
	inst.translateFrame(Point(0.0, start));
	radialDistance += start;

	inst.thermalExtraction();



	// "CT1"
	c1pit = firstChopperPos + inst.getC1shift();

	leftToGo = c1pit- bearingSpace -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}
       
	inst.addChopper(0.35*2.0 + lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;


	
	// "CT2"
	leftToGo = 7.0 + inst.getC1shift() - bearingSpace -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}
	inst.addChopper(0.4*2.0 + lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;

	
	


	

	//Straight to kink
	leftToGo = 70.0 -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}

	//kink
	inst.rotateFrame(0.2);
	
	leftToGo = 160.0 -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}


	//Reset coordinate system and start for cold beamline
	radialDistance = 0.0;
	inst.resetPort();

	//Hard rotation for sector specific 
	inst.rotateFrame(-60);
	inst.rotateToPort();
	
	inst.translateFrame(Point(0.0, start));
	radialDistance += start;

	//Perform cold extraction
	inst.coldExtraction();

	//Deploy cold beam
	inst.addGuideBlock(0.1, 8);
	radialDistance += start;

	//Kink
	inst.rotateFrame(0.5);

	
	// "CC1"
	leftToGo = 9.0- bearingSpace -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}
	inst.addChopper(0.64*2+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;

	
	// "CC3
	leftToGo = 13.0- bearingSpace -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}
	inst.addChopper(0.6*2.0 + lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;

	
	leftToGo = 24.0 -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}
	

	//Curved guide 125 m 1.2 km	
	inst.setCurvatureDirection(CURVE_LEFT);
	inst.addCurvedGuide(0.1, 125.0, 1200.0, 125.0/2.0);
	radialDistance += 125.0;


	leftToGo = 160.0 -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}

	
	inst.terminate();

}



void Sector::buildMIRACLES(Instrument& inst)
{
	const double start = 2.0;
	double radius = 5000.0;
	double radialDistance = 0.0;
	double leftToGo;
	double c1pit;

	if(exaggerate_geometry==true)
	  {
	    radius = radius / 500.0;
	    //kinkAngle = kinkAngle * 10.0;
	  }

	//COLD

	inst.translateFrame(Point(0.0, start));
	radialDistance += start;

	inst.coldExtraction();
	
	// Chopper 1 - 6.5
	c1pit = firstChopperPos + inst.getC1shift();

	leftToGo = c1pit- bearingSpace -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}

	inst.addChopper(0.7+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;


	// Chopper 2 - 7.03

	leftToGo = 7.03 + inst.getC1shift() - bearingSpace -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}

	inst.addChopper(0.7+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;


	
	// Chopper 3 - 20.5
	
	leftToGo = 20.5- bearingSpace -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}

	inst.addChopper(0.7+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;


	// Continue curve to 81 metres
	leftToGo = 81.0 - radialDistance;
	
	inst.setCurvatureDirection(CURVE_LEFT);
	inst.addCurvedGuide(0.1, leftToGo, radius, leftToGo/2.0);
	radialDistance += leftToGo;

	//Escape out to 160
	leftToGo = 160.0 - radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}


	inst.terminate();

}






void Sector::buildLOKI(Instrument& inst)
{
	const double start = 2.0;
	const double width = 0.04; //check with Andrew - maximum value is OK
	const double samplePos = 23.5;
	const double tankLen = 10.0 + 3.0; // detector length plus 1 m clearnance plus 2 m cave wall and beam stop
	const double tankWidth = 1.0 + 2.0 + 2.0;  // detector plus walking space plus 1 m shielding each side
	const double gclearance = 0.05;       //from NOSG group leader

	
	
	double r1 = 57.0;
	const double cl1 = firstChopperPos-start- bearingSpace;
	
	double r2 = 37.7;
	const double cl2 = 9.7-7.5- bearingSpace;
	
	double leftToGo = 0.0;
	double radialDistance = 0.0;

	if(exaggerate_geometry==true)
	  {
	    r1 = 10;
	    r2 = 10; 
	  }


	inst.translateFrame(Point(0.0, start));
	radialDistance += start;

	//COLD
	inst.coldExtraction();

	inst.setCurvatureDirection(CURVE_RIGHT);
	//void addCurvedGuide(double width, double length, double radius, int pieces);
	inst.addCurvedGuide(width+2.0*gclearance, cl1, r1, 6);
	radialDistance += cl1;

	//Chopper at 6.0 m
	inst.addChopper(0.7+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;

	inst.addGuideBlock(0.1, 7.5-radialDistance);
	radialDistance += 7.5-radialDistance;

	//Chopper at 9.7 m
	inst.addChopper(0.7+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;

	inst.setCurvatureDirection(CURVE_LEFT);
	inst.addCurvedGuide(width+2.0*gclearance, cl2, r2, 6);
	radialDistance += cl2;

	//Chopper at 11 m
	inst.addChopper(0.7+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;
	
	leftToGo = samplePos-radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}

	//Instrument Cave
	inst.addChopper(tankWidth, tankLen, 0.0);


	inst.terminate();
}





void Sector::buildSKADI(Instrument& inst)
{
	const double start = 2.0;
	double leftToGo = 0.0;
	double radialDistance = 0.0;


	inst.translateFrame(Point(0.0, start));
	radialDistance += start;

	//COLD
	inst.coldExtraction();


	inst.addGuideBlock(0.1, 11.0-start- bearingSpace);
	radialDistance += 11.0-start- bearingSpace;

	inst.addChopper(0.7+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;

	leftToGo = 30.0 -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}

	//Instrument Cave
	inst.addChopper(3.0, 20.0, 0.0);


	inst.terminate();
}





void Sector::buildSURFACESCATTERING(Instrument& inst)
{
  // a bit like LOKI but longer, like SKADI. - Hanna 4/NOV/2015
  
	const double start = 2.0;
	const double width = 0.04;
	const double gclearance = 0.02;
	double leftToGo = 0.0;
	double radialDistance = 0.0;


	double r1 = 57.0;
	const double cl1 = firstChopperPos-start- bearingSpace;
	
	double r2 = 37.7;
	const double cl2 = 9.7-7.5- bearingSpace;
	

	if(exaggerate_geometry==true)
	  {
	    r1 = 10;
	    r2 = 10; 
	  }


	inst.translateFrame(Point(0.0, start));
	radialDistance += start;

	//COLD
	inst.coldExtraction();

	inst.setCurvatureDirection(CURVE_RIGHT);
	//void addCurvedGuide(double width, double length, double radius, int pieces);
	inst.addCurvedGuide(width+2.0*gclearance, cl1, r1, 6);
	radialDistance += cl1;

	//Chopper at 6.0 m
	inst.addChopper(0.7+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;

	inst.addGuideBlock(0.1, 7.5-radialDistance);
	radialDistance += 7.5-radialDistance;

	//Chopper at 9.7 m
	inst.addChopper(0.7+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;

	inst.setCurvatureDirection(CURVE_LEFT);
	inst.addCurvedGuide(width+2.0*gclearance, cl2, r2, 6);
	radialDistance += cl2;

	//Chopper at 11 m
	inst.addChopper(0.7+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;

	

	leftToGo = 30.0 -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}

	//Instrument Cave
	inst.addChopper(3.0, 20.0, 0.0);


	inst.terminate();
}





void Sector::buildSLEIPNIR(Instrument& inst)
{
	const double start = 2.0;
	double radius1 = 57.0;
	double radius2 = 37.7;
	double leftToGo = 0.0;
	double radialDistance = 0.0;
	const double gclearance = 0.05;       //from NOSG group leader
	const double width = 0.07;




	if(exaggerate_geometry==true)
	  {
	    radius1 = radius1 / 10.0;
	    radius2 = radius2 / 10.0;
	  }



	inst.translateFrame(Point(0.0, start));
	radialDistance += start;

	//COLD
	inst.coldExtraction();
	
	//Curved guide in monolith out to 6.0 metres
	leftToGo = firstChopperPos -radialDistance- bearingSpace;
	inst.setCurvatureDirection(CURVE_LEFT);
	inst.addCurvedGuide(width+2.0*gclearance, leftToGo, radius1, leftToGo);
	radialDistance += leftToGo;

	// Chopper at 6 metres
	inst.addChopper(0.7+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;

	//Straight guide out to 7.5
	leftToGo = 7.5 -radialDistance;
	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}

	//Curved guide to 9.7 m
	leftToGo = 9.7 -radialDistance- bearingSpace;
	inst.addCurvedGuide(width+2.0*gclearance, leftToGo, radius2, leftToGo);
	radialDistance += leftToGo;

	//Chopper at 9.7	
	inst.addChopper(0.7+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;

	//Straight guide to 11 m then chopper
	leftToGo = 11.0 - bearingSpace-radialDistance;
	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}
	inst.addChopper(0.7+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;

	//Some shit about a collimator drum?  Forget it - just do guide to sample position

	leftToGo = 16.495 -radialDistance;
	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}

	//Instrument Cave
	inst.addChopper(2.0, 5.7, 0.0);


	inst.terminate();
}



void Sector::buildODIN(Instrument& inst)
{
	const double start = 6.0;
	//  ODIN Instrument specific from Stuart Pullen
	double radialDistance = 0.0;
	double leftToGo = 0.0;


	inst.translateFrame(Point(0.0, start));
	radialDistance += start;

	aimThermal(inst);

	inst.addGuideBlock(0.1, firstChopperPos-start- bearingSpace);
	radialDistance += firstChopperPos-start- bearingSpace;

	//	odin.addChopper(6.5, 0.0, 0.5, 0.7+lateralPadding, 0.0);
	inst.addChopper(0.7+lateralPadding, 0.5+motorSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;

	//	odin.addChopper(8.0, motorSpace, bearingSpace, 0.8+lateralPadding, 0.0);

	leftToGo = 8.5- bearingSpace -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}
	inst.addChopper(0.8+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;


	//	odin.addChopper(9.0, 0.0, 0.5, 0.7+lateralPadding, 0.0);
	leftToGo = 9.0- bearingSpace -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}
	inst.addChopper(0.7+lateralPadding, 0.5, 0.0);  //Prompt pulse chopper
	radialDistance += motorSpace+bearingSpace;




	//	odin.addChopper(11.9, motorSpace, bearingSpace, 1.0+lateralPadding, 0.0);
	leftToGo = 12.480- bearingSpace -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}
	inst.addChopper(1.0+lateralPadding, motorSpace+bearingSpace + 0.05, 0.0);  //Double disk
	radialDistance += motorSpace+bearingSpace;

	leftToGo = 17.70- bearingSpace -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}
	inst.addChopper(1.2+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;


	leftToGo = 25.9 - bearingSpace-radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}
	inst.addChopper(1.8+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;

	leftToGo = 32.0 - bearingSpace-radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}
	inst.addChopper(1.8+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;

	//	odin.addChopper(12.0, bearingSpace, motorSpace, 1.0+lateralPadding, 0.0);  // Probably covered OK with the previous choper at 11.9 metres.

	leftToGo = 50.0 -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}

	//Instrument Cave
	inst.addChopper(6.0+1.5*2.0, 18.0+1.5*2, 0.0);

	inst.terminate();
}



void Sector::buildFUND(Instrument& inst)
{
	const double start = 6.0;
	double radialDistance = 0.0;
	double leftToGo = 0.0;


	inst.translateFrame(Point(0.0, start));
	radialDistance += start;

	inst.addGuideBlock(0.1, firstChopperPos-start);
	radialDistance += firstChopperPos- bearingSpace-start;
	inst.rotateFrame(2.0);

	//	fund.addChopper(6.22+bearingSpace, motorSpace, bearingSpace, 0.45+lateralPadding, 0.0);
	inst.addChopper(0.45+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;


	inst.setCurvatureDirection(CURVE_RIGHT);
	inst.addCurvedGuide(0.1, 6, 300, 6);
	radialDistance += 6;
	inst.setCurvatureDirection(CURVE_LEFT);
	inst.addCurvedGuide(0.1, 6, 300, 6);
	radialDistance += 6;

	leftToGo = 50.0 -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}

	//Instrument Cave
	inst.addChopper(3.0, 5.0, 0.0);

	inst.terminate();
}


void Sector::buildESTIA(Instrument& inst)
{
	const double start = 2.0;
	double leftToGo = 0.0;
	double radialDistance = 0.0;
	double dQ = 1.27;
	

	//COLD

	inst.translateFrame(Point(0.0, start));
	radialDistance += start;

	inst.coldExtraction();

	leftToGo = 6.0 -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}

	inst.rotateFrame(-2.0*dQ);



	
	leftToGo = 12.5 - radialDistance - bearingSpace;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}

	inst.addChopper(0.7+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;
	

	leftToGo = 18.0 - radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}

	inst.rotateFrame(2.0*dQ);

	
	leftToGo = 30.0 - radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}

	inst.rotateFrame(-2.0*dQ);

	leftToGo = 36.0 - radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}


	//Instrument Cave plus entrance, shield, chicane
	inst.addChopper(8.0+1.0, 8.5 + 2.0, -2.5);



	inst.terminate();
}



void Sector::buildVOR(Instrument& inst)
{
	const double start = 2.0;
	double leftToGo = 0.0;
	double radialDistance = 0.0;


	inst.translateFrame(Point(0.0, start));
	radialDistance += start;
	//inst.bispectralExtraction();

	//COLD
	
	inst.addGuideBlock(0.1, 8.0-start- bearingSpace);
	radialDistance += 8.00-start- bearingSpace;

	//	vor.addChopper(6.22+bearingSpace, motorSpace, bearingSpace, 0.45+lateralPadding, 0.0);
	inst.addChopper(0.45+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;

	inst.addGuideBlock(0.1, 2.0);
	radialDistance += 2.0;

	inst.rotateFrame(2.0);

	inst.addGuideBlock(0.1, 6.0);
	radialDistance += 6.0;


	//Cave back offset = 7.5 metres
	leftToGo = 29.85 - 7.5 - radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}

	
	//Instrument Cave
	inst.addChopper(11.2, 18.0, 1.0);

	inst.terminate();
}



void Sector::buildVESPA(Instrument& inst)
{
	const double start = 2.0;
	double leftToGo = 0.0;
	double radialDistance = 0.0;

	//THERMAL

	inst.translateFrame(Point(0.0, start));
	inst.addGuideBlock(0.1, 6.45-start- bearingSpace);
	radialDistance += 6.45-start- bearingSpace;

	inst.thermalExtraction();
	
	//Chopper 1 @ 6.5
	inst.addChopper(0.45+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;




	
	//Chopper 2 @ 6.8
	leftToGo = 6.8- bearingSpace - radialDistance;
	if(leftToGo > 0.0)
	  {
	    inst.addGuideBlock(0.1, leftToGo);
	    radialDistance += leftToGo;
	  }

	inst.addChopper(0.45+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;





	//Chopper 3 @ 7.44
	leftToGo = 7.44- bearingSpace - radialDistance;
	if(leftToGo > 0.0)
	  {
	    inst.addGuideBlock(0.1, leftToGo);
	    radialDistance += leftToGo;
	  }

	inst.addChopper(0.45+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;
	


	//Chopper 4 @ 10 m
	leftToGo = 10.0- bearingSpace - radialDistance;
	if(leftToGo > 0.0)
	  {
	    inst.addGuideBlock(0.1, leftToGo);
	    radialDistance += leftToGo;
	  }

	inst.addChopper(0.45+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;




	//Chopper 5 @ 19
	leftToGo = 19.0- bearingSpace - radialDistance;
	if(leftToGo > 0.0)
	  {
	    inst.addGuideBlock(0.1, leftToGo);
	    radialDistance += leftToGo;
	  }

	inst.addChopper(0.45+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;




	//Chopper 6 @ 52.0
	leftToGo = 52.0- bearingSpace - radialDistance;
	if(leftToGo > 0.0)
	  {
	    inst.addGuideBlock(0.1, leftToGo);
	    radialDistance += leftToGo;
	  }

	inst.addChopper(0.45+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;


	
	//To Sample @ 59.0
	leftToGo = 59.0 - radialDistance;
	if(leftToGo > 0.0)
	  {
	    inst.addGuideBlock(0.1, leftToGo);
	    radialDistance += leftToGo;
	  }

	

	//Instrument Cave

	inst.addChopper(5.0, 4.0, 0.0);

	inst.terminate();
}




void Sector::buildFREIA(Instrument& inst)
{
	const double start = 2.0;
	double leftToGo = 0.0;
	double radialDistance = 0.0;
	double radius = 56.0;

	const double tankLen = 15.0;
	const double tankWidth = 3.0;
	
	const double curve1 = 4.0;

	
	if(exaggerate_geometry==true)
	  {
	    radius = 10;
	  }

	inst.translateFrame(Point(0.0, start));
	radialDistance += start;

	//COLD
	inst.coldExtraction();

	// Bender
	inst.setCurvatureDirection(CURVE_RIGHT);
	inst.addCurvedGuide(0.05, curve1, radius, 4);
	radialDistance += curve1;

	//Lay guide up to first chopper
	leftToGo = firstChopperPos - bearingSpace - radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}

	inst.addChopper(0.75+lateralPadding, 2.0*motorSpace+2.0*bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;



	// Chopper at 8.4 metres

	leftToGo = 8.4 - bearingSpace-radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}

	inst.addChopper(1.3+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;

	// Chopper at 9.9 metres

	leftToGo = 9.9 - bearingSpace-radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}

	inst.addChopper(0.75+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;


	// Chopper at 11.0 metres

	leftToGo = 11.0 - bearingSpace-radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}

	inst.addChopper(1.3+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;

	
	// Chopper at 15.0 metres

	leftToGo = 15.0- bearingSpace -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}

	inst.addChopper(0.7+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;

	
	// Chopper at 15.6 metres

	leftToGo = 15.6- bearingSpace -radialDistance;

	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.1, leftToGo);
		radialDistance += leftToGo;
	}

	inst.addChopper(1.5+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;
	
	
	// Move to sample position
	leftToGo = 25.0- bearingSpace -radialDistance;

		if(leftToGo > 0.0)
		{
			inst.addGuideBlock(0.1, leftToGo);
			radialDistance += leftToGo;
		}


	//Instrument Cave
	inst.addChopper(tankWidth+1, tankLen, 1.0);
	
	inst.terminate();
}







//////////////////////////////////////////////////////////
//														//
//				Constructor / Destructor				//
//				Operators etc							//
//														//
//////////////////////////////////////////////////////////




Sector::Sector()
{

	int i;

	num_instruments = 0;
	numWalls =0;
	sectorArc = 60.0;
	maximumThrow = 1.5;
	angle1=0.0;
	angle2=0.0;

	for(i=0; i<12; i++)
	{
		beamCentre[i] = 0.0;
		instrumentAllocation[i]=0;
	}

	instrumentsDistributed = false;
}












void Sector::buildANNI(Instrument& inst)
{
	const double start = 2.0;
	const double samplePos = 23.5;
	const double tankLen = 51.0-25.0;
	const double tankWidth = 5.0;
	const double gclearance = 0.05;       //from NOSG group leader

	double r1 = 270.0;
	const double cl1 = 8.0;
	
	double r2 = 84.0;
	const double cl2 = 2.5;

	
	double leftToGo = 0.0;
	double radialDistance = 0.0;

	


	inst.translateFrame(Point(0.0, start));
	radialDistance += start;

	//COLD
	inst.coldExtraction();

	//Trumpet
	inst.addGuideBlock(0.08+2*gclearance, 3.6);
	radialDistance += 3.6;

	//Straight 1
	leftToGo = firstChopperPos-radialDistance;
	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.12+2*gclearance, leftToGo);
		radialDistance += leftToGo;
	}

	//Chopper at 6.5 m
	inst.addChopper(0.8+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;

	leftToGo = 8.5-radialDistance;
	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.12+2*gclearance, leftToGo);
		radialDistance += leftToGo;
	}

	//Chopper at 8.5 m
	inst.addChopper(0.8+lateralPadding, motorSpace+bearingSpace, 0.0);
	radialDistance += motorSpace+bearingSpace;

	leftToGo = 8.5-radialDistance;
	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.12+2*gclearance, leftToGo);
		radialDistance += leftToGo;
	}

	

	leftToGo = 26.0-radialDistance;
	if(leftToGo > 0.0)
	{
		inst.addGuideBlock(0.12+2*gclearance, leftToGo);
		radialDistance += leftToGo;
	}

	//Chopper at 26.0 m - SPACE NEEDED FOR DOUBLE CHOPPER
	inst.addChopper(0.8+lateralPadding, 2*motorSpace+2*bearingSpace, 0.0);
	radialDistance += 2*motorSpace+2*bearingSpace;

	//Instrument Cave
	inst.addChopper(tankWidth, tankLen, 0.0);
	inst.terminate();
}











Sector::~Sector() {

}


Sector& Sector::operator=(const Sector& source)
{
	int i;

	this-> num_instruments = source.num_instruments;
	this-> numWalls = source.numWalls;
	this-> sectorArc = source.sectorArc;
	this-> maximumThrow = source.maximumThrow;
	this-> angle1= source.angle1;
	this-> angle2=source.angle2;

	this-> instrumentsDistributed = source.instrumentsDistributed;

	for(i=0; i<12; i++)
	{
		this->instruments[i] = source.instruments[i];
		this->beamCentre[i] = source.beamCentre[i];
		this->instrumentAllocation[i] = source.instrumentAllocation[i];
	}

	for(i=0; i<numWalls; i++)
	{
		this->wblock[i] = source.wblock[i];
	}

	return(*this);
}






//////////////////////////////////////////////////////////
//														//
//						Utilities					 	//
//														//
//////////////////////////////////////////////////////////






double Sector::getArcLength(void)
{
	return(sectorArc);
}

void Sector::setArcLength(const double arc)
{
  sectorArc = arc;
}


void Sector::addInstrument(Instrument inst)
{
	instruments[num_instruments] = inst;
	num_instruments++;
}


void Sector::listPortAllocations(void)
{
	int i;

	Instrument* inst;

	if(!instrumentsDistributed)
	{
		cerr << "Attempt to list port allocations without first distributing allocations." << endl;
		return;
	}

	cout << endl;

	for(i=0; i < 12; i++)
	{
		inst = & getInstrumentOnPort(i);

		cout << "beam port " << i << " "  << instrumentAllocation[i] << "th Instrument " << instNames[inst->id] << " centre " << instruments[instrumentAllocation[i]].get_beam_centre() << endl;
	}

	cout << endl;
}

int Sector::instAllocation(int rank)
{
	return instrumentAllocation[rank];
}

Instrument& Sector::getInstrumentOnPort(int port)
{
	return(instruments[instrumentAllocation[port]]);
}










//////////////////////////////////////////////////////////
//														//
//						Suite Functions				 	//
//														//
//////////////////////////////////////////////////////////



void Sector::distributeInstruments(double param[])
{
	//The meat of the layout work.  Put the instruments where they need to be.
	int i=0;
	double beamCentrePos=0.0;
	bool sorted=false;
	int numSwaps=0;

	int rank[12];  // rank of instrument in order of deployment from sector edge.  12 ports instruments comes from 60 degrees with 5 degrees separation.

	int temp;


	// Angular separation quantisation tuning.  To adjust the baseline separation, uncomment the following:
	//angle1 = param[0];
	// To fix the angular separation to a constant value in degrees, uncomment the next line and set the value:
	//angle1 = 5.0;

	//For tune a second alternate angle (e.g. 5-7-5-7-5-7) uncomment following line to have a different second separation
	//angle2 = param[1];
	//To make the second angular separation the same as the first (e.g. 5-5-5-5...) uncomment the following line
	//angle2 = angle1;
	//To fix the second angular separation to a constant value in degrees, do this:
	//angle2 = 7.5;

	//The next section shows how to have different angles based on each sector.

	switch(sectorToStudy)
	{
	case NORTH_SECTOR:
		angle1 = 6.0;
		angle2 = 6.0;
		break;

	case EAST_SECTOR:
		angle1 = 6.0;
		angle2 = 6.0;
		break;

	case SOUTH_SECTOR:
		angle1 = 5.3;
		angle2 = 6.7;
		break;

	default: //also WEST_SECTOR
		angle1 = 5.3;
		angle2 = 6.7;
		break;
	}
	
	
	if(!fix_all_instruments)
	  {
	    //Rank the instruments and bubble_sort
	    for(i=0; i<12; i++)
	      {
		rank[i] = i;
	      }
	    
	    while(sorted==false)
	      {
		numSwaps=0;

		//Sorting the ith instrument with i-1th instrument, so this loop starts at 1
		for(i=1; i<12; i++)
		  {
		    if(param[rank[i]+2] > param[rank[i-1]+2])
		      {
			//Instrument i is higher priority than instrument i-1, so swap their entries in sorted table
			temp = rank[i-1];
			rank[i-1] = rank[i];
			rank[i] = temp;
			numSwaps++;
		      }
		    if(param[rank[i]+2] == param[rank[i-1]+2])
		      {
			//Ranking score is equal, randomly swap them based on a toss of a coin
			if(mc -> toss_coin() == 0)
			  {
			    temp = rank[i-1];
			    rank[i-1] = rank[i];
			    rank[i] = temp;
			    numSwaps++;
			  }
			
		      }
		  }
		if(numSwaps==0)
		  {
		    sorted = true;
		  }
		else
		  {
		    sorted = false;
		  }
	      }
	  
	    for(i=0; i<12; i++)
	      {
		instrumentAllocation[i] = rank[i];
	      }

	    
	    
	  }
	else
	  {
	    // Manually enter instrument rank according to the manual order
	    for(i=0; i<12; i++)
	      {
		switch(sectorToStudy)
		  {
		  case WEST_SECTOR:
		    {
		      switch(instruments[i].id)
			{
			case NMX:
			  instrumentAllocation[i] = 0;
			  break;
			  
			case MIRACLES: 
			  instrumentAllocation[i] = 1;
			  break;

			case BEER: 
			  instrumentAllocation[i] = 2;
			  break;

			case CSPEC: 
			  instrumentAllocation[i] = 3;
			  break;
			  
			case TREX: 
			  instrumentAllocation[i] = 4;
			  break;
			  
			case MAGIC:
			  instrumentAllocation[i] = 5;
			  break;
			  
			case CAMEA:
			  instrumentAllocation[i] = 6;
			  break;
			  
			case HEIMDAL:
			  instrumentAllocation[i] = 7;
			  break;
			  
			case EMPTY:
			  instrumentAllocation[i] = 8;
			  break;
			  
			case EMPTY2:
			  instrumentAllocation[i] = 9;
			  break;
			  
			case EMPTY3:
			  instrumentAllocation[i] = 10;
			  break;
			  
			case EMPTY4:
			  instrumentAllocation[i] = 11;
			  break;			  
			}
		    }
		    break;
		  }
	      }
	  }
	
	

	beamCentrePos = 0.0;
	
	for(i=0; i<12; i++)
	  {
	    
	    if(i>0)
	      {
		
		if(i % 2 == 0)
		  {
		    beamCentrePos = beamCentrePos + angle2;
		  }
		else
		  {
		    beamCentrePos = beamCentrePos + angle1;
		  }
	      }
	    
	    instruments[instrumentAllocation[i]].set_beam_centre(beamCentrePos);
	    
	  }
	
	
	
	// Diagnostic sorting info

	// listPortAllocations();

	deployInstruments();

	instrumentsDistributed = true;
}




void Sector::deployInstruments(void)
{
	int port;
	int id;

	Point corners[4];
	double left, right, miny, maxy;
	double hallWidth;
	double hallLength;

	const double tConeX = 5.55;     // x value of target wedge nearest moderator
	const double tConeY = 2.28;     // y value of target wedge nearest moderator
	const double tConeXX = 20.549;  // x value of target wedge wide part
	const double tConeYY = 10.94;   // y value of target wedge wide part

	double sectRotation = -60.0;

	switch(sectorToStudy)
	{
	//This is accurate, at least as far as Ken's drawings are concerned.
	case EAST_SECTOR:
		numWalls = 0.0;

		//Create points in the corners with the origin at moderator centre, and 0 degrees aligned along the proton beam axis, left or right depending.
		//Sector radiates towards top left (-x, +y) direction on 30 degree beam port.  Updated 7/12/2015 based on Benjamin Davidge email 
		hallWidth = 51.1;
		hallLength = 62.5;

		left = -hallWidth;
		right = 0.0;
		miny = hallLength;
		maxy = hallLength+1.0;

		corners[0].set(left, miny);
		corners[1].set(left, maxy);
		corners[2].set(right, maxy);
		corners[3].set(right, miny);
		wblock[numWalls].setBlock(corners);
		wblock[numWalls].deployInPlace();
		this-> numWalls++;

		left = -hallWidth - 1.0;
		right = -hallWidth;
		miny = 0.0;
		maxy = hallLength;

		corners[0].set(left, miny);
		corners[1].set(left, maxy);
		corners[2].set(right, maxy);
		corners[3].set(right, miny);
		wblock[numWalls].setBlock(corners);
		wblock[numWalls].deployInPlace();
		this-> numWalls++;

		//Target building left side
	  left = -50.0;
	  right= -tConeXX;
	  maxy  = tConeYY;
	  miny = -tConeYY;

	  corners[0].set(left, miny);
	  corners[1].set(left, maxy);
	  corners[2].set(right, maxy);
	  corners[3].set(right, miny);
	  wblock[numWalls].setBlock(corners);
	  wblock[numWalls].deployInPlace();
	  this-> numWalls++;

	  //Target building wedge thing
	  //x = -5.0
	  //4.5 wide
	  left = -tConeXX;
	  right= -tConeX;
	  miny = -tConeYY;
	  maxy = tConeYY;

	  corners[0].set(left, miny);
	  corners[1].set(left, maxy);

	  miny = -tConeY;
	  maxy = tConeY;
	  
	  corners[2].set(right, maxy);
	  corners[3].set(right, miny);
	  wblock[numWalls].setBlock(corners);
	  wblock[numWalls].deployInPlace();
	  this-> numWalls++;

		break;


	case NORTH_SECTOR:
			numWalls = 0.0;

			//Create points in the corners with the origin at moderator centre, and 0 degrees aligned along the proton beam axis, left or right depending.
			//Sector radiates towards top left (-x, +y) direction on 30 degree beam port.

			//TOP WALL
			hallWidth = 54.0;
			hallLength = 28.0+15.0;

			left = -hallLength;
			right = 0.0;
			miny = hallWidth;
			maxy = hallWidth+1.0;

			corners[0].set(left, miny);
			corners[1].set(left, maxy);
			corners[2].set(right, maxy);
			corners[3].set(right, miny);
			wblock[numWalls].setBlock(corners);
			wblock[numWalls].deployInPlace();
			this-> numWalls++;


			//LEFT WALL
			left = -hallLength - 1.0;
			right = -hallLength;
			miny = 0.0;
			maxy = hallWidth;

			corners[0].set(left, miny);
			corners[1].set(left, maxy);
			corners[2].set(right, maxy);
			corners[3].set(right, miny);
			wblock[numWalls].setBlock(corners);
			wblock[numWalls].deployInPlace();
			this-> numWalls++;

			//NNBAR
			left = -hallLength;
			right = -10.0;
			miny = -1.0;
			maxy = 1.0;

			corners[0].set(left, miny);
			corners[1].set(left, maxy);
			corners[2].set(right, maxy);
			corners[3].set(right, miny);
			wblock[numWalls].setBlock(corners);
			wblock[numWalls].deployInPlace();
			this-> numWalls++;



			//Target building left side
			maxy = 50.0;
			miny = tConeXX;
			right  = tConeYY;
		        left = -tConeYY;
			
			corners[0].set(left, miny);
			corners[1].set(left, maxy);
			corners[2].set(right, maxy);
			corners[3].set(right, miny);
			wblock[numWalls].setBlock(corners);
			wblock[numWalls].deployInPlace();
			this-> numWalls++;
			
			//Target building wedge thing
			//x = -5.0
			//4.5 wide
			maxy = tConeXX;
			miny = tConeX;
			left = -tConeY;
			
			
			corners[0].set(left, miny);
			left = -tConeYY;
			
			corners[1].set(left, maxy);
			
			// miny = -4.5/2.0;
			// maxy = 4.5/2.0;

			right  = tConeYY;
			corners[2].set(right, maxy);
			right = tConeY;
			corners[3].set(right, miny);
			wblock[numWalls].setBlock(corners);
			wblock[numWalls].deployInPlace();
			this-> numWalls++;
			

			break;



	case SOUTH_SECTOR:

	  //Create points in the corners with the origin at moderator centre, and 0 degrees aligned along the proton beam axis, left or right depending.
	  //Sector radiates towards top left (-x, +y) direction on 30 degree beam port.
	  hallWidth = 135.5-54.0;
	  hallLength = 50.5;
	  
	  left = -hallLength/2.0;
	  right = hallLength/2.0;
	  miny = hallWidth;
	  maxy = hallWidth+1.0;
	  
	  corners[0].set(left, miny);
	  corners[1].set(left, maxy);
	  corners[2].set(right, maxy);
	  corners[3].set(right, miny);
	  wblock[numWalls].setBlock(corners);
	  wblock[numWalls].deployInPlace();
	  this-> numWalls++;
	  
	  left = -hallLength/2.0 - 1.0;
	  right = -hallLength/2.0;
	  miny = 3.0;
	  maxy = hallWidth;
	  
	  corners[0].set(left, miny);
	  corners[1].set(left, maxy);
	  corners[2].set(right, maxy);
	  corners[3].set(right, miny);
	  wblock[numWalls].setBlock(corners);
	  wblock[numWalls].deployInPlace();
	  this-> numWalls++;
	  
	  break;





	  
	case WEST_SECTOR:

	  //Target building left side
	  left = -50.0;
	  right= -21.0;
	  maxy  = 21.0/2.0;
	  miny = -21.0/2.0;

	  corners[0].set(left, miny);
	  corners[1].set(left, maxy);
	  corners[2].set(right, maxy);
	  corners[3].set(right, miny);
	  wblock[numWalls].setBlock(corners);
	  wblock[numWalls].deployInPlace();
	  this-> numWalls++;
	  
	  //Target building wedge thing
	  //x = -5.0
	  //4.5 wide
	  left = -tConeXX;
	  right= -tConeX;
	  miny = -tConeYY;
	  maxy = tConeYY;

	  corners[0].set(left, miny);
	  corners[1].set(left, maxy);

	  miny = -tConeY;
	  maxy = tConeY;
	  
	  corners[2].set(right, maxy);
	  corners[3].set(right, miny);
	  wblock[numWalls].setBlock(corners);
	  wblock[numWalls].deployInPlace();
	  this-> numWalls++;

	  // Guide hall north wall
	  corners[0].set(-12.2,40.75);
	  corners[1].set(-50.1,145.0);
	  corners[2].set(-49.1,146.0);
	  corners[3].set(-11.2,41.75);
	  wblock[numWalls].setBlock(corners);
	  wblock[numWalls].deployInPlace();
	  this-> numWalls++;

	  // Guide hall south wall
	  corners[0].set(-78.5,38.7);
	  corners[1].set(-135.2,71.7);
	  corners[2].set(-135.0,72.7);
	  corners[3].set(-78.0,39.7);
	  wblock[numWalls].setBlock(corners);
	  wblock[numWalls].deployInPlace();
	  this-> numWalls++;
	  
	  break;
	  

	default:
		//Do nothing - we ignore walls unless the sector needs it specifically
		break;
	}
	
	switch(sectorToStudy)
	  {
	    
	  case NORTH_SECTOR:
	    sectRotation = -89.99;
	    break;
	  default:
	    sectRotation = -60.0;
	    break;
	  }
	
	for(port=0; port<12; port++)
	{
		instruments[instrumentAllocation[port]].resetCoordinates();

		instruments[instrumentAllocation[port]].rotateFrame(sectRotation);
		
		instruments[instrumentAllocation[port]].rotateToPort();

		id = instruments[instrumentAllocation[port]].id;

		switch(id)
		{
		case BEER:
			buildBEER(instruments[instrumentAllocation[port]]);
			break;
		case NMX:
			buildNMX(instruments[instrumentAllocation[port]]);
			break;
		case CAMEA:
			buildCAMEA(instruments[instrumentAllocation[port]]);
			break;
		case CSPEC:
			buildCSPEC(instruments[instrumentAllocation[port]]);
			break;
		case MAGIC:
			buildMAGIC(instruments[instrumentAllocation[port]]);
			break;
		case TREX:
			buildTREX(instruments[instrumentAllocation[port]]);
			break;
		case HEIMDAL:
			buildHEIMDAL(instruments[instrumentAllocation[port]]);
			break;
		case MIRACLES:
			buildMIRACLES(instruments[instrumentAllocation[port]]);
			break;

		case SKADI :	//EMPTY
			buildSKADI(instruments[instrumentAllocation[port]]);
			break;
		case ODIN :	//EMPTY
			buildODIN(instruments[instrumentAllocation[port]]);
			break;
		case FUND :	//EMPTY
			buildFUND(instruments[instrumentAllocation[port]]);
			break;
		case ESTIA :	//EMPTY
			buildESTIA(instruments[instrumentAllocation[port]]);
			break;
		case VOR :	//EMPTY
			buildVOR(instruments[instrumentAllocation[port]]);
			break;
		case FREIA :
			buildFREIA(instruments[instrumentAllocation[port]]);
			break;
		case SLEIPNIR :
			buildSLEIPNIR(instruments[instrumentAllocation[port]]);
			break;
		case LOKI :
			buildLOKI(instruments[instrumentAllocation[port]]);
			break;
		case WANSE :
		  buildWANSE(instruments[instrumentAllocation[port]]);
		  break;
		case ESSENSE:
		  buildESSENSE(instruments[instrumentAllocation[port]]);
		  break;
		case VESPA:
		  buildVESPA(instruments[instrumentAllocation[port]]);
		  break;
		case ANNI:
		  buildANNI(instruments[instrumentAllocation[port]]);
		  break;
		case SURFACE_SCATTERING:
		  buildSURFACESCATTERING(instruments[instrumentAllocation[port]]);
		  break;
		default :	//EMPTY
		  buildEmpty(instruments[instrumentAllocation[port]]);
		  break;
		}
	}
}








//////////////////////////////////////////////////////////
//														//
//			Scoring & Figure of Merit Functions			//
//														//
//////////////////////////////////////////////////////////


int Sector::countHardInterferencesDelegated(bool verbose)
{
	int port;
	int icheck;
	int c;

	Instrument* instfocus;
	Instrument* instcheck;



	bool collision=false;

	int num_collisions=0;

	if(instrumentsDistributed == false)
	{
		cerr << "Attempt to analyse sector without distributing the instruments first." << endl;
	}


	for(port=0; port < 12; port++)
	{
		instfocus = & instruments[instrumentAllocation[port]];

		//Can double count for mutual interferences, but we have to check both directions rather than left only
		for(icheck = 0; icheck < 12; icheck++)
		{


			// Don't check a beam with itself.   Ignore empty beams and a beam that is outside the sector arc.
			if( (icheck != port) && instfocus->get_beam_centre() <= 60.0)
			{

				instcheck = & instruments[instrumentAllocation[icheck]];

				for(c=0; c < instcheck->get_num_choppers(); c++)
				{
					collision = instruments[instrumentAllocation[port]].checkChopperInterference(instruments[instrumentAllocation[icheck]].cblock[c]);
					if(collision)
						num_collisions++;
				}
			}
		}
	}
	return(num_collisions);
}




// TODO - Add count soft interferences, which prevent the guides from colliding with empty beamlines.  Cross-check this with "future instrument possibility" functions.

int Sector::countEmptyInterferencesDelegated(bool verbose)
{
  //Counts interferences with an empty beamline - prevents future instruments of course
	int port;
	int icheck;
	int c;
	int g;

	Instrument* instfocus;
	Instrument* instcheck;



	bool collision=false;

	int num_collisions=0;

	if(instrumentsDistributed == false)
	{
		cerr << "Attempt to analyse sector without distributing the instruments first." << endl;
	}


	for(port=0; port < 12; port++)
	{
		instfocus = & instruments[instrumentAllocation[port]];

		if( (instfocus->id == EMPTY) || (instfocus->id == EMPTY2) || (instfocus->id == EMPTY3) || (instfocus->id == EMPTY4) || (instfocus->id == EMPTY5) || (instfocus->id == EMPTY6) || (instfocus->id == EMPTY7) || (instfocus->id == EMPTY8) || (instfocus->id == EMPTY9) )
		  {

		//Can double count for mutual interferences, but we have to check both directions rather than left only
		for(icheck = 0; icheck < 12; icheck++)
		{


			// Don't check a beam with itself.  Don't check collisions with a beam that is outside the sector arc - we don't give a shit about non-existent beams!
			if( (icheck != port) && instfocus->get_beam_centre() <= 60.0)
			{
			  instcheck = & instruments[instrumentAllocation[icheck]];
			  

			      
			  			      
			      //First check the neighbour's choppers
			      for(c=0; c < instcheck->get_num_choppers(); c++)
				{
				  //Write this out in full because we are doing pass by reference, so that the blocks get tagged if they interefere
				  collision = instruments[instrumentAllocation[port]].checkGuideInterference(instruments[instrumentAllocation[icheck]].cblock[c]);
				  
				  if(collision)
				    {
				      num_collisions++;
				    }
				}
			      
			      //Now check the neighbour's guide pieces
			      for(g=0; g < instcheck->get_num_guideBlocks(); g++)
				{
				  //Write this out in full because we are doing pass by reference, so that the blocks get tagged if they interefere
				  collision = instruments[instrumentAllocation[port]].checkGuideInterference(instruments[instrumentAllocation[icheck]].gblock[g]);
				  
				  if(collision)
				    {
				      num_collisions++;
				    }
				}
			    }
		}
		  }
	}
	return(num_collisions);
}
  


int Sector::countGuideInterferencesDelegated(bool verbose)
{
	int port;
	int icheck;
	int c;
	int g;

	Instrument* instfocus;
	Instrument* instcheck;



	bool collision=false;

	int num_collisions=0;

	if(instrumentsDistributed == false)
	{
		cerr << "Attempt to analyse sector without distributing the instruments first." << endl;
	}


	for(port=0; port < 12; port++)
	{
		instfocus = & instruments[instrumentAllocation[port]];

		if( (instfocus->id != EMPTY) && (instfocus->id != EMPTY2) && (instfocus->id != EMPTY3) && (instfocus->id != EMPTY4) && (instfocus->id != EMPTY5) && (instfocus->id != EMPTY6) && (instfocus->id != EMPTY7) && (instfocus->id != EMPTY8) && (instfocus->id != EMPTY9) )
		  {

		//Can double count for mutual interferences, but we have to check both directions rather than left only
		for(icheck = 0; icheck < 12; icheck++)
		{


			// Don't check a beam with itself.  Don't check collisions with a beam that is outside the sector arc - we don't give a shit about non-existent beams!
			if( (icheck != port) && instfocus->get_beam_centre() <= 60.0)
			{
			  instcheck = & instruments[instrumentAllocation[icheck]];
			  
			  if( (instcheck->id != EMPTY) && (instcheck->id != EMPTY2) && (instcheck->id != EMPTY3) && (instcheck->id != EMPTY4) && (instcheck->id != EMPTY5) && (instcheck->id != EMPTY6) && (instcheck->id != EMPTY7) && (instcheck->id != EMPTY8) && (instcheck->id != EMPTY9) )
			    {
			      
			  			      
			      //First check the neighbour's choppers
			      for(c=0; c < instcheck->get_num_choppers(); c++)
				{
				  //Write this out in full because we are doing pass by reference, so that the blocks get tagged if they interefere
				  collision = instruments[instrumentAllocation[port]].checkGuideInterference(instruments[instrumentAllocation[icheck]].cblock[c]);
				  
				  if(collision)
				    {
				      num_collisions++;
				    }
				}
			      
			      //Now check the neighbour's guide pieces
			      for(g=0; g < instcheck->get_num_guideBlocks(); g++)
				{
				  //Write this out in full because we are doing pass by reference, so that the blocks get tagged if they interefere
				  collision = instruments[instrumentAllocation[port]].checkGuideInterference(instruments[instrumentAllocation[icheck]].gblock[g]);
				  
				  if(collision)
				    {
				      num_collisions++;
				    }
				}
			    }
			}
		}
		  }
	}
	return(num_collisions);
}








int Sector::countWallInterferencesDelegated(bool verbose)
{
	int wall;
	int icheck;
	int c;
	int g;

	Instrument* instcheck;



	bool collision=false;

	int num_collisions=0;

	if(instrumentsDistributed == false)
	{
		cerr << "Attempt to analyse sector wall interferences without distributing the instruments first." << endl;
	}


	for(wall=0; wall < numWalls; wall++)
	{

		//Can double count for mutual interferences, but we have to check both directions rather than left only
		for(icheck = 0; icheck < 12; icheck++)
		{

				instcheck = & instruments[instrumentAllocation[icheck]];

				//Ignore emtpy beams and higher tranche instruments

				if( (instcheck->id != EMPTY) && (instcheck->id != EMPTY2) && (instcheck->id != EMPTY3) && (instcheck->id != EMPTY4) && (instcheck->id != EMPTY5) && (instcheck->id != EMPTY6) && (instcheck->id != EMPTY7) && (instcheck->id != EMPTY8) && (instcheck->id != EMPTY9) && (instcheck->tranche < 3) )
				{
				//First check the instrument choppers
				for(c=0; c < instcheck->get_num_choppers(); c++)
				{
					//Write this out in full because we are doing pass by reference, so that the blocks get tagged if they interefere
					collision = wblock[wall].checkInterference(instruments[instrumentAllocation[icheck]].cblock[c]);

					if(collision)
					{
						num_collisions++;
						wblock[wall].setCollisionFlag(true);
						instruments[instrumentAllocation[icheck]].cblock[c].setCollisionFlag(true);
					}
				}

				//First check the instrument choppers
				for(g=0; g < instcheck->get_num_guideBlocks(); g++)
				{
					//Write this out in full because we are doing pass by reference, so that the blocks get tagged if they interefere
					collision = wblock[wall].checkInterference(instruments[instrumentAllocation[icheck]].gblock[g]);

					if(collision)
					{
						wblock[wall].setCollisionFlag(true);
						instruments[instrumentAllocation[icheck]].gblock[g].setCollisionFlag(true);
						num_collisions++;
					}
				}
				}
		}
	}
	return(num_collisions);
}





double Sector::scoreLateralShielding(bool verbose)
{
	int portref;
	int portcheck;
	int c;

	double minShielding=0.0;
	double distance=0.0;
	double score = 0.0;
	double scorept;

	Instrument* instfocus;
	Instrument* instcheck;



	if(instrumentsDistributed == false)
	{
		cerr << "Attempt to scoreLateralShielding in sector.cpp without distributing the instruments first." << endl;
	}


	if(verbose)
	{
		cout << "Lateral\t\ti\tj\tPair\tNChop\t\tSens1\tSens2\tScore" << endl;
	}

	for(portref=0; portref < 12; portref++)
	{
		instfocus = & instruments[instrumentAllocation[portref]];

		//Forget empty beams
		if( (instfocus->id != EMPTY) && (instfocus->id != EMPTY2) && (instfocus->id != EMPTY3) && (instfocus->id != EMPTY4) && (instfocus->id != EMPTY5) && (instfocus->id != EMPTY6) && (instfocus->id != EMPTY7) && (instfocus->id != EMPTY8) && (instfocus->id != EMPTY9) )

		  {

		//Can double count for mutual interferences, but we have to check both directions rather than left only
		for(portcheck = 0; portcheck < 12; portcheck++)
		{

		  instcheck = & instruments[instrumentAllocation[portcheck]];

			if(
			  (portcheck != portref) // Don't check a beam with itself.
			  && (instfocus->get_beam_centre() <= 60.0) //Don't check collisions with a beam that is outside the sector arc
			  && ( (portcheck - portref <= 2) || (portref-portcheck <= 2) ) // Only check neighbours and next-near neighbours.
			  && ( (instcheck->id != EMPTY) && (instcheck->id != EMPTY2) && (instcheck->id != EMPTY3) && (instcheck->id != EMPTY4) && (instcheck->id != EMPTY5) && (instcheck->id != EMPTY6) && (instcheck->id != EMPTY7) && (instcheck->id != EMPTY8) && (instcheck->id != EMPTY9) ) // Forget empty beams
			)
			{

				minShielding = 100.0;


				//Check icheck's choppers to instfocus guides and choppers
				for(c=0; c < instcheck->get_num_choppers(); c++)
				{
					distance = instfocus->getMinimumSeparation(instcheck->cblock[c]);

					if(distance < minShielding)
					{
						minShielding = distance;
					}
				}

//				//Check icheck's guides to instfocus guides and choppers
//				for(g=0; g < instcheck->get_num_guideBlocks(); g++)
//				{
//					//Write this out in full because we are doing pass by reference, so that the blocks get tagged if they interefere
//					distance = instruments[instrumentAllocation[port]].getMinimumSeparation(instruments[instrumentAllocation[icheck]].gblock[g]);
//
//					if(distance < minShielding)
//					{
//						minShielding = distance;
//					}
//				}

				//Badness score: goes up with sensitivity rating, goes down with increasing distance
				if(distance < 0.001)
					scorept = 1000.0;
				else
				  scorept = instfocus->backgroundSensitivityRating() / distance; // 1/R dependence approximately, for extended sources
				if(verbose)
					{
						cout << " " << distance << "\t" << portref << "\t" << portcheck << "\t" << instNames[instfocus->id] << "-" << instNames[instcheck->id] << "\t" << instcheck -> get_num_choppers() << "\t" << instfocus->backgroundSensitivityRating() << "\t" << instcheck->backgroundSensitivityRating() << "\t" << scorept << endl;
					}

				score = score + scorept;

			}
		}
		  }
	}

	if(verbose)
		cout << "   Total potential lateral shielding / background score (larger is worse):  " << score << endl;

	//Normalise to 10 roughly like the other parameters (tends to produce results in 100s

	score = score / 10.0;

	return(score);
}



int Sector::countDuplicatesQuiet(void)
{
	return (countDuplicates(false));
}


int Sector::countDuplicates(bool verbose)
{
	int i;
	int inst;
	int id;
	int checkid;

	int duplicates=0;

	for(inst=0; inst<12; inst++)
	{
		id = instruments[inst].id;

		for(i=0; i<12; i++)
		{
			checkid = instruments[i].id;

			if( (checkid != EMPTY) && (checkid != EMPTY2) && (checkid != EMPTY3) && (checkid != EMPTY4) && (checkid != EMPTY5) && (checkid != EMPTY6) && (checkid != EMPTY7) && (checkid != EMPTY8) && (checkid != EMPTY9) && (i != inst) )
			{
				if(checkid == id)
				{
					if(verbose)
					{
						cout << "Duplicate instrument " << instNames[checkid] << " = " << instNames[id] << endl;
					}
					duplicates++;
				}
			}
		}
	}
	return(duplicates);
}



int Sector::scoreFixedInstruments(bool verbose)
{
	int i;
	int inst;
	int id;
	int checkid;

	int score=0;

	
	for(inst=0; inst<12; inst++)
	{
		id = instruments[inst].id;

		if(fix_nmx || fix_all_instruments)
		  {
		    //Fix NMX at the first beam port
		    if(id == NMX)
		      {
			if(instruments[inst].get_beam_centre() > 1.0)
			  score+=1;
		      }
		  }
		if(fix_beer || fix_all_instruments)
		  {
		    //Fix BEER near the 2nd - 4th beam port
		    if(id == BEER)
		      {
			if(instruments[inst].get_beam_centre() < 5.0 || instruments[inst].get_beam_centre() > 20.0)
			  score +=1;
		      }
		  }
	}

	
	return(score);
}





int Sector::countInstruments()
{
	int i;

	int count=0;

	for(i=0; i<12; i++)
	{
		if(instruments[i].get_beam_centre() <= sectorArc)
			if( (instruments[i].id != EMPTY) && (instruments[i].id != EMPTY2) && (instruments[i].id != EMPTY3) && (instruments[i].id != EMPTY4) && (instruments[i].id != EMPTY5)  && (instruments[i].id != EMPTY6) && (instruments[i].id != EMPTY7) && (instruments[i].id != EMPTY8) && (instruments[i].id != EMPTY9))
				count++;
	}

	return(count);
}




int Sector::maxPossibleInstruments()
{
	int i;

	int count=0;

	for(i=0; i<12; i++)
	{
		if(instruments[i].get_beam_centre() <= sectorArc)
				count++;
	}

	return(count);
}



int Sector::countInstruments(int test_tranche)
{
	int i;

	int count=0;

	for(i=0; i<12; i++)
	{
		if( (instruments[i].get_beam_centre() <= sectorArc) && (instruments[i].tranche == test_tranche ) )
			if( (instruments[i].id != EMPTY) && (instruments[i].id != EMPTY2) && (instruments[i].id != EMPTY3) && (instruments[i].id != EMPTY4) && (instruments[i].id != EMPTY5)  && (instruments[i].id != EMPTY6) && (instruments[i].id != EMPTY7) && (instruments[i].id != EMPTY8) && (instruments[i].id != EMPTY9))
				count++;
	}

	return(count);
}




double Sector::scoreSpallationBackground(void)
{
  
  // This function returns the strength of the background coming directly from spallation in the target, and illuminating the beam ports.
  //   At most spallation sources, the high energy background is higher in the forwards direction.  At ESS, the large target wheel shields the forwards beam ports, so the largest backgrounds are in the backwards direction.
  
  
  
	int i;

	double ans=0.0;
	double portAngle;
	int sensitivity;

	//Leave the switch statement in for now.  It's possible (depending on how the beam port angles are defined) that the same code can be used for each sector.  Find out later...
	switch(sectorToStudy)
	{
	case EAST_SECTOR:
	  // Larger angles are better than smaller angles (further away from the proton beam)
	  for(i=0; i<12; i++)
	    {
	      portAngle = instruments[i].get_beam_centre();
	      sensitivity = instruments[i].backgroundSensitivityRating();
	      ans = ans + 90.0-portAngle * (double)sensitivity;
	    }
	  break;
	  
	default:
	  //West sector - smaller angles are better, self shielded by the proton wheel
	  //North sector - larger angles are worse than smaller angles (closer to the proton beam)
	  for(i=0; i<12; i++)
	    {
	      portAngle = instruments[i].get_beam_centre();
	      sensitivity = instruments[i].backgroundSensitivityRating();
	      ans = ans + portAngle * (double)sensitivity;
	    }
	  break;
	}
	
	//Normalise to per-beamport score
	ans = ans / 12.0;

	//Normalise to a zero-to-one score to put on same scale as the others.
	ans = ans / 60.0;

	return(ans);
}







double Sector::scoreNoisyNeighbours(void)
{
	int port;
	int icheck;

	Instrument* instfocus;
	Instrument* instcheck;

	double separation;
	double myAngle;
	double theirAngle;

	double noiseSensitivity;
	double neighbourNoisiness;

	double ans=0;


	if(instrumentsDistributed == false)
	{
		cerr << "Attempt to analyse noisy neighbours in sector without distributing the instruments first." << endl;
	}


	for(port=0; port < 12; port++)
	{
		instfocus = & instruments[instrumentAllocation[port]];

		//Can double count for mutual interferences, but we have to check both directions rather than left only
		for(icheck = 0; icheck < 12; icheck++)
		{

			// Don't check a beam with itself.  Don't check collisions with a beam that is outside the sector arc - we don't give a shit about non-existent beams!
			if( (icheck != port) && instfocus->get_beam_centre() <= 60.0)
			{

				instcheck = & instruments[instrumentAllocation[icheck]];

				myAngle = instfocus -> get_beam_centre();
				noiseSensitivity = instfocus -> backgroundSensitivityRating();

				theirAngle = instcheck -> get_beam_centre();
				neighbourNoisiness = instcheck -> noisinessRating();

				separation = myAngle - theirAngle;

				if(separation < 0.0)
					separation = -separation;

				ans = ans + noiseSensitivity * neighbourNoisiness / separation;	//Roughly 1/R for extended sources
			}
		}
	}

	//Normalise to per-instrument score
	ans = ans / 12.0;

	//Normalise to an "out of ten" score to put on same scale as the others.
	ans = ans / 70.0;

	return(ans);
}








//////////////////////////////////////////////////////////
//														//
//						Visualisation					//
//														//
//////////////////////////////////////////////////////////

void Sector::declareEukleides(std::ofstream& euk)
{
	int port;
	int wall;
	int i;

	const double northRadius = 15.0;
	const double eastRadius = 15.0;
	const double southRadius = 20.0;
	const double westRadius = 25.0;

	Instrument inst;

	euk << "bunkeredge = circle(O, ";

	switch(sectorToStudy)
	{
	case NORTH_SECTOR:
		euk << northRadius << ")" << endl;
		break;
	case EAST_SECTOR:
		euk << eastRadius << ")" << endl;
		break;
	case SOUTH_SECTOR:
		euk << southRadius << ")" << endl;
		break;
	case WEST_SECTOR:
		euk << westRadius << ")" << endl;
		break;
	default:
		euk << eastRadius << ")" << endl;
		break;
	}

	for(wall=0; wall < numWalls; wall++)
	{
		for(i=0; i<4; i++)
		{
			euk << "wall" << wall << "p" << i << " = point(" << wblock[wall].getPoint(i).getx() << ", " << wblock[wall].getPoint(i).gety() << ")" << endl;
		}

		euk << "wall" << wall << " = ";
		for(i=0; i<4; i++)
		{
			euk << "wall" << wall << "p" << i << ".";
		}
		euk << "wall" << wall << "p0" << endl;
	}

	for(port=0; port<12; port++)
	{
		inst = getInstrumentOnPort(port);
		inst.declareEukleides(euk);
	}
}

void Sector::drawEukleides(std::ofstream& euk)
{
	int port;
	int wall;

	Instrument inst;

	euk << "  bunkeredge 90 deg 190 deg black" << endl;

	for(wall=0; wall < numWalls; wall++)
	{
		if(!wblock[wall].hasCollisions())
			euk << "  wall" << wall << " " << "black" << std::endl;
		else
			euk << "  wall" << wall << " " << "red" << std::endl;
	}

	for(port=0; port<12; port++)
	{
		inst = getInstrumentOnPort(port);
		inst.drawEukleides(euk);
	}

}


void Sector::drawPovray(std::ofstream& pov)
{
	int port;
	int wall;
	int height = 12;

	double bunkerR;

	std::string colour;
	colour = "rgb <0.5, 0.5, 0.5>";
	std::string red;
	red = "Red";

	Instrument inst;

	pov << "//Target Monolith:" << endl;
	pov << "cylinder{ <0,-2,0>, <0,12,0>, 6  pigment { rgbf <0.5, 0.5, 0.5, 0.5> }}" << endl;

	switch(sectorToStudy)
	  {
	  case WEST_SECTOR:
	    bunkerR=24.5;
	    break;
	  default:
	    bunkerR=14.0;
	    break;
	  }

	pov << "//Bunker:" << endl;
	pov << "difference {" << endl;
	pov << "cylinder{ <0,-2,0>, <0,2,0>, " << bunkerR << "+3.2}" << endl;
	pov << "cylinder{ <0,-3,0>, <0,4,0>, " << bunkerR << "}" << endl;
	pov << "pigment{rgbf <0.5, 0.5, 1.0, 0.95>}}" << endl;

	
	for(wall=0; wall < numWalls; wall++)
	{
		if(!wblock[wall].hasCollisions())
		  wblock[wall].drawPovray(pov, colour, height);
		else
		  wblock[wall].drawPovray(pov, red, height);
	}

	
	for(port=0; port<12; port++)
	{
		inst = getInstrumentOnPort(port);
		inst.drawPovray(pov);
	}

}



void Sector::aimThermal(Instrument& inst)
{
	// Negative translation moves to the left, positive moves to the right
	// Negative rotation goes anti-clockwise, positive goes clockwise


	switch(sectorToStudy)
		{
	// case WEST_SECTOR:
	default :
		inst.translateFrame(Point(0.03,0.0));
		inst.rotateFrame(-1.1);

		}


}

void Sector::aimCold(Instrument& inst)
{
	// Negative translation moves to the left, positive moves to the right
	// Negative rotation goes anti-clockwise, positive goes clockwise

	switch(sectorToStudy)
	{
	default :
		inst.translateFrame(Point(-0.03,0.0));
		inst.rotateFrame(1.1);
	}
}




