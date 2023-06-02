/*
 * Location.h
 *
 *  Created on: 29 Apr 2023
 *      Author: 2002345
 */

#ifndef LOCATION_H_
#define LOCATION_H_

class Location {
public:
    int index;                  // Key or identifier for the location
    string locationName;        // Name of the location
    bool chargerInstalled;      // Indicates if a charger is installed at the location
    double chargingPrice;       // Price for charging at the location in dollars per kilowatt-hour

    void printLocation() {
        // Print the information about the location
        cout << setw(6) << index << setw(20) << locationName << setw(15);
        if (chargerInstalled)
            cout << "yes";
        else
            cout << "no";

        if (chargerInstalled && chargingPrice == 0)
            cout << setw(28) << "free of charge" << endl;
        else if (!chargerInstalled)
            cout << setw(17) << "N/A" << endl;
        else
            cout << setw(15) << "$" << setprecision(2) << chargingPrice << "/kWh" << endl;
    }

    bool operator<(const Location& l) const {
        // Compare the charging prices of two locations for sorting purposes
        return chargingPrice < l.chargingPrice;
    }
};

#endif /* LOCATION_H_ */
