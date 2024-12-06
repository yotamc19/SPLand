#include "Settlement.h"
#include <iostream>

Settlement::Settlement(const string &name, SettlementType type)
    : name(name), type(type) {}

Settlement::Settlement(const Settlement &other)  // Copy constructor
    : name(other.name), type(other.type) {}

Settlement &Settlement::operator=(
    const Settlement &other) {  // Copy assignment operator
    if (&other != this) {
        type = other.type;
    }
    return (*this);
}

const string &Settlement::getName() const { return name; }

SettlementType Settlement::getType() const { return type; }

const string Settlement::toString() const {
    string typeString = "Village";
    if (type == SettlementType::CITY) {
        typeString = "City";
    } else if (type == SettlementType::METROPOLIS) {
        typeString = "Metropolis";
    }
    return "Settlement Name: " + name + ", Type: " + typeString;
}