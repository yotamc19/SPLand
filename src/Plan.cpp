#include "Plan.h"

#include <iostream>
#include <vector>
#include "SelectionPolicy.h"
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
      economy_score(0),
      environment_score(0) {}

Plan::Plan(const Plan &other)
    : plan_id(other.plan_id),
      settlement(other.settlement),
      selectionPolicy((*other.selectionPolicy).clone()),
      status(other.status),
      facilities(other.facilities),
      underConstruction(other.underConstruction),
      facilityOptions(other.facilityOptions),
      life_quality_score(other.life_quality_score),
      economy_score(other.economy_score),
      environment_score(other.environment_score) {}

Plan &Plan::operator=(const Plan &other) {
    if (&other != this) {
        plan_id = other.plan_id;
        if (selectionPolicy) {
            delete selectionPolicy;
        }
        selectionPolicy = (*other.selectionPolicy).clone();  // CHECK THIS
        status = other.status;
        facilities = other.facilities;
        underConstruction = other.underConstruction;
        life_quality_score = other.life_quality_score;
        economy_score = other.economy_score;
        environment_score = other.environment_score;
    }
    return *this;
    // check in running time
}

int Plan::getlifeQualityScore() const { return life_quality_score; }

int Plan::getEconomyScore() const { return economy_score; }

int Plan::getEnvironmentScore() const { return environment_score; }

void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy) {
    (*this).selectionPolicy = selectionPolicy;
}

void Plan::step() {
    while (status == PlanStatus::AVALIABLE) {
        FacilityType ft = (*selectionPolicy).selectFacility(facilityOptions);
        Facility* toInsert = new Facility(ft, settlement.getName());
        (*this).addFacility(toInsert);
    }

    int underConstructionSize = underConstruction.size();
    for (int i = 0; i < underConstructionSize; i++) {
        (*underConstruction[i]).step();
    }

    for (int i = 0; i < underConstructionSize; i++) {
        if ((*underConstruction[i]).getTimeLeft() == 0) {
            facilities.push_back(underConstruction[i]);
            status = PlanStatus::AVALIABLE;
            life_quality_score += (*underConstruction[i]).getLifeQualityScore();
            economy_score += (*underConstruction[i]).getEconomyScore();
            environment_score += (*underConstruction[i]).getEnvironmentScore();
        }
    }

    for (int i = 0; i < underConstructionSize; i++) {
        if ((*underConstruction[i]).getTimeLeft() == 0) {
            underConstruction.erase(underConstruction.begin() + i);
            i--;
        }
    }


















    SettlementType settlementType = settlement.getType();
    int maxCapacity = 1;
    if (settlementType == SettlementType::CITY) {
        maxCapacity = 2;
    } else if (settlementType == SettlementType::METROPOLIS) {
        maxCapacity = 3;
    }

    int numOfFacilitiesToAdd = maxCapacity - underConstruction.size();

    vector<Facility *> temp;
    while (underConstruction.size() != 0) {
        Facility *toInsert(underConstruction[underConstruction.size() - 1]);
        FacilityStatus fs = (*toInsert).step();

        if (fs == FacilityStatus::OPERATIONAL) {
            facilities.push_back(toInsert);
        } else {
            temp.push_back(toInsert);
        }

        underConstruction.pop_back();
    }

    for (int i = 0; i < numOfFacilitiesToAdd; i++) {
        Facility *f =
            new Facility((*selectionPolicy).selectFacility(facilityOptions),
                         settlement.getName());
        // implement rule of 5
        addFacility(f);
    }

    int size = underConstruction.size();
    if (maxCapacity == size) {
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
    int facilitiesSize = facilities.size();
    for (int i = 0; i < facilitiesSize; i++) {
        stringAvailableFacilities += (*facilities[i]).toString();
    }

    string stringUnderConstructionFacilities = "";
    int underConstructionSize = facilityOptions.size();
    for (int i = 0; i < underConstructionSize; i++) {
        stringUnderConstructionFacilities += (*facilities[i]).toString();
    }

    string stringFacilitiesOptions = "";
    int facilityOptionsSize = facilityOptions.size();
    for (int i = 0; i < facilityOptionsSize; i++) {
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

Plan::~Plan() {
    delete selectionPolicy;
}
