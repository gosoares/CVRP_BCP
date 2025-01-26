#include "Node.h"

Node::Node()
    : id(0)
    , depth(0)
    , constraints()
    , status(UNPROCESSED)
    , lowerBound(-std::numeric_limits<double>::infinity())
    , objectiveValue(std::numeric_limits<double>::infinity()) {}

Node::Node(int64_t id, const Node& parent, const BranchConstraint& constraint)
    : id(id)
    , depth(parent.depth + 1)
    , constraints(parent.constraints)
    , status(UNPROCESSED)
    , lowerBound(-std::numeric_limits<double>::infinity())
    , objectiveValue(std::numeric_limits<double>::infinity()) {
    constraints.push_back(constraint);
}

Node::~Node() {}

int64_t Node::getId() const { return this->id; };
int64_t Node::getDepth() const { return this->depth; };
bool Node::isRoot() const { return this->depth == 0; };

double Node::getLowerBound() const { return lowerBound; };
double Node::getObjectiveValue() const { return objectiveValue; };
void Node::setLowerBound(double lowerBound) { this->lowerBound = lowerBound; };
void Node::setObjectiveValue(double objectiveValue) { this->objectiveValue = objectiveValue; };

bool Node::isSolved() const { return this->status != UNPROCESSED; };
bool Node::isIntegral() const { return this->status == INTEGRAL; };
bool Node::isInfeasible() const { return this->status == INFEASIBLE; };

void Node::setBranched() { this->status = BRANCHED; };
void Node::setIntegral() { this->status = INTEGRAL; };
void Node::setInfeasible() { this->status = INFEASIBLE; };
