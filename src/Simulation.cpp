
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
    // Initiating the values given from the configFile into Simulation's fields
    while (getline(configFile, line)) {
        cout << line << endl;
        vector<string> parsedArgs = Auxiliary::parseArguments(line);
        string argType = parsedArgs[0];

        if (argType == "settlement") {
            string name = parsedArgs[1];
            SettlementType type =
                static_cast<SettlementType>(stoi(parsedArgs[2]));
            Settlement *s = new Settlement(name, type);
            settlements.push_back(s);
        } else if (argType == "facility") {
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
            Settlement &s = getSettlement(settlementName);

            if (parsedArgs[2] == "nai") {
                NaiveSelection ns;
                Plan *p = new Plan(planCounter, s, &ns, facilitiesOptions);
                plans.push_back(*p);
            } else if (parsedArgs[2] == "bal") {
                BalancedSelection bs(0, 0, 0);
                Plan *p = new Plan(planCounter, s, &bs, facilitiesOptions);
                plans.push_back(*p);
            } else if (parsedArgs[2] == "eco") {
                EconomySelection es;
                Plan *p = new Plan(planCounter, s, &es, facilitiesOptions);
                plans.push_back(*p);
            } else if (parsedArgs[2] == "env") {
                SustainabilitySelection ss;
                Plan *p = new Plan(planCounter, s, &ss, facilitiesOptions);
                plans.push_back(*p);
            }
            planCounter++;
        }
    }
}

Simulation::Simulation(const Simulation &other)
    : isRunning(other.isRunning),
      planCounter(other.planCounter),
      actionsLog(other.actionsLog),
      plans(other.plans),
      settlements(other.settlements),
      facilitiesOptions(other.facilitiesOptions) {}

Simulation &Simulation::operator=(const Simulation &other) {
    if (&other != this) {
        isRunning = other.isRunning;
        planCounter = other.planCounter;
        plans = other.plans;
        facilitiesOptions = other.facilitiesOptions;

        while ((int)actionsLog.size() > 0) {
            actionsLog.erase(actionsLog.begin());
        }
        actionsLog.clear();
        int otherActionsLogSize = other.actionsLog.size();
        for (int i = 0; i < otherActionsLogSize; i++) {
            actionsLog.push_back((*other.actionsLog[i]).clone());
        }

        while ((int)settlements.size() > 0) {
            settlements.erase(settlements.begin());
        }
        settlements.clear();
        int settlementsSize = other.settlements.size();
        for (int i = 0; i < settlementsSize; i++) {
            Settlement *s = other.settlements[i];
            settlements.push_back(s);
        }
    }
    return (*this);
}

Simulation::Simulation(Simulation &&other) noexcept
    : isRunning(other.isRunning),
      planCounter(other.planCounter),
      actionsLog(move(other.actionsLog)),
      plans(move(other.plans)),
      settlements(move(other.settlements)),
      facilitiesOptions(move(other.facilitiesOptions)) {
    other.isRunning = false;
    other.planCounter = 0;
}

Simulation &Simulation::operator=(Simulation &&other) noexcept {
    if (&other != this) {
        int actionsLogSize = actionsLog.size();
        for (int i = 0; i < actionsLogSize; i++) {
            delete actionsLog[i];
        }
        actionsLog.clear();

        int settlementsSize = settlements.size();
        for (int i = 0; i < settlementsSize; i++) {
            delete settlements[i];
        }
        settlements.clear();

        isRunning = other.isRunning;
        planCounter = other.planCounter;
        actionsLog = move(other.actionsLog);
        plans = move(other.plans);
        settlements = move(other.settlements);
        facilitiesOptions = move(other.facilitiesOptions);

        other.isRunning = false;
        other.planCounter = 0;
        other.actionsLog.clear();
        other.plans.clear();
        other.settlements.clear();
        other.facilitiesOptions.clear();
    }
    return *this;
}

void Simulation::start() {
    open();
    cout << "The simulation has started" << endl;

    while (isRunning) {
        string line;
        getline(cin, line);
        vector<string> parsedArgs = Auxiliary::parseArguments(line);
        int parsedArgsSize = parsedArgs.size();

        // Differrentiating between different comands given by the user
        if (parsedArgs[0] == "step" && parsedArgsSize == 2) {
            int numOfSteps = stoi(parsedArgs[1]);
            SimulateStep action = SimulateStep(numOfSteps);
            action.act(*this);
        } else if (parsedArgs[0] == "plan" && parsedArgsSize == 3 &&
                   parsedArgs[2].length() == 3) {
            string settlementName = parsedArgs[1];
            string selectionPolicy = parsedArgs[2];
            AddPlan action = AddPlan(settlementName, selectionPolicy);
            action.act(*this);
        } else if (parsedArgs[0] == "settlement" && parsedArgsSize == 3 &&
                   stoi(parsedArgs[2]) >= 0 && stoi(parsedArgs[2]) < 3) {
            string settlementName = parsedArgs[1];
            SettlementType settlementType =
                static_cast<SettlementType>(stoi(parsedArgs[2]));
            AddSettlement action =
                AddSettlement(settlementName, settlementType);
            action.act(*this);
        } else if (parsedArgs[0] == "facility" && parsedArgsSize == 7 &&
                   stoi(parsedArgs[2]) >= 0 && stoi(parsedArgs[2]) < 3) {
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
        } else if (parsedArgs[0] == "planStatus" && parsedArgsSize == 2) {
            int planId = stoi(parsedArgs[1]);
            PrintPlanStatus action = PrintPlanStatus(planId);
            action.act(*this);
        } else if (parsedArgs[0] == "changePolicy" && parsedArgsSize == 3 &&
                   parsedArgs[2].length() == 3) {
            int planId = stoi(parsedArgs[1]);
            string newPolicy = parsedArgs[2];
            ChangePlanPolicy action = ChangePlanPolicy(planId, newPolicy);
            action.act(*this);
        } else if (parsedArgs[0] == "log" && parsedArgsSize == 1) {
            PrintActionsLog action = PrintActionsLog();
            action.act(*this);
        } else if (parsedArgs[0] == "close" && parsedArgsSize == 1) {
            Close action = Close();
            action.act(*this);
        } else if (parsedArgs[0] == "backup" && parsedArgsSize == 1) {
            BackupSimulation action = BackupSimulation();
            action.act(*this);
        } else if (parsedArgs[0] == "restore" && parsedArgsSize == 1) {
            RestoreSimulation action = RestoreSimulation();
            action.act(*this);
        } else {
            cout << "Please enter a valid command" << endl;
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

bool Simulation::addSettlement(Settlement *settlement) {
    bool isSettlementFound =
        Simulation::isSettlementExists((*settlement).getName());
    if (isSettlementFound) {
        return false;
    }
    settlements.push_back(settlement);
    return true;
}

bool Simulation::addFacility(FacilityType facility) {
    // Adding a facility only if it doesn't already exist
    bool isFacilityExists = false;
    int size = facilitiesOptions.size();
    for (int i = 0; i < size && !isFacilityExists; i++) {  //
        isFacilityExists =
            facility.getName() == facilitiesOptions[i].getName();  //
    }  //
    if (isFacilityExists) {  //
        return false;        //
    }  // Change to isFacilityExists()?
    facilitiesOptions.push_back(facility);
    return true;
}

bool Simulation::isPlanExists(int planId) {
    int size = plans.size();
    for (int i = 0; i < size; i++) {
        if (plans[i].getPlanID() == planId) {
            return true;
        }
    }
    return false;
}

bool Simulation::isSettlementExists(const string &settlementName) {
    bool isSettlementFound = false;
    int size = settlements.size();
    // Searching for settlementName in settlements
    for (int i = 0; i < size && !isSettlementFound; i++) {
        isSettlementFound = settlementName == (*settlements[i]).getName();
    }
    return isSettlementFound;
}

bool Simulation::isFacilityExists(const string &facilityName) {
    bool isFacilityExists = false;
    int size = facilitiesOptions.size();
    // Searching for facilityName in facilitiesOptions
    for (int i = 0; i < size && !isFacilityExists; i++) {
        isFacilityExists = facilityName == facilitiesOptions[i].getName();
    }
    return isFacilityExists;
}

Settlement &Simulation::getSettlement(const string &settlementName) {
    bool isSettlementFound = false;
    int size = settlements.size();
    // Searching for settlementName in settlements
    for (int i = 0; i < size && !isSettlementFound; i++) {
        isSettlementFound = settlementName == (*settlements[i]).getName();
        if (isSettlementFound) {
            return *settlements[i];
        }
    }
    cout << "Error: No settlement with this name exists" << endl;
    Settlement *np = nullptr;
    return *np;
}

// Assuming the plan exists
Plan &Simulation::getPlan(const int planID) {
    int size = plans.size();
    int returnIndex = 0;
    for (int i = 0; i < size; i++) {
        if (plans[i].getPlanID() == planID) {
            returnIndex = i;
            break;
        }
    }
    return plans[returnIndex];
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
        summary +=
            "PlanID: " + to_string(plans[i].getPlanID()) +
            "\nSettlementName: " + plans[i].getSettlementName() +
            "\nLifeQualityScore: " + to_string(plans[i].getlifeQualityScore()) +
            "\nEconomyScore: " + to_string(plans[i].getEconomyScore()) +
            "\nEnvironmentScore: " + to_string(plans[i].getEnvironmentScore());
        if (i != size - 1) {
            summary += "\n";
        }
    }
    cout << summary << endl;
    isRunning = false;
}

void Simulation::open() { isRunning = true; }

Simulation::~Simulation() {
    int actionsLogSize = actionsLog.size();
    for (int i = 0; i < actionsLogSize; i++) {
        delete actionsLog[i];
    }
    actionsLog.clear();
}
