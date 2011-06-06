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

namespace Instructions {
	
	void HELLOWORLD(Machine & machine){
		std::cout << "Hello World! (" << ++machine.helloworld_counter << ")" << std::endl;
	}
	
	void COORD(Machine & machine){
		std::cout << "Pos: " << machine.x<<"-"<<machine.y<<"-"<<machine.z<<std::endl;
		Tuple coord(3);
        coord.push(machine.x);
        coord.push(machine.y);
        coord.push(machine.z);
        machine.stack.push(coord);
	}
	
	void MOV(Machine & machine){
		machine.x += Number(machine.nextInt8());
		machine.y += Number(machine.nextInt8());
		machine.z += Number(machine.nextInt8());
	}
	
}

