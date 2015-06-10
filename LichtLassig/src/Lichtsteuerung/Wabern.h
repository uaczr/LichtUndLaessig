/*
 * Wabern.h
 *
 *  Created on: 09.06.2015
 *      Author: christoph
 */

#ifndef LICHTSTEUERUNG_WABERN_H_
#define LICHTSTEUERUNG_WABERN_H_
#include "Pattern.h"

class Wabern: public Pattern {
private:
	void event();
	void noEvent();
	void Rect();
	void Linear();
public:
	Wabern(ledscape_frame_t* iframe, ledscape_pixel_t* icolors, char* iColorOrder);
	virtual ~Wabern();
};

#endif /* LICHTSTEUERUNG_WABERN_H_ */