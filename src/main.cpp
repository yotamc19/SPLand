// #include "Simulation.h"
#include <iostream>

#include "Facility.h"
#include "Settlement.h"

using namespace std;
// Simulation* backup = nullptr;

int main(int argc, char** argv) {
    if (argc != 2) {
        cout << "usage: simulation <config_path>" << endl;
        return 0;
    }

    FacilityType ft("School", FacilityCategory::LIFE_QUALITY, 1, 3, 1, 1);
    Facility f(ft, "Beer Sheba");
    FacilityStatus fs = f.getStatus();
    if (fs == FacilityStatus::OPERATIONAL) {
        cout << "operation" << endl;
    } else {
        cout << "under construction" << endl;
    }

    string configurationFile = argv[1];
    // Simulation simulation(configurationFile);
    // simulation.start();
    // if(backup!=nullptr){
    // 	delete backup;
    // 	backup = nullptr;
    // }
    return 0;
}