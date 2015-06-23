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
	int n;
	void event();
	void noEvent();
	void tripleShot();
public:
	Strobe(ledscape_frame_t* iframe , ledscape_pixel_t* icolors, char* iColorOrder, int nBars, int nLedsProBar);
	virtual ~Strobe();
};

#endif /* LICHTSTEUERUNG_STROBE_H_ */
