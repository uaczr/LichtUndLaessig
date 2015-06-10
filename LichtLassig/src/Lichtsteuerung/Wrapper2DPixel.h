/*
 * Wrapper2DPixel.h
 *
 *  Created on: 31.05.2015
 *      Author: christoph
 */

#ifndef Wrapper2DPixel_H_
#define Wrapper2DPixel_H_
extern "C"{
#include "../Ledscape/include/ledscape.h"
}

class Wrapper2DPixel{
	//Pixel existiert??
	bool enabled;

	//Position im Raum
	double world[2];
	ledscape_pixel_t* pixel;

	//nächster Pixel
	Wrapper2DPixel* next;
public:
	Wrapper2DPixel();
	Wrapper2DPixel(double ix, double iy,ledscape_pixel_t* ipixel);
	~Wrapper2DPixel();
	void setNext(Wrapper2DPixel* inext);
	Wrapper2DPixel* getNext();

	void setCoordinates(double x, double y);
	double* getCoordinates();

	void setX(double x);
	double getX();

	void setY(double y);
	double getY();

	void setColor(color_channel_order_t color_channel_order,
			uint8_t r,
			uint8_t g,
			uint8_t b);
};

#endif /* Wrapper2DPixel_H_ */
