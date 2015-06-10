/*
 * Pattern.cpp
 *
 *  Created on: 08.06.2015
 *      Author: christoph
 */

#include "Pattern.h"
using namespace std;
Pattern::Pattern(ledscape_frame_t* iframe, ledscape_pixel_t* icolors, char* iColorOrder) {
	// TODO Auto-generated constructor stub
	ColorOrder = iColorOrder;
	colors = icolors;
	frame = iframe;
	deltat = 5;
	bpm = 500;
	counter = 0;
	numLeds = 100;
	targetStrip = 1;
	power = 1;
	targetCycles = bpm/deltat;
}
Pattern::Pattern(){

}

Pattern::~Pattern() {
	// TODO Auto-generated destructor stub
}

void Pattern::beat(int ideltat, double ibpm, double ipower, int iStrip, int iLeds, int iColor, int ispeed, int itype){
	color = iColor;
	counter = 0;
	deltat = ideltat;
	bpm = ibpm;
	numLeds = iLeds;
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
