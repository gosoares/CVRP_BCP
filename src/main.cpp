#include <argparse/argparse.hpp>

#include "BranchAndPrice.h"
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

    std::string instanceFile = program.get("instance");
    size_t slashPos = instanceFile.find_last_of('/');
    slashPos = slashPos == std::string::npos ? 0 : slashPos + 1;

    std::cout << "Solving: " << instanceFile.substr(slashPos, instanceFile.find_last_of(".") - slashPos) << std::endl
              << std::endl;

    const Instance instance(instanceFile);
    BranchAndPrice branchAndPrice(instance);
    branchAndPrice.solve();

    return 0;
}
