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

    // std::cout << "lambdas.size: " << this->masterModel.lambdas.getSize() << std::endl;
    // for (int64_t i = 0; i < this->masterModel.lambdas.getSize(); ++i) {
    //     if (this->masterModel.cplex.getValue(this->masterModel.lambdas[i]) > 1e-6)
    //         std::cout << "lambda[" << i << "]: " << this->masterModel.cplex.getValue(this->masterModel.lambdas[i])
    //                   << std::endl;
    // }
    std::cout << "Objective value: " << this->masterModel.getObjectiveValue() << std::endl;
}
