/*
 * Strobe.h
 *
 *  Created on: 09.06.2015
 *      Author: christoph
 */

#ifndef LICHTSTEUERUNG_STROBE_H_
#define LICHTSTEUERUNG_STROBE_H_
#include "Pattern.h"
class Strobe : public Pattern{
private:
	int n;
	long strobecounter;
	int switchcounter;
	int updownpos;
	bool updown;
	int pos[3];
	void event();
	void noEvent();
	void tripleShot();
	void standard();
	void switched();
	void switchedBLUEWHITE();
	void circle();
	void upanddown();
	void lowerhalf();
	void upperhalf();
	void random();

	void every4();
	void every8();


	void breakdetect();

public:
	Strobe(ledscape_frame_t* iframe , ledscape_pixel_t* icolors, char* iColorOrder, int nBars, int nLedsProBar);
	virtual ~Strobe();
};

#endif /* LICHTSTEUERUNG_STROBE_H_ */
