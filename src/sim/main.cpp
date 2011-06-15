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

#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>

#include <math.h>

#include <instructions.hpp>
#include <machine.hpp>
#include <types.hpp>
#include <data.hpp>
#include <stack.hpp>

using namespace std;

namespace {
	using namespace Instructions;
	
	Int8 script[] = { DEF_VM_OP, 0, 0, 0, 100, 1, 0, 100, 100,
	                  DEF_FUN_6_OP, REF_0_OP, GLO_REF_1_OP, INIT_FEEDBACK_OP, 0, ADD_OP, RET_OP,
	                  DEF_FUN_2_OP, LIT_2_OP, RET_OP,
	                  DEF_FUN_OP, 15, HELLOWORLD_OP, GLO_REF_0_OP, LIT_0_OP, LIT_1_OP, LIT_2_OP, FAB_TUP_OP, 3, TUP_MAP_OP, LIT_3_OP, LIT_1_OP, LIT_2_OP, FAB_TUP_OP, 3, VEC_ADD_OP, RET_OP,
	                  EXIT_OP };

	Int8 script1[] = { DEF_VM_OP, 0, 5, 0, 100, 1, 0, 100, 100,
					DEF_FUN_4_OP,
						REF_1_OP,REF_0_OP,MAX_OP,RET_OP,
					DEF_FUN_OP, 8,
						//CHECK_NB_OP,
						GLO_REF_0_OP,
						LIT8_OP,
						0,
						LIT8_OP,
						15,
						FOLD_HOOD_OP, 0,
						RET_OP,
					EXIT_OP };
	/*
	Int8 script2[] = { DEF_VM_OP, 0, 100, 0, 100, 1, 0, 100, 100,
					DEF_FUN_4_OP,
						REF_1_OP,REF_0_OP,MAX_OP,RET_OP,
					DEF_FUN_OP, 10,
						CHECK_NB_OP,
						GLO_REF_0_OP,
						LIT8_OP,
						2,
						LIT8_OP,
						50,
						FOLD_HOOD_OP, 0,
						CHECK_MSG_OP,
						RET_OP,
					EXIT_OP };
					
					*/
	Int8 script2[] = { DEF_VM_OP, 0, 5, 0, 100, 1, 0, 100, 100,
					DEF_FUN_4_OP,
						REF_1_OP,REF_0_OP,MIN_OP,RET_OP,
					DEF_FUN_OP, 7,
						GLO_REF_0_OP,
						INF_OP,
						LIT8_OP,
						100,
						FOLD_HOOD_OP, 0,
						RET_OP,
					EXIT_OP };					  
}

namespace { // Formatted output operators for the Data types
	
	ostream & operator << (ostream & out, Data const & data);
	
	// An address is expressed as an at sign ('@') followed by three decimal digits.
	ostream & operator << (ostream & out, Address const & address) {
		if (address >= script) return out << '@' << setw(3) << setfill('0') << address - script;
		else return out << "@???";
	}
	
	// A tuple is expressed as a space seperated list between square brackets.
	ostream & operator << (ostream & out, Tuple const & tuple) {
		out << '[';
		for(size_t i = 0; i < tuple.size(); i++){
			if (i) out << ' ';
			out << tuple[i];
		}
		out << ']';
		return out;
	}
	
	// The way data is expressed depends on its type.
	ostream & operator << (ostream & out, Data const & data) {
		switch(data.type()){
			case Data::Type_number   : out << data.asNumber (); break;
			case Data::Type_address  : out << data.asAddress(); break;
			case Data::Type_tuple    : out << data.asTuple  (); break;
			case Data::Type_undefined: out << "undefined";
		}
		return out;
	}
	
	// A stack is expressed as a space seperated list.
	ostream & operator << (ostream & out, Stack<Data> const & stack) {
		stringstream s;
		for(size_t i = 0; i < stack.size(); i++){
			if (i) s << ' ';
			s << stack.peek(i);
		}
		out << setfill(' ') << setw(32) << right << s.str();
		return out;
	}

}

namespace {
	const char * instruction_names[256] = {
#		define INSTRUCTION(name) #name,
#		define INSTRUCTION_N(name,n) #name "_" #n,
#		include <delftproto.instructions>
#		undef INSTRUCTION
#		undef INSTRUCTION_N
	};
	
	// Show the name of the current instruction, along with the current state of the machine.
	// Waits for the user to press [enter] afterwards.
	void show_instruction(Machine & machine, string name){
		cout << machine.currentAddress();
		for(size_t i = 0; i < machine.depth(); i++) cout << " | ";
		cout << ' ' << setw(20 - machine.depth()*3) << setfill(' ') << left << name;
		cout << "  " << machine.stack << "  " << machine.environment << "  " << machine.globals;
		cout.flush();
		cout<<endl;
		//cin.ignore();
	}
	
	// Run the machine until the current script is finished executing.
	void debug_run(Machine & machine){
		while(!machine.finished()){
			show_instruction(machine,instruction_names[*machine.currentAddress()]);
			machine.step();
		}
	}
}

void RefreshHood(Machine *machine,int number)
{
	//t for this machine
	//o for other machines			
	Machine *t,*o;
	
	//For every machine in the simulator
	for(int i=0;i<number;i++)
	{
		//point to a machine
		t=&machine[i];
		
		//for each other machine
		for(int j=0;j<number;j++)
		{
			//point to the other machine
			o=&machine[j];
			
			//Check if other machine is in the neighbor
			bool exists=true;
			NeighbourHood::iterator neighbour =t->hood.find(o->id);			
			if(neighbour==t->hood.end())
				exists=false;				
			//cout<<machine[i].id<<"-"<<machine[j].id<<endl;
			//cout << "\tPos: "<<"(" << machine[i].x<<","<<machine[i].y<<","<<machine[i].z<<"| "<<machine[i].range<<")"<<" - (" << machine[j].x<<","<<machine[j].y<<","<<machine[j].z<<"| "<<machine[j].range<<")"<<endl;			
			
			//calculate the distance between the two vectors
			int dist=sqrt( pow((o->x-t->x),2) + pow((o->y-t->y),2) + pow((o->z-t->z),2)   );
			
			//Check to see if it is in range and is not in the list yet
			if(dist <= t->range && !exists)
			{
				t->hood.add(o->id);
				
			}
			//Or if it is out of range and is still in the list
			else if(dist>t->range && exists)
					t->hood.remove(neighbour);			
		}
		cout<<"I am machine:"<<t->id.id<<endl;
		//t->hood.refresh();
		std::cout<<"---"<<std::endl;
		//cout<<machine[i].id<<":Now I have: "<<machine[i].hood.size()<<"-("<<machine[i].hood.total<<") neighbours"<<endl;
	}
}

int main(int argc, char**argv){
	std::srand(time(0));
	
	cout << "<--------------------------------- Please make sure your terminal is at least 128 chars wide --------------------------------->" << endl << endl;
	cout << "@### INSTRUCTION           stack                             environment                       globals                         " << endl << endl;
	int number =(argc>1)?atoi(argv[1]):2;
	cout<<"We start with "<<number<<" vms"<<endl;
    
	
	Machine machine[number];
	
	cout<<"Installing"<<endl;
	//Installation
	/*for(int i=0;i<number;i++)
	{
		cout<<"The range is "<< machine[i].range<<endl;
		//show_instruction(machine[i],"INSTALL");
		machine[i].install(script);
		debug_run(machine[i]);		
	}*/
	
	
	show_instruction(machine[0],"INSTALL");
	machine[0].id.id=1;
	
	machine[0].install(script1);
	debug_run(machine[0]);	
	machine[0].id.thisMachine=&machine[0].thisMachine();
	cout<<"The imports size:"<<machine[0].thisMachine().imports.size()<<endl;
	
	
	show_instruction(machine[1],"INSTALL");
	
	machine[1].id.id=2;
	machine[1].install(script2);
	debug_run(machine[1]);	
	machine[1].id.thisMachine=&machine[1].thisMachine();
	
	cout<<"manual preparation"<<endl;
	
	machine[0].x=0;
	machine[0].y=0;
	machine[0].z=0;
	machine[0].range=50;
	
	machine[1].x=0;
	machine[1].y=0;
	machine[1].z=0;
	machine[1].range=50;
	
	
	cout<<"Initializing hood"<<endl;
	//Initializing Hood
	RefreshHood(machine,number);
	
	cout<<"Running"<<endl;
	//Running
	for(int i=0;i<number;i++)
	{
		show_instruction(machine[i],"RUN");
		machine[i].run();
		
	}
	
	cout<<"Executing"<<endl;
	//Executing
	int rounds=0;
	while(1)
	{
		cout<<"------------------ round "<<rounds<<" started!-----------------"<<endl;
		for(int i=0;i<number;i++)
		{
			if(!machine[i].finished())
			{
				show_instruction(machine[i],instruction_names[*machine[i].currentAddress()]);
				machine[i].step();
			}
		}
		RefreshHood(machine,number);
		cout<<"------------------ round "<<rounds++<<" ended!------------------"<<endl;
		cin.ignore();
	}
	
	exit(0);
}
