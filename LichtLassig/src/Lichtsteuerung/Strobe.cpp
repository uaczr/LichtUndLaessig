/*
 * Strobe.cpp
 *
 *  Created on: 09.06.2015
 *      Author: christoph
 */

#include "Strobe.h"

Strobe::Strobe(ledscape_frame_t* iframe , ledscape_pixel_t* icolors, char* iColorOrder, int nBars, int nLedsProBar) {
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
	n = 0;
}

Strobe::~Strobe() {
	// TODO Auto-generated destructor stub
}

void Strobe::event(){
	n = 0;
	switch(type){
	case 0:
		tripleShot();
		break;

	}
}

void Strobe::noEvent(){
	switch(type){
	case 0:
		tripleShot();
		break;

	}
}

void Strobe::tripleShot(){
	if(counter%speed <= 5  && n < 3)
	{

		for(int i = 0; i < numLeds; i++){
			ledscape_set_color(frame, color_channel_order_from_string(ColorOrder), targetStrip, i, colors[color].a, colors[color].b, colors[color].c );
		}
		if(counter%speed == 0)
			n++;
	}
	else
	{
		for(int i = 0; i < numLeds; i++){
			ledscape_set_color(frame, color_channel_order_from_string(ColorOrder), targetStrip, i, 0, 0, 0 );
		}
	}
}
