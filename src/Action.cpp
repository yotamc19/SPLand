#include "Action.h"

#include <iostream>

#include "Plan.h"
#include "SelectionPolicy.h"
#include "Settlement.h"
using namespace std;
extern Simulation* backup;

// BASE_ACTION:
BaseAction::BaseAction() : errorMsg(""), status(ActionStatus::COMPLETED) {}

ActionStatus BaseAction::getStatus() const { return status; }

void BaseAction::complete() { status = ActionStatus::COMPLETED; }

void BaseAction::error(string errorMsg) {
    status = ActionStatus::ERROR;
    (*this).errorMsg = errorMsg;
    cout << errorMsg << endl;
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
    for (int i = 0; i < numOfSteps; i++) {
        simulation.step();
    }
    complete();
    simulation.addAction((*this).clone());
}

const string SimulateStep::toString() const {
    return "step " + to_string(numOfSteps) + " " + statusToString();
}

SimulateStep* SimulateStep::clone() const { return new SimulateStep(*this); }

// ADD_PLAN:
AddPlan::AddPlan(const string& settlementName, const string& selectionPolicy)
    : settlementName(settlementName), selectionPolicy(selectionPolicy) {}

void AddPlan::act(Simulation& simulation) {
    // Checking if there is already a plan for said settlement?
    bool isSettlementExists = simulation.isSettlementExists(settlementName);
    if (!isSettlementExists) {
        error("Error: No settlement exists in the simulation");
        simulation.addAction((*this).clone());
        return;
    }

    // Building a new settlement and adding it with the right selection policy
    Settlement& s = simulation.getSettlement(settlementName);
    SelectionPolicy* sp = nullptr;
    if (selectionPolicy == "nai") {
        sp = new NaiveSelection();
    } else if (selectionPolicy == "bal") {
        sp = new BalancedSelection(0, 0, 0);
    } else if (selectionPolicy == "eco") {
        sp = new EconomySelection();
    } else if (selectionPolicy == "env") {
        sp = new SustainabilitySelection();
    } else {
        error("Error: Not a valid selection policy");
        simulation.addAction((*this).clone());
        return;
    }
    simulation.addPlan(s, sp);
    complete();
    simulation.addAction((*this).clone());
}

const string AddPlan::toString() const {
    return "plan for " + settlementName + " " + statusToString();
}

AddPlan* AddPlan::clone() const { return new AddPlan(*this); }

// ADD_SETTLEMENT:
AddSettlement::AddSettlement(const string& settlementName,
                             SettlementType settlementType)
    : settlementName(settlementName), settlementType(settlementType) {}

void AddSettlement::act(Simulation& simulation) {
    // Checking if the settlement is already in the simulation
    bool isSettlementExists = simulation.isSettlementExists(settlementName);
    if (isSettlementExists) {
        error("Error: This settlemet exists already");
        simulation.addAction((*this).clone());
        return;
    }

    // Adding the settlement
    Settlement* s = new Settlement(settlementName, settlementType);
    simulation.addSettlement(s);
    complete();
    simulation.addAction((*this).clone());
}

AddSettlement* AddSettlement::clone() const { return new AddSettlement(*this); }

const string AddSettlement::toString() const {
    return "settlement " + settlementName + " " +
           to_string((int)settlementType) + " " + statusToString();
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
    // Checking if the facility is already in the simulation
    bool isFacilityExists = simulation.isFacilityExists(facilityName);
    if (isFacilityExists) {
        error("Error: This facility exists already");
        simulation.addAction((*this).clone());
        return;
    }

    // Adding the facility
    FacilityType fs(facilityName, facilityCategory, price, lifeQualityScore,
                    economyScore, environmentScore);
    simulation.addFacility(fs);
    complete();
    simulation.addAction((*this).clone());
}

AddFacility* AddFacility::clone() const { return new AddFacility(*this); }

const string AddFacility::toString() const {
    return "facility " + facilityName + " " + to_string((int)facilityCategory) +
           " " + statusToString();
}

// PRINT_PLAN_STATUS:
PrintPlanStatus::PrintPlanStatus(int planId) : planId(planId) {}

void PrintPlanStatus::act(Simulation& simulation) {
    bool isPlanExists = simulation.isPlanExists(planId);
    if (isPlanExists) {
        cout << simulation.getPlan(planId).toString() << endl;
        complete();
    } else {
        error("Error: Plan doesn't exsist");
    }

    simulation.addAction((*this).clone());
}

PrintPlanStatus* PrintPlanStatus::clone() const {
    return new PrintPlanStatus(*this);
}

const string PrintPlanStatus::toString() const {
    return "planStatus " + to_string(planId) + " " + statusToString();
}

// CHANGE_PLAN_POLICY:
ChangePlanPolicy::ChangePlanPolicy(const int planId, const string& newPolicy)
    : planId(planId), newPolicy(newPolicy) {}

void ChangePlanPolicy::act(Simulation& simulation) {
    // Finding the correct policy to which to change, and changing accordingly
    SelectionPolicy* sp = nullptr;
    Plan p(simulation.getPlan(planId));
    if (newPolicy == "nai") {
        sp = new NaiveSelection();
    } else if (newPolicy == "bal") {
        sp = new BalancedSelection(0, 0, 0);
    } else if (newPolicy == "eco") {
        sp = new EconomySelection();
    } else {
        sp = new SustainabilitySelection();
    }
    simulation.getPlan(planId).setSelectionPolicy(sp);
    complete();
    simulation.addAction(clone());
}

ChangePlanPolicy* ChangePlanPolicy::clone() const {
    return new ChangePlanPolicy(*this);
}

const string ChangePlanPolicy::toString() const {
    return "changePlan " + to_string(planId) + " to " + newPolicy + " " +
           statusToString();
}

// PRINT_ACTIONS_LOG:
PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(Simulation& simulation) {
    // Getting the actionsLog, going through it, and printing one by one
    vector<BaseAction*> actions = simulation.getActionsLog();
    int actionsSize =
        static_cast<vector<BaseAction*>::size_type>(actions.size());
    for (int i = 0; i < actionsSize; i++) {
        cout << (*actions[i]).toString() << endl;
    }
    complete();
    simulation.addAction((*this).clone());
}

PrintActionsLog* PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);
}

const string PrintActionsLog::toString() const {
    return "printActionLog " + statusToString();
}

// CLOSE:
Close::Close() {}

void Close::act(Simulation& simulation) {
    simulation.addAction((*this).clone());
    complete();
    simulation.close();
}

Close* Close::clone() const { return new Close(*this); }

const string Close::toString() const { return "close " + statusToString(); }

// BACKUP_SIMULATION:
BackupSimulation::BackupSimulation() {}

void BackupSimulation::act(Simulation& simulation) {
    backup = new Simulation(simulation);
    complete();
    simulation.addAction((*this).clone());
}

BackupSimulation* BackupSimulation::clone() const {
    return new BackupSimulation(*this);
}

const string BackupSimulation::toString() const {
    return "backup " + statusToString();
}

// RESTORE_SIMULATION:
RestoreSimulation::RestoreSimulation() {}

void RestoreSimulation::act(Simulation& simulation) {
    if (backup == nullptr) {
        error("Error: No backup was created");
    } else {
        simulation = *backup;
        complete();
    }
    simulation.addAction((*this).clone());
}

RestoreSimulation* RestoreSimulation::clone() const {
    return new RestoreSimulation(*this);
}

const string RestoreSimulation::toString() const {
    return "restore " + statusToString();
}