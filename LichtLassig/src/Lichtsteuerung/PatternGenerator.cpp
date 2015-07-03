/*
 * PatternGenerator.cpp
 *
 *  Created on: 02.06.2015
 *      Author: christoph
 */

#include "PatternGenerator.h"

#define BARNUM 6
#define LEDSPBARNUM 18
PatternGenerator::PatternGenerator(int num_strips, int num_leds) {
	// TODO Auto-generated constructor stub
	nstrips = num_strips;
	nleds = num_leds;
	beat = false;
	running = false;
	leds = ledscape_init_dmx(nleds);
	frame = ledscape_frame(leds, 0);
	preframe = new ledscape_frame_t[108];
	DMXInfo = (dmxChannels_t*) leds->pru1->data_ram;
	inDMX = (uint8_t*) leds->pru1->data_ram;
	inDMX += 40;
	for (int i = 0; i < 108; i++) {
		for (int j = 0; j < 8; j++) {
			preframe[i].strip[j].a = 0;
			preframe[i].strip[j].b = 0;
			preframe[i].strip[j].c = 0;
		}
	}
	bpmTime = 500;

	colors = new ledscape_pixel_t[20];
	colors_undimmed = new ledscape_pixel_t[20];
	pattern1 = new Pattern*[10];
	pattern2 = new Pattern*[10];
	pattern3 = new Pattern*[10];
	pattern4 = new Pattern*[10];
	pattern5 = new Pattern*[10];
	pattern1[0] = static_cast<Pattern*>(new Wabern(preframe, colors, "GRB",
	BARNUM,
	LEDSPBARNUM));
	pattern1[1] = static_cast<Pattern*>(new Balls(preframe, colors, "GRB",
	BARNUM,
	LEDSPBARNUM));
	pattern1[2] = static_cast<Pattern*>(new Strobe(preframe, colors, "GRB",
	BARNUM,
	LEDSPBARNUM));
	pattern2[0] = static_cast<Pattern*>(new Wabern(preframe, colors, "GRB",
	BARNUM,
	LEDSPBARNUM));
	pattern2[1] = static_cast<Pattern*>(new Balls(preframe, colors, "GRB",
	BARNUM,
	LEDSPBARNUM));
	pattern2[2] = static_cast<Pattern*>(new Strobe(preframe, colors, "GRB",
	BARNUM,
	LEDSPBARNUM));
	pattern3[0] = static_cast<Pattern*>(new Wabern(preframe, colors, "GRB",
	BARNUM,
	LEDSPBARNUM));
	pattern3[1] = static_cast<Pattern*>(new Balls(preframe, colors, "GRB",
	BARNUM,
	LEDSPBARNUM));
	pattern3[2] = static_cast<Pattern*>(new Strobe(preframe, colors, "GRB",
	BARNUM,
	LEDSPBARNUM));
	pattern4[0] = static_cast<Pattern*>(new Wabern(preframe, colors, "GRB",
	BARNUM,
	LEDSPBARNUM));
	pattern4[1] = static_cast<Pattern*>(new Balls(preframe, colors, "GRB",
	BARNUM,
	LEDSPBARNUM));
	pattern4[2] = static_cast<Pattern*>(new Strobe(preframe, colors, "GRB",
	BARNUM,
	LEDSPBARNUM));
	pattern5[0] = static_cast<Pattern*>(new Wabern(preframe, colors, "GRB", 2,
			28));
	pattern5[1] = static_cast<Pattern*>(new Balls(preframe, colors, "GRB", 2,
			28));
	pattern5[2] = static_cast<Pattern*>(new Strobe(preframe, colors, "GRB", 2,
			28));
	setColors();
}

PatternGenerator::~PatternGenerator() {
	// TODO Auto-generated destructor stub
	ledscape_close(leds);
	mbeat.~mutex();
}

void PatternGenerator::sigBeat(PatternGenerator *generator, double ibpmTime,
		double ipower) {
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

void PatternGenerator::loop(PatternGenerator *generator,
		boost::asio::deadline_timer* t, int ideltat) {
	generator->deltat = ideltat;
	if (generator->running) {
		t->expires_at(
				t->expires_at() + boost::posix_time::milliseconds(ideltat));
		t->async_wait(
				boost::bind(PatternGenerator::loop, generator, t, ideltat));
	}
	//std::cout << "loop" << std::endl;
	generator->frameBlack();
	generator->mbeat.try_lock();
	generator->switcher();
	generator->mbeat.unlock();

	/*if(generator->beat)
	 {
	 generator->mbeat.unlock();
	 //generator->pattern1[0]->beat(ideltat, generator->bpmTime, generator->power, 0, 2, 150, 4);
	 //generator->pattern1[1]->beat(ideltat, generator->bpmTime, generator->power, 0, 2, 20, 0);
	 //generator->pattern2[0]->beat(ideltat, generator->bpmTime, generator->power, 1, 2, 150, 4);
	 //generator->pattern2[1]->beat(ideltat, generator->bpmTime, generator->power, 1, 2, 20, 0);
	 generator->pattern2[2]->beat(ideltat, generator->bpmTime, generator->power, 0, 2, 50, 3);
	 }
	 else
	 {
	 generator->mbeat.unlock();
	 //generator->pattern1[0]->noBeat();
	 //generator->pattern1[1]->noBeat();
	 //generator->pattern2[0]->noBeat();
	 //generator->pattern2[1]->noBeat();
	 generator->pattern2[2]->noBeat();
	 }*/
	//cout << "new" << endl;
	ledscape_wait(generator->leds);
	for (int i = 0; i < 108; i++) {
		for (int j = 0; j < 8; j++) {
			generator->frame[i].strip[j].a = generator->preframe[i].strip[j].a;
			generator->frame[i].strip[j].b = generator->preframe[i].strip[j].b;
			generator->frame[i].strip[j].c = generator->preframe[i].strip[j].c;
		}
	}
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
	 cout << generator->DMXInfo->channel5 << endl << endl;*/
	/*for(int j = 0; j < 7; j++){
	 cout << (int)generator->inDMX[j] << "\t";
	 }
	 cout << endl;*/

}

void PatternGenerator::frameBlack() {

	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < nleds; j++) {
			ledscape_set_color(preframe, COLOR_ORDER_BRG, i, j, 0, 0, 0);
		}
	}

}
void PatternGenerator::frameWhite() {
	/*for(int i = 0; i < 2; i++){
	 for(int j = 0; j < nleds; j++){
	 ledscape_set_color(frame, COLOR_ORDER_GRB, i, j, 255, 255, 255);
	 }
	 }*/

}

void PatternGenerator::setColors() {
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

	///yellow
	colors[5].a = 255;
	colors[5].b = 30;
	colors[5].c = 0;

	///white
	colors_undimmed[0].a = 255;
	colors_undimmed[0].b = 255;
	colors_undimmed[0].c = 255;

	///black
	colors_undimmed[1].a = 0;
	colors_undimmed[1].b = 0;
	colors_undimmed[1].c = 0;

	///blue
	colors_undimmed[2].a = 0;
	colors_undimmed[2].b = 0;
	colors_undimmed[2].c = 255;

	///green
	colors_undimmed[3].a = 0;
	colors_undimmed[3].b = 255;
	colors_undimmed[3].c = 0;

	///red
	colors_undimmed[4].a = 255;
	colors_undimmed[4].b = 0;
	colors_undimmed[4].c = 0;

	///red
	colors_undimmed[5].a = 255;
	colors_undimmed[5].b = 30;
	colors_undimmed[5].c = 0;

	//white for moon
	colors_undimmed[6].a = 30;
	colors_undimmed[6].b = 30;
	colors_undimmed[6].c = 30;

	//blue for moon
	colors_undimmed[7].a = 0;
	colors_undimmed[7].b = 0;
	colors_undimmed[7].c = 30;

}
void PatternGenerator::dimm(double dim) {
	for (int i = 0; i < 20; i++) {
		colors[i].a = (uint8_t) quadApp(0, colors_undimmed[i].a, 255, dim);
		colors[i].b = (uint8_t) quadApp(0, colors_undimmed[i].b, 255, dim);
		colors[i].c = (uint8_t) quadApp(0, colors_undimmed[i].c, 255, dim);

	}
}

double PatternGenerator::quadApp(double amp1, double amp2, double deltax,
		double x) {
	if (amp1 > amp2) {
		//cout << "1 ";
		return (double) (amp1 - amp2) / (deltax * deltax) * x * x
				- (double) 2 * (amp1 - amp2) / (deltax) * x + amp1;
	} else {
		//cout << "2 ";
		return (double) (amp2 - amp1) / (deltax * deltax) * x * x + amp1;
	}
}

void PatternGenerator::switcher() {
	/*
	 * DMX 1.Kanal: Patternkombi
	 * DMX 2.Kanal: PatternSuperpos
	 * DMX 3.Kanal: PatternSpeed
	 *
	 */

	//cout << (uint8_t) inDMX[1] / 255 * 6 << endl;
	dimm(double(inDMX[4]));
	if (inDMX[6] <= 4) {
		//cout << (uint8_t) ((double) inDMX[1] / 255 * 6) <<endl;
		switch ((uint8_t) ((double) inDMX[1] / 255 * 6)) ///Patternkombi
		{
		case 0: {

			if (inDMX[2] < (int) (1 * 255 / 5)) ///Patternsuperpos
					{
				///Pattern1-Ebene0 -- Wabern
				pactive[1] = true;
				pcolor[1] = 2; //0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[1] = inDMX[3];
				ptype[1] = 6; //0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
				///Pattern1-Ebene1 -- Balls
				pactive[2] = false;
				pcolor[2] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[2] = inDMX[3];
				ptype[2] = 1;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
				///Pattern1-Ebene2 -- Strobe
				pactive[3] = false;
				pcolor[3] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[3] = inDMX[3];
				ptype[3] = 1;				//0 Triple	1 Standard	3 Switched
				///Pattern2-Ebene0 -- Wabern
				pactive[4] = false;
				pcolor[4] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[4] = inDMX[3];
				ptype[4] = 1;//0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
				///Pattern1-Ebene1 -- Balls
				pactive[5] = false;
				pcolor[5] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[5] = inDMX[3];
				ptype[5] = 2;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
				///Pattern1-Ebene2 -- Strobe
				pactive[6] = false;
				pcolor[6] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[6] = inDMX[3];
				ptype[6] = 1;				//0 Triple	1 Standard	3 Switched

				pactive[7] = true;
				pcolor[7] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[7] = inDMX[3];
				ptype[7] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[8] = false;
				pcolor[8] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[8] = inDMX[3];
				ptype[8] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[9] = false;
				pcolor[9] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[9] = inDMX[3];
				ptype[9] = 1;				//0 Triple	1 Standard	3 Switched

			}
			if (inDMX[2] > (int) (1 * 255 / 5)
					&& inDMX[2] < (int) (2 * 255 / 5)) {
				///Pattern1-Ebene0 -- Wabern
				pactive[1] = false;
				pcolor[1] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[1] = inDMX[3];
				ptype[1] = 1;//0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
				///Pattern1-Ebene1 -- Balls
				pactive[2] = true;
				pcolor[2] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[2] = inDMX[3];
				ptype[2] = 2;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
				///Pattern1-Ebene2 -- Strobe
				pactive[3] = false;
				pcolor[3] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[3] = inDMX[3];
				ptype[3] = 1;				//0 Triple	1 Standard	3 Switched
				///Pattern2-Ebene0 -- Wabern
				pactive[4] = false;
				pcolor[4] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[4] = inDMX[3];
				ptype[4] = 1;//0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
				///Pattern1-Ebene1 -- Balls
				pactive[5] = false;
				pcolor[5] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[5] = inDMX[3];
				ptype[5] = 2;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
				///Pattern1-Ebene2 -- Strobe
				pactive[6] = false;
				pcolor[6] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[6] = inDMX[3];
				ptype[6] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[7] = false;
				pcolor[7] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[7] = inDMX[3];
				ptype[7] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[8] = true;
				pcolor[8] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[8] = inDMX[3];
				ptype[8] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[9] = false;
				pcolor[9] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[9] = inDMX[3];
				ptype[9] = 1;				//0 Triple	1 Standard	3 Switched

			}
			if (inDMX[2] > (int) (2 * 255 / 5)
					&& inDMX[2] < (int) (3 * 255 / 5)) {
				///Pattern1-Ebene0 -- Wabern
				pactive[1] = true;
				pcolor[1] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[1] = inDMX[3];
				ptype[1] = 1;//0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
				///Pattern1-Ebene1 -- Balls
				pactive[2] = true;
				pcolor[2] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[2] = inDMX[3];
				ptype[2] = 2;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
				///Pattern1-Ebene2 -- Strobe
				pactive[3] = false;
				pcolor[3] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[3] = inDMX[3];
				ptype[3] = 1;				//0 Triple	1 Standard	3 Switched
				///Pattern2-Ebene0 -- Wabern
				pactive[4] = false;
				pcolor[4] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[4] = inDMX[3];
				ptype[4] = 1;//0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
				///Pattern1-Ebene1 -- Balls
				pactive[5] = false;
				pcolor[5] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[5] = inDMX[3];
				ptype[5] = 2;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
				///Pattern1-Ebene2 -- Strobe
				pactive[6] = false;
				pcolor[6] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[6] = inDMX[3];
				ptype[6] = 0;				//0 Triple	1 Standard	3 Switched
				pactive[7] = false;
				pcolor[7] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[7] = inDMX[3];
				ptype[7] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[8] = true;
				pcolor[8] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[8] = inDMX[3];
				ptype[8] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[9] = false;
				pcolor[9] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[9] = inDMX[3];
				ptype[9] = 1;				//0 Triple	1 Standard	3 Switched

			}
			if (inDMX[2] > (int) (3 * 255 / 5)
					&& inDMX[2] < (int) (4 * 255 / 5)) {
				///Pattern1-Ebene0 -- Wabern
				pactive[1] = true;
				pcolor[1] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[1] = inDMX[3];
				ptype[1] = 1;//0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
				///Pattern1-Ebene1 -- Balls
				pactive[2] = true;
				pcolor[2] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[2] = inDMX[3];
				ptype[2] = 1;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
				///Pattern1-Ebene2 -- Strobe
				pactive[3] = false;
				pcolor[3] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[3] = inDMX[3];
				ptype[3] = 0;				//0 Triple	1 Standard	3 Switched
				///Pattern2-Ebene0 -- Wabern
				pactive[4] = false;
				pcolor[4] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[4] = inDMX[3];
				ptype[4] = 1;//0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
				///Pattern1-Ebene1 -- Balls
				pactive[5] = false;
				pcolor[5] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[5] = inDMX[3];
				ptype[5] = 2;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
				///Pattern1-Ebene2 -- Strobe
				pactive[6] = true;
				pcolor[6] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[6] = inDMX[3];
				ptype[6] = 5;//0 Triple	1 Standard	2 Switched 3 switched Blue	4 Circle	5 every4	6 every8
				pactive[7] = false;
				pcolor[7] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[7] = inDMX[3];
				ptype[7] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[8] = false;
				pcolor[8] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[8] = inDMX[3];
				ptype[8] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[9] = true;
				pcolor[9] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[9] = inDMX[3];
				ptype[9] = 10;				//0 Triple	1 Standard	3 Switched

			}
			if (inDMX[2] > (int) (4 * 255 / 5)
					&& inDMX[2] <= (int) (5 * 255 / 5)) {
				///Pattern1-Ebene0 -- Wabern
				pactive[1] = true;
				pcolor[1] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[1] = inDMX[3];
				ptype[1] = 1;//0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
				///Pattern1-Ebene1 -- Balls
				pactive[2] = true;
				pcolor[2] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[2] = inDMX[3];
				ptype[2] = 1;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
				///Pattern1-Ebene2 -- Strobe
				pactive[3] = false;
				pcolor[3] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[3] = inDMX[3];
				ptype[3] = 1;				//0 Triple	1 Standard	3 Switched
				///Pattern2-Ebene0 -- Wabern
				pactive[4] = false;
				pcolor[4] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[4] = inDMX[3];
				ptype[4] = 1;//0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
				///Pattern1-Ebene1 -- Balls
				pactive[5] = false;
				pcolor[5] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[5] = inDMX[3];
				ptype[5] = 2;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
				///Pattern1-Ebene2 -- Strobe
				pactive[6] = true;
				pcolor[6] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[6] = inDMX[3];
				ptype[6] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[7] = false;
				pcolor[7] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[7] = inDMX[3];
				ptype[7] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[8] = false;
				pcolor[8] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[8] = inDMX[3];
				ptype[8] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[9] = false;
				pcolor[9] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[9] = inDMX[3];
				ptype[9] = 1;				//0 Triple	1 Standard	3 Switched

			}

		}
			break;
		case 1: {

			if (inDMX[2] < (int) (1 * 255 / 5)) ///Patternsuperpos
					{
				///Pattern1-Ebene0 -- Wabern
				pactive[1] = true;
				pcolor[1] = 2; //0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[1] = inDMX[3];
				ptype[1] = 4; //0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
				///Pattern1-Ebene1 -- Balls
				pactive[2] = false;
				pcolor[2] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[2] = inDMX[3];
				ptype[2] = 1;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
				///Pattern1-Ebene2 -- Strobe
				pactive[3] = false;
				pcolor[3] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[3] = inDMX[3];
				ptype[3] = 1;				//0 Triple	1 Standard	3 Switched
				///Pattern2-Ebene0 -- Wabern
				pactive[4] = false;
				pcolor[4] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[4] = inDMX[3];
				ptype[4] = 4;//0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
				///Pattern1-Ebene1 -- Balls
				pactive[5] = false;
				pcolor[5] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[5] = inDMX[3];
				ptype[5] = 2;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
				///Pattern1-Ebene2 -- Strobe
				pactive[6] = false;
				pcolor[6] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[6] = inDMX[3];
				ptype[6] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[7] = false;
				pcolor[7] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[7] = inDMX[3];
				ptype[7] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[8] = false;
				pcolor[8] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[8] = inDMX[3];
				ptype[8] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[9] = false;
				pcolor[9] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[9] = inDMX[3];
				ptype[9] = 1;				//0 Triple	1 Standard	3 Switched

			}
			if (inDMX[2] > (int) (1 * 255 / 5)
					&& inDMX[2] < (int) (2 * 255 / 5)) {
				///Pattern1-Ebene0 -- Wabern
				pactive[1] = true;
				pcolor[1] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[1] = inDMX[3];
				ptype[1] = 1;//0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
				///Pattern1-Ebene1 -- Balls
				pactive[2] = true;
				pcolor[2] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[2] = inDMX[3];
				ptype[2] = 4;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
				///Pattern1-Ebene2 -- Strobe
				pactive[3] = false;
				pcolor[3] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[3] = inDMX[3];
				ptype[3] = 1;				//0 Triple	1 Standard	3 Switched
				///Pattern2-Ebene0 -- Wabern
				pactive[4] = false;
				pcolor[4] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[4] = inDMX[3];
				ptype[4] = 1;//0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
				///Pattern1-Ebene1 -- Balls
				pactive[5] = false;
				pcolor[5] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[5] = inDMX[3];
				ptype[5] = 4;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
				///Pattern1-Ebene2 -- Strobe
				pactive[6] = false;
				pcolor[6] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[6] = inDMX[3];
				ptype[6] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[7] = false;
				pcolor[7] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[7] = inDMX[3];
				ptype[7] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[8] = false;
				pcolor[8] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[8] = inDMX[3];
				ptype[8] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[9] = false;
				pcolor[9] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[9] = inDMX[3];
				ptype[9] = 1;				//0 Triple	1 Standard	3 Switched

			}
			if (inDMX[2] > (int) (2 * 255 / 5)
					&& inDMX[2] < (int) (3 * 255 / 5)) {
				///Pattern1-Ebene0 -- Wabern
				pactive[1] = true;
				pcolor[1] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[1] = inDMX[3];
				ptype[1] = 1;//0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
				///Pattern1-Ebene1 -- Balls
				pactive[2] = true;
				pcolor[2] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[2] = inDMX[3];
				ptype[2] = 4;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
				///Pattern1-Ebene2 -- Strobe
				pactive[3] = false;
				pcolor[3] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[3] = inDMX[3];
				ptype[3] = 1;				//0 Triple	1 Standard	3 Switched
				///Pattern2-Ebene0 -- Wabern
				pactive[4] = false;
				pcolor[4] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[4] = inDMX[3];
				ptype[4] = 1;//0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
				///Pattern1-Ebene1 -- Balls
				pactive[5] = true;
				pcolor[5] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[5] = inDMX[3];
				ptype[5] = 4;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
				///Pattern1-Ebene2 -- Strobe
				pactive[6] = false;
				pcolor[6] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[6] = inDMX[3];
				ptype[6] = 0;				//0 Triple	1 Standard	3 Switched
				pactive[7] = false;
				pcolor[7] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[7] = inDMX[3];
				ptype[7] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[8] = false;
				pcolor[8] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[8] = inDMX[3];
				ptype[8] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[9] = false;
				pcolor[9] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[9] = inDMX[3];
				ptype[9] = 1;				//0 Triple	1 Standard	3 Switched

			}
			if (inDMX[2] > (int) (3 * 255 / 5)
					&& inDMX[2] < (int) (4 * 255 / 5)) {
				///Pattern1-Ebene0 -- Wabern
				pactive[1] = false;
				pcolor[1] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[1] = inDMX[3];
				ptype[1] = 1;//0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
				///Pattern1-Ebene1 -- Balls
				pactive[2] = false;
				pcolor[2] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[2] = inDMX[3];
				ptype[2] = 1;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
				///Pattern1-Ebene2 -- Strobe
				pactive[3] = true;
				pcolor[3] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[3] = inDMX[3];
				ptype[3] = 0;				//0 Triple	1 Standard	3 Switched
				///Pattern2-Ebene0 -- Wabern
				pactive[4] = false;
				pcolor[4] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[4] = inDMX[3];
				ptype[4] = 1;//0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
				///Pattern1-Ebene1 -- Balls
				pactive[5] = false;
				pcolor[5] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[5] = inDMX[3];
				ptype[5] = 2;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
				///Pattern1-Ebene2 -- Strobe
				pactive[6] = true;
				pcolor[6] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[6] = inDMX[3];
				ptype[6] = 0;				//0 Triple	1 Standard	3 Switched
				pactive[7] = false;
				pcolor[7] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[7] = inDMX[3];
				ptype[7] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[8] = false;
				pcolor[8] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[8] = inDMX[3];
				ptype[8] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[9] = false;
				pcolor[9] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[9] = inDMX[3];
				ptype[9] = 1;				//0 Triple	1 Standard	3 Switched

			}
			if (inDMX[2] > (int) (4 * 255 / 5)
					&& inDMX[2] <= (int) (5 * 255 / 5)) {
				///Pattern1-Ebene0 -- Wabern
				pactive[1] = false;
				pcolor[1] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[1] = inDMX[3];
				ptype[1] = 1;//0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
				///Pattern1-Ebene1 -- Balls
				pactive[2] = false;
				pcolor[2] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[2] = inDMX[3];
				ptype[2] = 1;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
				///Pattern1-Ebene2 -- Strobe
				pactive[3] = true;
				pcolor[3] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[3] = inDMX[3];
				ptype[3] = 1;				//0 Triple	1 Standard	3 Switched
				///Pattern2-Ebene0 -- Wabern
				pactive[4] = false;
				pcolor[4] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[4] = inDMX[3];
				ptype[4] = 1;//0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
				///Pattern1-Ebene1 -- Balls
				pactive[5] = false;
				pcolor[5] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[5] = inDMX[3];
				ptype[5] = 2;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
				///Pattern1-Ebene2 -- Strobe
				pactive[6] = true;
				pcolor[6] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[6] = inDMX[3];
				ptype[6] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[7] = false;
				pcolor[7] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[7] = inDMX[3];
				ptype[7] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[8] = false;
				pcolor[8] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[8] = inDMX[3];
				ptype[8] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[9] = false;
				pcolor[9] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[9] = inDMX[3];
				ptype[9] = 1;				//0 Triple	1 Standard	3 Switched

			}

		}
			break;
		case 2: {

			if (inDMX[2] < (int) (1 * 255 / 5)) ///Patternsuperpos
					{
				///Pattern1-Ebene0 -- Wabern
				pactive[1] = true;
				pcolor[1] = 2; //0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[1] = inDMX[3];
				ptype[1] = 2; //0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
				///Pattern1-Ebene1 -- Balls
				pactive[2] = false;
				pcolor[2] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[2] = inDMX[3];
				ptype[2] = 1;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
				///Pattern1-Ebene2 -- Strobe
				pactive[3] = false;
				pcolor[3] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[3] = inDMX[3];
				ptype[3] = 1;				//0 Triple	1 Standard	3 Switched
				///Pattern2-Ebene0 -- Wabern
				pactive[4] = true;
				pcolor[4] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[4] = inDMX[3];
				ptype[4] = 2;//0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
				///Pattern1-Ebene1 -- Balls
				pactive[5] = false;
				pcolor[5] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[5] = inDMX[3];
				ptype[5] = 2;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
				///Pattern1-Ebene2 -- Strobe
				pactive[6] = false;
				pcolor[6] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[6] = inDMX[3];
				ptype[6] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[7] = false;
				pcolor[7] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[7] = inDMX[3];
				ptype[7] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[8] = false;
				pcolor[8] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[8] = inDMX[3];
				ptype[8] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[9] = false;
				pcolor[9] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[9] = inDMX[3];
				ptype[9] = 1;				//0 Triple	1 Standard	3 Switched

			}
			if (inDMX[2] > (int) (1 * 255 / 5)
					&& inDMX[2] < (int) (2 * 255 / 5)) {
				///Pattern1-Ebene0 -- Wabern
				pactive[1] = true;
				pcolor[1] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[1] = inDMX[3];
				ptype[1] = 3;//0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
				///Pattern1-Ebene1 -- Balls
				pactive[2] = false;
				pcolor[2] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[2] = inDMX[3];
				ptype[2] = 4;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
				///Pattern1-Ebene2 -- Strobe
				pactive[3] = false;
				pcolor[3] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[3] = inDMX[3];
				ptype[3] = 1;				//0 Triple	1 Standard	3 Switched
				///Pattern2-Ebene0 -- Wabern
				pactive[4] = true;
				pcolor[4] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[4] = inDMX[3];
				ptype[4] = 3;//0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
				///Pattern1-Ebene1 -- Balls
				pactive[5] = false;
				pcolor[5] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[5] = inDMX[3];
				ptype[5] = 4;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
				///Pattern1-Ebene2 -- Strobe
				pactive[6] = false;
				pcolor[6] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[6] = inDMX[3];
				ptype[6] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[7] = false;
				pcolor[7] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[7] = inDMX[3];
				ptype[7] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[8] = false;
				pcolor[8] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[8] = inDMX[3];
				ptype[8] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[9] = false;
				pcolor[9] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[9] = inDMX[3];
				ptype[9] = 1;				//0 Triple	1 Standard	3 Switched

			}
			if (inDMX[2] > (int) (2 * 255 / 5)
					&& inDMX[2] < (int) (3 * 255 / 5)) {
				///Pattern1-Ebene0 -- Wabern
				pactive[1] = false;
				pcolor[1] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[1] = inDMX[3];
				ptype[1] = 1;//0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
				///Pattern1-Ebene1 -- Balls
				pactive[2] = false;
				pcolor[2] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[2] = inDMX[3];
				ptype[2] = 3;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
				///Pattern1-Ebene2 -- Strobe
				pactive[3] = true;
				pcolor[3] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[3] = inDMX[3];
				ptype[3] = 2;				//0 Triple	1 Standard	3 Switched
				///Pattern2-Ebene0 -- Wabern
				pactive[4] = false;
				pcolor[4] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[4] = inDMX[3];
				ptype[4] = 1;//0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
				///Pattern1-Ebene1 -- Balls
				pactive[5] = false;
				pcolor[5] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[5] = inDMX[3];
				ptype[5] = 5;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
				///Pattern1-Ebene2 -- Strobe
				pactive[6] = true;
				pcolor[6] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[6] = inDMX[3];
				ptype[6] = 2;				//0 Triple	1 Standard	3 Switched
				pactive[7] = false;
				pcolor[7] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[7] = inDMX[3];
				ptype[7] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[8] = false;
				pcolor[8] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[8] = inDMX[3];
				ptype[8] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[9] = false;
				pcolor[9] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[9] = inDMX[3];
				ptype[9] = 1;				//0 Triple	1 Standard	3 Switched

			}
			if (inDMX[2] > (int) (3 * 255 / 5)
					&& inDMX[2] < (int) (4 * 255 / 5)) {
				///Pattern1-Ebene0 -- Wabern
				pactive[1] = false;
				pcolor[1] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[1] = inDMX[3];
				ptype[1] = 1;//0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
				///Pattern1-Ebene1 -- Balls
				pactive[2] = false;
				pcolor[2] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[2] = inDMX[3];
				ptype[2] = 1;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
				///Pattern1-Ebene2 -- Strobe
				pactive[3] = true;
				pcolor[3] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[3] = inDMX[3];
				ptype[3] = 3;				//0 Triple	1 Standard	3 Switched
				///Pattern2-Ebene0 -- Wabern
				pactive[4] = false;
				pcolor[4] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[4] = inDMX[3];
				ptype[4] = 1;//0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
				///Pattern1-Ebene1 -- Balls
				pactive[5] = false;
				pcolor[5] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[5] = inDMX[3];
				ptype[5] = 2;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
				///Pattern1-Ebene2 -- Strobe
				pactive[6] = true;
				pcolor[6] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[6] = inDMX[3];
				ptype[6] = 3;				//0 Triple	1 Standard	3 Switched
				pactive[7] = false;
				pcolor[7] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[7] = inDMX[3];
				ptype[7] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[8] = false;
				pcolor[8] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[8] = inDMX[3];
				ptype[8] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[9] = false;
				pcolor[9] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[9] = inDMX[3];
				ptype[9] = 1;				//0 Triple	1 Standard	3 Switched

			}
			if (inDMX[2] > (int) (4 * 255 / 5)
					&& inDMX[2] <= (int) (5 * 255 / 5)) {
				///Pattern1-Ebene0 -- Wabern
				pactive[1] = false;
				pcolor[1] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[1] = inDMX[3];
				ptype[1] = 1;//0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
				///Pattern1-Ebene1 -- Balls
				pactive[2] = false;
				pcolor[2] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[2] = inDMX[3];
				ptype[2] = 1;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
				///Pattern1-Ebene2 -- Strobe
				pactive[3] = true;
				pcolor[3] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[3] = inDMX[3];
				ptype[3] = 4;				//0 Triple	1 Standard	3 Switched
				///Pattern2-Ebene0 -- Wabern
				pactive[4] = false;
				pcolor[4] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[4] = inDMX[3];
				ptype[4] = 1;//0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
				///Pattern1-Ebene1 -- Balls
				pactive[5] = false;
				pcolor[5] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[5] = inDMX[3];
				ptype[5] = 2;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
				///Pattern1-Ebene2 -- Strobe
				pactive[6] = true;
				pcolor[6] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[6] = inDMX[3];
				ptype[6] = 4;				//0 Triple	1 Standard	3 Switched
				pactive[7] = false;
				pcolor[7] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[7] = inDMX[3];
				ptype[7] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[8] = false;
				pcolor[8] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[8] = inDMX[3];
				ptype[8] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[9] = false;
				pcolor[9] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[9] = inDMX[3];
				ptype[9] = 1;				//0 Triple	1 Standard	3 Switched

			}
			break;
		}
		case 3: {

			if (inDMX[2] < (int) (1 * 255 / 5)) ///Patternsuperpos
					{
				///Pattern1-Ebene0 -- Wabern
				pactive[1] = true;
				pcolor[1] = 4; //0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[1] = inDMX[3];
				ptype[1] = 5; //0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
				///Pattern1-Ebene1 -- Balls
				pactive[2] = false;
				pcolor[2] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[2] = inDMX[3];
				ptype[2] = 1;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
				///Pattern1-Ebene2 -- Strobe
				pactive[3] = false;
				pcolor[3] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[3] = inDMX[3];
				ptype[3] = 1;				//0 Triple	1 Standard	3 Switched
				///Pattern2-Ebene0 -- Wabern
				pactive[4] = false;
				pcolor[4] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[4] = inDMX[3];
				ptype[4] = 2;//0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
				///Pattern1-Ebene1 -- Balls
				pactive[5] = false;
				pcolor[5] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[5] = inDMX[3];
				ptype[5] = 2;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
				///Pattern1-Ebene2 -- Strobe
				pactive[6] = false;
				pcolor[6] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[6] = inDMX[3];
				ptype[6] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[7] = false;
				pcolor[7] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[7] = inDMX[3];
				ptype[7] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[8] = false;
				pcolor[8] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[8] = inDMX[3];
				ptype[8] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[9] = false;
				pcolor[9] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[9] = inDMX[3];
				ptype[9] = 1;				//0 Triple	1 Standard	3 Switched

			}
			if (inDMX[2] > (int) (1 * 255 / 5)
					&& inDMX[2] < (int) (2 * 255 / 5)) {
				///Pattern1-Ebene0 -- Wabern
				pactive[1] = true;
				pcolor[1] = 4;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[1] = inDMX[3];
				ptype[1] = 5;//0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
				///Pattern1-Ebene1 -- Balls
				pactive[2] = true;
				pcolor[2] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[2] = inDMX[3];
				ptype[2] = 8;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
				///Pattern1-Ebene2 -- Strobe
				pactive[3] = false;
				pcolor[3] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[3] = inDMX[3];
				ptype[3] = 1;				//0 Triple	1 Standard	3 Switched
				///Pattern2-Ebene0 -- Wabern
				pactive[4] = false;
				pcolor[4] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[4] = inDMX[3];
				ptype[4] = 3;//0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
				///Pattern1-Ebene1 -- Balls
				pactive[5] = false;
				pcolor[5] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[5] = inDMX[3];
				ptype[5] = 4;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
				///Pattern1-Ebene2 -- Strobe
				pactive[6] = false;
				pcolor[6] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[6] = inDMX[3];
				ptype[6] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[7] = false;
				pcolor[7] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[7] = inDMX[3];
				ptype[7] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[8] = false;
				pcolor[8] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[8] = inDMX[3];
				ptype[8] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[9] = false;
				pcolor[9] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[9] = inDMX[3];
				ptype[9] = 1;				//0 Triple	1 Standard	3 Switched

			}
			if (inDMX[2] > (int) (2 * 255 / 5)
					&& inDMX[2] < (int) (3 * 255 / 5)) {
				///Pattern1-Ebene0 -- Wabern
				pactive[1] = true;
				pcolor[1] = 4;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[1] = inDMX[3];
				ptype[1] = 5;//0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
				///Pattern1-Ebene1 -- Balls
				pactive[2] = true;
				pcolor[2] = 6;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[2] = inDMX[3];
				ptype[2] = 9;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
				///Pattern1-Ebene2 -- Strobe
				pactive[3] = false;
				pcolor[3] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[3] = inDMX[3];
				ptype[3] = 2;				//0 Triple	1 Standard	3 Switched
				///Pattern2-Ebene0 -- Wabern
				pactive[4] = false;
				pcolor[4] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[4] = inDMX[3];
				ptype[4] = 1;//0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
				///Pattern1-Ebene1 -- Balls
				pactive[5] = false;
				pcolor[5] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[5] = inDMX[3];
				ptype[5] = 5;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
				///Pattern1-Ebene2 -- Strobe
				pactive[6] = false;
				pcolor[6] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[6] = inDMX[3];
				ptype[6] = 2;				//0 Triple	1 Standard	3 Switched
				pactive[7] = false;
				pcolor[7] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[7] = inDMX[3];
				ptype[7] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[8] = false;
				pcolor[8] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[8] = inDMX[3];
				ptype[8] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[9] = false;
				pcolor[9] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[9] = inDMX[3];
				ptype[9] = 1;				//0 Triple	1 Standard	3 Switched

			}
			if (inDMX[2] > (int) (3 * 255 / 5)
					&& inDMX[2] < (int) (4 * 255 / 5)) {
				///Pattern1-Ebene0 -- Wabern
				pactive[1] = false;
				pcolor[1] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[1] = inDMX[3];
				ptype[1] = 1;//0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
				///Pattern1-Ebene1 -- Balls
				pactive[2] = false;
				pcolor[2] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[2] = inDMX[3];
				ptype[2] = 1;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
				///Pattern1-Ebene2 -- Strobe
				pactive[3] = true;
				pcolor[3] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[3] = inDMX[3];
				ptype[3] = 3;				//0 Triple	1 Standard	3 Switched
				///Pattern2-Ebene0 -- Wabern
				pactive[4] = false;
				pcolor[4] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[4] = inDMX[3];
				ptype[4] = 1;//0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
				///Pattern1-Ebene1 -- Balls
				pactive[5] = false;
				pcolor[5] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[5] = inDMX[3];
				ptype[5] = 2;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
				///Pattern1-Ebene2 -- Strobe
				pactive[6] = true;
				pcolor[6] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[6] = inDMX[3];
				ptype[6] = 3;				//0 Triple	1 Standard	3 Switched
				pactive[7] = false;
				pcolor[7] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[7] = inDMX[3];
				ptype[7] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[8] = false;
				pcolor[8] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[8] = inDMX[3];
				ptype[8] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[9] = false;
				pcolor[9] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[9] = inDMX[3];
				ptype[9] = 1;				//0 Triple	1 Standard	3 Switched

			}
			if (inDMX[2] > (int) (4 * 255 / 5)
					&& inDMX[2] <= (int) (5 * 255 / 5)) {
				///Pattern1-Ebene0 -- Wabern
				pactive[1] = false;
				pcolor[1] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[1] = inDMX[3];
				ptype[1] = 1;//0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
				///Pattern1-Ebene1 -- Balls
				pactive[2] = false;
				pcolor[2] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[2] = inDMX[3];
				ptype[2] = 1;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
				///Pattern1-Ebene2 -- Strobe
				pactive[3] = true;
				pcolor[3] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[3] = inDMX[3];
				ptype[3] = 4;				//0 Triple	1 Standard	3 Switched
				///Pattern2-Ebene0 -- Wabern
				pactive[4] = false;
				pcolor[4] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[4] = inDMX[3];
				ptype[4] = 1;//0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
				///Pattern1-Ebene1 -- Balls
				pactive[5] = false;
				pcolor[5] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[5] = inDMX[3];
				ptype[5] = 2;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
				///Pattern1-Ebene2 -- Strobe
				pactive[6] = true;
				pcolor[6] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[6] = inDMX[3];
				ptype[6] = 4;				//0 Triple	1 Standard	3 Switched
				pactive[7] = false;
				pcolor[7] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[7] = inDMX[3];
				ptype[7] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[8] = false;
				pcolor[8] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[8] = inDMX[3];
				ptype[8] = 1;				//0 Triple	1 Standard	3 Switched
				pactive[9] = false;
				pcolor[9] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
				pspeed[9] = inDMX[3];
				ptype[9] = 1;				//0 Triple	1 Standard	3 Switched

			}
			break;
		}

		}

	} else {
		if (inDMX[5] >= 0 && inDMX[5] < (int) (1 * 255 / 7)) ///Patternsuperpos
				{
			///Pattern1-Ebene0 -- Wabern
			pactive[1] = false;
			pcolor[1] = 2; //0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[1] = inDMX[3];
			ptype[1] = 1; //0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
			///Pattern1-Ebene1 -- Balls
			pactive[2] = false;
			pcolor[2] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[2] = inDMX[3];
			ptype[2] = 1;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
			///Pattern1-Ebene2 -- Strobe
			pactive[3] = true;
			pcolor[3] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[3] = inDMX[6];
			ptype[3] = 1;				//0 Triple	1 Standard	3 Switched
			///Pattern2-Ebene0 -- Wabern
			pactive[4] = false;
			pcolor[4] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[4] = inDMX[3];
			ptype[4] = 1;//0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
			///Pattern1-Ebene1 -- Balls
			pactive[5] = false;
			pcolor[5] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[5] = inDMX[3];
			ptype[5] = 2;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
			///Pattern1-Ebene2 -- Strobe
			pactive[6] = true;
			pcolor[6] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[6] = inDMX[6];
			ptype[6] = 1;				//0 Triple	1 Standard	3 Switched
			pactive[7] = false;
			pcolor[7] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[7] = inDMX[3];
			ptype[7] = 1;				//0 Triple	1 Standard	3 Switched
			pactive[8] = false;
			pcolor[8] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[8] = inDMX[3];
			ptype[8] = 1;				//0 Triple	1 Standard	3 Switched
			pactive[9] = false;
			pcolor[9] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[9] = inDMX[3];
			ptype[9] = 1;				//0 Triple	1 Standard	3 Switched

		}
		if (inDMX[5] > (int) (1 * 255 / 7) && inDMX[5] < (int) (2 * 255 / 7)) ///Patternsuperpos
				{
			///Pattern1-Ebene0 -- Wabern
			pactive[1] = false;
			pcolor[1] = 2; //0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[1] = inDMX[3];
			ptype[1] = 1; //0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
			///Pattern1-Ebene1 -- Balls
			pactive[2] = false;
			pcolor[2] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[2] = inDMX[3];
			ptype[2] = 1;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
			///Pattern1-Ebene2 -- Strobe
			pactive[3] = true;
			pcolor[3] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[3] = inDMX[6];
			ptype[3] = 1;				//0 Triple	1 Standard	3 Switched
			///Pattern2-Ebene0 -- Wabern
			pactive[4] = false;
			pcolor[4] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[4] = inDMX[3];
			ptype[4] = 1;//0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
			///Pattern1-Ebene1 -- Balls
			pactive[5] = false;
			pcolor[5] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[5] = inDMX[3];
			ptype[5] = 2;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
			///Pattern1-Ebene2 -- Strobe
			pactive[6] = true;
			pcolor[6] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[6] = inDMX[6];
			ptype[6] = 1;				//0 Triple	1 Standard	3 Switched
			pactive[7] = false;
			pcolor[7] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[7] = inDMX[3];
			ptype[7] = 1;				//0 Triple	1 Standard	3 Switched
			pactive[8] = false;
			pcolor[8] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[8] = inDMX[3];
			ptype[8] = 1;				//0 Triple	1 Standard	3 Switched
			pactive[9] = false;
			pcolor[9] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[9] = inDMX[3];
			ptype[9] = 1;				//0 Triple	1 Standard	3 Switched

		}
		if (inDMX[5] > (int) (2 * 255 / 7) && inDMX[5] < (int) (3 * 255 / 7)) ///Patternsuperpos
				{
			///Pattern1-Ebene0 -- Wabern
			pactive[1] = false;
			pcolor[1] = 2; //0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[1] = inDMX[3];
			ptype[1] = 1; //0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
			///Pattern1-Ebene1 -- Balls
			pactive[2] = false;
			pcolor[2] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[2] = inDMX[3];
			ptype[2] = 1;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
			///Pattern1-Ebene2 -- Strobe
			pactive[3] = true;
			pcolor[3] = 4;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[3] = inDMX[6];
			ptype[3] = 1;				//0 Triple	1 Standard	3 Switched
			///Pattern2-Ebene0 -- Wabern
			pactive[4] = false;
			pcolor[4] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[4] = inDMX[3];
			ptype[4] = 1;//0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
			///Pattern1-Ebene1 -- Balls
			pactive[5] = false;
			pcolor[5] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[5] = inDMX[3];
			ptype[5] = 2;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
			///Pattern1-Ebene2 -- Strobe
			pactive[6] = true;
			pcolor[6] = 4;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[6] = (uint8_t) ((double) inDMX[6] * 0.9);
			ptype[6] = 1;				//0 Triple	1 Standard	3 Switched
			pactive[7] = false;
			pcolor[7] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[7] = inDMX[3];
			ptype[7] = 1;				//0 Triple	1 Standard	3 Switched
			pactive[8] = false;
			pcolor[8] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[8] = inDMX[3];
			ptype[8] = 1;				//0 Triple	1 Standard	3 Switched
			pactive[9] = false;
			pcolor[9] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[9] = inDMX[3];
			ptype[9] = 1;				//0 Triple	1 Standard	3 Switched

		}
		if (inDMX[5] > (int) (3 * 255 / 7) && inDMX[5] < (int) (4 * 255 / 7)) ///Patternsuperpos
				{
			///Pattern1-Ebene0 -- Wabern
			pactive[1] = false;
			pcolor[1] = 2; //0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[1] = inDMX[3];
			ptype[1] = 1; //0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
			///Pattern1-Ebene1 -- Balls
			pactive[2] = false;
			pcolor[2] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[2] = inDMX[3];
			ptype[2] = 1;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
			///Pattern1-Ebene2 -- Strobe
			pactive[3] = true;
			pcolor[3] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[3] = inDMX[6];
			ptype[3] = 7;				//0 Triple	1 Standard	3 Switched
			///Pattern2-Ebene0 -- Wabern
			pactive[4] = false;
			pcolor[4] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[4] = inDMX[3];
			ptype[4] = 1;//0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
			///Pattern1-Ebene1 -- Balls
			pactive[5] = false;
			pcolor[5] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[5] = inDMX[3];
			ptype[5] = 2;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
			///Pattern1-Ebene2 -- Strobe
			pactive[6] = true;
			pcolor[6] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[6] = (uint8_t) ((double) inDMX[6] * 0.9);
			ptype[6] = 7;				//0 Triple	1 Standard	3 Switched
			pactive[7] = false;
			pcolor[7] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[7] = inDMX[3];
			ptype[7] = 1;				//0 Triple	1 Standard	3 Switched
			pactive[8] = false;
			pcolor[8] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[8] = inDMX[3];
			ptype[8] = 1;				//0 Triple	1 Standard	3 Switched
			pactive[9] = false;
			pcolor[9] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[9] = inDMX[3];
			ptype[9] = 1;				//0 Triple	1 Standard	3 Switched

		}
		if (inDMX[5] > (int) (4 * 255 / 7) && inDMX[5] < (int) (5 * 255 / 7)) ///Patternsuperpos
				{
			///Pattern1-Ebene0 -- Wabern
			pactive[1] = false;
			pcolor[1] = 2; //0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[1] = inDMX[3];
			ptype[1] = 1; //0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
			///Pattern1-Ebene1 -- Balls
			pactive[2] = false;
			pcolor[2] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[2] = inDMX[3];
			ptype[2] = 1;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
			///Pattern1-Ebene2 -- Strobe
			pactive[3] = true;
			pcolor[3] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[3] = inDMX[6];
			ptype[3] = 8;				//0 Triple	1 Standard	3 Switched
			///Pattern2-Ebene0 -- Wabern
			pactive[4] = false;
			pcolor[4] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[4] = inDMX[3];
			ptype[4] = 1;//0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
			///Pattern1-Ebene1 -- Balls
			pactive[5] = false;
			pcolor[5] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[5] = inDMX[3];
			ptype[5] = 2;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
			///Pattern1-Ebene2 -- Strobe
			pactive[6] = true;
			pcolor[6] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[6] = inDMX[6];
			ptype[6] = 8;				//0 Triple	1 Standard	3 Switched
			pactive[7] = false;
			pcolor[7] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[7] = inDMX[3];
			ptype[7] = 1;				//0 Triple	1 Standard	3 Switched
			pactive[8] = false;
			pcolor[8] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[8] = inDMX[3];
			ptype[8] = 1;				//0 Triple	1 Standard	3 Switched
			pactive[9] = false;
			pcolor[9] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[9] = inDMX[3];
			ptype[9] = 1;				//0 Triple	1 Standard	3 Switched

		}
		if (inDMX[5] > (int) (5 * 255 / 7) && inDMX[5] < (int) (6 * 255 / 7)) ///Patternsuperpos
				{
			///Pattern1-Ebene0 -- Wabern
			pactive[1] = false;
			pcolor[1] = 2; //0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[1] = inDMX[3];
			ptype[1] = 1; //0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
			///Pattern1-Ebene1 -- Balls
			pactive[2] = false;
			pcolor[2] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[2] = inDMX[3];
			ptype[2] = 1;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
			///Pattern1-Ebene2 -- Strobe
			pactive[3] = true;
			pcolor[3] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[3] = inDMX[6];
			ptype[3] = 9;				//0 Triple	1 Standard	3 Switched
			///Pattern2-Ebene0 -- Wabern
			pactive[4] = false;
			pcolor[4] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[4] = inDMX[3];
			ptype[4] = 1;//0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
			///Pattern1-Ebene1 -- Balls
			pactive[5] = false;
			pcolor[5] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[5] = inDMX[3];
			ptype[5] = 2;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
			///Pattern1-Ebene2 -- Strobe
			pactive[6] = true;
			pcolor[6] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[6] = inDMX[6];
			ptype[6] = 9;				//0 Triple	1 Standard	3 Switched
			pactive[7] = false;
			pcolor[7] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[7] = inDMX[3];
			ptype[7] = 1;				//0 Triple	1 Standard	3 Switched
			pactive[8] = false;
			pcolor[8] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[8] = inDMX[3];
			ptype[8] = 1;				//0 Triple	1 Standard	3 Switched
			pactive[9] = false;
			pcolor[9] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[9] = inDMX[3];
			ptype[9] = 1;				//0 Triple	1 Standard	3 Switched

		}
		if (inDMX[5] > (int) (6 * 255 / 7) && inDMX[5] < (int) (7 * 255 / 7)) ///Patternsuperpos
				{
			///Pattern1-Ebene0 -- Wabern
			pactive[1] = false;
			pcolor[1] = 2; //0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[1] = inDMX[3];
			ptype[1] = 1; //0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
			///Pattern1-Ebene1 -- Balls
			pactive[2] = false;
			pcolor[2] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[2] = inDMX[3];
			ptype[2] = 1;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
			///Pattern1-Ebene2 -- Strobe
			pactive[3] = true;
			pcolor[3] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[3] = inDMX[6];
			ptype[3] = 10;				//0 Triple	1 Standard	3 Switched
			///Pattern2-Ebene0 -- Wabern
			pactive[4] = false;
			pcolor[4] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[4] = inDMX[3];
			ptype[4] = 1;//0 Rect	1 Linear	2 Switched	3 Circle 	4 Equalizer
			///Pattern1-Ebene1 -- Balls
			pactive[5] = false;
			pcolor[5] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[5] = inDMX[3];
			ptype[5] = 2;//0 Rising	1 Falling	2 Exploding	3 ExpDim	4 Spring	5 Raining
			///Pattern1-Ebene2 -- Strobe
			pactive[6] = true;
			pcolor[6] = 2;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[6] = inDMX[6];
			ptype[6] = 10;				//0 Triple	1 Standard	3 Switched
			pactive[7] = false;
			pcolor[7] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[7] = inDMX[3];
			ptype[7] = 1;				//0 Triple	1 Standard	3 Switched
			pactive[8] = false;
			pcolor[8] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[8] = inDMX[3];
			ptype[8] = 1;				//0 Triple	1 Standard	3 Switched
			pactive[9] = false;
			pcolor[9] = 0;//0 Weiß 	1 Schwarz 	2 Blau		3 Grün		4 Rot
			pspeed[9] = inDMX[3];
			ptype[9] = 1;				//0 Triple	1 Standard	3 Switched

		}

	}

	if (beat) {
		/*cout << "DMX Channels \t";
		 for (int j = 1; j < 7; j++) {
		 cout << (int) inDMX[j] << "\t";
		 }
		 cout << endl;*/

		if (pactive[1]) {
			pattern1[0]->beat(deltat, bpmTime, power, 0, pcolor[1], pspeed[1],
					ptype[1]);
			pattern3[0]->beat(deltat, bpmTime, power, 3, pcolor[1], pspeed[1],
					ptype[1]);

		}
		if (pactive[2]) {
			pattern1[1]->beat(deltat, bpmTime, power, 0, pcolor[2], pspeed[2],
					ptype[2]);
			pattern3[1]->beat(deltat, bpmTime, power, 3, pcolor[2], pspeed[2],
					ptype[2]);
		}
		if (pactive[3]) {
			pattern1[2]->beat(deltat, bpmTime, power, 0, pcolor[3], pspeed[3],
					ptype[3]);
			pattern3[2]->beat(deltat, bpmTime, power, 3, pcolor[3], pspeed[3],
					ptype[3]);
		}
		if (pactive[4]) {
			pattern2[0]->beat(deltat, bpmTime, power, 1, pcolor[4], pspeed[4],
					ptype[4]);
			pattern4[0]->beat(deltat, bpmTime, power, 4, pcolor[4], pspeed[4],
					ptype[4]);

		}
		if (pactive[5]) {
			pattern2[1]->beat(deltat, bpmTime, power, 1, pcolor[5], pspeed[5],
					ptype[5]);
			pattern4[1]->beat(deltat, bpmTime, power, 4, pcolor[5], pspeed[5],
					ptype[5]);
		}
		if (pactive[6]) {
			pattern2[2]->beat(deltat, bpmTime, power, 1, pcolor[6], pspeed[6],
					ptype[6]);
			pattern4[2]->beat(deltat, bpmTime, power, 4, pcolor[6], pspeed[6],
					ptype[6]);
		}

		if (pactive[7]) {
			pattern5[0]->beat(deltat, bpmTime, power, 5, pcolor[7], pspeed[7],
					ptype[7]);
		}
		if (pactive[8]) {
			pattern5[1]->beat(deltat, bpmTime, power, 5, pcolor[8], pspeed[8],
					ptype[8]);
		}
		if (pactive[9]) {
			pattern5[2]->beat(deltat, bpmTime, power, 5, pcolor[9], pspeed[9],
					ptype[9]);

		}
	} else {
		if (pactive[1]) {
			pattern1[0]->noBeat(pcolor[1], pspeed[1], ptype[1]);
			pattern3[0]->noBeat(pcolor[1], pspeed[1], ptype[1]);
		}
		if (pactive[2]) {
			pattern1[1]->noBeat(pcolor[2], pspeed[2], ptype[2]);
			pattern3[1]->noBeat(pcolor[2], pspeed[2], ptype[2]);
		}
		if (pactive[3]) {
			pattern1[2]->noBeat(pcolor[3], pspeed[3], ptype[3]);
			pattern3[2]->noBeat(pcolor[3], pspeed[3], ptype[3]);
		}
		if (pactive[4]) {
			pattern2[0]->noBeat(pcolor[4], pspeed[4], ptype[4]);
			pattern4[0]->noBeat(pcolor[4], pspeed[4], ptype[4]);
		}
		if (pactive[5]) {
			pattern2[1]->noBeat(pcolor[5], pspeed[5], ptype[5]);
			pattern4[1]->noBeat(pcolor[5], pspeed[5], ptype[5]);
		}
		if (pactive[6]) {
			pattern2[2]->noBeat(pcolor[6], pspeed[6], ptype[6]);
			pattern4[2]->noBeat(pcolor[6], pspeed[6], ptype[6]);
		}
		if (pactive[7]) {
			pattern5[0]->noBeat(pcolor[7], pspeed[7], ptype[7]);
		}
		if (pactive[8]) {
			pattern5[1]->noBeat(pcolor[8], pspeed[8], ptype[8]);
		}
		if (pactive[9]) {
			pattern5[2]->noBeat(pcolor[9], pspeed[9], ptype[9]);
		}

	}
}
