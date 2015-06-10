/*
 * Strobe.cpp
 *
 *  Created on: 09.06.2015
 *      Author: christoph
 */

#include "Strobe.h"

Strobe::Strobe(ledscape_frame_t* iframe, ledscape_pixel_t* icolors, char* iColorOrder) {
	// TODO Auto-generated constructor stub
	ColorOrder = iColorOrder;
	colors = icolors;
	frame = iframe;
	deltat = 5;
	bpm = 500;
	counter = 0;
	numLeds = 100;
	targetStrip = 1;
	speed = 500;
	targetCycles = bpm/deltat;
	power = 1;
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

	if(counter%speed <= 3 && n < 3 && power > 1.1)
	{
		///cout << "Fuck" <<endl;
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
