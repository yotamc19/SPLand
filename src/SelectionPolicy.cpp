using namespace std;

#include "SelectionPolicy.h"

#include <cstdint>
#include <iostream>
#include <stdexcept>

// NAIVE SELECTION:
NaiveSelection::NaiveSelection() : lastSelectedIndex(-1) {}

const FacilityType &NaiveSelection::selectFacility(
    const vector<FacilityType> &facilitiesOptions) {
    // Selecting the next available facility
    if (facilitiesOptions.size() == 0) {
        cout << "Error: No facilities to choose from" << endl;
        const FacilityType* np = nullptr;
        return *np;
    }
    lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
    return facilitiesOptions[lastSelectedIndex];
}

const string NaiveSelection::toString() const { return "nai"; }

NaiveSelection *NaiveSelection::clone() const {
    return new NaiveSelection(*this);
}

// BALANCED SELECTION:
BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore,
                                     int EnvironmentScore)
    : LifeQualityScore(LifeQualityScore),
      EconomyScore(EconomyScore),
      EnvironmentScore(EnvironmentScore) {}

const FacilityType &BalancedSelection::selectFacility(
    const vector<FacilityType> &facilitiesOptions) {
    // Selecting the facility which results in the 'minimal distance' between
    // the scores
    if (facilitiesOptions.size() == 0) {
        cout << "Error: No facilities to choose from" << endl;
        const FacilityType* np = nullptr;
        return *np;
    }
    int bestOptionIndex = 0, bestDistance = INT32_MAX;
    int size = facilitiesOptions.size();
    for (int i = 0; i < size; i++) {
        // Calculating the distance caused by the current option
        int tempLifeScore =
            LifeQualityScore + facilitiesOptions[i].getLifeQualityScore();
        int tempEcoScore =
            EconomyScore + facilitiesOptions[i].getEconomyScore();
        int tempEnvScore =
            EnvironmentScore + facilitiesOptions[i].getEnvironmentScore();
        int tempDistance =
            std::max(std::max(tempLifeScore, tempEcoScore), tempEnvScore) -
            std::min(std::min(tempLifeScore, tempEcoScore), tempEnvScore);

        // Choosing the better distance for the choice
        if (tempDistance < bestDistance) {
            bestDistance = tempDistance;
            bestOptionIndex = i;
        }
    }
    return facilitiesOptions[bestOptionIndex];
}

void BalancedSelection::setScores(int potentialLifeQualityScore,
                                  int potentialEconomyScore,
                                  int potentialEnvironmentScore) {
    LifeQualityScore = potentialLifeQualityScore;
    EconomyScore = potentialEconomyScore;
    EnvironmentScore = potentialEnvironmentScore;
}

const string BalancedSelection::toString() const { return "bal"; }

BalancedSelection *BalancedSelection::clone() const {
    return new BalancedSelection(*this);
}

// ECONOMY SELECTION:
EconomySelection::EconomySelection() : lastSelectedIndex(-1) {}

const FacilityType &EconomySelection::selectFacility(
    const vector<FacilityType> &facilitiesOptions) {
    // Selecting the next available facility which is also of "Economy" type
    if (facilitiesOptions.size() == 0) {
        cout << "Error: No facilities to choose from" << endl;
        const FacilityType* np = nullptr;
        return *np;
    }
    lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();

    // Cycling through the vector until an economy FacilityType or full cycle is
    // reached
    int i = 0;
    bool isReachedEconomy = false;
    int size = facilitiesOptions.size();
    for (; i < size && !isReachedEconomy; i++) {
        isReachedEconomy = facilitiesOptions[(i + lastSelectedIndex) %
                                             facilitiesOptions.size()]
                               .getCategory() == FacilityCategory::ECONOMY;
    }
    // Checking if a fitting facility was reached
    if (!isReachedEconomy) {
        cout << "Error: No economy facilities" << endl;
        const FacilityType* np = nullptr;
        return *np;
    }
    lastSelectedIndex = (i + lastSelectedIndex - 1) % facilitiesOptions.size();
    return facilitiesOptions[lastSelectedIndex];
}

const string EconomySelection::toString() const { return "eco"; }

EconomySelection *EconomySelection::clone() const {
    return new EconomySelection(*this);
}

// SUSTAINABILITY SELECTION:
SustainabilitySelection::SustainabilitySelection() : lastSelectedIndex(-1) {}

const FacilityType &SustainabilitySelection::selectFacility(
    const vector<FacilityType> &facilitiesOptions) {
    // Selecting the next available facility which is also of "Environment" type
    if (facilitiesOptions.size() == 0) {
        cout << "Error: No facilities to choose from" << endl;
        const FacilityType* np = nullptr;
        return *np;
    }
    lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();

    // Cycling through the vector until an environment FacilityType or full
    // cycle is reached
    int i = 0;
    bool isReachedEnvironment = false;
    int size = facilitiesOptions.size();
    for (; i < size && !isReachedEnvironment; i++) {
        isReachedEnvironment =
            facilitiesOptions[(i + lastSelectedIndex) %
                              facilitiesOptions.size()]
                .getCategory() == FacilityCategory::ENVIRONMENT;
    }
    // Checking if a fitting facility was reached
    if (!isReachedEnvironment) {
        cout << "Error: No environment facilities" << endl;
        const FacilityType* np = nullptr;
        return *np;
    }
    lastSelectedIndex = (i + lastSelectedIndex) % facilitiesOptions.size();
    return facilitiesOptions[lastSelectedIndex];
}

const string SustainabilitySelection::toString() const { return "env"; }

SustainabilitySelection *SustainabilitySelection::clone() const {
    return new SustainabilitySelection(*this);
}