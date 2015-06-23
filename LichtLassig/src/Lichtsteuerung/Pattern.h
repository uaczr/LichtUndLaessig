/*
 * Pattern.h
 *
 *  Created on: 08.06.2015
 *      Author: christoph
 */

#ifndef LICHTSTEUERUNG_PATTERN_H_
#define LICHTSTEUERUNG_PATTERN_H_
#include <iostream>
#include <queue>
extern "C"{
#include "../Ledscape/include/ledscape.h"
}
using namespace std;
struct pixel{
	bool active;
	int color;

	pixel(){
		active = false;
		color = 0;
	}
	pixel operator=(pixel a){
		a.active = active;
		a.color = color;
		return a;
	}
};
class Pattern {
protected:
	int targetStrip;
	int numLeds;
	int numLedsProBar;
	int numBars;
	int deltat;
	double bpm;
	int counter;
	int targetCycles;
	int speed;
	int type;
	int color;
	double power;
	ledscape_frame_t* frame;
	ledscape_pixel_t* colors;
	char* ColorOrder;
	virtual void event() = 0;
	virtual void noEvent() = 0;
	double linearApp(double amp1, double amp2, double deltax, double x);

public:
	Pattern();
	Pattern(ledscape_frame_t* iframe , ledscape_pixel_t* icolors, char* iColorOrder, int nBars, int nLedsProBar);
	virtual ~Pattern();
	void beat(int ideltat, double ibpm, double ipower, int iStrip, int iColor, int speed, int type);
	void noBeat();
};

#endif /* LICHTSTEUERUNG_PATTERN_H_ */
