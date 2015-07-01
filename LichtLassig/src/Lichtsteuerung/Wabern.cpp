/*
 * Wabern.cpp
 *
 *  Created on: 09.06.2015
 *      Author: christoph
 */

#include "Wabern.h"

Wabern::Wabern(ledscape_frame_t* iframe, ledscape_pixel_t* icolors,
		char* iColorOrder, int nBars, int nLedsProBar) {
	// TODO Auto-generated constructor stub
	ColorOrder = iColorOrder;
	colors = icolors;
	frame = iframe;
	deltat = 5;
	bpm = 500;
	counter = 0;
	numBars = nBars;
	numLedsProBar = nLedsProBar;
	numLeds = nBars * nLedsProBar;
	targetStrip = 1;
	power = 1;
	color = 1;
	type = 0;
	speed = 100;
	beatcounter = 0;
	generalcounter = 0;
	targetCycles = bpm / deltat;
	pliste = new pixel[nLedsProBar];
	for (int i = 0; i < nLedsProBar; i++) {
		pliste[i].active = false;
		pliste[i].color = 0;
	}
	top = 0;
	dim[0] = 1;
	dim[1] = 1;
	dim[2] = 1;
	oldheight = 0;
	newheight = 0;
	beatsSince = 0;
}

Wabern::~Wabern() {
	// TODO Auto-generated destructor stub
}

void Wabern::event() {
	speed = 143 + (int) ((double) speed / 255 * 122);
	beatcounter++;
	genbeatcounter++;
	switch (type) {

	case 0:
		//std::cout << speed*bpm/255 << endl;
		Rect();
		break;
	case 1:
		//std::cout << speed*bpm/255 << endl;
		Dimming();
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
	case 5:
		DimmingAmbi();
		break;
	case 6:
		LinearInter();
		break;
	case 7:
		Ambilight();
		break;

	}
	//
}
void Wabern::noEvent() {
	speed = 143 + (int) ((double) speed / 255 * 122);
	switch (type) {

	case 0:
		Rect();
		break;
	case 1:
		Dimming();
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
	case 5:
		DimmingAmbi();
		break;
	case 6:
		LinearInter();
		break;
	case 7:
		Ambilight();
		break;

	}
}

void Wabern::Dimming() {
	if (beatcounter % 128 < 64) {
		dim[0] = quadApp(1, 0.1, 64, beatcounter % 128);
		dim[1] = dim[0];
		dim[2] = dim[0];
	}
	if (beatcounter % 128 > 64) {
		dim[0] = quadApp(0.1, 1, 64, (beatcounter % 128) - 64);
		dim[1] = dim[0];
		dim[2] = dim[0];
	}
	Linear();

}

void Wabern::DimmingAmbi() {
	if (generalcounter % 5000 < 2500) {
		dim[0] = quadApp(1, 0.2, 2500, generalcounter % 5000);
		dim[1] = dim[0];
		dim[2] = dim[0];
	}
	if (generalcounter % 5000 > 2500) {
		dim[0] = quadApp(0.2, 1, 2500, (generalcounter % 5000) - 2500);
		dim[1] = dim[0];
		dim[2] = dim[0];
	}
	//cout << dim[0] << endl;
	AmbilightNebel();
}
void Wabern::Ambilight() {
	for (int i = 0; i < numLedsProBar; i++) {
		pliste[i].active = true;
		pliste[i].color = color;
	}
	drawEqual();
}

void Wabern::AmbilightNebel() {
	if(generalcounter%250 == 0){
		oldheight = newheight;
		newheight = ((double)rand()/RAND_MAX * (numLedsProBar-5));

	}
	double pos = linearApp(oldheight, newheight, 250, generalcounter%250);
	//cout << pos << endl;
	for(int i = 0; i < numLedsProBar; i++){
		pliste[i].active = true;
		pliste[i].color = color;
		pliste[i].r = dim[0] * colors[color].a;
		pliste[i].g = dim[1] * colors[color].b;
		pliste[i].b = dim[2] * colors[color].c;
		if(i > pos){
			pliste[i].r = dim[0] * quadApp( 0,colors[color].a, (double)numLedsProBar-pos, numLedsProBar-i);
			pliste[i].g = dim[1] * quadApp( 0,colors[color].b, (double)numLedsProBar-pos, numLedsProBar-i);
			pliste[i].b = dim[2] * quadApp( 0,colors[color].c, (double)numLedsProBar-pos, numLedsProBar-i);
		}
	}
	drawColorEqual();
}
void Wabern::Rect() {
	if (counter == 0) {
		//std::cout << "FUCK2" << endl;
		for (int i = 0; i < numLeds; i++) {
			ledscape_set_color(frame,
					color_channel_order_from_string(ColorOrder), targetStrip, i,
					colors[color].a, colors[color].b, colors[color].c);
			//ledscape_set_color(frame, color_channel_order_from_string(ColorOrder), 1, i, colors[color].a, colors[color].b, colors[color].c );
		}
	}

	if (counter * deltat >= speed * bpm / 255) {

		for (int i = 0; i < numLeds; i++) {
			ledscape_set_color(frame,
					color_channel_order_from_string(ColorOrder), targetStrip, i,
					0, 0, 0);
			//ledscape_set_color(frame, color_channel_order_from_string(ColorOrder), 1, i, 0, 0, 0 );
		}
	}
}

void Wabern::Linear() {
	if (counter * deltat < speed * bpm / 255) {
		double temp = (1 - counter * deltat / (speed * bpm / 255));
		int r = (int) ((colors[color].a * temp * dim[0]));
		int g = (int) ((colors[color].b * temp * dim[1]));
		int b = (int) ((colors[color].c * temp * dim[2]));

		for (int i = 0; i < numLeds; i++) {
			ledscape_set_color(frame,
					color_channel_order_from_string(ColorOrder), targetStrip, i,
					r, g, b);
			//ledscape_set_color(frame, color_channel_order_from_string(ColorOrder), 1, i, r, g, b );
		}

	}
	if (counter * deltat >= speed * bpm / 255) {

		for (int i = 0; i < numLeds; i++) {
			ledscape_set_color(frame,
					color_channel_order_from_string(ColorOrder), targetStrip, i,
					0, 0, 0);
			//ledscape_set_color(frame, color_channel_order_from_string(ColorOrder), 1, i, 0, 0, 0 );
		}
	}
}

void Wabern::Switched() {
	if (counter == 0) {
		count++;
		//cout << numBars<<endl;
	}
	int r = (int) linearApp((double) colors[color].a, 0, speed * bpm / 255,
			counter * deltat) * dim[0];
	int g = (int) linearApp((double) colors[color].b, 0, speed * bpm / 255,
			counter * deltat) * dim[1];
	int b = (int) linearApp((double) colors[color].c, 0, speed * bpm / 255,
			counter * deltat) * dim[2];
	//cout << r << " " << g << " " << b << endl;
	if (counter * deltat < speed * bpm / 255) {
		if (count % 2) {
			for (int i = 0; i < numBars; i += 2) {
				for (int j = i * numLedsProBar; j < (i + 1) * numLedsProBar;
						j++) {

					ledscape_set_color(frame,
							color_channel_order_from_string(ColorOrder),
							targetStrip, j, r, g, b);
				}
			}
		} else {
			for (int i = 1; i < numBars; i += 2) {
				for (int j = i * numLedsProBar; j < (i + 1) * numLedsProBar;
						j++) {
					ledscape_set_color(frame,
							color_channel_order_from_string(ColorOrder),
							targetStrip, j, r, g, b);

				}
			}
		}
	}
}

void Wabern::Circle() {
	if (counter == 0) {
		count++;
	}
	int r = (int) linearApp((double) colors[color].a, 0, speed * bpm / 255,
			counter * deltat) * dim[0];
	int g = (int) linearApp((double) colors[color].b, 0, speed * bpm / 255,
			counter * deltat) * dim[1];
	int b = (int) linearApp((double) colors[color].c, 0, speed * bpm / 255,
			counter * deltat) * dim[2];
	if (counter * deltat < speed * bpm / 255) {
		for (int j = (count % numBars) * numLedsProBar;
				j < ((count % numBars) + 1) * numLedsProBar; j++) {
			ledscape_set_color(frame,
					color_channel_order_from_string(ColorOrder), targetStrip, j,
					r, g, b);
		}

	}

}
void Wabern::Energy() {
	for (int i = 0; i < numLedsProBar; i++) {
		pliste[i].set(false, color, colors[color].a, colors[color].b,
				colors[color].c);
	}
	if (counter == 0) {
		top = power * (numLedsProBar * 3) / 4;
		if (top >= numLedsProBar)
			top = numLedsProBar - 1;
		beatsSince = 0;
	}
	if(counter > targetCycles+3 && beatsSince < 3){
		counter = 0;
		beatsSince ++;
		top = 0.7*top;
	}
	int acttop = linearApp(top, 0, targetCycles, counter);
	for (int i = 0; i <= acttop; i++) {
		pliste[i].active = true;
		if (i > (numLedsProBar * 2) / 3) {
			pliste[i].color = 4;
		}

	}
	drawEqual();
}
void Wabern::Haze() {
	//if(7)

}
void Wabern::LinearInter() {

	if (counter * deltat < speed * bpm / 255) {
		double temp = (1 - counter * deltat / (speed * bpm / 255));
		int r = (int) ((colors[color].a * temp));
		int g = (int) ((colors[color].b * temp));
		int b = (int) ((colors[color].c * temp));

		if (genbeatcounter % 128 < 64) {
			double dimmstufe = (double) (genbeatcounter % 64) / 64 * 100;
			for (int j = 0; j < numBars; j++) {
				for (int i = 0; i < numLedsProBar; i++) {
					if (i < numLedsProBar / 2)
						ledscape_set_color(frame,
								color_channel_order_from_string(ColorOrder),
								targetStrip, i + j * numLedsProBar,
								(int) quadApp(r, dimmstufe,
										(double) (numLedsProBar / 2), i),
								(int) quadApp(g, dimmstufe,
										(double) (numLedsProBar / 2), i),
								(int) quadApp(b, dimmstufe,
										(double) (numLedsProBar / 2), i));
					else
						ledscape_set_color(frame,
								color_channel_order_from_string(ColorOrder),
								targetStrip, i + j * numLedsProBar,
								(int) quadApp(dimmstufe, r,
										(double) numLedsProBar / 2,
										i - numLedsProBar / 2),
								(int) quadApp(dimmstufe, g,
										(double) numLedsProBar / 2,
										i - numLedsProBar / 2),
								(int) quadApp(dimmstufe, b,
										(double) numLedsProBar / 2,
										i - numLedsProBar / 2));

					//ledscape_set_color(frame, color_channel_order_from_string(ColorOrder), 1, i, r, g, b );
				}
			}
		} else {
			double dimmstufe = 100 - (double) (genbeatcounter % 64) / 64 * 100;
			for (int j = 0; j < numBars; j++) {
				for (int i = 0; i < numLedsProBar; i++) {
					if (i < numLedsProBar / 2)
						ledscape_set_color(frame,
								color_channel_order_from_string(ColorOrder),
								targetStrip, i + j * numLedsProBar,
								(int) quadApp(r, dimmstufe,
										(double) (numLedsProBar / 2), i),
								(int) quadApp(g, dimmstufe,
										(double) (numLedsProBar / 2), i),
								(int) quadApp(b, dimmstufe,
										(double) (numLedsProBar / 2), i));
					else
						ledscape_set_color(frame,
								color_channel_order_from_string(ColorOrder),
								targetStrip, i + j * numLedsProBar,
								(int) quadApp(dimmstufe, r,
										(double) numLedsProBar / 2,
										i - numLedsProBar / 2),
								(int) quadApp(dimmstufe, g,
										(double) numLedsProBar / 2,
										i - numLedsProBar / 2),
								(int) quadApp(dimmstufe, b,
										(double) numLedsProBar / 2,
										i - numLedsProBar / 2));

					//ledscape_set_color(frame, color_channel_order_from_string(ColorOrder), 1, i, r, g, b );
				}
			}
		}

	}
	if (counter * deltat >= speed * bpm / 255) {

		for (int i = 0; i < numLeds; i++) {
			ledscape_set_color(frame,
					color_channel_order_from_string(ColorOrder), targetStrip, i,
					0, 0, 0);
			//ledscape_set_color(frame, color_channel_order_from_string(ColorOrder), 1, i, 0, 0, 0 );
		}
	}
}
