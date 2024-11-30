#include "SelectionPolicy.h"
#include <stdexcept>
#include <iostream>
#include <cstdint>

// NAIVE SELECTION:
NaiveSelection::NaiveSelection()
    : lastSelectedIndex(-1) {}

const FacilityType &NaiveSelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    if (facilitiesOptions.size() == 0)
    {
        throw std::invalid_argument("Recieved empty vector");
    }
    lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
    return facilitiesOptions[lastSelectedIndex];
}

const string NaiveSelection::toString() const
{
    return "Naive selection policy";
}

NaiveSelection *NaiveSelection::clone() const
{
    return new NaiveSelection(*this);
}

// BALANCED SELECTION:
BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore)
    : LifeQualityScore(LifeQualityScore), EconomyScore(EconomyScore), EnvironmentScore(EnvironmentScore) {}

const FacilityType &BalancedSelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    if (facilitiesOptions.size() == 0)
    {
        throw std::invalid_argument("Recieved empty vector");
    }
    int bestOptionIndex = 0, bestDistance = INT32_MAX;
    for (int i = 0; i < facilitiesOptions.size(); i++)
    {
        // Calculating the distance caused by the current option
        int tempLifeScore = LifeQualityScore + facilitiesOptions[i].getLifeQualityScore();
        int tempEcoScore = EconomyScore + facilitiesOptions[i].getEconomyScore();
        int tempEnvScore = EnvironmentScore + facilitiesOptions[i].getEnvironmentScore();
        int tempDistance = std::max(tempLifeScore, tempEcoScore, tempEnvScore) - std::min(tempLifeScore, tempEcoScore, tempEnvScore);

        // Dictating the next distance for the choice
        if (tempDistance < bestDistance)
        {
            bestDistance = tempDistance;
            bestOptionIndex = i;
        }
    }
    return facilitiesOptions[bestOptionIndex];
}

const string BalancedSelection::toString() const {}

BalancedSelection *BalancedSelection::clone() const {}

// ECONOMY SELECTION:
EconomySelection::EconomySelection()
    : lastSelectedIndex(-1) {}

const FacilityType &EconomySelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    if (facilitiesOptions.size() == 0)
    {
        throw std::invalid_argument("Recieved empty vector");
    }
    lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();

    // Cycling through the vector until an economy FacilityType or full cycle is reached
    int i = 0;
    bool isReachedEconomy = false;
    for (; i < facilitiesOptions.size() && !isReachedEconomy; i++)
    {
        isReachedEconomy = facilitiesOptions[(i + lastSelectedIndex) % facilitiesOptions.size()].getCategory() == FacilityCategory::ECONOMY;
    }

    if (!isReachedEconomy)
    {
        throw std::invalid_argument("No economy facilities in the vector");
    }
    lastSelectedIndex = (i + lastSelectedIndex) % facilitiesOptions.size();
    return facilitiesOptions[lastSelectedIndex];
}

const string EconomySelection::toString() const {}

EconomySelection *EconomySelection::clone() const {}

// SUSTAINABILITY SELECTION:
SustainabilitySelection::SustainabilitySelection()
    : lastSelectedIndex(-1) {}

const FacilityType &SustainabilitySelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    if (facilitiesOptions.size() == 0)
    {
        throw std::invalid_argument("Recieved empty vector");
    }
    lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();

    // Cycling through the vector until an environment FacilityType or full cycle is reached
    int i = 0;
    bool isReachedEnvironment = false;
    for (; i < facilitiesOptions.size() && !isReachedEnvironment; i++)
    {
        isReachedEnvironment = facilitiesOptions[(i + lastSelectedIndex) % facilitiesOptions.size()].getCategory() == FacilityCategory::ENVIRONMENT;
    }

    if (!isReachedEnvironment)
    {
        throw std::invalid_argument("No environment facilities in the vector");
    }
    lastSelectedIndex = (i + lastSelectedIndex) % facilitiesOptions.size();
    return facilitiesOptions[lastSelectedIndex];
}

const string SustainabilitySelection::toString() const
{
}

SustainabilitySelection *SustainabilitySelection::clone() const {}

// we should add clone and tostring functions :)