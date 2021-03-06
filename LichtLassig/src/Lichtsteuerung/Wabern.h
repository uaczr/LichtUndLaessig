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
	uint8_t count;
	int top;
	uint genbeatcounter;
	double oldheight;
	double newheight;
	int beatsSince;
	void event();
	void noEvent();
	void Rect();
	void Linear();
	void LinearInter();
	void Switched();
	void Circle();
	void Energy();
	void Haze();
	void Dimming();
	void DimmingAmbi();
	void Ambilight();
	void AmbilightNebel();

	void AmbilightAuto();
public:
	Wabern(ledscape_frame_t* iframe , ledscape_pixel_t* icolors, char* iColorOrder, int nBars, int nLedsProBar);
	virtual ~Wabern();
};

#endif /* LICHTSTEUERUNG_WABERN_H_ */
