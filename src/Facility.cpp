using namespace std;

#include "Facility.h"

FacilityType::FacilityType(const string &name, const FacilityCategory category,
                           const int price, const int lifeQuality_score,
                           const int economy_score, const int environment_score)
    : name(name),
      category(category),
      price(price),
      lifeQuality_score(lifeQuality_score),
      economy_score(economy_score),
      environment_score(environment_score) {}

FacilityType::FacilityType(const FacilityType &other)
    : name(other.name),
      category(other.category),
      price(other.price),
      lifeQuality_score(other.lifeQuality_score),
      economy_score(other.economy_score),
      environment_score(other.environment_score) {}

FacilityType &FacilityType::operator=(const FacilityType &other) {
    return (*this);
}

const string &FacilityType::getName() const { return name; }

FacilityCategory FacilityType::getCategory() const { return category; }

int FacilityType::getCost() const { return price; }

int FacilityType::getLifeQualityScore() const { return environment_score; }

int FacilityType::getEnvironmentScore() const { return environment_score; }

int FacilityType::getEconomyScore() const { return economy_score; }

// now Facility

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
      timeLeft(type.getCost()) {}

Facility::Facility(const Facility &other)
    : FacilityType(other),
      settlementName(other.settlementName),
      status(other.status),
      timeLeft(other.timeLeft) {}

Facility &Facility::operator=(const Facility &other) {
    if (&other != this) {
        status = other.status;
        timeLeft = other.timeLeft;
    }
    return (*this);
}

const string &Facility::getSettlementName() const { return settlementName; }

int Facility::getTimeLeft() const { return timeLeft; }

FacilityStatus Facility::step() {
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
    string categoryString = "Life quality";
    if (category == FacilityCategory::ECONOMY) {
        categoryString = "City";
    } else if (category == FacilityCategory::ENVIRONMENT) {
        categoryString = "Environment";
    }

    return "Facility Name: " + name + ",\nSettlement Name: " + settlementName +
           ",\nCategory: " + categoryString + ",\nPrice: " + to_string(price) +
           ",\nLife quality score: " + to_string(lifeQuality_score) +
           ",\nEconomy score: " + to_string(economy_score) +
           ",\nEnvironment score: " + to_string(environment_score);
}