#include "SelectionPolicy.h"

// Naive

NaiveSelection::NaiveSelection()
		: lastSelectedIndex(0) {}

const FacilityType& NaiveSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
	return facilitiesOptions[0];
}

const string NaiveSelection::toString() const {
	return "Naive selection policy";
}

NaiveSelection *NaiveSelection::clone() const {
	return new NaiveSelection(*this);
}

// Balanced

BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore)
		: lifeQualityScore(LifeQualityScore), economyScore(EconomyScore), environmentScore(EnvironmentScore) {}

const FacilityType& BalancedSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
	
}


class BalancedSelection: public SelectionPolicy {
    public:
        BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore);
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        const string toString() const override;
        BalancedSelection *clone() const override;
        ~BalancedSelection() override = default;
    private:
        int LifeQualityScore;
        int EconomyScore;
        int EnvironmentScore;
};