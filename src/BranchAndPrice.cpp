#include "BranchAndPrice.h"

#include <format>
#include <vector>

BranchAndPrice::BranchAndPrice(const Instance& instance)
    : instance(instance)
    , masterModel(instance)
    , pricing(instance)
    , cutsSeparator(instance)
    , upperBound(std::numeric_limits<double>::infinity())
    , bestSolution()
    , nSolvedNodes(0)
    , nextNodeId(1) {}

BranchAndPrice::~BranchAndPrice() {}

void BranchAndPrice::solve() {
    this->startTime = std::chrono::steady_clock::now();

    std::vector<Node*> openNodes;
    openNodes.push_back(new Node());  // root node

    std::cout << " Current Node | Solution Value  |    Nodes       |\n";
    std::cout << "   id | depth |   obj   |  best | solved |  open |  time\n";
    std::cout << "----------------------------------------------------\n";

    while (!openNodes.empty()) {
        Node* node = openNodes.back();
        openNodes.pop_back();

        if (ceil(node->getLowerBound() - 1e-6) >= this->upperBound - 1e-6) {
            delete node;  // node cannot lead to a better solution
            continue;
        }

        const std::vector<double>& solution = this->solveNode(*node);
        this->nSolvedNodes++;

        const bool boundPruned = ceil(node->getLowerBound() - 1e-6) >= this->upperBound - 1e-6;
        this->printLogLine(*node, openNodes.size(), boundPruned);

        if (node->isInfeasible()) {
            delete node;
            continue;  // pruned by infeasibility
        };

        if (boundPruned) {
            delete node;
            continue;  // pruned by bound
        }

        if (node->isIntegral()) {
            this->upperBound = node->getObjectiveValue();
            this->bestSolution = solution;
            delete node;
            continue;  // pruned by integrality
        }

        const std::vector<int64_t>& branchingSet = this->cutsSeparator.getBranchingSet(solution);
        const std::vector<int64_t> cutSet = this->instance.getCutSet(branchingSet);

        openNodes.push_back(new Node(this->nextNodeId++, *node, BranchConstraint{cutSet, BD_RIGHT}));
        openNodes.push_back(new Node(this->nextNodeId++, *node, BranchConstraint{cutSet, BD_LEFT}));
        delete node;
    }
}

const std::vector<double>& BranchAndPrice::solveNode(Node& node) {
    this->masterModel.applyBranchConstraints(node.getConstraints());

    bool hasNewCuts = false;
    const std::vector<double>* x;
    do {
        this->cutsSeparator.applyNewCutsTo(this->masterModel);
        this->solveColumnGeneration(node);
        x = &(this->masterModel.getOriginalSolution());

        if (node.isIntegral() || node.isInfeasible()) break;

        hasNewCuts = this->cutsSeparator.capacityCuts(*x);
    } while (hasNewCuts);

    if (!node.isIntegral() && !node.isInfeasible()) node.setBranched();

    this->masterModel.clearBranchConstraints();

    return *x;
}

void BranchAndPrice::solveColumnGeneration(Node& node) {
    double reducedCost = 0;
    for (int64_t it = 0; true; it++) {
        this->masterModel.solve();

        const IloNumArray& prices = this->masterModel.getPrices();
        reducedCost = this->pricing.solve(prices);
        const std::vector<std::vector<int64_t> >& qPaths = this->pricing.getSolutionQPaths();

        if (qPaths.empty()) break;

        for (const auto& qPath : qPaths) {
            this->masterModel.addColumn(qPath);
        }
    }

    if (this->masterModel.isInfeasible()) {
        node.setInfeasible();
        return;
    }

    node.setObjectiveValue(this->masterModel.getObjectiveValue());
    node.setLowerBound(this->masterModel.getObjectiveValue() + reducedCost);
    if (this->isIntegral(this->masterModel.getSolution())) node.setIntegral();
}

bool BranchAndPrice::isIntegral(const std::vector<double>& x) const {
    for (int64_t e = 0; e < x.size(); e++) {
        if (std::abs(x[e] - std::round(x[e])) > 1e-6) return false;
    }
    return true;
}

void BranchAndPrice::printLogLine(const Node& node, int64_t nOpenNodes, bool boundPruned) const {
    const auto now = std::chrono::steady_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - this->startTime).count();
    const auto prefix = node.isIntegral() ? "*" : node.isInfeasible() ? "x" : boundPruned ? "p" : " ";
    std::string best;
    if (this->upperBound == std::numeric_limits<double>::infinity()) {
        best = " ----";
    } else {
        const int64_t upperBound = std::round(this->upperBound);
        best = std::format("{:5d}", upperBound);
    }
    std::cout << std::format(
        "{:s}{:4d} | {:5d} | {:7.2f} | {:s} | {:6d} | {:4d} | {:6d}\n",
        prefix,
        node.getId(),
        node.getDepth(),
        node.getObjectiveValue(),
        best,
        this->nSolvedNodes,
        nOpenNodes,
        duration
    );
}
