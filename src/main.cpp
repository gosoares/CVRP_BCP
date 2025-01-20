#include <ilcplex/ilocplex.h>

#include <argparse/argparse.hpp>
#include <cassert>
#include <format>
#include <iostream>
#include <random>
#include <vector>

#include "Instance.h"
#include "MasterModel.h"
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

    MasterModel master(instance);
    master.solve();

    return 0;
}
