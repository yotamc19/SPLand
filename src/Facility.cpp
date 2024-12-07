using namespace std;

#include "Facility.h"

#include <iostream>

// FACILITY_TYPE:
FacilityType::FacilityType(const string &name, const FacilityCategory category,
                           const int price, const int lifeQuality_score,
                           const int economy_score, const int environment_score)
    : name(name),
      category(category),
      price(price),
      lifeQuality_score(lifeQuality_score),
      economy_score(economy_score),
      environment_score(environment_score) {}

FacilityType::FacilityType(const FacilityType &other)  // Copy constructor
    : name(other.name),
      category(other.category),
      price(other.price),
      lifeQuality_score(other.lifeQuality_score),
      economy_score(other.economy_score),
      environment_score(other.environment_score) {}

FacilityType &FacilityType::operator=(const FacilityType &other) {
    return (*this);
}
// SettlementName is const, fix?

const string &FacilityType::getName() const { return name; }

FacilityCategory FacilityType::getCategory() const { return category; }

int FacilityType::getCost() const { return price; }

int FacilityType::getLifeQualityScore() const { return lifeQuality_score; }

int FacilityType::getEnvironmentScore() const { return environment_score; }

int FacilityType::getEconomyScore() const { return economy_score; }

// FACILITY:

Facility::Facility(const string &name, const string &settlementName,
                   const FacilityCategory category, const int price,
                   const int lifeQuality_score, const int economy_score,
                   const int environment_score)
    : FacilityType(name, category, price, lifeQuality_score, economy_score,
                   environment_score),
      settlementName(settlementName),
      status(FacilityStatus::UNDER_CONSTRUCTIONS),
      timeLeft(price) {}

Facility::Facility(const FacilityType &type, const string &settlementName)
    : FacilityType(type),
      settlementName(settlementName),
      status(FacilityStatus::UNDER_CONSTRUCTIONS),
      timeLeft(price) {}

Facility::Facility(const Facility &other)  // Copy constructor
    : FacilityType(other),
      settlementName(other.settlementName),
      status(other.status),
      timeLeft(other.timeLeft) {}

Facility &Facility::operator=(
    const Facility &other) {  // Copy assignment operator
    if (&other != this) {
        status = other.status;
        timeLeft = other.timeLeft;
    }
    return (*this);
}
// A lot of const fields, fix?

int Facility::getTimeLeft() const { return timeLeft; }

FacilityStatus Facility::step() {
    // Subtracting from the time left of the facilities under construction
    if (status == FacilityStatus::UNDER_CONSTRUCTIONS) {
        timeLeft--;
        if (timeLeft == 0) {
            status = FacilityStatus::OPERATIONAL;
        }
    }
    return status;
}

void Facility::setStatus(FacilityStatus status) { (*this).status = status; }

const FacilityStatus &Facility::getStatus() const { return status; }

const string Facility::toString() const {
    string stringStatus = "UNDER_CONSTRUCTIONS";
    if (status == FacilityStatus::OPERATIONAL) {
        stringStatus = "OPERATIONAL";
    }

    return "FacilityName: " + name + "\nFacilityStatus: " + stringStatus;
}