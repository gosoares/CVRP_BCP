#include "Constraints.h"

Constraints::Constraints(IloEnv& env, int64_t nEdges)
    : constraints(env)
    , edgeConstraints(nEdges, std::vector<int64_t>()) {}

Constraints::~Constraints() { constraints.end(); }

void Constraints::add(const IloRange& constraint, const std::vector<int64_t>& edges) {
    int64_t consId = this->constraints.getSize();
    this->constraints.add(constraint);

    for (int64_t e : edges) {
        this->edgeConstraints[e].push_back(consId);
    }
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
