#include "Action.h"

#include <iostream>

#include "Plan.h"
#include "SelectionPolicy.h"
#include "Settlement.h"
using namespace std;
extern Simulation* backup;

// BASE_ACTION:
BaseAction::BaseAction() {}

ActionStatus BaseAction::getStatus() const { return status; }

void BaseAction::complete() { status = ActionStatus::COMPLETED; }

void BaseAction::error(string errorMsg) {
    status = ActionStatus::ERROR;
    (*this).errorMsg = errorMsg;
}

const string& BaseAction::getErrorMsg() const { return errorMsg; }

const string BaseAction::statusToString() const {
    string statusString = "COMPLETED";
    if (status == ActionStatus::ERROR) {
        statusString = "ERROR";
    }
    return statusString;
}

// SIMULATE_STEP
SimulateStep::SimulateStep(const int numOfSteps) : numOfSteps(numOfSteps) {}

void SimulateStep::act(Simulation& simulation) {
    cout << toString() << endl;

    for (int i = 0; i < numOfSteps; i++) {
        simulation.step();
    }
    (*this).complete();
    simulation.addAction(this);
}

const string SimulateStep::toString() const {
    return "Action: Simulate step with " + to_string(numOfSteps) +
           " steps,\nStatus: " + statusToString();
}

SimulateStep* SimulateStep::clone() const { return new SimulateStep(*this); }

// ADD_PLAN:
AddPlan::AddPlan(const string& settlementName, const string& selectionPolicy)
    : settlementName(settlementName), selectionPolicy(selectionPolicy) {}

void AddPlan::act(Simulation& simulation) {
    cout << toString() << endl;

    // Checking if there is already a plan for said settlement?         COME
    // BACK
    bool isSettlementExists = simulation.isSettlementExists(settlementName);
    if (!isSettlementExists) {
        error("No settlement exists in the simulation");
        simulation.addAction(this);
        return;
    }

    // Building a new settlement and adding it with the right selection policy
    Settlement s(simulation.getSettlement(settlementName));
    if (selectionPolicy == "nai") {
        NaiveSelection ns;
        simulation.addPlan(s, &ns);
    } else if (selectionPolicy == "bal") {
        BalancedSelection bs(0, 0, 0);
        simulation.addPlan(s, &bs);
    } else if (selectionPolicy == "eco") {
        EconomySelection es;
        simulation.addPlan(s, &es);
    } else {
        SustainabilitySelection ss;
        simulation.addPlan(s, &ss);
    }
    (*this).complete();
    simulation.addAction(this);
}

const string AddPlan::toString() const {
    return "Action: Add plan for " + settlementName + " with policy " +
           selectionPolicy + ",\nStatus: " + statusToString();
}

AddPlan* AddPlan::clone() const { return new AddPlan(*this); }

// ADD_SETTLEMENT:
AddSettlement::AddSettlement(const string& settlementName,
                             SettlementType settlementType)
    : settlementName(settlementName), settlementType(settlementType) {}

void AddSettlement::act(Simulation& simulation) {
    cout << toString() << endl;

    // Checking if the settlement is already in the simulation
    bool isSettlementExists = simulation.isSettlementExists(settlementName);
    if (isSettlementExists) {
        error("This settlemet exists already");
        simulation.addAction(this);
        return;
    }

    // Adding the settlement
    Settlement s(settlementName, settlementType);
    simulation.addSettlement(s);
    (*this).complete();
    simulation.addAction(this);
}

AddSettlement* AddSettlement::clone() const { return new AddSettlement(*this); }

const string AddSettlement::toString() const {
    return "Action: Add settlement " + settlementName +
           ",\nStatus: " + statusToString();
}

// ADD_FACILITY:
AddFacility::AddFacility(const string& facilityName,
                         const FacilityCategory facilityCategory,
                         const int price, const int lifeQualityScore,
                         const int economyScore, const int environmentScore)
    : facilityName(facilityName),
      facilityCategory(facilityCategory),
      price(price),
      lifeQualityScore(lifeQualityScore),
      economyScore(economyScore),
      environmentScore(environmentScore) {}

void AddFacility::act(Simulation& simulation) {
    cout << toString() << endl;

    // Checking if the facility is already in the simulation
    bool isFacilityExists = simulation.isFacilityExists(facilityName);
    if (isFacilityExists) {
        error("This facility exists already");
        simulation.addAction(this);
        return;
    }

    // Adding the facility
    FacilityType fs(facilityName, facilityCategory, price, lifeQualityScore,
                    economyScore, environmentScore);
    simulation.addFacility(fs);
    (*this).complete();
    simulation.addAction(this);
}

AddFacility* AddFacility::clone() const { return new AddFacility(*this); }

const string AddFacility::toString() const {
    return "Action: Add facility " + facilityName +
           ",\nStatus: " + statusToString();
}

// PRINT_PLAN_STATUS:
PrintPlanStatus::PrintPlanStatus(int planId) : planId(planId) {}

void PrintPlanStatus::act(Simulation& simulation) {
    cout << toString() << endl;

    simulation.getPlan(planId).printStatus();
    (*this).complete();
    simulation.addAction(this);
}

PrintPlanStatus* PrintPlanStatus::clone() const {
    return new PrintPlanStatus(*this);
}

const string PrintPlanStatus::toString() const {
    return "Action: Print plan " + to_string(planId) +
           " status,\nStatus: " + statusToString();
}

// CHANGE_PLAN_POLICY:
ChangePlanPolicy::ChangePlanPolicy(const int planId, const string& newPolicy)
    : planId(planId), newPolicy(newPolicy) {}

void ChangePlanPolicy::act(Simulation& simulation) {
    cout << toString() << endl;

    // Finding the correct policy to which to change, and changing accordingly
    Plan p(simulation.getPlan(planId));
    if (newPolicy == "nai") {
        NaiveSelection ns;
        p.setSelectionPolicy(&ns);
    } else if (newPolicy == "bal") {
        BalancedSelection bs(0, 0, 0);
        p.setSelectionPolicy(&bs);
    } else if (newPolicy == "eco") {
        EconomySelection es;
        p.setSelectionPolicy(&es);
    } else {
        SustainabilitySelection ss;
        p.setSelectionPolicy(&ss);
    }
    (*this).complete();
    simulation.addAction(this);
}

ChangePlanPolicy* ChangePlanPolicy::clone() const {
    return new ChangePlanPolicy(*this);
}

const string ChangePlanPolicy::toString() const {
    return "Action: Change plan policy for planId: " + to_string(planId) +
           ",\nStatus: " + statusToString();
}

// PRINT_ACTIONS_LOG:
PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(Simulation& simulation) {
    cout << toString() << endl;

    // Getting the actionsLog, going through it, and printing one by one
    vector<BaseAction*> actions = simulation.getActionsLog();
    int actionsSize =
        static_cast<vector<BaseAction*>::size_type>(actions.size());
    for (int i = 0; i < actionsSize; i++) {
        cout << (*actions[i]).toString() << endl;
    }
    (*this).complete();
    simulation.addAction(this);
}

PrintActionsLog* PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);
}

const string PrintActionsLog::toString() const {
    return "Action: Print all actions log,\nStatus: " + statusToString();
}

// CLOSE:
Close::Close() {}

void Close::act(Simulation& simulation) {
    cout << toString() << endl;

    simulation.addAction(this);
    (*this).complete();
    simulation.close();
}

Close* Close::clone() const { return new Close(*this); }

const string Close::toString() const {
    return "Action: Close simulation,\nStatus: " + statusToString();
}

// BACKUP_SIMULATION:
BackupSimulation::BackupSimulation() {}

void BackupSimulation::act(Simulation& simulation) {
    cout << toString() << endl;

    backup = new Simulation(simulation);
    (*this).complete();
    simulation.addAction(this);
}

BackupSimulation* BackupSimulation::clone() const {
    return new BackupSimulation(*this);
}

const string BackupSimulation::toString() const {
    return "Action: Backup simulation,\nStatus: " + statusToString();
}

// RESTORE_SIMULATION:
RestoreSimulation::RestoreSimulation() {}

void RestoreSimulation::act(Simulation& simulation) {
    cout << toString() << endl;

    if (backup == nullptr) {
        error("No backup was created");
    } else {
        simulation = *backup;
        (*this).complete();
    }
    simulation.addAction(this);
}

RestoreSimulation* RestoreSimulation::clone() const {
    return new RestoreSimulation(*this);
}

const string RestoreSimulation::toString() const {
    return "Action: Restore simulation,\nStatus: " + statusToString();
}