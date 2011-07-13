// This file is part of DelftProto.
// 
// DelftProto is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option) any
// later version.
// 
// DelftProto is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
// PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License along
// with DelftProto.  If not, see <http://www.gnu.org/licenses/>.

/// \file
/// The extension of the Machine

/** \file
 * The extension of the Machine
 * 
 * The Sim_Machine class inherits both the Machine and the cSimpleModule
 * Machine is the class of the Virtual Machine
 * cSimpleModule is an Omnet++ class for simple Modules
 */
 
#include <types.hpp>
#include <omnetpp.h>

struct Sim_Machine :  Machine, cSimpleModule {

	// positioning & transmission range variables
	double x,y,z,transmission_range;
	
	// for sensing. Not using at this moment
	bool sense;
	
	// variables for leds
	bool green;
	bool red;
	
	// timing variable. SET_DT and DT opcode
	double dt;

	Sim_Machine() : x(0),y(0),z(0),transmission_range(0),sense(false),green(false),red(false),dt(0.2){}
	
	protected:
	    // The following redefined virtual function holds the algorithm.
	    virtual void initialize() = 0;
	    virtual void handleMessage(cMessage *msg) = 0;
};

#undef  Machine
#define Machine Sim_Machine
