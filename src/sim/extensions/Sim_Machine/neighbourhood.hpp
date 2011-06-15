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

//#include <iostream>
#include <types.hpp>

struct Sim_NeighbourHood : NeighbourHood {
	Number x,y,z,range;
	Sim_NeighbourHood() : x(0),y(0),z(0),range(0) {}
	/*inline void refresh() {
			for(iterator i = begin(); i != end(); i++)
			{
				std::cout<<"Machine ID "<<i->id.id<<std::endl;
				for(int j=0;j<(int)i->imports.size();j++)
				{	
					std::cout<<"\t\twas:"<<i->imports[j].asNumber();				
					i->imports[j] =(((i->id).thisMachine)->imports[j]);
					//std::cout<<"\t\t"<<(((i->id).thisMachine)->imports[j]).asNumber()<<std::endl;
					std::cout<<"\t\t\tIs:"<<i->imports[j].asNumber()<<std::endl;
					//(n->neighbour).imports[i] = (id.thisMachine)->imports[i];
				}
			}
			
		}*/
};

#undef  NeighbourHood
#define NeighbourHood Sim_NeighbourHood

