#include <argparse/argparse.hpp>
#include <iostream>

#include "Instance.h"

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

    return 0;
}
