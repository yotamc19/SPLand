#pragma once
#include <string>
#include <vector>

#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
using std::string;
using std::vector;

class BaseAction;
class SelectionPolicy;

class Simulation {
   public:
    Simulation(const string& configFilePath);
    Simulation(const Simulation& other);
    Simulation& operator=(const Simulation& other);
    Simulation(Simulation&& other) noexcept;
    Simulation& operator=(Simulation&& other) noexcept;
    void start();
    void addPlan(const Settlement& settlement,
                 SelectionPolicy* selectionPolicy);
    void addAction(BaseAction* action);
    bool addSettlement(Settlement* settlement);
    bool addFacility(FacilityType facility);
    bool isPlanExists(int planId);
    bool isSettlementExists(const string& settlementName);
    bool isFacilityExists(const string& facilityName);
    Settlement& getSettlement(const string& settlementName);
    Plan& getPlan(const int planID);
    const vector<BaseAction*>& getActionsLog();
    void step();
    void close();
    void open();
    ~Simulation();

   private:
    bool isRunning;
    int planCounter;  // For assigning unique plan IDs
    vector<BaseAction*> actionsLog;
    vector<Plan> plans;
    vector<Settlement*> settlements;
    vector<FacilityType> facilitiesOptions;
};