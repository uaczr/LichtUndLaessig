/*
 * Balls.cpp
 *
 *  Created on: 09.06.2015
 *      Author: christoph
 */

#include "Balls.h"

Balls::Balls(ledscape_frame_t* iframe, ledscape_pixel_t* icolors,
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
	raindrops = new bool[numBars * numLedsProBar];
	raintime = 0;
	posx = 0;
	posy = 0;
	dir = true;
	generalcounter = 0;
	for (int i = 0; i < numBars * numLedsProBar; i++) {
		raindrops[i] = false;
	}
	pliste = new pixel[nLedsProBar];
	for (int i = 0; i < nLedsProBar; i++) {
		pliste[i].active = false;
		pliste[i].color = 0;
	}

	for (int i = 0; i < LENGTH; i++) {
		liste[i].active = false;
		liste[i].color = 0;
		liste[i].position = 0;
	}
	power = 1;
	raincounter = 0;
	beatcounter = 0;
	dim[0] = 1;
	dim[1] = 1;
	dim[2] = 1;
	posmoon = 2;
	posmoon1 = 3;
}

Balls::~Balls() {
	// TODO Auto-generated destructor stub
}

int Balls::findEmpty() {
	for (int i = 0; i < LENGTH; i++) {
		if (!liste[i].active) {
			return i;
		}
	}
	return 0;
}

void Balls::event() {
	beatcounter++;
	raincounter++;
	speed = (uint) (((double) speed / 255) * 20);
	if (speed <= 5) {
		speed = 5;
	}
	//cout << speed <<endl;
	switch (type) {
	case 0:
		rising();
		break;
	case 1:
		falling();
		break;
	case 2:
		expandimp();
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
	case 6:
		imploding();
		break;
	case 7:
		sideways();
		break;
	case 8:
		moon();
		break;
	case 9:
		mooning();
		break;
	}
}
void Balls::noEvent() {
	raincounter++;
	speed = (uint) (((double) speed / 255) * 20);
	if (speed <= 5) {
		speed = 5;
	}
	switch (type) {
	case 0:
		rising();
		break;
	case 1:
		falling();
		break;
	case 2:
		expandimp();
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
	case 6:
		imploding();
		break;
	case 7:
		sideways();
		break;
	case 8:
		moon();
		break;
	case 9:
		mooning();
		break;
	}
}

void Balls::mooning() {
	if (beatcounter % 80 < 16) {
		moon();
	}
	if (beatcounter % 80 < 32 && beatcounter % 80 >= 16) {
		dim[0] = quadApp(0.0, 1.0, 16, beatcounter % 16);
		dim[1] = dim[0];
		dim[2] = dim[1];
		color = 7;
		moon2();
	}
	if (beatcounter % 80 < 48 && beatcounter % 80 >= 32) {
		dim[0] = quadApp(1.0, 0.0, 16, beatcounter % 16);
		dim[1] = dim[0];
		dim[2] = dim[1];
		color = 7;
		moon2();
	}
	if (beatcounter % 80 < 64 && beatcounter % 80 >= 48) {
		dim[0] = quadApp(0.0, 1.0, 16, beatcounter % 16);
		dim[1] = dim[0];
		dim[2] = dim[1];
		color = 6;
		moon2();
	}
	if (beatcounter % 80 < 80 && beatcounter % 80 >= 64) {
		dim[0] = quadApp(1.0, 0.0, 16, beatcounter % 16);
		dim[1] = dim[0];
		dim[2] = dim[1];
		color = 6;
		moon2();
	}
}
void Balls::expandimp() {
	if (beatcounter % 64 < 15) {

		exploding();
	}
	if (beatcounter % 64 == 15) {
		explodingND();
	}
	if (beatcounter % 64 > 15 && beatcounter % 64 < 31) {
		imploding();
	}
	if (beatcounter == 31) {

		implodingND();
	}
	if (beatcounter % 64 > 31 && beatcounter % 64 < 47) {
		falling();
	}
	if (beatcounter % 64 == 47) {
		fallingND();
	}
	if (beatcounter % 64 > 47 && beatcounter % 64 < 63) {
		rising();
	}

	if (beatcounter == 63) {
		risingND();
	}
	if (counter > targetCycles && counter % targetCycles == 10) {
		beatcounter++;
	}

}
void Balls::rising() {
	//cout << speed << endl;
	if (counter == 0) {
		pliste[0].active = true;
		pliste[0].color = color;
	}
	pixel old, act;
	if (counter % ((int) (speed)) == 0) {
		old = pliste[0];
		pliste[0].active = false;
		for (int i = 1; i < numLedsProBar; i++) {
			act = pliste[i];
			pliste[i] = old;
			old = act;
		}

	}
	drawEqual();
}

void Balls::risingND() {
	//cout << speed << endl;
	if (counter == 0) {
		pliste[0].active = true;
		pliste[0].color = color;
	}
	pixel old, act;
	if (counter % ((int) (speed)) == 0) {
		old = pliste[0];
		//pliste[0].active = false;
		for (int i = 1; i < numLedsProBar; i++) {
			act = pliste[i];
			pliste[i] = old;
			old = act;
		}

	}
	drawEqual();
}

void Balls::falling() {
	if (counter == 0) {
		pliste[numLedsProBar - 1].active = true;
		pliste[numLedsProBar - 1].color = color;
	}
	pixel old, act;
	if (counter % ((int) (speed)) == 0) {
		old = pliste[numLedsProBar - 1];

		pliste[numLedsProBar - 1].active = false;
		for (int i = numLedsProBar - 2; i >= 0; i--) {
			act = pliste[i];

			pliste[i] = old;

			old = act;
		}

	}
	drawEqual();
}
void Balls::fallingND() {
	if (counter == 0) {
		pliste[numLedsProBar - 1].active = true;
		pliste[numLedsProBar - 1].color = color;
	}
	pixel old, act;
	if (counter % ((int) (speed)) == 0) {
		old = pliste[numLedsProBar - 1];

		//pliste[numLedsProBar - 1].active = false;
		for (int i = numLedsProBar - 2; i >= 0; i--) {
			act = pliste[i];

			pliste[i] = old;

			old = act;
		}

	}
	drawEqual();
}

void Balls::raining() {
	if (raincounter % 311 == raintime) {
		raintime = rand() % 311;
		raindrops[0] = true;
	}
	if (raincounter % 20 == 0) {
		bool old, act;
		old = raindrops[0];
		raindrops[0] = false;
		for (int i = 1; i < numBars * numLedsProBar - 1; i++) {
			act = raindrops[i];
			raindrops[i] = old;
			if (old) {
				//ledscape_set_color(frame, color_channel_order_from_string(ColorOrder), targetStrip, i-1, 0.05*colors[color].a, 0.05*colors[color].b, 0.05*colors[color].c );
				ledscape_set_color(frame,
						color_channel_order_from_string(ColorOrder),
						targetStrip, i, 0.2 * colors[color].a,
						0.2 * colors[color].b, 0.2 * colors[color].c);
				ledscape_set_color(frame,
						color_channel_order_from_string(ColorOrder),
						targetStrip, i + 1, 1 * colors[color].a,
						1 * colors[color].b, 1 * colors[color].c);

			}
			old = act;
		}
	} else {
		for (int i = 2; i < numBars * numLedsProBar - 1; i++) {
			if (raindrops[i]) {
				//ledscape_set_color(frame, color_channel_order_from_string(ColorOrder), targetStrip, i-2, 0.05*colors[color].a, 0.05*colors[color].b, 0.05*colors[color].c );
				ledscape_set_color(frame,
						color_channel_order_from_string(ColorOrder),
						targetStrip, i - 1, 0.05 * colors[color].a,
						0.05 * colors[color].b, 0.05 * colors[color].c);
				ledscape_set_color(frame,
						color_channel_order_from_string(ColorOrder),
						targetStrip, i, 0.1 * colors[color].a,
						0.1 * colors[color].b, 0.1 * colors[color].c);
				ledscape_set_color(frame,
						color_channel_order_from_string(ColorOrder),
						targetStrip, i + 1, 1 * colors[color].a,
						1 * colors[color].b, 1 * colors[color].c);
			}
		}
	}
}

void Balls::exploding() {
	if (counter == 0) {
		pliste[numLedsProBar / 2].active = true;
		pliste[numLedsProBar / 2].color = color;
		pliste[numLedsProBar / 2 - 1].active = true;
		pliste[numLedsProBar / 2 - 1].color = color;
	}

	pixel old, act;
	if (counter % ((int) (speed)) == 0) {
		old = pliste[numLedsProBar / 2];
		pliste[numLedsProBar / 2].active = false;
		pliste[numLedsProBar / 2 - 1].active = false;
		for (int i = 1; i < numLedsProBar / 2; i++) {
			act = pliste[numLedsProBar / 2 + i];
			pliste[numLedsProBar / 2 + i] = old;
			pliste[numLedsProBar / 2 - 1 - i] = old;
			old = act;
		}

	}
	drawEqual();
}

void Balls::imploding() {
	if (counter == 0) {
		pliste[0].active = true;
		pliste[0].color = color;
		pliste[numLedsProBar - 1].active = true;
		pliste[numLedsProBar - 1].color = color;
	}
	pixel old, act;
	if (counter % ((int) (speed)) == 0) {
		old = pliste[0];

		pliste[0].active = false;
		pliste[numLedsProBar - 1].active = false;
		for (int i = 1; i < numLedsProBar / 2; i++) {
			act = pliste[i];

			pliste[i] = old;
			pliste[numLedsProBar - 1 - i] = old;
			old = act;

		}

	}
	drawEqual();
}

void Balls::implodingND() {

	if (counter == 0) {
		pliste[0].active = true;
		pliste[0].color = color;
		pliste[numLedsProBar - 1].active = true;
		pliste[numLedsProBar - 1].color = color;
		for (int i = 1; i < numLedsProBar - 1; i++) {
			pliste[i].active = false;
			pliste[i].color = color;
		}
	}
	pixel old, act;
	if (counter % (int) ((double) targetCycles / (numLedsProBar / 2)) == 0
			&& counter > 0) {
		old = pliste[0];
		for (int i = 1; i < numLedsProBar / 2; i++) {
			act = pliste[i];

			pliste[i] = old;
			pliste[numLedsProBar - 1 - i] = old;
			old = act;

		}

	}
	drawEqual();
}
void Balls::explodingND() {

	if (counter == 0) {
		pliste[numLedsProBar / 2].active = true;
		pliste[numLedsProBar / 2].color = color;
		pliste[numLedsProBar / 2 - 1].active = true;
		pliste[numLedsProBar / 2 - 1].color = color;
		for (int i = numLedsProBar / 2 + 1; i < numLedsProBar - 1; i++) {
			pliste[i].active = false;
			pliste[i].color = color;
			pliste[numLedsProBar / 2 - 1 - i].active = false;
			pliste[numLedsProBar / 2 - 1 - i].color = color;
		}
	}
	pixel old, act;
	if (counter % (int) ((double) targetCycles / (numLedsProBar / 2)) == 0
			&& counter > 1) {
		old = pliste[numLedsProBar / 2];
		for (int i = 1; i < numLedsProBar / 2; i++) {
			act = pliste[numLedsProBar / 2 + i];

			pliste[numLedsProBar / 2 + i] = old;
			pliste[numLedsProBar / 2 - 1 - i] = old;
			old = act;

		}

	}
	drawEqual();
}

void Balls::explodingDimmed() {
	if (counter == 0) {
		pliste[numLedsProBar / 2].active = true;
		pliste[numLedsProBar / 2].color = color;
		pliste[numLedsProBar / 2].r = colors[color].a;
		pliste[numLedsProBar / 2].g = colors[color].b;
		pliste[numLedsProBar / 2].b = colors[color].c;
	}

	pixel old, act;
	if (counter % ((int) (speed)) == 0) {
		old = pliste[numLedsProBar / 2];
		pliste[numLedsProBar / 2].active = false;
		for (int i = 1; i < numLedsProBar / 2; i++) {
			act = pliste[numLedsProBar / 2 + i];
			pliste[numLedsProBar / 2 + i] = old;

			if (pliste[numLedsProBar / 2 + i].active) {
				pliste[numLedsProBar / 2 + i].r = (int) linearApp(
						(double) colors[pliste[numLedsProBar / 2 + i].color].a,
						0, numLedsProBar / 2, i);
				pliste[numLedsProBar / 2 + i].g = (int) linearApp(
						(double) colors[pliste[numLedsProBar / 2 + i].color].b,
						0, numLedsProBar / 2, i);
				pliste[numLedsProBar / 2 + i].b = (int) linearApp(
						(double) colors[pliste[numLedsProBar / 2 + i].color].c,
						0, numLedsProBar / 2, i);
				//cout << pliste[numLedsProBar/2 + i].b << endl;
			}
			pliste[numLedsProBar / 2 - i] = pliste[numLedsProBar / 2 + i];
			old = act;
		}

	}
	drawColorEqual();
}

void Balls::spring() {
	int pos;
	for (int i = 0; i < numLedsProBar; i++) {
		pliste[i].active = false;
	}
	if (counter == 0) {
		for (int i = 0; i < numLedsProBar; i++) {
			pliste[i].set(false, color, colors[color].a, colors[color].b,
					colors[color].c);
		}
		springAmps[0] = numLedsProBar / 2 * 0.7 * power;
		if (springAmps[0] > numLedsProBar / 2 - 1)
			springAmps[0] = numLedsProBar / 2 - 1;
		springAmps[1] = (-1) * numLedsProBar / 2 * 4 / 5;
		springAmps[2] = numLedsProBar / 2 * 3 / 5;
		springAmps[3] = (-1) * numLedsProBar / 2 * 2 / 5;
		springAmps[4] = numLedsProBar / 2 * 1 / 5;
	}
	if (counter < targetCycles / 5) {
		pos = linearAppPM(springAmps[0], springAmps[1], targetCycles / 5,
				counter % (targetCycles / 5));
		pliste[numLedsProBar / 2 + pos].active = true;
		//cout << pos << endl;
	}
	if (counter > targetCycles / 5 && counter < 2 * targetCycles / 5) {
		pos = linearAppPM(springAmps[1], springAmps[2], targetCycles / 5,
				counter % (targetCycles / 5));
		pliste[numLedsProBar / 2 + pos].active = true;
		//cout << pos << endl;
	}
	if (counter > 2 * targetCycles / 5 && counter < 3 * targetCycles / 5) {
		pos = linearAppPM(springAmps[2], springAmps[3], targetCycles / 5,
				counter % (targetCycles / 5));
		pliste[numLedsProBar / 2 + pos].active = true;
		//cout << pos << endl;
	}
	if (counter > 3 * targetCycles / 5 && counter < 4 * targetCycles / 5) {
		pos = linearAppPM(springAmps[3], springAmps[4], targetCycles / 5,
				counter % (targetCycles / 5));
		pliste[numLedsProBar / 2 + pos].active = true;
		//cout << pos << endl;
	}
	if (counter > 4 * targetCycles / 5 && counter < 5 * targetCycles / 5) {
		pos = linearAppPM(springAmps[4], 0, targetCycles / 5,
				counter % (targetCycles / 5));
		pliste[numLedsProBar / 2 + pos].active = true;
		//cout << pos << endl;
	}
	drawEqual();

}

void Balls::sideways() {
	if (counter == 0) {
		posx = (raincounter * 119) % numBars;
		for (int j = 0; j < numBars; j++) {
			for (int i = 0; i < (29 * j * beatcounter) % 3; i++) {
				drawPixel(j, ((beatcounter + i + j) * 29) % numLedsProBar);
			}
		}
	} else {
		for (int j = 0; j < numBars; j++) {
			for (int i = 0; i < (29 * j * beatcounter) % 3; i++) {
				drawPixel(j, ((beatcounter + i + j) * 29) % numLedsProBar);
			}
		}
	}

}

void Balls::moon() {
	if (counter == 0) {
		if (dir) {
			dir = false;
		} else {
			dir = true;
		}
		posmoon = posmoon1;
	}

	if (counter > targetCycles) {
		drawColorEqual();
	} else {
		if (dir) {
			for (int i = 0; i < numLedsProBar; i++) {
				pliste[i].active = false;
			}
			posmoon1 = linearApp(posmoon, numLedsProBar - 4, targetCycles,
					counter);
			pixel colorpix;
			//cout << (int) colorpix.r << " " << (int) colorpix.g << " " << (int) colorpix.b << " "<< " " <<(int) posmoon1 + 3 << endl;
			getPixelColor(1, (int) posmoon1 + 3, &colorpix);

			for (int i = posmoon1 + 1; i <= posmoon1 + 3; i++) {
				pliste[i].active = true;
				pliste[i].color = color;
				pliste[i].r = quadApp(colorpix.r, (double) 0.3 * colorpix.r, 3,
						i - (posmoon1 - 2));
				pliste[i].g = quadApp(colorpix.g, (double) 0.3 * colorpix.g, 3,
						i - (posmoon1 - 2));
				pliste[i].b = quadApp(colorpix.b, (double) 0.3 * colorpix.b, 3,
						i - (posmoon1 - 2));
			}
			getPixelColor(1, (int) posmoon1 - 2, &colorpix);
			for (int i = posmoon1 - 2; i <= posmoon1; i++) {
				pliste[i].active = true;
				pliste[i].color = color;
				pliste[i].r = quadApp((double) 0.3 * colorpix.r, colorpix.r, 3,
						i - (posmoon1));
				pliste[i].g = quadApp((double) 0.3 * colorpix.g, colorpix.g, 3,
						i - (posmoon1));
				pliste[i].b = quadApp((double) 0.3 * colorpix.b, colorpix.b, 3,
						i - (posmoon1));
			}
		} else {
			for (int i = 0; i < numLedsProBar; i++) {
				pliste[i].active = false;
			}
			posmoon1 = linearApp(posmoon, 2, targetCycles, counter);
			pixel colorpix;
			//cout << (int) colorpix.r << " " << (int) colorpix.g << " " << (int) colorpix.b << " " <<(int) posmoon1 + 3 << endl;
			getPixelColor(1, (int) posmoon1 + 3, &colorpix);

			for (int i = posmoon1 + 1; i <= posmoon1 + 3; i++) {
				pliste[i].active = true;
				pliste[i].color = color;
				pliste[i].r = quadApp(colorpix.r, (double) 0.3 * colorpix.r, 3,
						i - (posmoon1 - 2));
				pliste[i].g = quadApp(colorpix.g, (double) 0.3 * colorpix.g, 3,
						i - (posmoon1 - 2));
				pliste[i].b = quadApp(colorpix.b, (double) 0.3 * colorpix.b, 3,
						i - (posmoon1 - 2));
			}
			getPixelColor(1, (int) posmoon1 - 2, &colorpix);
			for (int i = posmoon1 - 2; i <= posmoon1; i++) {
				pliste[i].active = true;
				pliste[i].color = color;
				pliste[i].r = quadApp((double) 0.3 * colorpix.r, colorpix.r, 3,
						i - (posmoon1));
				pliste[i].g = quadApp((double) 0.3 * colorpix.g, colorpix.g, 3,
						i - (posmoon1));
				pliste[i].b = quadApp((double) 0.3 * colorpix.b, colorpix.b, 3,
						i - (posmoon1));
			}
		}
		drawColorEqual();
	}
}

void Balls::moon2() {
	if (counter == 0) {
		if (dir) {
			dir = false;
		} else {
			dir = true;
		}
		posmoon = posmoon1;
	}

	if (counter > targetCycles) {
		drawColorEqual();
	} else {
		if (dir) {
			for (int i = 0; i < numLedsProBar; i++) {
				pliste[i].active = false;
			}
			posmoon1 = linearApp(posmoon, numLedsProBar - 4, targetCycles,
					counter);
			pixel colorpix;
			//cout << (int) colorpix.r << " " << (int) colorpix.g << " " << (int) colorpix.b << " "<< " " <<(int) posmoon1 + 3 << endl;
			getPixelColor(1, (int) posmoon1 + 3, &colorpix);

			for (int i = posmoon1 + 1; i <= posmoon1 + 3; i++) {
				pliste[i].active = true;
				pliste[i].color = color;
				pliste[i].r = quadApp(dim[0] * colors[color].a, colorpix.r, 3,
						i - (posmoon1));
				pliste[i].g = quadApp(dim[0] * colors[color].b, colorpix.g, 3,
						i - (posmoon1));
				pliste[i].b = quadApp(dim[0] * colors[color].c, colorpix.b, 3,
						i - (posmoon1));
			}
			getPixelColor(1, (int) posmoon1 - 2, &colorpix);
			for (int i = posmoon1 - 2; i <= posmoon1; i++) {
				pliste[i].active = true;
				pliste[i].color = color;
				pliste[i].r = quadApp(colorpix.r, dim[0] * colors[color].a, 3,
						i - (posmoon1 - 2));
				pliste[i].g = quadApp(colorpix.g, dim[0] * colors[color].b, 3,
						i - (posmoon1 - 2));
				pliste[i].b = quadApp(colorpix.b, dim[0] * colors[color].c, 3,
						i - (posmoon1 - 2));
			}
		} else {
			for (int i = 0; i < numLedsProBar; i++) {
				pliste[i].active = false;
			}
			posmoon1 = linearApp(posmoon, 2, targetCycles, counter);
			pixel colorpix;
			//cout << (int) colorpix.r << " " << (int) colorpix.g << " " << (int) colorpix.b << " " <<(int) posmoon1 + 3 << endl;
			getPixelColor(1, (int) posmoon1 + 3, &colorpix);

			for (int i = posmoon1 + 1; i <= posmoon1 + 3; i++) {
				pliste[i].active = true;
				pliste[i].color = color;
				pliste[i].r = quadApp(dim[0] * colors[color].a, colorpix.r, 3,
						i - (posmoon1));
				pliste[i].g = quadApp(dim[0] * colors[color].b, colorpix.g, 3,
						i - (posmoon1));
				pliste[i].b = quadApp(dim[0] * colors[color].c, colorpix.b, 3,
						i - (posmoon1));
			}
			getPixelColor(1, (int) posmoon1 - 2, &colorpix);
			for (int i = posmoon1 - 2; i <= posmoon1; i++) {
				pliste[i].active = true;
				pliste[i].color = color;
				pliste[i].r = quadApp(colorpix.r, dim[0] * colors[color].a, 3,
						i - (posmoon1 - 2));
				pliste[i].g = quadApp(colorpix.g, dim[0] * colors[color].b, 3,
						i - (posmoon1 - 2));
				pliste[i].b = quadApp(colorpix.b, dim[0] * colors[color].c, 3,
						i - (posmoon1 - 2));
			}
		}
		drawColorEqual();
	}
}
