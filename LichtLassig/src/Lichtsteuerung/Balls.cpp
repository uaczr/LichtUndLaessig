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
	}
}
void Balls::noEvent(){
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
	}
}

void Balls::rising(){
	if(counter == 0){
		pliste[0].active = true;
		pliste[0].color = color;
	}
	pixel old, act;
	if(counter%speed == 0){
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
	if(counter%speed == 0){
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

void Balls::exploding(){
	if(counter == 0){
		pliste[numLedsProBar/2].active = true;
		pliste[numLedsProBar/2].color = color;
	}

	pixel old, act;
	if(counter%speed == 0){
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
	if(counter%speed == 0){
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

void Balls::drawEqual(){
	for(int i = 0; i < numBars; i++){
		for(int j = 0; j < numLedsProBar; j++){
			if(pliste[j].active){
				ledscape_set_color(frame, color_channel_order_from_string(ColorOrder), targetStrip, i*numLedsProBar+j, colors[pliste[j].color].a, colors[pliste[j].color].b, colors[pliste[j].color].c );
			}
		}
	}
}
void Balls::drawColorEqual(){
	for(int i = 0; i < numBars; i++){
		for(int j = 0; j < numLedsProBar; j++){
			if(pliste[j].active){
				ledscape_set_color(frame, color_channel_order_from_string(ColorOrder), targetStrip, i*numLedsProBar+j, pliste[j].r, pliste[j].g, pliste[j].b );
			}
		}
	}
}
