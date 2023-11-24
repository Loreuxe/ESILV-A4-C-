#include "BlackScholesMCPricer.h"
#include "BlackScholesPricer.h"
#include <cmath>
#include "stdexcept"
using namespace std;

BlackScholesMCPricer::BlackScholesMCPricer(Option* option, double initial_price, double interest_rate, double volatility) : option_(option), initial_price_(initial_price), interest_rate_(interest_rate), volatility_(volatility) {
    if (option_ == nullptr) { throw invalid_argument("Option must be initialize as it's a pointer"); }
}

vector<double> BlackScholesMCPricer::generate(int nb_paths = 1) {
    double S = initial_price_;
    double K = option_->GetStrike();
    double T = option_->GetExpiry();
    double r = interest_rate_;
    double sigma = volatility_;
    vector<double> t;
    vector<double> prices = { S };

    NbPaths += nb_paths;

    for (int i = 1; i <= nb_paths; i++) {
        t.push_back(i / nb_paths);
    }

    if (option_ -> GetOptionNature() == OptionNature::Vanilla) {
        double z = MT::rand_norm();
        double s = S * exp((r - 0.5 * pow(sigma, 2)) * 1 + sigma * sqrt(1) * z);

        prices.push_back(s);
        return prices;
    }

    else {
        // vector<double> time = AsianOption::getTimeSteps();
        for (int j = 1; j <= nb_paths; j++) {
            double z = MT::rand_norm();
            double s = prices[j - 1] * exp((r - 0.5 * pow(sigma, 2)) * (t[j] - t[j - 1]) + sigma * sqrt(t[j] - t[j - 1]) * z);
            prices.push_back(s);
        }
        return prices; 
    }

    if (option_ -> GetOptionNature() == OptionNature::Vanilla) { current_estimate = exp(-r * T) * option_ -> payoff(prices[1]) / nb_paths; }

    else { current_estimate = exp(-r * T) * option_->payoffPath(prices) / nb_paths; }
}

double BlackScholesMCPricer::operator()() const{
    return current_estimate;
}

std::vector<double> BlackScholesMCPricer::confidenceInterval()
{
    vector<double> ci;

    if (NbPaths == 0)
    {
        throw std::invalid_argument("Paths must be different to 0");
    }

    double mean = 0;
    double std = 0;

    for (int k = 1; k < NbPaths; k++) { //on d�marre � 1 car prices[0] est le prix a t = 0
        mean += prices[k];
    }
    mean = mean / prices.size();

    //On veut l'ecart tye des prix ou la volatilit� ???

    vector<double> rendements;

    for (int i = 2; i < NbPaths; i++)

    {
        rendements.push_back((prices[i] - prices[i - 1]) / prices[i - 1]);
    }


    double ecart_carres = 0;
    for (int i = 0; i < rendements.size(); i++)
    {
        int a = 0;

        a = a + i;

        double moyenne = a / rendements.size();

        double ecart_carres = 0;

        ecart_carres = (moyenne - i) * (moyenne - i);

    }
    double ecartype = std::sqrt(ecart_carres / rendements.size() - 1);


    double lower_bound = mean - 1.96 * (ecartype / std::sqrt(NbPaths));
    double upper_bound = mean + 1.96 * (ecartype / std::sqrt(NbPaths));

    ci = { lower_bound, upper_bound };
    return ci;

}


BlackScholesMCPricer::~BlackScholesMCPricer()
{
    // Release any dynamically allocated memory
    if (option_ != nullptr)
    {
        delete option_;
        option_ = nullptr; // Set the pointer to null to prevent double deletion.
    }
}

int BlackScholesMCPricer::getNbPaths() const{
    return NbPaths;
}