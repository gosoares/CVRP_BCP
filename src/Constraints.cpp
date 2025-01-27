#include "Constraints.h"

Constraints::Constraints(IloEnv& env, int64_t nEdges)
    : constraints()
    , edgeConstraints(nEdges, std::vector<int64_t>()) {}

Constraints::~Constraints() { constraints.end(); }

void Constraints::add(const IloRange& constraint, const std::vector<int64_t>& edges) {
    int64_t consId = this->constraints.size();
    this->constraints.push_back(constraint);

    for (int64_t e : edges) {
        this->edgeConstraints[e].push_back(consId);
    }
}

void Constraints::addColumn(const IloNumVar& lambda, const std::map<int64_t, int64_t>& q) {
    // q[e] = number of times edge e appears in the route
    std::map<int64_t, int64_t> constraintCoef;

    for (const auto& el : q) {
        for (const auto& c : this->forEdge(el.first)) {
            constraintCoef.try_emplace(c.id(), 0);
            constraintCoef[c.id()] += el.second;
        }
    }

    for (const auto& el : constraintCoef) {
        this->constraints[el.first].setLinearCoef(lambda, el.second);
    }
}

void Constraints::clearFrom(IloModel& model) {
    for (int64_t e = 0; e < this->edgeConstraints.size(); e++) {
        this->edgeConstraints[e].clear();
    }
    for (IloRange& constraint : this->constraints) {
        model.remove(constraint);
        constraint.end();
    }
    this->constraints.clear();
}

IloRange& Constraints::operator[](int64_t i) { return this->constraints[i]; }

const IloRange Constraints::operator[](int64_t i) const { return this->constraints[i]; }

Constraints::EdgeConstraintsIterator Constraints::forEdge(int64_t e) { return EdgeConstraintsIterator(*this, e); }

/**** EdgeConstraintsIterator ****/

Constraints::EdgeConstraintsIterator::EdgeConstraintsIterator(Constraints& constraints, int64_t edge, int64_t i)
    : constraints(constraints)
    , i(i)
    , edge(edge) {}

int64_t Constraints::EdgeConstraintsIterator::id() const {
    return this->constraints.edgeConstraints[this->edge][this->i];
}

const IloRange& Constraints::EdgeConstraintsIterator::constraint() const {
    return this->constraints.constraints[this->id()];
}

const Constraints::EdgeConstraintsIterator& Constraints::EdgeConstraintsIterator::operator*() const { return *this; }

const Constraints::EdgeConstraintsIterator& Constraints::Constraints::EdgeConstraintsIterator::operator++() {
    this->i++;
    return *this;
}

bool Constraints::EdgeConstraintsIterator::operator!=(const EdgeConstraintsIterator& other) {
    return this->edge == other.edge && this->i != other.i;
}

Constraints::EdgeConstraintsIterator Constraints::Constraints::EdgeConstraintsIterator::begin() {
    return Constraints::EdgeConstraintsIterator(this->constraints, this->edge, 0);
}

Constraints::EdgeConstraintsIterator Constraints::EdgeConstraintsIterator::end() {
    return EdgeConstraintsIterator(this->constraints, this->edge, constraints.edgeConstraints[edge].size());
}
