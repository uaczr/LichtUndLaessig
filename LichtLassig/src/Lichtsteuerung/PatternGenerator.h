/*
 * PatternGenerator.h
 *
 *  Created on: 02.06.2015
 *      Author: christoph
 */

#ifndef LICHTSTEUERUNG_PATTERNGENERATOR_H_
#define LICHTSTEUERUNG_PATTERNGENERATOR_H_
extern "C"{
#include "../Ledscape/include/ledscape.h"
#include <stdint.h>
}
#include <boost/thread/mutex.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>
#include "Pattern.h"
#include "Wabern.h"
#include "Balls.h"
#include "Strobe.h"

typedef struct dmxChannels
{
	// in the DDR shared with the PRU
	uintptr_t pixels_dma;

	// Length in pixels of the longest LED strip.
	unsigned num_pixels;

	// write 1 to start, 0xFF to abort. will be cleared when started
	volatile unsigned command;

	// will have a non-zero response written when done
	volatile unsigned response;

	volatile unsigned int channel1;
	volatile unsigned int channel2;
	volatile unsigned int channel3;
	volatile unsigned int channel4;
	volatile unsigned int channel5;
	volatile unsigned int channel6;
} __attribute__((__packed__)) dmxChannels_t;

class PatternGenerator {
	Pattern** pattern1;
	Pattern** pattern2;
	boost::mutex mbeat;
	boost::mutex mbpm;
	boost::mutex mpower;
	bool beat;
	ledscape_t* leds;
	dmxChannels_t* DMXInfo;
	uint8_t* inDMX;
	ledscape_frame_t* frame;
	int nstrips;
	int nleds;
	double bpmTime;
	double power;
	ledscape_pixel_t* colors;
	void setColors();
public:
	bool running;
	PatternGenerator(int num_strips, int num_leds);
	virtual ~PatternGenerator();
	static void sigBeat(PatternGenerator *generator, double ibpmTime, double ipower);
	static void loop(PatternGenerator *generator, boost::asio::deadline_timer* t, int deltat);
	void frameBlack();
	void frameWhite();
};

#endif /* LICHTSTEUERUNG_PATTERNGENERATOR_H_ */
