#include "BranchAndPrice.h"

BranchAndPrice::BranchAndPrice(const Instance& instance)
    : instance(instance)
    , masterModel(instance)
    , pricing(instance) {}

BranchAndPrice::~BranchAndPrice() {}

void BranchAndPrice::solve() {
    while (true) {
        this->masterModel.solve();
        const IloNumArray& prices = this->masterModel.getPrices();

        this->pricing.solve(prices);
        const std::vector<std::vector<int64_t> >& qPaths = this->pricing.getSolutionQPaths();

        if (qPaths.empty()) break;

        for (const auto& qPath : qPaths) {
            this->masterModel.addColumn(qPath);
        }
    }

    std::cout << "Objective value: " << this->masterModel.getObjectiveValue() << std::endl;
}
