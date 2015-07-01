

#include <iostream>
#include <marsyas/system/MarSystemManager.h>

extern "C"{
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <inttypes.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
}
#include "Musikanalyse/Musikanalysis.h"
#include "Lichtsteuerung/PatternGenerator.h"
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>


using namespace std;
using namespace Marsyas;


int main() {
	PatternGenerator generator(8, 108);
	generator.running = true;
	boost::asio::io_service io;
	boost::asio::deadline_timer t(io, boost::posix_time::milliseconds(8));
	t.async_wait(boost::bind(PatternGenerator::loop, &generator, &t, 8));
	Musikanalysis musik(&generator);
	musik.start();
	std::cout << "m" << std::endl;
	io.run();
	cin.get();
	musik.stop();
	generator.running = false;

}
