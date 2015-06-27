/*
 * Strobe.cpp
 *
 *  Created on: 09.06.2015
 *      Author: christoph
 */

#include "Strobe.h"

Strobe::Strobe(ledscape_frame_t* iframe, ledscape_pixel_t* icolors,
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
	targetCycles = bpm / deltat;

	pliste = new pixel[nLedsProBar];
	switchcounter = 0;
	for (int i = 0; i < nLedsProBar; i++) {
		pliste[i].active = false;
		pliste[i].color = 0;
	}
	n = 0;
	strobecounter = 0;
	pliste = new pixel[nLedsProBar];
	for (int i = 0; i < nLedsProBar; i++) {
		pliste[i].set(true, 0, 0, 0, 0);
	}
}

Strobe::~Strobe() {
	// TODO Auto-generated destructor stub
}

void Strobe::event() {
	n = 0;
	strobecounter++;
	speed = (uint) (((double) speed / 255) * 50);
	if (speed <= 5) {
		speed = 5;
	}
	switch (type) {
	case 0:
		tripleShot();
		break;
	case 1:
		standard();
		break;
	case 2:
		switched();
		break;
	case 3:
		switchedBLUEWHITE();
		break;
	case 4:
		circle();
		break;

	}
}

void Strobe::noEvent() {
	strobecounter++;
	speed = (uint) (((double) speed / 255) * 35);
	if (speed <= 5) {
		speed = 5;
	}
	switch (type) {
	case 0:
		tripleShot();
		break;
	case 1:
		standard();
		break;
	case 2:
		switched();
		break;
	case 3:
		switchedBLUEWHITE();
		break;
	case 4:
		circle();
		break;
	}
}

void Strobe::tripleShot() {
	if (counter % speed <= 5 && n < 3) {

		for (int i = 0; i < numLeds; i++) {
			ledscape_set_color(frame,
					color_channel_order_from_string(ColorOrder), targetStrip, i,
					colors[color].a, colors[color].b, colors[color].c);
		}
		if (counter % speed == 0)
			n++;
	} else {
		for (int i = 0; i < numLeds; i++) {
			ledscape_set_color(frame,
					color_channel_order_from_string(ColorOrder), targetStrip, i,
					0, 0, 0);
		}
	}
}

void Strobe::standard() {
	for (int i = 0; i < numLedsProBar; i++) {
		pliste[i].set(false, color, colors[color].a, colors[color].b,
				colors[color].c);
	}

	if (strobecounter % (speed) < 3) {
		for (int i = 0; i < numLedsProBar; i++) {
			pliste[i].active = true;
		}
	}
	drawEqual();
}
void Strobe::switched() {
	for (int i = 0; i < numLedsProBar; i++) {
		pliste[i].set(false, color, colors[color].a, colors[color].b,
				colors[color].c);
	}

	if (strobecounter % (speed) == 0)
		switchcounter++;

	if (strobecounter % (speed) < 3) {
		if (switchcounter % 2 == 0) {
			for (int i = 0; i < numBars; i += 2) {
				for (int j = 0; j < numLedsProBar; j++) {

					ledscape_set_color(frame,
							color_channel_order_from_string(ColorOrder),
							targetStrip, i * numLedsProBar + j,
							colors[pliste[j].color].a,
							colors[pliste[j].color].b,
							colors[pliste[j].color].c);

				}
			}
		} else {
			for (int i = 1; i < numBars; i += 2) {
				for (int j = 0; j < numLedsProBar; j++) {

					ledscape_set_color(frame,
							color_channel_order_from_string(ColorOrder),
							targetStrip, i * numLedsProBar + j,
							colors[pliste[j].color].a,
							colors[pliste[j].color].b,
							colors[pliste[j].color].c);

				}
			}
		}

	}

}

void Strobe::switchedBLUEWHITE() {
	for (int i = 0; i < numLedsProBar; i++) {
		pliste[i].set(false, color, colors[color].a, colors[color].b,
				colors[color].c);
	}

	if (strobecounter % (speed) == 0)
		switchcounter++;

	if (strobecounter % (speed) < 3) {
		if (switchcounter % 2 == 0) {
			for (int i = 0; i < numBars; i += 2) {
				for (int j = 0; j < numLedsProBar; j++) {

					ledscape_set_color(frame,
							color_channel_order_from_string(ColorOrder),
							targetStrip, i * numLedsProBar + j,
							colors[pliste[j].color].a,
							colors[pliste[j].color].b,
							colors[pliste[j].color].c);

				}
			}
		} else {
			for (int i = 1; i < numBars; i += 2) {
				for (int j = 0; j < numLedsProBar; j++) {

					ledscape_set_color(frame,
							color_channel_order_from_string(ColorOrder),
							targetStrip, i * numLedsProBar + j, 255, 255, 255);

				}
			}
		}

	}

}
void Strobe::circle() {
	for (int i = 0; i < numLedsProBar; i++) {
		pliste[i].set(false, color, colors[color].a, colors[color].b,
				colors[color].c);
	}

	if (strobecounter % (speed) == 0)
		switchcounter++;

	if (strobecounter % (speed) < 3) {

		//for (int i = 0; i < numBars; i += 2) {
		for (int j = 0; j < numLedsProBar; j++) {

			ledscape_set_color(frame,
					color_channel_order_from_string(ColorOrder), targetStrip,
					(switchcounter % numBars) * numLedsProBar + j,
					colors[pliste[j].color].a, colors[pliste[j].color].b,
					colors[pliste[j].color].c);

		}
		//}

	}

}
