#include "MasterModel.h"

#include <format>
#include <map>
#include <string>

#define artificialCost 9999999

IloNumVar& MasterModel::newArtificial() {
    IloNumVar a(this->objective(artificialCost));
    a.setName(std::format("a_{}", this->artificials.getSize() + 1).c_str());
    this->artificials.add(a);
    return this->artificials[this->artificials.getSize() - 1];
}

MasterModel::MasterModel(const Instance& instance)
    : instance(instance)
    , env()
    , model(env)
    , cplex(model)
    , objective(IloAdd(model, IloMinimize(env)))
    , constraints(env)
    , edgeConstraints(instance.getNbEdges(), std::vector<int64_t>())
    , lambdas(env)
    , edgeLambdas(instance.getNbEdges(), std::vector<int64_t>())
    , prices(env, instance.getNbEdges())
    , artificials(env)
    , solution() {
    this->cplex.setOut(env.getNullStream());
    this->cplex.setParam(IloCplex::Param::Threads, 1);

    // ensures that each client is visited exactly once
    for (int64_t v : this->instance.getClientIdxs()) {
        this->constraints.add(IloAdd(this->model, this->newArtificial() == 2));

        int64_t consId = this->constraints.getSize() - 1;
        for (int64_t w : this->instance.getVertexIdxs()) {
            if (v == w) continue;
            this->edgeConstraints[instance.getEdgeId(v, w)].push_back(consId);
        }
    }

    // ensures that K vehicles must leave and enter the depot
    this->constraints.add(IloAdd(this->model, this->newArtificial() == 2 * instance.getNbVehicles()));
    int64_t consId = this->constraints.getSize() - 1;
    for (int64_t w : this->instance.getClientIdxs()) {
        this->edgeConstraints[instance.getEdgeId(0, w)].push_back(consId);
    }

    // ensures that each edge that do not go through the depot is visited at most once
    for (int64_t e : this->instance.getEdgeIdxs()) {
        std::pair<int64_t, int64_t> edge = instance.getEdge(e);
        if (edge.first == 0 || edge.second == 0) continue;

        this->constraints.add(IloAdd(this->model, this->newArtificial() <= 1));
        int64_t consId = this->constraints.getSize() - 1;
        this->edgeConstraints[e].push_back(consId);
    }
}

MasterModel::~MasterModel() {
    model.end();
    cplex.end();
    objective.end();
    constraints.end();
    lambdas.end();
    prices.end();
    artificials.end();
}

void MasterModel::solve() { this->cplex.solve(); }

void MasterModel::addColumn(const std::vector<int64_t>& qRouteEdges) {  // TODO need/can i optimize it?
    int64_t objCoef = 0;
    std::map<int64_t, int64_t> edgesCount;
    for (int64_t e : qRouteEdges) {
        objCoef += this->instance.getDistance(e);

        edgesCount.try_emplace(e, 0);
        edgesCount[e]++;
    }

    int64_t lambdaIdx = this->lambdas.getSize();
    IloNumVar lambda(objective(objCoef));
    lambda.setName(std::format("l_{}", lambdaIdx).c_str());
    this->lambdas.add(lambda);

    std::map<int64_t, int64_t> constraintCoef;

    for (const auto& el : edgesCount) {
        this->edgeLambdas[el.first].push_back(lambdaIdx);
        for (auto c : this->edgeConstraints[el.first]) {
            constraintCoef.try_emplace(c, 0);
            constraintCoef[c] += el.second;
        }
    }

    for (const auto& el : constraintCoef) {
        this->constraints[el.first].setLinearCoef(lambda, el.second);
    }
}

const std::vector<double>& MasterModel::getSolution() {
    this->solution.resize(this->lambdas.getSize());
    for (int64_t i = 0; i < this->lambdas.getSize(); ++i) {
        this->solution[i] = this->cplex.getValue(this->lambdas[i]);
    }
    return solution;
}

const IloNumArray& MasterModel::getPrices() {
    for (int64_t e = 0; e < this->instance.getNbEdges(); e++) {
        prices[e] = instance.getDistance(e);

        for (int64_t c : this->edgeConstraints[e]) {
            prices[e] -= cplex.getDual(constraints[c]);
        }
    }

    return prices;
}

double MasterModel::getObjectiveValue() const { return this->cplex.getValue(this->objective); }

bool MasterModel::isInfeasible() const {
    if (this->cplex.getStatus() == IloAlgorithm::Infeasible) return true;
    for (int i = 0; i < this->artificials.getSize(); i++) {
        if (this->cplex.getValue(this->artificials[i]) > 1e-6) return true;
    }
    return false;
}
