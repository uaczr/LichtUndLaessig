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
	void standard();

public:
	Balls(ledscape_frame_t* iframe, ledscape_pixel_t* icolors, char* iColorOrder);
	virtual ~Balls();
};

#endif /* LICHTSTEUERUNG_BALLS_H_ */
