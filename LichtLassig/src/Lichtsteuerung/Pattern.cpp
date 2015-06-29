/*
 * Pattern.cpp
 *
 *  Created on: 08.06.2015
 *      Author: christoph
 */

#include "Pattern.h"
using namespace std;
Pattern::Pattern(ledscape_frame_t* iframe, ledscape_pixel_t* icolors,
		char* iColorOrder, int nBars, int nLedsProBar) {
	// TODO Auto-generated constructor stub
	ColorOrder = iColorOrder;
	colors = icolors;
	frame = iframe;
	deltat = 5;
	bpm = 500;
	counter = 0;
	numBars = nBars;
	numLedsProBar = nLedsProBar;
	numLeds = nBars * nLedsProBar;
	targetStrip = 1;
	power = 1;
	color = 1;
	type = 0;
	speed = 100;
	targetCycles = bpm / deltat;
	targetBeats = 0;
	pliste = new pixel[nLedsProBar];
	for (int i = 0; i < nLedsProBar; i++) {
		pliste[i].active = false;
		pliste[i].color = 0;
	}
}
Pattern::Pattern() {

}

Pattern::~Pattern() {
	// TODO Auto-generated destructor stub
}

void Pattern::beat(int ideltat, double ibpm, double ipower, int iStrip,
		int iColor, int ispeed, int itype) {
	color = iColor;
	counter = 0;
	deltat = ideltat;
	bpm = ibpm;
	targetStrip = iStrip;
	targetCycles = bpm / deltat;
	speed = ispeed;
	type = itype;
	power = ipower;
	event();

}
void Pattern::noBeat(int iColor, int ispeed, int itype) {
	counter++;
	speed = ispeed;
	type = itype;
	color = iColor;
	noEvent();
}

double Pattern::linearApp(double amp1, double amp2, double deltax, double x) {
	double a = (((amp2 - amp1) / deltax) * x + amp1);
	if (a < 0)
		a = 0;

	return a;
}

double Pattern::dmmApp(double amp1, double amp2, double deltax, double x){
	double amp = amp1-amp2;
	double a = 0.3;
	double b = 0.1;
	//cout << amp << " "<< a*amp << " " << (double)1/3*deltax << " " << x << endl;
	if(x < (double)1/3*deltax){
		return linearApp(amp, a*amp, 1/3*deltax, x);
	}
	if(x < (double)2/3*deltax){
		return linearApp(a * amp, b*amp, 1/3*deltax, x-1/3*deltax);
	}
	if(x < deltax){
		return linearApp(b * amp, 0, 1/3*deltax, x-2/3*deltax);
	}
	return 0;

}
double Pattern::linearAppPM(double amp1, double amp2, double deltax, double x) {
	double a = (((amp2 - amp1) / deltax) * x + amp1);

	return a;
}
void Pattern::drawEqual() {
	for (int i = 0; i < numBars; i++) {
		for (int j = 0; j < numLedsProBar; j++) {
			//cout << pliste[j].active <<" ";
			if (pliste[j].active == true) {

				ledscape_set_color(frame,
						color_channel_order_from_string(ColorOrder),
						targetStrip, i * numLedsProBar + j,
						colors[pliste[j].color].a, colors[pliste[j].color].b,
						colors[pliste[j].color].c);
			}
		}
		//cout << endl;
	}
}
void Pattern::drawColorEqual() {
	for (int i = 0; i < numBars; i++) {
		for (int j = 0; j < numLedsProBar; j++) {

			if (pliste[j].active) {
				ledscape_set_color(frame,
						color_channel_order_from_string(ColorOrder),
						targetStrip, i * numLedsProBar + j, pliste[j].r,
						pliste[j].g, pliste[j].b);
			}
		}
	}
}
