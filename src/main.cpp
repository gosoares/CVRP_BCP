#include <argparse/argparse.hpp>
#include <iostream>

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

    std::cout << "Instance: " << program.get<std::string>("instance") << std::endl;
    return 0;
}
