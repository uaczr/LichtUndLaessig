/*
 * Wrapper2D.h
 *
 *  Created on: 31.05.2015
 *      Author: christoph
 */

#ifndef WRAPPER2D_H_
#define WRAPPER2D_H_

#include "Wrapper2DPixel.h"
#include "../../CSV/csv.h"
#include <string>
#include <vector>
#include "../Ledscape/include/ledscape.h"



using namespace std;
struct Wrapper2Dfield{
	double x0;
	double x1;
	double y0;
	double y1;
	Wrapper2DPixel* first;
};

class Wrapper2D {
private:
	//Höhe und Breite des Raumes in Metern
	double width;
	double length;
	int m, n;

	//Auflösung in x und y
	int xres,yres;
	//WSControll wscontroller;

	//2D Raumraster
	Wrapper2Dfield** raster;

	void initFromCSV(string path);

public:
	Wrapper2D(string path);
	virtual ~Wrapper2D();
};

#endif /* WRAPPER2D_H_ */
