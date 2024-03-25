#include <iostream>
#include "PBEngine.h"
#include "Exception.h"

using namespace std;

int main()
{
	try 
	{ 
		PhysBamt::Engine::InitializeEngine("Phys-Bamt", 800, 800); 
	}
	catch (Exception exc) 
	{ 
		cerr << exc.what() << endl;
		return 0; 
	}

	PhysBamt::Engine::StartEngine();

	return 0;
}