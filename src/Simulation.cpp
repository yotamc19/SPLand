#include "Simulation.h"

#include <fstream>
#include <iostream>
#include <string>

#include "Action.h"
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
        cout << line << endl;
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
        } else if (argType == "plan") {
            string settlementName = parsedArgs[1];
            // settlement name might not be in the simulation yet or at all
            Settlement s(Simulation::getSettlement(settlementName));

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

void Simulation::start() {
    open();
    cout << "The simulation has started" << endl;

    while (isRunning) {
        string line;
        getline(cin, line);
        vector<string> parsedArgs = Auxiliary::parseArguments(line);
        if (parsedArgs[0] == "step") {
            int numOfSteps = stoi(parsedArgs[1]);
            SimulateStep action = SimulateStep(numOfSteps);
            action.act(*this);
        } else if (parsedArgs[0] == "plan") {
            string settlementName = parsedArgs[1];
            string settlementPolicy = parsedArgs[2];
            AddPlan action = AddPlan(settlementName, settlementPolicy);
            action.act(*this);
        } else if (parsedArgs[0] == "settlement") {
            string settlementName = parsedArgs[1];
            SettlementType settlementType =
                static_cast<SettlementType>(stoi(parsedArgs[2]));
            AddSettlement action =
                AddSettlement(settlementName, settlementType);
            action.act(*this);
        } else if (parsedArgs[0] == "facility") {
            string facilityName = parsedArgs[1];
            FacilityCategory facilityCategory =
                static_cast<FacilityCategory>(stoi(parsedArgs[2]));
            int price = stoi(parsedArgs[3]);
            int lifeQualityScore = stoi(parsedArgs[4]);
            int economyScore = stoi(parsedArgs[5]);
            int environmentScore = stoi(parsedArgs[6]);
            AddFacility action =
                AddFacility(facilityName, facilityCategory, price,
                            lifeQualityScore, economyScore, environmentScore);
            action.act(*this);
        } else if (parsedArgs[0] == "planStatus") {
            int planId = stoi(parsedArgs[1]);
            PrintPlanStatus action = PrintPlanStatus(planId);
            action.act(*this);
        } else if (parsedArgs[0] == "changePolicy") {
            int planId = stoi(parsedArgs[1]);
            string newPolicy = parsedArgs[2];
            ChangePlanPolicy action = ChangePlanPolicy(planId, newPolicy);
            action.act(*this);
        } else if (parsedArgs[0] == "log") {
            PrintActionsLog action = PrintActionsLog();
            action.act(*this);
        } else if (parsedArgs[0] == "close") {
            Close action = Close();
            action.act(*this);
        } else if (parsedArgs[0] == "backup") {
            BackupSimulation action = BackupSimulation();
            action.act(*this);
        } else if (parsedArgs[0] == "restore") {
            RestoreSimulation action = RestoreSimulation();
            action.act(*this);
        }
    }
}

void Simulation::addPlan(const Settlement &settlement,
                         SelectionPolicy *selectionPolicy) {
    Plan p(planCounter, settlement, selectionPolicy, facilitiesOptions);
    plans.push_back(p);
    planCounter++;
}

void Simulation::addAction(BaseAction *action) { actionsLog.push_back(action); }

bool Simulation::addSettlement(Settlement settlement) {
    bool isSettlementFound =
        Simulation::isSettlementExists(settlement.getName());
    if (isSettlementFound) {
        return false;
    }
    settlements.push_back(settlement);
    return true;
}

bool Simulation::addFacility(FacilityType facility) {
    bool isFacilityExists = false;
    int size = facilitiesOptions.size();
    for (int i = 0; i < size && !isFacilityExists; i++) {
        isFacilityExists = facility.getName() == facilitiesOptions[i].getName();
    }
    if (isFacilityExists) {
        return false;
    }
    facilitiesOptions.push_back(facility);
    return true;
}

bool Simulation::isSettlementExists(const string &settlementName) {
    bool isSettlementFound = false;
    int size = settlements.size();
    for (int i = 0; i < size && !isSettlementFound; i++) {
        isSettlementFound = settlementName == settlements[i].getName();
    }
    return isSettlementFound;
}

bool Simulation::isFacilityExists(const string &facilityName) {
    bool isFacilityExists = false;
    int size = facilitiesOptions.size();
    for (int i = 0; i < size && !isFacilityExists; i++) {
        isFacilityExists = facilityName == facilitiesOptions[i].getName();
    }
    return isFacilityExists;
}

Settlement &Simulation::getSettlement(const string &settlementName) {
    bool isSettlementFound = false;
    int size = settlements.size();
    cout << to_string(size) << endl;
    for (int i = 0; i < size && !isSettlementFound; i++) {
        isSettlementFound = settlementName == settlements[i].getName();
        cout << isSettlementFound << endl;
        if (isSettlementFound) {
            Settlement &s(settlements[i]);
            return s;
        }
    }
    throw invalid_argument(
        "No settlement with this name exists in the simulation");
}

Plan &Simulation::getPlan(const int planID) {
    int size = plans.size();
    if (planID >= size) {
        throw invalid_argument("No plan with this planID exists");
    }
    return plans[planID];
}

const vector<BaseAction *> &Simulation::getActionsLog() { return actionsLog; }

void Simulation::step() {
    int size = plans.size();
    for (int i = 0; i < size; i++) {
        plans[i].step();
    }
}

void Simulation::close() {
    string summary = "";
    int size = plans.size();
    for (int i = 0; i < size; i++) {
        summary += "PlanID: " + to_string(i) + "\nSettlementName: " + plans[0].getSettlement().getName() + "\nLifeQualityScore: " + to_string(plans[0].getlifeQualityScore()) + "\nEconomyScore: " + to_string(plans[0].getEconomyScore()) + "\nEnvironmentScore: " + to_string(plans[0].getEnvironmentScore()) + "\n";
    }
    cout << summary << endl;
    isRunning = false;
}

void Simulation::open() {
    isRunning = true;
}