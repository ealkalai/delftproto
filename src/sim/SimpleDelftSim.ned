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


/// \file This file describes the network for the Omnet
// Each Vm is a Machine
simple Vm
{
    parameters:
    	//Define the parameters
    	double x=default(0); // X coordinate
    	double y=default(0); // Y coordinate
    	double z=default(0); // Z coordinate
    	double transmission_range=default(0); // Transmission range
    	
    	double dt=default(0.01); // dt for the machine
    	
    	double StartUpDelay=default(0); // Time to wait before the vm starts executing the opcodes
    	
    	@display("i=device/cpu"); // Define the icon that the node will have in the simulator
    gates:
        inout con[]; // In this version no gates are used.
}

// Definition of the network
network Simulation
{
    submodules:
    	// Vector with the virtual machines
        node[100]: Vm{
        	// Change any parameter you want.
        	// You can also apply values to the parameters via an xml file
        	x=uniform(0,1000); // random number for x pos
        	y=uniform(0,1000); // random number for y pos
        	z=0;//uniform(0,1000);
        	StartUpDelay=uniform(0,100);
        	transmission_range=250;
        };    
}

