/*
 * Balls.h
 *
 *  Created on: 09.06.2015
 *      Author: christoph
 */

#ifndef LICHTSTEUERUNG_BALLS_H_
#define LICHTSTEUERUNG_BALLS_H_
#include "Pattern.h"
#define LENGTH 20
struct ball{
	bool active;
	int position;
	int color;
};

class Balls : public Pattern{
	ball liste[LENGTH];

	void noEvent();
	void event();
	int findEmpty();
	void rising();
	void risingND();
	void falling();
	void fallingND();
	void exploding();
	void explodingND();
	void explodingDimmed();
	void spring();
	void raining();
	void implodingND();
	void imploding();
	void expandimp();
	void sideways();
	void moon();
	void moon2();
	void mooning();
	bool dir;
	double posmoon;
	double posmoon1;
	int posx;
	int posy;
	int impcount;
	int springAmps[5];
	bool* raindrops;
	int raintime;
	long raincounter;


public:
	Balls(ledscape_frame_t* iframe , ledscape_pixel_t* icolors, char* iColorOrder, int nBars, int nLedsProBar);
	virtual ~Balls();
};

#endif /* LICHTSTEUERUNG_BALLS_H_ */
