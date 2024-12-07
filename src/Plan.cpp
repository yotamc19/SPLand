#include "Plan.h"

#include <iostream>
#include <sstream>
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
      facilities(),
      underConstruction(),
      facilityOptions(facilityOptions),
      life_quality_score(0),
      economy_score(0),
      environment_score(0),
      potenitial_life_quality_score(0),
      potenitial_economy_score(0),
      potenitial_environment_score(0) {}

Plan::Plan(const Plan &other)  // Copy constructor
    : plan_id(other.plan_id),
      settlement(other.settlement),
      selectionPolicy((*other.selectionPolicy).clone()),
      status(other.status),
      facilities(other.facilities),
      underConstruction(other.underConstruction),
      facilityOptions(other.facilityOptions),
      life_quality_score(other.life_quality_score),
      economy_score(other.economy_score),
      environment_score(other.environment_score),
      potenitial_life_quality_score(other.potenitial_life_quality_score),
      potenitial_economy_score(other.potenitial_economy_score),
      potenitial_environment_score(other.potenitial_environment_score) {}

Plan &Plan::operator=(const Plan &other) {  // Copy assignment operator
    if (&other != this) {
        delete selectionPolicy;

        plan_id = other.plan_id;
        selectionPolicy = (*other.selectionPolicy).clone();
        status = other.status;
        facilities = other.facilities;
        underConstruction = other.underConstruction;
        life_quality_score = other.life_quality_score;
        economy_score = other.economy_score;
        environment_score = other.environment_score;
        potenitial_life_quality_score = other.potenitial_life_quality_score;
        potenitial_economy_score = other.potenitial_economy_score;
        potenitial_environment_score = other.potenitial_environment_score;
    }
    return *this;
}

Plan::Plan(Plan &&other) noexcept
    : plan_id(other.plan_id),
      settlement(other.settlement),
      selectionPolicy(other.selectionPolicy),
      status(other.status),
      facilities(move(other.facilities)),
      underConstruction(move(other.underConstruction)),
      facilityOptions(other.facilityOptions),
      life_quality_score(other.life_quality_score),
      economy_score(other.economy_score),
      environment_score(other.environment_score),
      potenitial_life_quality_score(other.potenitial_life_quality_score),
      potenitial_economy_score(other.potenitial_economy_score),
      potenitial_environment_score(other.potenitial_environment_score) {
    other.selectionPolicy = nullptr;
    int facilitiesSize = other.facilities.size();
    for (int i = 0; i < facilitiesSize; i++) {
        delete other.facilities[i];
    }
    other.facilities.clear();

    int underConstructionSize = other.underConstruction.size();
    for (int i = 0; i < underConstructionSize; i++) {
        delete other.underConstruction[i];
    }
    other.underConstruction.clear();

    other.status = PlanStatus::AVALIABLE;
}

Plan &Plan::operator=(Plan &&other) noexcept {
    if (&other != this) {
        int facilitiesSize = facilities.size();
        for (int i = 0; i < facilitiesSize; i++) {
            delete facilities[i];
        }
        facilities.clear();

        int underConstructionSize = underConstruction.size();
        for (int i = 0; i < underConstructionSize; i++) {
            delete underConstruction[i];
        }
        underConstruction.clear();

        delete selectionPolicy;
        selectionPolicy = nullptr;

        plan_id = other.plan_id;
        selectionPolicy = other.selectionPolicy;
        status = other.status;
        facilities = move(other.facilities);
        underConstruction = move(other.underConstruction);
        life_quality_score = other.life_quality_score;
        economy_score = other.economy_score;
        environment_score = other.environment_score;
        potenitial_life_quality_score = other.potenitial_life_quality_score;
        potenitial_economy_score = other.potenitial_economy_score;
        potenitial_environment_score = other.potenitial_environment_score;

        other.selectionPolicy = nullptr;
        other.facilities.clear();
        other.underConstruction.clear();
        other.status = PlanStatus::AVALIABLE;
    }
    return *this;
}

int Plan::getPlanID() const { return plan_id; }
// Settlement name is const, fix?

int Plan::getlifeQualityScore() const { return life_quality_score; }

int Plan::getEconomyScore() const { return economy_score; }

int Plan::getEnvironmentScore() const { return environment_score; }

int Plan::getPotenitialLifeQualityScore() const {
    return potenitial_life_quality_score;
}

int Plan::getPotenitialEconomyScore() const { return potenitial_economy_score; }

int Plan::getPotenitialEnvironmentScore() const {
    return potenitial_environment_score;
}

void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy) {
    delete (*this).selectionPolicy;
    (*this).selectionPolicy = selectionPolicy;
}

void Plan::step() {
    // Adding as many new facilities as the capacity allows
    while (status == PlanStatus::AVALIABLE) {
        if ((*selectionPolicy).toString() == "bal") {
            (*dynamic_cast<BalancedSelection *>(selectionPolicy))
                .setScores(potenitial_life_quality_score,
                           potenitial_economy_score,
                           potenitial_environment_score);
        }
        const FacilityType &ft =
            (*selectionPolicy).selectFacility(facilityOptions);
        potenitial_life_quality_score += ft.getLifeQualityScore();
        potenitial_economy_score += ft.getEconomyScore();
        potenitial_environment_score += ft.getEnvironmentScore();
        Facility *toInsert = new Facility(ft, settlement.getName());
        cout << "INSERT: " + (*toInsert).getName() << endl;
        ////////// COME BACK
        addFacility(toInsert);
        delete toInsert; //this causes issues
    }

    // Updating all of the facilities under construction
    int underConstructionSize = underConstruction.size();
    for (int i = 0; i < underConstructionSize; i++) {
        (*underConstruction[i]).step();
    }

    // Relocating the facilities who have now finished construction,
    // and updating the scores and the plan status accordingly
    for (int i = 0; i < underConstructionSize; i++) {
        if ((*underConstruction[i]).getTimeLeft() == 0) {
            facilities.push_back(underConstruction[i]);
            status = PlanStatus::AVALIABLE;
            life_quality_score += (*underConstruction[i]).getLifeQualityScore();
            economy_score += (*underConstruction[i]).getEconomyScore();
            environment_score += (*underConstruction[i]).getEnvironmentScore();
        }
    }
    for (int i = 0; i < (int)underConstruction.size(); i++) {
        if ((*underConstruction[i]).getTimeLeft() == 0) {
            underConstruction.erase(underConstruction.begin() + i);
            if (i > 0) {
                i--;  // This is because when an item is removed, the remaining
                      // items all move 1 index 'below'
            }
        }
    }
}

void Plan::printStatus() {
    string stringStatus = "AVAILABLE";
    if (status == PlanStatus::BUSY) {
        stringStatus = "BUSY";
    }
    cout << "PlanStatus: " + stringStatus << endl;
}

const vector<Facility *> &Plan::getFacilities() const { return facilities; }

void Plan::addFacility(Facility *facility) {
    if (status == PlanStatus::AVALIABLE) {
        underConstruction.push_back(facility);

        int underConstructionSize = underConstruction.size();
        if (underConstructionSize == (int)settlement.getType() + 1) {
            status = PlanStatus::BUSY;
        }
    } else {
        cout << "Error: Plan is busy" << endl;
    }
}

const string Plan::getSettlementName() const { return settlement.getName(); }

const string Plan::toString() const {
    string stringStatus = "AVAILABLE";
    if (status == PlanStatus::BUSY) {
        stringStatus = "BUSY";
    }

    string stringUnderConstructionFacilities = "";
    int underConstructionSize = underConstruction.size();
    for (int i = 0; i < underConstructionSize; i++) {
        stringUnderConstructionFacilities +=
            "\n" + (*underConstruction[i]).toString();
    }

    string stringAvailableFacilities = "";
    int facilitiesSize = facilities.size();
    for (int i = 0; i < facilitiesSize; i++) {
        stringAvailableFacilities += "\n" + (*facilities[i]).toString();
    }

    string finalStr = "PlanID: " + to_string(plan_id) +
                      "\nSettlementName: " + getSettlementName() +
                      "\nPlanStatus: " + stringStatus +
                      "\nSelectionPolicy: " + (*selectionPolicy).toString() +
                      "\nLifeQualityScore: " + to_string(life_quality_score) +
                      "\nEconomyScore: " + to_string(economy_score) +
                      "\nEnvironmentScore: " + to_string(environment_score) +
                      stringUnderConstructionFacilities +
                      stringAvailableFacilities;

    return finalStr;
}

Plan::~Plan() { delete selectionPolicy; }