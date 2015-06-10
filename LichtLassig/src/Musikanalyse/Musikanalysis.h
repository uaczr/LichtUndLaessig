/*
 * Musikanalysis.h
 *
 *  Created on: 30.05.2015
 *      Author: christoph
 */

#ifndef MUSIKANALYSIS_H_
#define MUSIKANALYSIS_H_
#include <iostream>
#include <marsyas/system/MarSystemManager.h>
#include <marsyas/system/MarSystem.h>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include "../Lichtsteuerung/PatternGenerator.h"


using namespace Marsyas;
class Musikanalysis {
private:
	MarSystemManager mng;
	MarSystem* topnet;
	MarSystem* fanout;
	MarSystem* analysis1;
	MarSystem* analysis2;
	PatternGenerator* generator;
	boost::mutex m;
	bool running;
public:
	Musikanalysis(PatternGenerator* generator);
	virtual ~Musikanalysis();
	static void runAnalysis(Musikanalysis *music);
	void start();
	void stop();

};

#endif /* MUSIKANALYSIS_H_ */
