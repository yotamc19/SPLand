#pragma once
#include <vector>
#include "Facility.h"
#include "SelectionPolicy.h"
#include "Settlement.h"
using std::vector;

enum class PlanStatus {
    AVALIABLE,
    BUSY,
};

class Plan {
   public:
    Plan(const int planId, const Settlement &settlement,
         SelectionPolicy *selectionPolicy,
         const vector<FacilityType> &facilityOptions);
    Plan(const Plan& other);
    Plan& operator=(const Plan& other);
    Plan(Plan&& other) noexcept;
    Plan& operator=(Plan&& other) noexcept;
    int getPlanID() const;
    int getlifeQualityScore() const;
    int getEconomyScore() const;
    int getEnvironmentScore() const;
    int getPotenitialLifeQualityScore() const;
    int getPotenitialEconomyScore() const;
    int getPotenitialEnvironmentScore() const;
    void setSelectionPolicy(SelectionPolicy *selectionPolicy);
    void step();
    void printStatus();
    const vector<Facility *> &getFacilities() const;
    void addFacility(Facility *facility);
    const string toString() const;
    ~Plan();

   private:
    int plan_id;
    const Settlement &settlement;
    SelectionPolicy
        *selectionPolicy;  // What happens if we change this to a reference?
    PlanStatus status;
    vector<Facility *> facilities;
    vector<Facility *> underConstruction;
    const vector<FacilityType> &facilityOptions;
    int life_quality_score, economy_score, environment_score;
    int potenitial_life_quality_score, potenitial_economy_score, potenitial_environment_score;
};