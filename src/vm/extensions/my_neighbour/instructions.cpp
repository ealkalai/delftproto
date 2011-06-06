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
#include <neighbour.hpp>

namespace Instructions {
	/*
	void HELLOWORLD(Machine & machine){
		std::cout << "Hello World! (" << ")" << std::endl;
	}
	*/
	
	void CHECK_NB(Machine & machine){
		std::cout << "I have "<<machine.hood.size() << " neighbours"<<std::endl;
	}
	
	void SEND(Machine & machine){
		int numOfData = machine.nextInt8();
		Tuple data(numOfData);
		for(int i=0;i<numOfData;i++)
		{
			data.push(machine.nextInt8());
		}
		
		machine.stack.push(data);
	}	
}

