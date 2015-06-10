/*
 * Balls.cpp
 *
 *  Created on: 09.06.2015
 *      Author: christoph
 */

#include "Balls.h"

Balls::Balls(ledscape_frame_t* iframe, ledscape_pixel_t* icolors, char* iColorOrder) {
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
		liste[findEmpty()].color = color;
		liste[findEmpty()].position = 0;
		liste[findEmpty()].active = true;
	}
	for(int i = 0; i < LENGTH; i++){
		if(liste[i].active){
			ledscape_set_color(frame, color_channel_order_from_string(ColorOrder), targetStrip, liste[i].position, colors[liste[i].color].a, colors[liste[i].color].b, colors[liste[i].color].c );
			if(counter%speed == 0)
				liste[i].position++;
			if(liste[i].position > numLeds)
				liste[i].active = false;
		}
	}

}
