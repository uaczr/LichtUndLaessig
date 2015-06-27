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
	raindrops = new bool[numBars*numLedsProBar];
	raintime = 0;
	for(int i = 0; i < numBars*numLedsProBar; i++){
		raindrops[i] = false;
	}
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
	raincounter = 0;
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
	raincounter++;
	speed = (uint)(((double)speed/255)*35);
	if(speed <= 5){
		speed = 5;
	}
	//cout << speed <<endl;
	switch(type)
	{
	case 0:
		rising();
		break;
	case 1:
		falling();
		break;
	case 2:
		exploding();
		break;
	case 3:
		explodingDimmed();
		break;
	case 4:
		spring();
		break;
	case 5:
		raining();
		break;
	}
}
void Balls::noEvent(){
	raincounter++;
	speed = (uint)(((double)speed/255)*35);
	if(speed <= 5){
		speed = 5;
	}
	switch(type)
	{
	case 0:
		rising();
		break;
	case 1:
		falling();
		break;
	case 2:
		exploding();
		break;
	case 3:
		explodingDimmed();
		break;
	case 4:
		spring();
		break;
	case 5:
		raining();
		break;
	}
}

void Balls::rising(){
	//cout << speed << endl;
	if(counter == 0){
		pliste[0].active = true;
		pliste[0].color = color;
	}
	pixel old, act;
	if(counter%((int)(speed)) == 0){
		old = pliste[0];
		pliste[0].active = false;
		for(int i = 1; i < numLedsProBar; i++){
			act = pliste[i];
			pliste[i] = old;
			old = act;
		}

	}
	drawEqual();
}

void Balls::falling(){
	if(counter == 0){
		pliste[numLedsProBar-1].active = true;
		pliste[numLedsProBar-1].color = color;
	}
	pixel old, act;
	if(counter%((int)(speed)) == 0){
		old = pliste[numLedsProBar-1];

		pliste[numLedsProBar-1].active = false;
		for(int i = numLedsProBar-2; i >= 0; i--){
			act = pliste[i];

			pliste[i]= old;

			old = act;
		}

	}
	drawEqual();
}

void Balls::raining(){
	if(raincounter % 311 == raintime){
		raintime = rand() % 311;
		raindrops[0] = true;
	}
	if(raincounter % 20 == 0){
		bool old, act;
		old = raindrops[0];
		raindrops[0] = false;
		for(int i = 1; i < numBars* numLedsProBar-1; i++){
			act = raindrops[i];
			raindrops[i] = old;
			if(old){
				//ledscape_set_color(frame, color_channel_order_from_string(ColorOrder), targetStrip, i-1, 0.05*colors[color].a, 0.05*colors[color].b, 0.05*colors[color].c );
				ledscape_set_color(frame, color_channel_order_from_string(ColorOrder), targetStrip, i, 0.2*colors[color].a, 0.2*colors[color].b, 0.2*colors[color].c );
				ledscape_set_color(frame, color_channel_order_from_string(ColorOrder), targetStrip, i+1, 1*colors[color].a, 1*colors[color].b, 1*colors[color].c );

			}
			old = act;
		}
	}
	else
	{
		for(int i = 2; i < numBars* numLedsProBar-1; i++){
			if(raindrops[i]){
				//ledscape_set_color(frame, color_channel_order_from_string(ColorOrder), targetStrip, i-2, 0.05*colors[color].a, 0.05*colors[color].b, 0.05*colors[color].c );
				ledscape_set_color(frame, color_channel_order_from_string(ColorOrder), targetStrip, i-1, 0.05*colors[color].a, 0.05*colors[color].b, 0.05*colors[color].c );
				ledscape_set_color(frame, color_channel_order_from_string(ColorOrder), targetStrip, i, 0.1*colors[color].a, 0.1*colors[color].b, 0.1*colors[color].c );
				ledscape_set_color(frame, color_channel_order_from_string(ColorOrder), targetStrip, i+1, 1*colors[color].a, 1*colors[color].b, 1*colors[color].c );
			}
		}
	}
}

void Balls::exploding(){
	if(counter == 0){
		pliste[numLedsProBar/2].active = true;
		pliste[numLedsProBar/2].color = color;
	}

	pixel old, act;
	if(counter%((int)(speed)) == 0){
		old = pliste[numLedsProBar/2];
		pliste[numLedsProBar/2].active = false;
		for(int i = 1; i < numLedsProBar/2; i++){
			act = pliste[numLedsProBar/2 + i];
			pliste[numLedsProBar/2 + i] = old;
			pliste[numLedsProBar/2 - i] = old;
			old = act;
		}

	}
	drawEqual();
}
void Balls::explodingDimmed(){
	if(counter == 0){
		pliste[numLedsProBar/2].active = true;
		pliste[numLedsProBar/2].color = color;
		pliste[numLedsProBar/2].r = colors[color].a;
		pliste[numLedsProBar/2].g = colors[color].b;
		pliste[numLedsProBar/2].b = colors[color].c;
	}

	pixel old, act;
	if(counter%((int)(speed)) == 0){
		old = pliste[numLedsProBar/2];
		pliste[numLedsProBar/2].active = false;
		for(int i = 1; i < numLedsProBar/2; i++){
			act = pliste[numLedsProBar/2 + i];
			pliste[numLedsProBar/2 + i] = old;

			if(pliste[numLedsProBar/2 + i].active){
				pliste[numLedsProBar/2 + i].r = (int)linearApp((double)colors[pliste[numLedsProBar/2 + i].color].a, 0,numLedsProBar/2, i);
				pliste[numLedsProBar/2 + i].g = (int)linearApp((double)colors[pliste[numLedsProBar/2 + i].color].b, 0,numLedsProBar/2, i);
				pliste[numLedsProBar/2 + i].b = (int)linearApp((double)colors[pliste[numLedsProBar/2 + i].color].c, 0,numLedsProBar/2, i);
				//cout << pliste[numLedsProBar/2 + i].b << endl;
			}
			pliste[numLedsProBar/2 - i] = pliste[numLedsProBar/2 + i];
			old = act;
		}

	}
	drawColorEqual();
}

void Balls::spring(){
	int pos;
	for(int i = 0; i < numLedsProBar; i++){
		pliste[i].active = false;
	}
	if(counter == 0)
	{
		for(int i = 0; i < numLedsProBar; i++){
				pliste[i].set(false, color, colors[color].a, colors[color].b, colors[color].c);
		}
		springAmps[0] = numLedsProBar/2 * 0.7 * power;
		if(springAmps[0] > numLedsProBar/2-1)
			springAmps[0] = numLedsProBar/2 - 1;
		springAmps[1] = (-1) * numLedsProBar/2 *4/5;
		springAmps[2] = numLedsProBar/2 *3/5;
		springAmps[3] = (-1) * numLedsProBar/2 *2/5;
		springAmps[4] = numLedsProBar/2 *1/5;
	}
	if(counter < targetCycles/5){
		pos = linearAppPM(springAmps[0], springAmps[1],targetCycles/5, counter%(targetCycles/5));
		pliste[numLedsProBar/2 + pos].active = true;
		//cout << pos << endl;
	}
	if(counter > targetCycles/5 && counter < 2*targetCycles/5){
		pos = linearAppPM(springAmps[1], springAmps[2],targetCycles/5, counter%(targetCycles/5));
		pliste[numLedsProBar/2 + pos].active = true;
		//cout << pos << endl;
	}
	if(counter > 2*targetCycles/5 && counter < 3*targetCycles/5){
		pos = linearAppPM(springAmps[2], springAmps[3],targetCycles/5, counter%(targetCycles/5));
		pliste[numLedsProBar/2 + pos].active = true;
		//cout << pos << endl;
	}
	if(counter > 3*targetCycles/5 && counter < 4*targetCycles/5){
		pos = linearAppPM(springAmps[3], springAmps[4],targetCycles/5, counter%(targetCycles/5));
		pliste[numLedsProBar/2 + pos].active = true;
		//cout << pos << endl;
	}
	if(counter > 4*targetCycles/5 && counter < 5*targetCycles/5){
		pos = linearAppPM(springAmps[4], 0,targetCycles/5, counter%(targetCycles/5));
		pliste[numLedsProBar/2 + pos].active = true;
		//cout << pos << endl;
	}
	drawEqual();

}


