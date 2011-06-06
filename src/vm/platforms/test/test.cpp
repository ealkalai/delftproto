#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>

#include <instructions.hpp>
#include <machine.hpp>
#include <types.hpp>
#include <data.hpp>
#include <stack.hpp>
#include <neighbour.hpp>

using namespace std;

int main(int argc, char* argv[])
{
	int num=1;
	if(argc>1)
		num=atoi(argv[1]);
	
	cout<<"The number of machines is: "<<num<<endl;	
	
	Machine m;
}