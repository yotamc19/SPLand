#include "Simulation.h"

#include <fstream>
#include <iostream>
#include <string>

#include "Auxiliary.h"
#include "Facility.h"
#include "SelectionPolicy.h"
#include "Settlement.h"
using namespace std;

Simulation::Simulation(const string &configFilePath) {
    ifstream configFile(configFilePath);
    string line;
    planCounter = 0;
    while (getline(configFile, line)) {
        vector<string> parsedArgs = Auxiliary::parseArguments(line);
        string argType = parsedArgs[0];
        if (argType == "Settlement") {
            string name = parsedArgs[1];
            SettlementType type =
                static_cast<SettlementType>(stoi(parsedArgs[2]));
            Settlement s(name, type);
            settlements.push_back(s);
        } else if (argType == "Facility") {
            string name = parsedArgs[1];
            FacilityCategory category =
                static_cast<FacilityCategory>(stoi(parsedArgs[2]));
            int price = stoi(parsedArgs[3]);
            int lifeQualityScore = stoi(parsedArgs[4]);
            int economyScore = stoi(parsedArgs[5]);
            int environmentScore = stoi(parsedArgs[6]);
            FacilityType ft(name, category, price, lifeQualityScore,
                            economyScore, environmentScore);
            facilitiesOptions.push_back(ft);
        } else {  // plan
            string settlementName = parsedArgs[1];
            Settlement s = Simulation::getSettlement(settlementName);

            if (parsedArgs[2] == "nai") {
                NaiveSelection ns;
                Plan p(planCounter, s, &ns, facilitiesOptions);
                plans.push_back(p);
            } else if (parsedArgs[2] == "bal") {
                BalancedSelection bs(0, 0, 0);
                Plan p(planCounter, s, &bs, facilitiesOptions);
                plans.push_back(p);
            } else if (parsedArgs[2] == "eco") {
                EconomySelection es;
                Plan p(planCounter, s, &es, facilitiesOptions);
                plans.push_back(p);
            } else {
                SustainabilitySelection ss;
                Plan p(planCounter, s, &ss, facilitiesOptions);
                plans.push_back(p);
            }
            planCounter++;
        }
    }
}

void Simulation::start() {}

void Simulation::addPlan(const Settlement &settlement,
                         SelectionPolicy *selectionPolicy) {
    Plan p(planCounter, settlement, selectionPolicy, facilitiesOptions);
    plans.push_back(p);
    planCounter++;
}

void Simulation::addAction(BaseAction *action) { actionsLog.push_back(action); }

bool Simulation::addSettlement(Settlement settlement) {
    bool isSettlementExists =
        Simulation::isSettlementExists(settlement.getName());
    if (isSettlementExists) {
        return false;
    }
    settlements.push_back(settlement);
    return true;
}

bool Simulation::addFacility(FacilityType facility) {
    bool isFacilityExistsAlready = false;
    int i = 0;
    while (!isFacilityExistsAlready) {
        isFacilityExistsAlready =
            facility.getName() == facilitiesOptions[i].getName();
        i++;
    }
    if (isFacilityExistsAlready) {
        return false;
    }
    facilitiesOptions.push_back(facility);
    return true;
}

bool Simulation::isSettlementExists(const string &settlementName) {
    bool isSettlementExists = false;
    int i = 0;
    while (!isSettlementExists && i < settlements.size()) {
        isSettlementExists = settlementName == settlements[i].getName();
        i++;
    }
    return isSettlementExists;
}

Settlement &Simulation::getSettlement(const string &settlementName) {
    int i = 0;
    bool isSettlementFound = false;
    while (!isSettlementFound) {
        isSettlementFound = settlementName == settlements[i].getName();
        i++;
    }
    Settlement &s = settlements[i - 1];
    return s;
}

Plan &Simulation::getPlan(const int planID) { return plans[planID]; }

void Simulation::step() {
    for (int i = 0; i < plans.size(); i++) {
        plans[i].step();
    }
}

void Simulation::close() {}

void Simulation::open() {}

// finish start and close and open :)