/*
 * PatternGenerator.cpp
 *
 *  Created on: 02.06.2015
 *      Author: christoph
 */

#include "PatternGenerator.h"
PatternGenerator::PatternGenerator(int num_strips, int num_leds) {
	// TODO Auto-generated constructor stub
	nstrips = num_strips;
	nleds = num_leds;
	beat = false;
	running = false;
	leds = ledscape_init_dmx(nleds);
	frame = ledscape_frame(leds, 0);
	DMXInfo = (dmxChannels_t*)leds->pru1->data_ram;
	inDMX = (uint8_t*)leds->pru1->data_ram;
	inDMX += 40;

	bpmTime = 500;

	colors = new ledscape_pixel_t[20];
	pattern1 = new Pattern*[10];
	pattern2 = new Pattern*[10];
	pattern1[0] = static_cast<Pattern*>(new Wabern(frame, colors, "GRB"));
	pattern1[1] = static_cast<Pattern*>(new Balls(frame, colors, "GRB"));
	pattern2[0] = static_cast<Pattern*>(new Wabern(frame, colors, "GRB"));
	pattern2[1] = static_cast<Pattern*>(new Balls(frame, colors, "GRB"));
	//pattern2[2] = static_cast<Pattern*>(new Strobe(frame, colors, "GRB"));
	setColors();
}

PatternGenerator::~PatternGenerator() {
	// TODO Auto-generated destructor stub
	ledscape_close(leds);
	mbeat.~mutex();
}

void PatternGenerator::sigBeat(PatternGenerator *generator, double ibpmTime, double ipower)
{
	//std::cout << "signal" << std::endl;
	generator->mbeat.lock();
	generator->beat = true;
	generator->mbeat.unlock();
	generator->mbpm.lock();
	generator->bpmTime = ibpmTime;
	generator->mbpm.unlock();
	generator->mpower.lock();
	generator->power = ipower;
	generator->mpower.unlock();
}

void PatternGenerator::loop(PatternGenerator *generator, boost::asio::deadline_timer* t, int deltat)
{
	if(generator->running){
		t->expires_at(t->expires_at() + boost::posix_time::milliseconds(deltat));
		t->async_wait(boost::bind(PatternGenerator::loop, generator, t, deltat));
	}
	//std::cout << "loop" << std::endl;
	generator->mbeat.try_lock();
	if(generator->beat)
	{
		generator->mbeat.unlock();
		generator->pattern1[0]->beat(deltat, generator->bpmTime, generator->power, 0, generator->nleds, 2, 100, 1);
		generator->pattern2[0]->beat(deltat, generator->bpmTime, generator->power, 1, generator->nleds, 2, 100, 1);
		//generator->pattern1[1]->beat(deltat, generator->bpmTime, generator->power, 0, generator->nleds, 0, 8, 0);
		//generator->pattern2[1]->beat(deltat, generator->bpmTime, generator->power, 1, generator->nleds, 0, 8, 0);
		//generator->pattern2[2]->beat(deltat, generator->bpmTime, generator->power, 1, generator->nleds, 0, 15, 0);
	}
	else
	{
		generator->mbeat.unlock();
		generator->pattern1[0]->noBeat();
		generator->pattern2[0]->noBeat();
		//generator->pattern1[1]->noBeat();
		//generator->pattern2[1]->noBeat();
		//generator->pattern2[2]->noBeat();
	}
	//cout << "new" << endl;
	ledscape_wait(generator->leds);
	ledscape_draw(generator->leds, 0);
	generator->mbeat.lock();
	generator->beat = false;
	generator->mbeat.unlock();
	generator->DMXInfo->command = 1;
	/*cout << generator->DMXInfo->command << endl;
	cout << generator->DMXInfo->channel1 << endl;
	cout << generator->DMXInfo->channel2 << endl;
	cout << generator->DMXInfo->channel3 << endl;
	cout << generator->DMXInfo->channel4 << endl;
	cout << generator->DMXInfo->channel5 << endl << endl;
	for(int j = 0; j < 7; j++){
		cout << (int)generator->inDMX[j] << "\t";
	}
	cout << endl;*/

}

void PatternGenerator::frameBlack()
{

	/*for(int i = 0; i < 2; i++){
		for(int j = 0; j < nleds; j++){
			ledscape_set_color(frame, COLOR_ORDER_BRG, i, j, 0, 0, 0);
		}
	}*/


}
void PatternGenerator::frameWhite()
{
	/*for(int i = 0; i < 2; i++){
		for(int j = 0; j < nleds; j++){
			ledscape_set_color(frame, COLOR_ORDER_GRB, i, j, 255, 255, 255);
		}
	}*/


}

void PatternGenerator::setColors(){
	///white
	colors[0].a = 255;
	colors[0].b = 255;
	colors[0].c = 255;

	///black
	colors[1].a = 0;
	colors[1].b = 0;
	colors[1].c = 0;

	///blue
	colors[2].a = 0;
	colors[2].b = 0;
	colors[2].c = 255;

	///green
	colors[3].a = 0;
	colors[3].b = 255;
	colors[3].c = 0;

	///red
	colors[4].a = 255;
	colors[4].b = 0;
	colors[4].c = 0;
}
