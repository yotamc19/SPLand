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
    cout << toString() << endl;

    if (facilitiesOptions.size() == 0) {
        throw std::invalid_argument("Recieved empty vector");
    }
    lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
    return facilitiesOptions[lastSelectedIndex];
}

const string NaiveSelection::toString() const {
    return "Naive selection policy";
}

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
    cout << toString() << endl;

    if (facilitiesOptions.size() == 0) {
        throw std::invalid_argument("Recieved empty vector");
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

const string BalancedSelection::toString() const {
    return "Balanced selection policy";
}

BalancedSelection *BalancedSelection::clone() const {
    return new BalancedSelection(*this);
}

// ECONOMY SELECTION:
EconomySelection::EconomySelection() : lastSelectedIndex(-1) {}

const FacilityType &EconomySelection::selectFacility(
    const vector<FacilityType> &facilitiesOptions) {
    // Selecting the next available facility which is also of "Economy" type
    cout << toString() << endl;

    if (facilitiesOptions.size() == 0) {
        throw std::invalid_argument("Recieved empty vector");
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
        throw std::invalid_argument("No economy facilities in the vector");
    }
    lastSelectedIndex = (i + lastSelectedIndex) % facilitiesOptions.size();
    return facilitiesOptions[lastSelectedIndex];
}

const string EconomySelection::toString() const {
    return "Economy selection policy";
}

EconomySelection *EconomySelection::clone() const {
    return new EconomySelection(*this);
}

// SUSTAINABILITY SELECTION:
SustainabilitySelection::SustainabilitySelection() : lastSelectedIndex(-1) {}

const FacilityType &SustainabilitySelection::selectFacility(
    const vector<FacilityType> &facilitiesOptions) {
    // Selecting the next available facility which is also of "Environment" type
    cout << toString() << endl;

    if (facilitiesOptions.size() == 0) {
        throw std::invalid_argument("Recieved empty vector");
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
        throw std::invalid_argument("No environment facilities in the vector");
    }
    lastSelectedIndex = (i + lastSelectedIndex) % facilitiesOptions.size();
    return facilitiesOptions[lastSelectedIndex];
}

const string SustainabilitySelection::toString() const {
    return "Sustainability selection policy";
}

SustainabilitySelection *SustainabilitySelection::clone() const {
    return new SustainabilitySelection(*this);
}