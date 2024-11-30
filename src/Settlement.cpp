#include "Settlement.h"

Settlement::Settlement(const string &name, SettlementType type)
    : name(name), type(type) {}

const string &Settlement::getName() const {
    return name;
}

SettlementType Settlement::getType() const {
    return type;
}

const string Settlement::toString() const {
    string typeString = "Village";
		if (type == SettlementType::CITY) {
			typeString = "City";
		} else if (type == SettlementType::METROPOLIS) {
			typeString = "Metropolis";
		}

    return "Settlement Name: " + name + ", Type: " + typeString;
}
