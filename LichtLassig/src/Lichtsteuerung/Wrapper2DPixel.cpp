/*
 * Wrapper2DPixel.cpp
 *
 *  Created on: 31.05.2015
 *      Author: christoph
 */

#include "Wrapper2DPixel.h"

Wrapper2DPixel::Wrapper2DPixel() {
	// TODO Auto-generated constructor stub
	pixel = 0;
	enabled = 0;
	next = 0;
}
Wrapper2DPixel::Wrapper2DPixel(double ix, double iy,ledscape_pixel_t* ipixel ){
	world[0] = ix;
	world[1] = iy;
	pixel = ipixel;
	enabled = true;
	next = 0;
}
Wrapper2DPixel::~Wrapper2DPixel() {
	// TODO Auto-generated destructor stub
}

void Wrapper2DPixel::setColor(color_channel_order_t color_channel_order,
		uint8_t r,
		uint8_t g,
		uint8_t b){
	ledscape_pixel_set_color(pixel, color_channel_order, r, g, b);

}

void Wrapper2DPixel::setNext(Wrapper2DPixel* inext){
	next = inext;
}
Wrapper2DPixel* Wrapper2DPixel::getNext(){
	return next;
}

void Wrapper2DPixel::setCoordinates(double x, double y){
	world[0] = x;
	world[1] = y;
}
double* Wrapper2DPixel::getCoordinates(){
	return world;
}

void Wrapper2DPixel::setX(double x){
	world[0] = x;
}
double Wrapper2DPixel::getX(){
	return world[0];
}

void Wrapper2DPixel::setY(double y){
	world[1] = y;
}
double Wrapper2DPixel::getY(){
	return  world[1];
}
