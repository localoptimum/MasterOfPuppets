#include <string>
#include "monte_carlo.h"
#include "Matrix.h"

#ifndef GLOBALS_H_

#define GLOBALS_H_

//Global variable.  This needs to be updated on the fly for some problems
extern int num_params;
extern int mode;

const extern bool exaggerate_geometry;
const extern bool fix_nmx;
const extern bool fix_beer;
const extern bool fix_all_instruments;
const extern bool genericInstruments;

extern monte_carlo* mc;

const extern Matrix unitMatrix;


//Enumerate sectors
enum{NORTH_SECTOR, EAST_SECTOR, SOUTH_SECTOR, WEST_SECTOR};

extern const int sectorToStudy;

//Enumerate curvature directions
enum{CURVE_LEFT, CURVE_RIGHT};

//Noisiness
enum{QUIET, NOISY, VERY_NOISY};

//Sensitivity
enum{NOT_SENSITIVE, SENSITIVE, VERY_SENSITIVE};



//Enumerate instrument types

enum {EMPTY, BEER, NMX, CAMEA, CSPEC, MAGIC, TREX, HEIMDAL, MIRACLES, EMPTY2, EMPTY3, EMPTY4, ESSENSE, SKADI, FREIA, ODIN, FUND, ESTIA, DIFF, VOR, LOKI, DREAM, VESPA, HOD, SLEIPNIR, SURFACE_SCATTERING, WANSE, ANNI, EMPTY5, EMPTY6, EMPTY7, EMPTY8, EMPTY9};
const std::string instNames[] = {"EMPTY", "BEER",  "NMX", "CAMEA", "CSPEC", "MAGIC", "TREX",   "HEIMDAL", "MIRACLES", "EMPTY2", "EMPTY3", "EMPTY4", "ESSENSE", "SKADI", "FREIA",  "ODIN", "FUND",   "ESTIA", "DIFF",  "VOR",    "LOKI", "DREAM", "VESPA", "HOD", "SLEIPNIR", "SURFACE_SCATTERING", "WANSE", "ANNI", "EMPTY5", "EMPTY6", "EMPTY7", "EMPTY8", "EMPTY9"};
const std::string codeNames[] = {"EMPTY", "UINEN", "FUI", "NESSA", "VANA",  "VAIRE", "TULKAS", "LORIEN",  "MANDOS",   "EMPTY2", "EMPTY3", "EMPTY4", "ESTE",   "OROME", "NIENNA", "AULE", "MELKOR", "VARDA", "MANWE", "YAVANNA", "ULMO", "OSSE",  "WWWWW", "HHH", "SSSSSSSS", "RRRRRRRRRRRRRRRRRR", "WANSE", "MMMMM", "EMPTY5", "EMPTY6", "EMPTY7", "EMPTY8", "EMPTY9"};

//Enum of chopper movement variables
enum{
  //0,1 are angles
  //2,13 are beam ports
  C1 = 14,
  C2 = 15,
  C3 = 16,
  C4 = 17,
  C5 = 18,
  C6 = 19,
  C7 = 20,
};

#endif
