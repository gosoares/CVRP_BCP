#include <argparse/argparse.hpp>
#include <cassert>
#include <format>
#include <iostream>
#include <random>
#include <vector>

#include "Instance.h"
#include "Pricing.h"

int main(int argc, char** argv) {
    argparse::ArgumentParser program("BinPacking");
    program.add_argument("instance").help("Instance file").required();

    try {
        program.parse_args(argc, argv);
    } catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        return 1;
    }

    Instance instance(program.get("instance"));

    std::cout << "V: " << instance.getV() << std::endl;
    std::cout << "Vehicle capacity: " << instance.getVehicleCapacity() << std::endl;

    std::cout << "Distances: " << std::endl;
    for (int64_t i = 0; i < instance.getV(); i++) {
        for (int64_t j = 0; j < instance.getV(); j++) {
            std::cout << instance.getDistance(i, j) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Demands: ";
    for (int64_t i = 0; i < instance.getV(); i++) {
        std::cout << instance.getDemand(i) << " ";
    }
    std::cout << std::endl << std::endl;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(-2.0, 10.0);
    std::vector<std::vector<double> > prices(instance.getV(), std::vector<double>(instance.getV()));
    for (int64_t i = 0; i < instance.getV(); i++) {
        for (int64_t j = i + 1; j < instance.getV(); j++) {
            prices[i][j] = dist(gen);
            prices[j][i] = prices[i][j];
        }
    }
    std::cout << "Prices: " << std::endl;
    for (int64_t i = 0; i < instance.getV(); i++) {
        for (int64_t j = 0; j < instance.getV(); j++) {
            std::cout << std::format("{:4.1f} ", prices[i][j]);
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << std::endl;

    Pricing pricing(instance);
    pricing.solve(prices);

    std::cout << "Min cost: " << std::endl << "    ";
    for (int64_t v = 0; v < instance.getV(); v++) {
        std::cout << std::format("     {:2d}     ", v);
    }
    std::cout << std::endl;
    for (int64_t d = 0; d <= instance.getVehicleCapacity(); d++) {
        std::cout << std::format("{:2d}", d) << ": ";
        for (int64_t v = 0; v < instance.getV(); v++) {
            std::cout << std::format("{{{:4.1f}, {:3d}}}", pricing.getMinCost(d, v), pricing.getBefore(d, v)) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << std::endl;

    for (int64_t d = 0; d <= instance.getVehicleCapacity(); d++) {
        for (int64_t v = 1; v < instance.getV(); v++) {
            double qRouteCost = pricing.getMinCost(d, v) + prices[v][0];
            if (qRouteCost > -1e-6) continue;
            std::cout << std::format(" d={:2d} cost={:5.2f}  | ", d, qRouteCost);
            int64_t u = v, newU;
            int64_t du = d;
            double realCost = prices[v][0];
            do {
                std::cout << u << " ";
                newU = pricing.getBefore(du, u);
                du -= instance.getDemand(u);
                realCost += prices[newU][u];
                u = newU;
            } while (u > 0);
            assert(abs(realCost - qRouteCost) < 1e-6);
            std::cout << std::endl;
        }
    }

    return 0;
}
