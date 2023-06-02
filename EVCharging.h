/*
 * EVCharging.h
 *
 *  Created on: 29 Apr 2023
 *      Author: 2002345
 */

#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>


#ifndef EVCHARGING_H_
#define EVCHARGING_H_

class EVCharging {// you may also declare this class as an extension of WeightedGraph
private:
	map<int, Location> locations;       // Map to store locations
	int numberOfLocations;              // Number of locations
	WeightedGraphType* graph;           // Pointer to WeightedGraphType object
public:
	EVCharging();
	~EVCharging();
	void inputLocations();              // Read locations from file
	void printLocations();              // Print the list of locations
	void printAdjacencyMatrix();        // Print the adjacency matrix
void runMenu();

	// Task 3
	void printAscendingPrice();         // Print locations with charging stations in ascending order of charging price

	// Task 4
	void printAdjacentLocations();      // Print adjacent locations

    //Task 5
    void printNearestCheapStation();

    //Task 6
    void printNearestChargingStation();

    //Task 7
    void printLowestTotalLocation();

    //Task 8
    void findMinCostPath();
};

EVCharging::EVCharging() {
	inputLocations();                   // Read locations from file
	graph = new WeightedGraphType(numberOfLocations);   // Create a WeightedGraphType object based on the number of locations



}

EVCharging::~EVCharging() {
	delete graph;                       // Deallocate memory for the graph object
}

void printLocation(const Location& location) {
    cout << setw(6) << location.index << setw(20) << location.locationName << setw(15);
    if (location.chargerInstalled)
        cout << "yes";
    else
        cout << "no";

    if (location.chargerInstalled && location.chargingPrice == 0)
        cout << setw(28) << "free of charge" << endl;
    else if (!location.chargerInstalled)
        cout << setw(17) << "N/A" << endl;
    else
        cout << setw(15) << "$" << setprecision(2) << location.chargingPrice << "/kWh" << endl;
}


void EVCharging::inputLocations() {
	ifstream infile;
	char fileName[50] = "Locations.txt";

	infile.open(fileName);

	if (!infile) {
		cout << "Cannot open input file." << endl;
		return;
	}

	int locationIndex = 0;

	while (!infile.eof()) {
		Location s;
		string charger;
		string price;
		while (!infile.eof()) {
			getline(infile, s.locationName, ',');
			getline(infile, charger, ',');
			getline(infile, price);
			s.chargerInstalled = (stoi(charger) == 1) ? true : false;
			s.chargingPrice = stod(price);
			s.index = locationIndex;
			locations[locationIndex] = s;
			locationIndex++;
		}
	}

	numberOfLocations = locationIndex;
}





void EVCharging::runMenu() {
    int option;

    while (true) {
        cout << "\n### Please select an option: ###" << endl;
        cout << "1) Print Locations" << endl;
        cout << "2) Print Adjacency Matrix" << endl;
        cout << "3) Print charging stations in ascending order of charging price" << endl;
        cout << "4) Print list of adjacent locations with charging stations." << endl;
        cout << "5) Print nearest location with lowest charging cost." << endl;
        cout << "6) Print nearest location with a charging station." << endl;
        cout << "7) Print location with the lowest total cost of traveling and charging." << endl;
        cout << "8) Print the shortest path between origin and destination" << endl;
        cout << "9) Exit" << endl;

        cout << "\nEnter your choice: ";
        cin >> option;
        cout << endl;

        switch (option) {
            case 1:
                printLocations();
                break;
            case 2:
                printAdjacencyMatrix();
                break;
            case 3:
                printAscendingPrice();
                break;
            case 4:
                printAdjacentLocations();
                break;
            case 5:
                printNearestCheapStation();
                break;
            case 6:
                printNearestChargingStation();
                break;
            case 7:
                printLowestTotalLocation();
                break;
            case 8:
                findMinCostPath();
                break;
            case 9:
                cout << "Exiting the program. Goodbye!" << endl;
                return;  // Exit the function and end the program
            default:
                cout << "Invalid option. Please try again." << endl;
                break;
        }
    }
}


//TASK 1
void EVCharging::printLocations() {
	cout << "List of locations and charging information " << endl;
	cout << setw (8) << "Index" << setw (20) << "Location name" << setw (20) <<"Charging station" << setw(20) << "Charging price" << endl;

	map<int, Location>::iterator it = locations.begin();

	for (; it != locations.end(); it++) {
		it->second.printLocation();
	}

    cout << endl;
}

//TASK2
void EVCharging::printAdjacencyMatrix() {
	cout << "Adjacency matrix (0 means no direct connection, non-zero value represents the distance of adjacent locations)\n" << endl;
	cout << setw(13) << " ";
	for (int i = 0; i < numberOfLocations; i++) {
		cout << setw(13) << locations[i].locationName;
	}
	cout << endl;
	for (int i = 0; i < numberOfLocations; i++) {
		cout << setw(13) << locations[i].locationName;
		for (int j = 0; j < numberOfLocations; j++) {
			cout << setw(13) << (graph->getWeight(i,j) == DBL_MAX ? 0.0 : graph->getWeight(i,j));
		}
		cout << endl;
	}
}


// TASK 3
void EVCharging::printAscendingPrice(){
	// Create a vector to store the locations
    vector<Location> stationPrices;

    // Populate the vector with locations that have charging stations
	for (auto it = locations.begin(); it != locations.end(); ++it) {
		const Location& location = it->second;
		if (location.chargerInstalled) {
			stationPrices.push_back(location);
		}
	}

    // Sort the vector in ascending order based on charging price
    sort(stationPrices.begin(), stationPrices.end());

    // Print the sorted locations
    cout << "\nList of locations with charging stations in ascending order of charging price:" << endl;
    cout << setw(8) << "Index" << setw(20) << "Location name" << setw(20) << "Charging station" << setw(20) << "Charging price" << endl;

    for ( Location& location : stationPrices) {
        location.printLocation();
    }

    cout << endl;
}


//TASK 4
void EVCharging::printAdjacentLocations() {
    string userInput;
        
    cout << "Input name of city: ";
    cin >> userInput;

    cout << "The adjacent locations to " << userInput << " with available chargers are: \n";

    //Used to check if we found it
    bool foundAdjacentLocations = false; 

    // Iterate over the locations to find the user input city
    for (auto it = locations.begin(); it != locations.end(); ++it) {
        const Location& location = it->second;

        if (location.locationName == userInput) {
            list<int> adjList = graph->getAdjacencyList(it->first);
            list<int>::iterator adjIt;

            // Iterate over the adjacency list of the found city
            for (adjIt = adjList.begin(); adjIt != adjList.end(); ++adjIt) {
                Location& adjLocation = locations[*adjIt];

                // Check if the adjacent location has a charging station
                if (adjLocation.chargerInstalled) {
                    cout << adjLocation.locationName << setw(15) << "$" << adjLocation.chargingPrice << endl;
                    foundAdjacentLocations = true;
                }
            }
            
            break; // Exit the loop once the location is found
        }
    }

    // Print message if no adjacent locations with chargers are found
    if (!foundAdjacentLocations) {
        cout << "No adjacent locations with available chargers found." << endl;
    }
}


//TASK 5
void EVCharging::printNearestCheapStation() {
    string userInput;
    srand(time(0));

    cout << "Input name of current city: ";
    cin.ignore();
    getline(cin, userInput);

    int EVTravelCost = 0.10; // $0.10/km 
    int chargingAmount = rand() % 41 + 10; // random value between 10 and 50 

    bool foundAdjacentLocations = false; 

    // Iterate over the locations to find the user input city
    for (auto it = locations.begin(); it != locations.end(); ++it) {
        const Location& location = it->second;

        if (location.locationName == userInput) { 
            list<int> adjList = graph->getAdjacencyList(it->first);// create a list containing the adjacency 
            list<int>::iterator adjIt;

            double lowestCost = numeric_limits<double>::max();
            int nearestLocationIndex = -1;

            // Iterate over the adjacency list of the found city
            for (adjIt = adjList.begin(); adjIt != adjList.end(); ++adjIt) {
                Location& adjLocation = locations[*adjIt];

                // Check if the adjacent location has a charging station
                if (adjLocation.chargerInstalled) {
                    //We get the distance between each connected stations
                    double distance = graph->getWeight(it->first, *adjIt);
                    //Use the formula
                    double chargingCost = adjLocation.chargingPrice * chargingAmount;
                    // calculate the travel cost
                    double travelCost = 2 * EVTravelCost * distance;
                    // Calculate the total cost
                    double totalCost = travelCost + chargingCost;

                    //Comparing to find lowest
                    if (totalCost < lowestCost) {
                        lowestCost = totalCost;
                        nearestLocationIndex = *adjIt;
                    }

                    foundAdjacentLocations = true;
                }
            }

            if (nearestLocationIndex != -1) {
                cout << "The nearest location with the lowest charging cost is: " << locations[nearestLocationIndex].locationName << endl;
                cout << "Total cost: $" << fixed << setprecision(2) << lowestCost << endl;
            }

            break; // Exit the loop once the location is found
        }
    }

    // Print message if no adjacent locations with chargers are found
    if (!foundAdjacentLocations) {
        cout << "No adjacent locations with available chargers found." << endl;
    }
}

//TASK 6
void EVCharging::printNearestChargingStation() {
    string currentLocation;
    cout << "Enter your current location: ";
    cin.ignore(); // Ignore newline character from previous input
    getline(cin, currentLocation);

    int currentLocationIndex = -1;

    // Find the index of the current location using a range based for loop
    for (const auto& location : locations) {
        if (location.second.locationName == currentLocation) {
            currentLocationIndex = location.second.index;
            break;
        }
    }

    //error checking 
    if (currentLocationIndex == -1) {
        cout << "Invalid current location. Please try again." << endl;
        return;
    }

    // Calculate the shortest distances from the current location to all other locations
    graph->shortestPath(currentLocationIndex);

    int nearestLocationIndex = -1;
    double lowestDistance = DBL_MAX; // max possible val

    // Find the nearest charging station excluding the current location
    for (const auto& location : locations) {
        if (location.second.index != currentLocationIndex && location.second.chargerInstalled) {
            double distance = graph->smallestWeight[location.second.index];
            if (distance < lowestDistance) {
                nearestLocationIndex = location.second.index;
                lowestDistance = distance;
            }
        }
    }

    if (nearestLocationIndex == -1) {
        cout << "No nearest charging station found." << endl;
    } else {
        cout << "The nearest charging station is: " << locations[nearestLocationIndex].locationName << endl;
        cout << "Distance(km): " << lowestDistance << endl;
    }
}

//TASK 7
void EVCharging::printLowestTotalLocation(){
    string userInput;
    srand(time(0));

    cout << "Input name of current city: ";
    cin.ignore();
    getline(cin, userInput);

    double EVTravelCost = 0.10; // $0.10/km
    int chargingAmount = rand() % 41 + 10; // random value between 10 and 50


    bool foundAdjacentLocations = false;

    // Iterate over the locations to find the user input city
    for (auto it = locations.begin(); it != locations.end(); ++it) {
        const Location& location = it->second;

        if (location.locationName == userInput) {
            list<int> adjList = graph->getAdjacencyList(it->first); // create a list containing the adjacency
            list<int>::iterator adjIt;

            double lowestCost = numeric_limits<double>::max(); // largest possible value
            int nearestLocationIndex = -1;
            double finalChargingCost = 0.0;
            double finalTravelCost = 0.0;

            // Iterate over the adjacency list of the found city
            for (adjIt = adjList.begin(); adjIt != adjList.end(); ++adjIt) {
                Location& adjLocation = locations[*adjIt];

                // Check if the adjacent location has a charging station
                if (adjLocation.chargerInstalled) {
                    // Get the distance between each connected stations
                    double distance = graph->getWeight(it->first, *adjIt);
                    // Use the formula
                    double chargingCost = adjLocation.chargingPrice * chargingAmount;
                    // Calculate the travel cost
                    double travelCost = 2 * EVTravelCost * distance;

                    // Comparing to find the lowest cost
                    if ((travelCost + chargingCost) < lowestCost) {
                        lowestCost = travelCost + chargingCost;
                        nearestLocationIndex = *adjIt;
                        finalChargingCost = chargingCost;
                        finalTravelCost = travelCost;
                    }

                    foundAdjacentLocations = true;
                }
            }

            if (nearestLocationIndex != -1) {
                cout << "The nearest location with the lowest charging cost is: " << locations[nearestLocationIndex].locationName << endl;
                cout << "Charging amount: " << chargingAmount << "kWh" << endl;
                cout << "Charging Cost: $" << fixed << setprecision(2) << finalChargingCost << endl;
                cout << "Travel Cost: $" << fixed << setprecision(2) << finalTravelCost << endl;
                cout << "Total cost: $" << fixed << setprecision(2) << lowestCost << endl;
            }

            break; // Exit the loop once the location is found
        }
    }

    // Print message if no adjacent locations with chargers are found
    if (!foundAdjacentLocations) {
        cout << "No adjacent locations with available chargers found." << endl;
    }
}

//TASK 8
void EVCharging::findMinCostPath() {
    string origin, destination;


    cout << "Enter the origin city: ";
    cin.ignore();
    getline(cin, origin);

    cout << "Enter the destination city: ";
    getline(cin, destination);

    int chargingAmount = rand() % 41 + 10; // random value between 10 and 50
    cout << "Charging Amount: " << chargingAmount << endl;


    int originIndex = -1;
    int destinationIndex = -1;

    //setting the index of
    for (auto it = locations.begin(); it != locations.end(); ++it) {
        const Location& location = it->second;

        if (location.locationName == origin) {
            originIndex = it->first;
        } else if (location.locationName == destination) {
            destinationIndex = it->first;
        }

        if (originIndex != -1 && destinationIndex != -1) {
            break;
        }
    }

    if (originIndex == -1) {
        cout << "Origin city not found." << endl;
        return;
    }

    if (destinationIndex == -1) {
        cout << "Destination city not found." << endl;
        return;
    }

    list<int> adjList = graph->getAdjacencyList(originIndex);
    list<int>::iterator adjIt;

    double lowestCost = numeric_limits<double>::max();
    int nearestLocationIndex = -1;
    double EVTravelCost = 0.10; // $0.10/km

    // Iterate over the adjacency list of the origin city
    for (adjIt = adjList.begin(); adjIt != adjList.end(); ++adjIt) {
        Location& adjLocation = locations[*adjIt];

        // Check if the adjacent location is the destination
        if (*adjIt == destinationIndex) {
            // Check if the adjacent location has a charging station
            if (adjLocation.chargerInstalled) {
                // Get the distance between the origin and destination
                double distance = graph->getWeight(originIndex, destinationIndex);
                // Calculate the charging cost
                double chargingCost = adjLocation.chargingPrice * chargingAmount;
                // Calculate the travel cost
                double travelCost = 2 * EVTravelCost * distance;
                // Calculate the total cost
                double totalCost = travelCost + chargingCost;

                // Update the lowest cost and nearest location index if applicable
                if (totalCost < lowestCost) {
                    lowestCost = totalCost;
                    nearestLocationIndex = *adjIt;
                }
            }
        }
    }

    if (nearestLocationIndex != -1) {
        cout << "The path with the minimal charging cost and travel cost:" << endl;
        cout << "Origin: " << locations[originIndex].locationName << endl;
        cout << "Destination: " << locations[destinationIndex].locationName << endl;
        cout << "Charging amount: " << chargingAmount << " kWh" << endl;
        cout << "Total cost: $" << fixed << setprecision(2) << lowestCost << endl;

        cout << "Path: ";
        int currentLocationIndex = originIndex;
        cout << locations[currentLocationIndex].locationName;

        while (currentLocationIndex != destinationIndex) {
            list<int> adjList = graph->getAdjacencyList(currentLocationIndex);
            double minCost = numeric_limits<double>::max();
            int nextLocationIndex = -1;

            for (int adjIndex : adjList) {
                Location& adjLocation = locations[adjIndex];
                double distance = graph->getWeight(currentLocationIndex, adjIndex);
                double chargingCost = adjLocation.chargingPrice * chargingAmount;
                double travelCost = 2 * EVTravelCost * distance;
                double totalCost = travelCost + chargingCost;

                if (totalCost < minCost) {
                    minCost = totalCost;
                    nextLocationIndex = adjIndex;
                }
            }

            if (nextLocationIndex != -1) {
                cout << " -> " << locations[nextLocationIndex].locationName;
                currentLocationIndex = nextLocationIndex;
            } else {
                break;
            }
        }

        cout << endl;
    } else {
        cout << "No path with the minimal charging cost and travel cost found." << endl;
    }
}







#endif /* EVCHARGING_H_ */
