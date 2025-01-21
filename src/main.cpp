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

    const Instance instance(program.get("instance"));
    BranchAndPrice branchAndPrice(instance);
    branchAndPrice.solve();

    return 0;
}
