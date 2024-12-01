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

const int Plan::getlifeQualityScore() const { return life_quality_score; }

const int Plan::getEconomyScore() const { return economy_score; }

const int Plan::getEnvironmentScore() const { return environment_score; }

void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy) {
    (*this).selectionPolicy = selectionPolicy;
}

void Plan::step() {
    int tempSize = underConstruction.size();
    Facility *temp[tempSize];
    for (int i = 0; i < tempSize; i++) {
        temp[i] = underConstruction[i];
    }

    while (underConstruction.size() != 0) {
        underConstruction.pop_back();
    }

    for (int i = 0; i < tempSize; i++) {
        FacilityStatus status = (*temp[i]).step();
        if (status == FacilityStatus::OPERATIONAL) {
            facilities.push_back(temp[i]);
            life_quality_score += (*temp[i]).getLifeQualityScore();
            economy_score += (*temp[i]).getEconomyScore();
            environment_score += (*temp[i]).getEnvironmentScore();
        } else {
            underConstruction.push_back(temp[i]);
        }
    }

    SettlementType setType = settlement.getType();
    int setMaxCapacity = 1;
    if (setType == SettlementType::CITY) {
        setMaxCapacity = 2;
    } else if (setType == SettlementType::METROPOLIS) {
        setMaxCapacity = 3;
    }
    if (setMaxCapacity == underConstruction.size()) {
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