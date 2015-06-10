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
class Pattern {
protected:
	int targetStrip;
	int numLeds;
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

public:
	Pattern();
	Pattern(ledscape_frame_t* iframe , ledscape_pixel_t* icolors, char* iColorOrder);
	virtual ~Pattern();
	void beat(int ideltat, double ibpm, double ipower, int iStrip, int iLeds, int iColor, int speed, int type);
	void noBeat();
};

#endif /* LICHTSTEUERUNG_PATTERN_H_ */
