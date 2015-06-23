/*
 * Pattern.cpp
 *
 *  Created on: 08.06.2015
 *      Author: christoph
 */

#include "Pattern.h"
using namespace std;
Pattern::Pattern(ledscape_frame_t* iframe, ledscape_pixel_t* icolors, char* iColorOrder, int nBars, int nLedsProBar) {
	// TODO Auto-generated constructor stub
	ColorOrder = iColorOrder;
	colors = icolors;
	frame = iframe;
	deltat = 5;
	bpm = 500;
	counter = 0;
	numBars = nBars;
	numLedsProBar = nLedsProBar;
	numLeds = nBars*nLedsProBar;
	targetStrip = 1;
	power = 1;
	color = 1;
	type = 0;
	speed = 100;
	targetCycles = bpm/deltat;
}
Pattern::Pattern(){

}

Pattern::~Pattern() {
	// TODO Auto-generated destructor stub
}

void Pattern::beat(int ideltat, double ibpm, double ipower, int iStrip, int iColor, int ispeed, int itype){
	color = iColor;
	counter = 0;
	deltat = ideltat;
	bpm = ibpm;

	targetStrip = iStrip;
	targetCycles = bpm/deltat;
	speed = ispeed;
	type = itype;
	power = ipower;
	event();

}
void Pattern::noBeat(){
	counter++;
	noEvent();
}

double Pattern::linearApp(double amp1, double amp2, double deltax, double x){
	double a = (((amp2-amp1)/deltax)*x + amp1);
	if(a < 0)
		a = 0;

	return a;
}
