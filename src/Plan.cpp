#include "Plan.h"

#include <iostream>
#include <vector>
using namespace std;

Plan::Plan(const int planId, const Settlement &settlement,
           SelectionPolicy *selectionPolicy,
           const vector<FacilityType> &facilityOptions)
    : plan_id(planId),
      settlement(settlement),
      selectionPolicy(selectionPolicy),
      status(PlanStatus::AVALIABLE),
      facilities(vector<Facility *>()),
      underConstruction(vector<Facility *>()),
      facilityOptions(facilityOptions),
      life_quality_score(0),
      economy_score(economy_score),
      environment_score(environment_score) {}

int Plan::getlifeQualityScore() const { return life_quality_score; }

int Plan::getEconomyScore() const { return economy_score; }

int Plan::getEnvironmentScore() const { return environment_score; }

void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy) {
    (*this).selectionPolicy = selectionPolicy;
}

void Plan::step() {
    SettlementType settlementType = settlement.getType();
    int maxCapacity = 1;
    if (settlementType == SettlementType::CITY) {
        maxCapacity = 2;
    } else if (settlementType == SettlementType::METROPOLIS) {
        maxCapacity = 3;
    }

    int numOfFacilitiesToAdd = maxCapacity - underConstruction.size();

    vector<Facility*> temp;
    while (underConstruction.size() != 0) {
        Facility* toInsert(underConstruction[underConstruction.size() - 1]);
        FacilityStatus fs = (*toInsert).step();

        if (fs == FacilityStatus::OPERATIONAL) {
            facilities.push_back(toInsert);
        } else {
            temp.push_back(toInsert);
        }

        underConstruction.pop_back();
    }

    for (int i = 0; i < numOfFacilitiesToAdd; i++) {
        Facility *f = new Facility((*selectionPolicy).selectFacility(facilityOptions), settlement.getName());
        // implement rule of 5
        addFacility(f);
    }

    if (maxCapacity == underConstruction.size()) {
        status = PlanStatus::BUSY;
    } else {
        status = PlanStatus::AVALIABLE;
    }
}

void Plan::printStatus() {
    string stringStatus = "Available";
    if (status == PlanStatus::BUSY) {
        stringStatus = "Busy";
    }
    cout << stringStatus << endl;
}

const vector<Facility *> &Plan::getFacilities() const { return facilities; }

void Plan::addFacility(Facility *facility) {
    if (status == PlanStatus::BUSY) {
        throw invalid_argument("Cant add to busy plan");
    }

    underConstruction.push_back(facility);
}

const string Plan::toString() const {
    string stringStatus = "Available";
    if (status == PlanStatus::BUSY) {
        stringStatus = "Busy";
    }

    string stringAvailableFacilities = "";
    for (int i = 0; i < facilities.size(); i++) {
        stringAvailableFacilities += (*facilities[i]).toString();
    }

    string stringUnderConstructionFacilities = "";
    for (int i = 0; i < underConstruction.size(); i++) {
        stringUnderConstructionFacilities += (*facilities[i]).toString();
    }

    string stringFacilitiesOptions = "";
    for (int i = 0; i < facilityOptions.size(); i++) {
        stringFacilitiesOptions += (*facilities[i]).toString();
    }

    string finalStr =
        "Plan id: " + to_string(plan_id) + ",\n" + settlement.toString() +
        ",\n" + (*selectionPolicy).toString() + ",\nStatus: " + stringStatus +
        "\nFacilities options: " + stringFacilitiesOptions +
        ",\nUnder construction facilities: " +
        stringUnderConstructionFacilities +
        ",\nAvailable facilities: " + stringAvailableFacilities +
        ",\nLife quality score: " + to_string(life_quality_score) +
        ",\nEconomy score: " + to_string(economy_score) +
        ",\nEnvironment score: " + to_string(environment_score);

    return finalStr;
}

const Settlement &Plan::getSettlement() { return settlement; }
