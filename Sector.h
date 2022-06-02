/*
 * Sector.h
 *
 *  Created on: 3 Dec 2014
 *      Author: phillipbentley
 */

#include "Instrument.h"
#include "Block.h"

#ifndef SECTOR_H_
#define SECTOR_H_



class Sector {
public:
	Sector();
	Sector(double arcLength);
	virtual ~Sector();

	void addInstrument(Instrument i);

	void setAngle1(double angle1);
	void setAngle2(double angle2);
	void distributeInstruments(double parameters[]);

	void deployInstruments(void);


	//int countHardInterferences(bool verbose);
	int countHardInterferencesDelegated(bool verbose);

	//int countGuideInterferences(bool verbose);
	//int countFutureGuideInterferences(bool verbose);
	int countEmptyInterferencesDelegated(bool verbose);
	int countGuideInterferencesDelegated(bool verbose);
	int countWallInterferencesDelegated(bool verbose);

	int scoreFixedInstruments(bool verbose);

	
	double scoreLateralShielding(bool verbose);


	int countDuplicatesQuiet(void);
	int countDuplicates(bool verbose);
	int countInstruments();
	int countInstruments(int tranche);
	int maxPossibleInstruments(void);


	double scoreSpallationBackground(void);
	double scoreNoisyNeighbours(void);

	double getArcLength(void);
	void setArcLength(const double arc);

	int instAllocation(int rank);

	void listPortAllocations(void);

	void declareEukleides(std::ofstream& os);
	void drawEukleides(std::ofstream& os);
	void drawPovray(std::ofstream& os);

	Instrument instruments[12];

	Instrument& getInstrumentOnPort(int num);

	Sector& operator=(const Sector& source);

private:

	int num_instruments;

	int numWalls;

	double angle1;
	double angle2;
	double beamCentre[12];
	double sectorArc;
	double maximumThrow;

	bool instrumentsDistributed;

	int instrumentAllocation[12];

	Block wblock[100];

	void buildBEER(Instrument& inst);
	void buildCAMEA(Instrument& inst);
	void buildCSPEC(Instrument& inst);
	void buildEmpty(Instrument& inst);
	void buildESTIA(Instrument& inst);
	void buildFUND(Instrument& inst);
	void buildHEIMDAL(Instrument& inst);
	void buildMAGIC(Instrument& inst);
	void buildMIRACLES(Instrument& inst);
	void buildNMX(Instrument& inst);
	void buildESSENSE(Instrument& inst);
	void buildODIN(Instrument& inst);
	void buildSKADI(Instrument& inst);
	void buildSLEIPNIR(Instrument& inst);
	void buildLOKI(Instrument& inst);
	void buildTREX(Instrument& inst);
	void buildVOR(Instrument& inst);
	void buildFREIA(Instrument& inst);
	void buildWANSE(Instrument& inst);
	void buildVESPA(Instrument& inst);
	void buildANNI(Instrument& inst);
	void buildSURFACESCATTERING(Instrument& inst);

	void aimThermal(Instrument& inst);
	void aimCold(Instrument& inst);

};

#endif /* SECTOR_H_ */
