/*
 * Wabern.cpp
 *
 *  Created on: 09.06.2015
 *      Author: christoph
 */

#include "Wabern.h"

Wabern::Wabern(ledscape_frame_t* iframe , ledscape_pixel_t* icolors, char* iColorOrder, int nBars, int nLedsProBar){
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
	top = 0;
}

Wabern::~Wabern() {
	// TODO Auto-generated destructor stub
}

void Wabern::event(){
	if(speed == 0)
		speed = 1;
	switch(type){

	case 0 :
		//std::cout << speed*bpm/255 << endl;
		Rect();
		break;
	case 1:
		//std::cout << speed*bpm/255 << endl;
		Linear();
		break;
	case 2:
		Switched();
		break;
	case 3:
		Circle();
		break;
	case 4:
		Energy();
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
	case 2:
		Switched();
		break;
	case 3:
		Circle();
		break;
	case 4:
		Energy();
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
			//ledscape_set_color(frame, color_channel_order_from_string(ColorOrder), 1, i, r, g, b );
		}

	}
	if(counter*deltat >= speed*bpm/255){

		for(int i = 0; i < numLeds; i++){
			ledscape_set_color(frame, color_channel_order_from_string(ColorOrder), targetStrip, i, 0, 0, 0 );
			//ledscape_set_color(frame, color_channel_order_from_string(ColorOrder), 1, i, 0, 0, 0 );
		}
	}
}

void Wabern::Switched(){
	if(counter == 0)
	{
		count++;
		//cout << numBars<<endl;
	}
	int r = (int) linearApp((double)colors[color].a, 0, speed*bpm/255, counter*deltat);
	int g = (int) linearApp((double)colors[color].b, 0, speed*bpm/255, counter*deltat);
	int b = (int) linearApp((double)colors[color].c, 0, speed*bpm/255, counter*deltat);
	//cout << r << " " << g << " " << b << endl;
	if(counter*deltat <  speed*bpm/255){
		if(count%2){
			for(int i = 0; i < numBars ; i+=2){
				for(int j = i*numLedsProBar; j < (i+1) * numLedsProBar; j++){

					ledscape_set_color(frame, color_channel_order_from_string(ColorOrder), targetStrip, j, r, g, b );
				}
			}
		}
		else
		{
			for(int i = 1; i < numBars ; i+=2){
				for(int j = i*numLedsProBar; j < (i+1) * numLedsProBar; j++){
					ledscape_set_color(frame, color_channel_order_from_string(ColorOrder), targetStrip, j, r, g, b );

				}
			}
		}
	}
}

void Wabern::Circle(){
	if(counter == 0)
	{
		count++;
	}
	int r = (int) linearApp((double)colors[color].a, 0, speed*bpm/255, counter*deltat);
	int g = (int) linearApp((double)colors[color].b, 0, speed*bpm/255, counter*deltat);
	int b = (int) linearApp((double)colors[color].c, 0, speed*bpm/255, counter*deltat);
	if(counter*deltat <  speed*bpm/255){
		for(int j = (count%numBars)*numLedsProBar; j < ((count%numBars)+1) * numLedsProBar; j++){
				ledscape_set_color(frame, color_channel_order_from_string(ColorOrder), targetStrip, j, r, g, b );
		}

	}

}
void Wabern::Energy(){
	for(int i = 0; i < numLedsProBar; i++){
		pliste[i].set(false, color, colors[color].a, colors[color].b, colors[color].c);
	}
	if(counter == 0){
		top = power * numLedsProBar/2;
		if(top >= numLedsProBar)
			top = numLedsProBar-1;
	}
	int acttop = linearApp(top, 0, targetCycles, counter);
	for(int i = 0; i <= acttop; i++){
		pliste[i].active = true;
		if(i>numLedsProBar/2){
			pliste[i].color = 4;
		}

	}
	drawEqual();
}
void Wabern::Haze(){

}
