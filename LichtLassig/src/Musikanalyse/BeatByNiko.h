/*
** Copyright (C) 1998-2010 George Tzanetakis <gtzan@cs.uvic.ca>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifndef MARSYAS_BEATBYNIKO_H
#define MARSYAS_BEATBYNIKO_H

#include <marsyas/system/MarSystem.h>

namespace Marsyas
{
/**
	\class BeatByNiko
	\ingroup Processing
	\brief Basic example on how to use controls efficiently in MarSystems

	This example is the same as Gain; it scales the output by
	multiplying each sample with a real number.

	Controls:
	- \b mrs_real/gain [w] : sets the gain multiplier.
	- \b mrs_bool/dummy [rw] : does nothing.
*/

class marsyas_EXPORT BeatByNiko: public MarSystem
{
private:

  /// Add specific controls needed by this MarSystem.
  void addControls();

  /// Reads changed controls and sets up variables if necessary.
  void myUpdate(MarControlPtr sender);
  MarControlPtr ctrl_meanSamples;
  MarControlPtr ctrl_threshold;
  MarControlPtr ctrl_lowercutofffreq;
  MarControlPtr ctrl_uppercutofffreq;
  MarControlPtr ctrl_freq;

  mrs_natural meanSamples;
  mrs_real threshold;
  mrs_real lowercutoff;
  mrs_real uppercutoff;
  mrs_real freq;

  mrs_natural lowercbin;
  mrs_natural uppercbin;
  mrs_realvec temp;
  mrs_realvec buffer;
  mrs_real mean;

  mrs_real meanlt;
  mrs_natural counter;
  mrs_real old;


public:
  /// BeatByNiko constructor.
  BeatByNiko(std::string name);

  /// BeatByNiko copy constructor.
  BeatByNiko(const BeatByNiko& a);

  /// BeatByNiko destructor.
  ~BeatByNiko();

  /// Implementation of the MarSystem::clone() method.
  MarSystem* clone() const;

  /// Implementation of the MarSystem::myProcess method.
  void myProcess(realvec& in, realvec& out);
};

}
//namespace Marsyas

#endif
//MARSYAS_BEATBYNIKO_H

