cmake_minimum_required(VERSION 3.22)
project(CVRP VERSION 0.1.0 LANGUAGES C CXX)

include_directories("${CPLEX_DIR}/cplex/include" "${CPLEX_DIR}/concert/include")
link_directories("${CPLEX_DIR}/cplex/lib/${CPLEX_ARCH}/static_pic" "${CPLEX_DIR}/concert/lib/${CPLEX_ARCH}/static_pic")

# fetch latest argparse
include(FetchContent)
FetchContent_Declare(
    argparse
    GIT_REPOSITORY https://github.com/p-ranav/argparse.git
)
FetchContent_MakeAvailable(argparse)

add_executable(CVRP src/main.cpp
    src/Instance.h src/Instance.cpp
    src/Pricing.h src/Pricing.cpp
    src/Labels.h src/Labels.cpp
    src/Constraints.h src/Constraints.cpp
    src/MasterModel.h src/MasterModel.cpp
    src/Node.h src/Node.cpp
    src/BranchAndPrice.h src/BranchAndPrice.cpp
    src/CutsSeparator.h src/CutsSeparator.cpp
)

add_subdirectory(cvrpsep)
target_include_directories(CVRP PRIVATE cvrpsep)

target_link_libraries(CVRP ilocplex concert cplex m pthread argparse cvrpsep)
