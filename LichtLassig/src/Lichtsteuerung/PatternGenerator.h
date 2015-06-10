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

class PatternGenerator {
	Pattern** pattern1;
	Pattern** pattern2;
	boost::mutex mbeat;
	boost::mutex mbpm;
	boost::mutex mpower;
	bool beat;
	ledscape_t* leds;
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
