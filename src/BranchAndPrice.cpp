#include "BranchAndPrice.h"

#include <vector>

BranchAndPrice::BranchAndPrice(const Instance& instance)
    : instance(instance)
    , masterModel(instance)
    , pricing(instance)
    , cutsSeparator(instance) {}

BranchAndPrice::~BranchAndPrice() {}

void BranchAndPrice::solve() {
    bool hasNewCuts = false;
    do {
        this->cutsSeparator.applyNewCutsTo(this->masterModel);
        this->solveColumnGeneration();
        const auto& x = this->masterModel.getOriginalSolution();
        std::vector<int64_t> branchingSet;
        this->cutsSeparator.getBranchingSet(x, branchingSet);
        // !fixme capacity cuts are not working
        hasNewCuts = this->cutsSeparator.capacityCuts(x);
    } while (hasNewCuts);
}

void BranchAndPrice::solveColumnGeneration() {
    this->startTime = std::chrono::steady_clock::now();

    std::cout << "it   |     obj       | time" << std::endl;

    for (int64_t it = 0; true; it++) {
        this->masterModel.solve();
        const IloNumArray& prices = this->masterModel.getPrices();

        const auto now = std::chrono::steady_clock::now();
        const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - this->startTime).count();
        std::cout << std::format("{:4d} | {:13.2f} | {:6d}", it, this->masterModel.getObjectiveValue(), duration)
                  << std::endl;

        this->pricing.solve(prices);
        const std::vector<std::vector<int64_t> >& qPaths = this->pricing.getSolutionQPaths();

        if (qPaths.empty()) break;

        for (const auto& qPath : qPaths) {
            this->masterModel.addColumn(qPath);
        }
    }

    this->endTime = std::chrono::steady_clock::now();
    const auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(this->endTime - this->startTime).count();

    std::cout << "Objective value: " << this->masterModel.getObjectiveValue() << std::endl;
    std::cout << "Duration: " << duration << " ms" << std::endl;
}
