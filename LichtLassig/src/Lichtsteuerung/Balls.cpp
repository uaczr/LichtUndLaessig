/*
 * Balls.cpp
 *
 *  Created on: 09.06.2015
 *      Author: christoph
 */

#include "Balls.h"

Balls::Balls(ledscape_frame_t* iframe , ledscape_pixel_t* icolors, char* iColorOrder, int nBars, int nLedsProBar) {
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
	pliste = new pixel[nLedsProBar];
	for(int i = 0; i < nLedsProBar; i++){
		pliste[i].active = false;
		pliste[i].color = 0;
	}

	for (int i = 0; i < LENGTH; i++){
		liste[i].active = false;
		liste[i].color = 0;
		liste[i].position = 0;
	}
	power = 1;
}

Balls::~Balls() {
	// TODO Auto-generated destructor stub
}

int Balls::findEmpty(){
	for(int i = 0; i < LENGTH; i++){
		if(!liste[i].active)
		{
			return i;
		}
	}
	return 0;
}

void Balls::event(){
	switch(type)
	{
	case 0:
		standard();
		break;
	}
}
void Balls::noEvent(){
	switch(type)
	{
	case 0:
		standard();
		break;
	}
}

void Balls::standard(){
	if(counter == 0){
		pliste[0].active = true;
		pliste[0].color = color;
	}
	pixel old, act;
	if(counter%speed == 0){
		old.active = pliste[0].active;
		old.color = pliste[0].color;
		pliste[0].active = false;
		for(int i = 1; i < numLedsProBar; i++){
			act.active = pliste[i].active;
			act.color = pliste[i].color;
			pliste[i].active = old.active;
			pliste[i].color = old.color;
			old.active = act.active;
			old.color = act.color;
		}

	}
	drawEqual();
}

void Balls::drawEqual(){
	for(int i = 0; i < numBars; i++){
		for(int j = 0; j < numLedsProBar; j++){
			if(pliste[j].active){
				ledscape_set_color(frame, color_channel_order_from_string(ColorOrder), targetStrip, i*numLedsProBar+j, colors[pliste[j].color].a, colors[pliste[j].color].b, colors[pliste[j].color].c );
			}
		}
	}
}
