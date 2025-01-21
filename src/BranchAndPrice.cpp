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

        std::cout << "Prices: ";
        for (int64_t e = 0; e < this->instance.getNbEdges(); e++) {
            std::cout << prices[e] << " ";
        }
        std::cout << std::endl << std::endl;

        this->pricing.solve(prices);
        const std::vector<std::vector<int64_t> >& qPaths = this->pricing.getSolutionQPaths();

        // std::cout << "QPaths: " << std::endl;
        // for (const auto& qPath : qPaths) {
        //     for (int64_t e : qPath) {
        //         std::cout << e << " ";
        //     }
        //     std::cout << std::endl;
        // }
        // std::cout << std::endl << std::endl << std::endl;

        if (qPaths.empty()) break;

        for (const auto& qPath : qPaths) {
            this->masterModel.addColumn(qPath);
        }
    }

    std::cout << "Objective value: " << this->masterModel.getObjectiveValue() << std::endl;
}
