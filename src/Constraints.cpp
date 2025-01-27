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

IloRange Constraints::operator[](int64_t i) const { return this->constraints[i]; }

const std::vector<int64_t>& Constraints::forEdge(int64_t e) const { return this->edgeConstraints[e]; }
