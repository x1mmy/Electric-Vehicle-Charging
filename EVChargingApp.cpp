#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <map>

using namespace std;

#include "Location.h"
#include "WeightedGraph.h"
#include "EVCharging.h"

int main() {
	int userInput;
	EVCharging charging;

	charging.runMenu(); 
	// charging.printAdjacentLocations();

	return 0;
}
