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

#include <string.h>
#include <omnetpp.h>

#include "ExistenceMsg_m.h"
#include <machine.hpp>


namespace {
	using namespace Instructions;
	
	//Include the script from file "script" Each instruction is separated by comma
	uint8_t script[] = {
		#include "script"
	};
}

class Vm : public Machine
{
	private:
		cMessage *event; // pointer to the event object which we'll use for timing
		
	public:
		Vm(); // Constructor
		virtual ~Vm(); // Destructor
	
	protected:
		virtual void initialize(); // Installation of the script into the machine
	    	virtual void handleMessage(cMessage *msg);

	    	void refreshHood(); //Function to refresh imports of each neighbour
};

// The module class needs to be registered with OMNeT++
Define_Module(Vm);

Vm::Vm()
{
	event=NULL;
}

Vm::~Vm()
{
	cancelAndDelete(event);
}

unsigned int counter=0; // just a dummy implementation of giving an id

void Vm::initialize()
{
	// Assign a unique id to the vm
	id.id=counter++;
	cDisplayString& dispStr = getDisplayString();
	dispStr.setTagArg("i",1,"white");

	// Read the parameters for the position
	x=par("x");
	y=par("y");
	z=0;//par("z");
	//ev<<"(x,y,z)="<<x<<","<<y<<","<<z<<")"<<endl;
	transmission_range=par("transmission_range");
	
	//Read the dt parameter from ned file
	dt = par("dt");

	// also place the node in the right position. Visual only
	// in code already has the values
	dispStr.setTagArg("p",0,x);
	dispStr.setTagArg("p",1,y);
	
	// To display transmition transmission_range
	//dispStr.setTagArg("r",0,transmission_range);
	
	// Create a new message for sending to yourself
	event = new cMessage("event");

	// Install the script
        install(script);
        while(!finished()){
         	step();
      	}
        
        // Schedule a message to send to yourself
        // Each time you receive a selfmessage then each time
        // to execute a new step
	scheduleAt(par("StartUpDelay"),event);
}

void Vm::handleMessage(cMessage *msg)
{
	// When you receive a selfmessage then execute a round
	if(msg->isSelfMessage())
	{
		green = red = false;		
	
		//Refresh the Neighbourhood
		refreshHood();		

		// Run the machine
		//ev<<"New round"<<endl;
		run();		
		while(!finished()) step();
				
		//Check you leds
		cDisplayString& dispStr = getDisplayString();		
		// Check if there is any light to be open
		if(green)
		{
			ev<<id.id<<": Green: "<<simTime()<<endl;
			dispStr.setTagArg("i",1,"green"); // Change the display argument to green.
		}
		else
			dispStr.setTagArg("i",1,"white");				
			//bubble("I am green");
		if(red)
			dispStr.setTagArg("i",1,"red");				
		
		
		// Resend a message for the next round
		scheduleAt(simTime()+1.0,event);	
	}
}

void Vm::refreshHood()
{
	// Refresh hood to see if other vms are in your transmission_range
	
	// Check all the modules! It would be better if you could check only those in transmission_range
	for (cSubModIterator iter(*getParentModule()); !iter.end(); iter++)
	{
		// Cast the Omnet submodule to VM
		Vm *vm = check_and_cast<Vm *>(iter());
					
		//Check if other machine is in the neighbor
		bool exists=true;
		NeighbourHood::iterator neighbour =hood.find(vm->id);			
		if(neighbour==hood.end())
			exists=false;				
		
		//If already exist then refresh the imports
		if(exists)
		{

			neighbour->imports = vm->thisMachine().imports;
			/*for(int d=0;d<vm->thisMachine().imports.size();d++)
			{
				//neighbour->imports[d]=vm->thisMachine().imports[d];
				ev<<neighbour->imports[d].asNumber()<<" - ";
			}
			ev<<endl;
			//ev<<"refreshed imports"<<endl;
			*/
		}
		
		//calculate the distance between the two vectors
		int dist=sqrt( pow((vm->x-x),2) + pow((vm->y-y),2) + pow((vm->z-z),2)   );
	
		//Check to see if it is in transmission_range and is not in the list yet
		if(dist <= transmission_range && !exists)
		{
			hood.add(vm->id);					
			//EV<<"added n\n";
		}
		//Or if it is out of transmission_range and is still in the list
		else if(dist>transmission_range && exists)
				hood.remove(neighbour);
	}
}
