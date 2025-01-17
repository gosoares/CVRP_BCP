#include "MasterModel.h"

#include <format>
#include <string>

EdgeConstraints::EdgeConstraints(const IloCplex& cplex)
    : cplex(cplex)
    , constraints(cplex.getEnv())
    , duals(cplex.getEnv()) {}

void EdgeConstraints::add(const IloRange& constraint) {
    this->constraints.add(constraint);
    this->duals.add(0);
}

const IloNumArray& EdgeConstraints::getDuals() const {
    this->cplex.getDuals(this->duals, this->constraints);
    return this->duals;
}

MasterModel::MasterModel(IloEnv& env, const Instance& instance)
    : instance(instance)
    , edgesLength(env, instance.getNbEdges())
    , env(env)
    , model(env)
    , cplex(model)
    , objective(IloAdd(model, IloMinimize(env)))
    , visitedOnceConstraints(IloAdd(model, IloRangeArray(env, instance.getN(), 2, 2)))
    , nbVehiclesConstraint(IloAdd(model, IloRange(env, 2 * instance.getNbVehicles(), 2 * instance.getNbVehicles())))
    , capacityConstraints(IloAdd(model, IloRangeArray(env)))
    , maxTraversalConstraints(
          IloAdd(model, IloRangeArray(env, instance.getNbEdges() - instance.getN(), -IloInfinity, 1)))
    , lambdas(env)
    , prices(env, instance.getNbEdges())
    , artificials(env, 2, 0, IloInfinity)
    , solution()
    , edgeConstraints(instance.getNbEdges(), EdgeConstraints(this->cplex)) {
    this->cplex.setOut(env.getNullStream());
    this->cplex.setParam(IloCplex::Param::Threads, 1);

    // create artificial variables
    artificials[0].setName("a_1");
    for (int64_t i = 0; i < instance.getN(); ++i) {
        this->visitedOnceConstraints[i].setLinearCoef(artificials[0], 2);
    }
    artificials[1].setName("a_2");
    this->nbVehiclesConstraint.setLinearCoef(artificials[1], 2);

    // populate edgesLength
    for (int64_t e = 0; e < instance.getNbEdges(); e++) {
        edgesLength[e] = instance.getDistance(e);
    }

    // populate edgeConstraints
    int64_t e = 0;  // edge id

    // depot edges {0, j}
    for (int64_t j = 1; j < this->instance.getV(); j++) {
        this->edgeConstraints[e].add(this->visitedOnceConstraints[j - 1]);
        this->edgeConstraints[e].add(this->nbVehiclesConstraint);
        e++;
    }

    // customer edges {i, j}
    int64_t maxTraversalCId = 0;
    for (int64_t i = 1; i < this->instance.getV(); i++) {
        for (int64_t j = i + 1; j < this->instance.getV(); j++) {
            this->edgeConstraints[e].add(this->visitedOnceConstraints[i - 1]);
            this->edgeConstraints[e].add(this->visitedOnceConstraints[j - 1]);
            this->edgeConstraints[e].add(this->maxTraversalConstraints[maxTraversalCId++]);
            e++;
        }
    }
}

MasterModel::~MasterModel() {
    cplex.end();
    model.end();
    objective.end();
    visitedOnceConstraints.end();
    nbVehiclesConstraint.end();
    capacityConstraints.end();
    maxTraversalConstraints.end();
    lambdas.end();
    prices.end();
    artificials.end();
}

void MasterModel::solve() { this->cplex.solve(); }

void MasterModel::addColumn(const IloNumArray& qRoute) {
    IloNumVar lambda(objective(IloScalProd(this->edgesLength, qRoute)));
    lambda.setName(std::format("l_{}", this->lambdas.getSize()).c_str());
    // TODO
    // IloNumVar lambda(objective(1) + constraints(column));
    // lambda.setName(std::format("l_{}", this->lambdas.getSize()).c_str());
    // this->lambdas.add(lambda);
}

const std::vector<double>& MasterModel::getSolution() {
    this->solution.resize(this->lambdas.getSize());
    for (int i = 0; i < this->lambdas.getSize(); ++i) {
        this->solution[i] = this->cplex.getValue(this->lambdas[i]);
    }
    return solution;
}

const IloNumArray& MasterModel::getPrices() {
    for (int e = 0; e < this->instance.getNbEdges(); e++) {
        prices[e] = instance.getDistance(e);
        const IloNumArray& duals = this->edgeConstraints[e].getDuals();
        for (int i = 0; i < duals.getSize(); i++) {
            prices[e] -= duals[i];
        }
    }
}

double MasterModel::getObjectiveValue() const { return this->cplex.getValue(this->objective); }

bool MasterModel::isInfeasible() const {
    return this->cplex.getStatus() == IloAlgorithm::Infeasible || this->cplex.getValue(this->artificials[0]) > 1e-6 ||
           this->cplex.getValue(this->artificials[1]) > 1e-6;
}
