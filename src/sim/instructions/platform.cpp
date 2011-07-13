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

#include <machine.hpp>
#include <instructions.hpp>

namespace Instructions {
	
	using namespace std;
	
	void RED(Machine & machine)
	{
		Number red=machine.stack.peek().asNumber();

		if(red == 1){
			 machine.red=true;
		}else{
			machine.red=false;
		}
	}
	
	void GREEN(Machine & machine)
	{
		//machine.green=true;
		Number green=machine.stack.peek().asNumber();

		if(green == 1){
			 machine.green=true;	
		}else{
			machine.green=false;
		}


	}
	
	void DT(Machine & machine)
	{
		machine.stack.push(machine.dt);
	}
	
	
	
	void SENSE(Machine & machine)
	{
		// ...
	}
	
	void COORD(Machine & machine)
	{
		//EV << "Pos: " << machine.x<<"-"<<machine.y<<"-"<<machine.z<<std::endl;
	
		Tuple coord(3);
		coord.push(machine.x);
		coord.push(machine.y);
		coord.push(machine.z);
		
		machine.stack.push(coord);
	}

	void MOV(Machine & machine)
	{
		
		//pop from stack a vector
		Tuple vec(3);
		vec = machine.stack.popTuple();
	
		machine.x += vec[0].asNumber();//Number(machine.nextInt8());
		machine.y += vec[1].asNumber();//Number(machine.nextInt8());
		machine.z += vec.size() > 2 ? vec[2].asNumber() : 0;//Number(machine.nextInt8());
	
		EV<<"Just moved"<<endl;
	}
	
	//Should be moved to an extention to the neighbor!
	void NBR_RANGE(Machine & machine)
	{
		EV<<"NBR_RANGE"<<endl;
		
		machine.stack.push(	sqrt(	machine.currentNeighbour().x * machine.currentNeighbour().x +
										machine.currentNeighbour().y * machine.currentNeighbour().y +
										machine.currentNeighbour().z * machine.currentNeighbour().z));
				 
	}
	
	void NBR_VEC(Machine & machine)
	{
		EV<<"NBR_VEC_OP"<<endl;
		
		machine.nextInt8();
		Tuple coord(3);
		coord.push(machine.currentNeighbour().x);
		coord.push(machine.currentNeighbour().y);
		coord.push(machine.currentNeighbour().z);
		
		machine.stack.push(coord);
	}
	
	void INFINITESIMAL(Machine & machine)
	{
		Number radius = machine.transmission_range;

		machine.stack.push(radius * radius * M_PI / (machine.hood.size() ) +1);
	}	
}
