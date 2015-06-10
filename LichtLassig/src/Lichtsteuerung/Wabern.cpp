/*
 * Wabern.cpp
 *
 *  Created on: 09.06.2015
 *      Author: christoph
 */

#include "Wabern.h"

Wabern::Wabern(ledscape_frame_t* iframe, ledscape_pixel_t* icolors, char* iColorOrder){
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
}

Wabern::~Wabern() {
	// TODO Auto-generated destructor stub
}

void Wabern::event(){
	switch(type){

	case 0 :
		//std::cout << speed*bpm/255 << endl;
		Rect();
		break;
	case 1:
		//std::cout << speed*bpm/255 << endl;
		Linear();
		break;
	}
	//
}
void Wabern::noEvent(){
	switch(type){

	case 0 :
		Rect();
		break;
	case 1:
		Linear();
		break;
	}
}




void Wabern::Rect(){
	if(counter == 0){
		//std::cout << "FUCK2" << endl;
		for(int i = 0; i < numLeds; i++){
			ledscape_set_color(frame, color_channel_order_from_string(ColorOrder), targetStrip, i, colors[color].a, colors[color].b, colors[color].c );
			//ledscape_set_color(frame, color_channel_order_from_string(ColorOrder), 1, i, colors[color].a, colors[color].b, colors[color].c );
		}
	}

	if(counter*deltat >= speed*bpm/255){

		for(int i = 0; i < numLeds; i++){
			ledscape_set_color(frame, color_channel_order_from_string(ColorOrder), targetStrip, i, 0, 0, 0 );
			//ledscape_set_color(frame, color_channel_order_from_string(ColorOrder), 1, i, 0, 0, 0 );
		}
	}
}

void Wabern::Linear(){
	if(counter*deltat < speed*bpm/255){
		double temp = (1-counter*deltat/(speed*bpm/255));
		int r = (int)((colors[color].a*temp));
		int g = (int)((colors[color].b*temp));
		int b = (int)((colors[color].c*temp));

		for(int i = 0; i < numLeds; i++){
			ledscape_set_color(frame, color_channel_order_from_string(ColorOrder), targetStrip, i, r, g, b );
			ledscape_set_color(frame, color_channel_order_from_string(ColorOrder), 1, i, r, g, b );
		}

	}
	if(counter*deltat >= speed*bpm/255){

		for(int i = 0; i < numLeds; i++){
			ledscape_set_color(frame, color_channel_order_from_string(ColorOrder), targetStrip, i, 0, 0, 0 );
			ledscape_set_color(frame, color_channel_order_from_string(ColorOrder), 1, i, 0, 0, 0 );
		}
	}
}
