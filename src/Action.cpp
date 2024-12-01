#include "Action.h"

#include <iostream>

#include "Plan.h"
#include "SelectionPolicy.h"
#include "Settlement.h"
using namespace std;

BaseAction::BaseAction() {}

ActionStatus BaseAction::getStatus() const { return status; }

void BaseAction::complete() {}

void BaseAction::error(string errorMsg) {}

const string& BaseAction::getErrorMsg() const { return errorMsg; }

SimulateStep::SimulateStep(const int numOfSteps) : numOfSteps(numOfSteps) {}

void SimulateStep::act(Simulation& simulation) {
    for (int i = 0; i < numOfSteps; i++) {
        simulation.step();
    }
    simulation.addAction(this);
}

const string SimulateStep::toString() const {
    return "step " + to_string(numOfSteps) + " COMPLETED";
}

SimulateStep* SimulateStep::clone() const { return new SimulateStep(*this); }

AddPlan::AddPlan(const string& settlementName, const string& selectionPolicy)
    : settlementName(settlementName), selectionPolicy(selectionPolicy) {}

void AddPlan::act(Simulation& simulation) {
    Settlement s = simulation.getSettlement(settlementName);
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
}

const string AddPlan::toString() const {
    return "plan for " + settlementName + " CREATED";
}

AddPlan* AddPlan::clone() const { return new AddPlan(*this); }

AddSettlement::AddSettlement(const string& settlementName,
                             SettlementType settlementType)
    : settlementName(settlementName), settlementType(settlementType) {}

void AddSettlement::act(Simulation& simulation) {
    Settlement s(settlementName, settlementType);
    bool status = simulation.addSettlement(s);
    // what to do with it?
}

AddSettlement* AddSettlement::clone() const { return new AddSettlement(*this); }

const string AddSettlement::toString() const {
    return "settlement " + settlementName + " CREATED";
}

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
    FacilityType fs(facilityName, facilityCategory, price, lifeQualityScore,
                    economyScore, environmentScore);
    bool status = simulation.addFacility(fs);
    // what to do with it?
}

AddFacility* AddFacility::clone() const { return new AddFacility(*this); }

const string AddFacility::toString() const {
    return "facility " + facilityName + " CREATED";
}

PrintPlanStatus::PrintPlanStatus(int planId) : planId(planId) {}

void PrintPlanStatus::act(Simulation& simulation) {
    Plan p = simulation.getPlan(planId);
    cout << p.toString() << endl;
}

PrintPlanStatus* PrintPlanStatus::clone() const {
    return new PrintPlanStatus(*this);
}

const string PrintPlanStatus::toString() const {
    return "printed plan status for planId: " + to_string(planId) +
           " COMPLETED";
}

ChangePlanPolicy::ChangePlanPolicy(const int planId, const string& newPolicy)
    : planId(planId), newPolicy(newPolicy) {}

void ChangePlanPolicy::act(Simulation& simulation) {
	Plan p = simulation.getPlan(planId);
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
}

ChangePlanPolicy* ChangePlanPolicy::clone() const {
	return new ChangePlanPolicy(*this);
}

const string ChangePlanPolicy::toString() const {
	return "changed plan policy for planId: " + to_string(planId) + " COMPLETED";
}

PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(Simulation& simulation) {
	// should we add function in simulation???
}

PrintActionsLog* PrintActionsLog::clone() const {
	return new PrintActionsLog(*this);
}

const string PrintActionsLog::toString() const {
	return "printed actions log COMPLETED";
}

Close::Close() {}

void Close::act(Simulation& simulation) {
	simulation.close();
}

Close* Close::clone() const {
	return new Close(*this);
}

const string Close::toString() const {
	return "simulation closed COMPLETED";
}

BackupSimulation::BackupSimulation() {}

void BackupSimulation::act(Simulation& simulation) {
	
}
