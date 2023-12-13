#include "Option.h"

Option::Option() {}


bool Option::isAmericanOption() { return false; }

bool Option::isAsianOption() { return false; }

double Option::payoffPath(std::vector<double> past_prices) const {
	std::cout << "payoffPath in"<<std::endl;
	double price = 0;
	for (int i = 0; i < past_prices.size(); i++) {
		price += past_prices[i];
	}

	if (price / past_prices.size() >= GetStrike()) {
		return price / past_prices.size() - GetStrike();
	}
	else { return 0.0; }
};

std::vector<double> Option::getTimeSteps() const { return std::vector<double> {0.0}; }
