cmake_minimum_required(VERSION 3.22)    # Minimum cmake version required

project(ATPGK)    # Name of the project

set(CMAKE_CXX_STANDARD 17)    # Define the C++ standard version

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_BINARY_DIR}/conan_toolchain.cmake)
set(CMAKE_POLICY_DEFAULT_CMP0091 NEW)
set(PROJECT_ROOT_PATH ${CMAKE_SOURCE_DIR} CACHE STRING "Path of the root directory of the project")

configure_file(config.h.in config.h)

include_directories(include)    # Defining the include directory
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include(${CMAKE_TOOLCHAIN_FILE})    # Including the conan built cmake toolchain

enable_testing()

# ---------------------------------------------
# ------ Define the dependency libraries ------
# ---------------------------------------------

find_package(nlohmann_json REQUIRED)
find_package(Boost REQUIRED COMPONENT program_options)
find_package(GTest REQUIRED)

# ---------------------------------------------
# -------- Declare and link libraries ---------
# ---------------------------------------------

# --------------- CIRCUIT_TREE ----------------
file(GLOB CIRCUIT_TREE_SRC "src/tree/*.cpp")
file(GLOB YOSYS_GATE_SRC "src/tree/Yosys/*.cpp")
add_library(CIRCUIT_TREE SHARED ${CIRCUIT_TREE_SRC} ${YOSYS_GATE_SRC})

# ----------------- PARSERS -------------------
set(PARSER_SRC_PATH src/parser)
set(PARSER_BASIC_SRC ${PARSER_SRC_PATH}/gate.cpp ${PARSER_SRC_PATH}/parsed_circuit.cpp ${PARSER_SRC_PATH}/parser.cpp)

set(PARSER_JSON_SRC ${PARSER_BASIC_SRC} ${PARSER_SRC_PATH}/yosys_json_parser.cpp)
add_library(PARSER_JSON SHARED ${PARSER_JSON_SRC})
target_link_libraries(PARSER_JSON PUBLIC nlohmann_json::nlohmann_json)

# -------------- BUILDER API ------------------
add_library(BUILDER_API SHARED src/builder_API/builder_API.cpp)

# --------------- FAULT API -------------------
add_library(FAULT_API SHARED src/fault_API/fault_API.cpp)
target_link_libraries(FAULT_API PUBLIC BUILDER_API CIRCUIT_TREE)

# ---------------- READER ---------------------
add_library(READER SHARED src/reader/reader.cpp)
target_link_libraries(READER PUBLIC BUILDER_API PARSER_JSON CIRCUIT_TREE)

# ---------------- WRITER ---------------------
add_library(WRITER_TXT SHARED src/writer/writer.cpp src/writer/writer_txt.cpp)
add_library(WRITER_JSON SHARED src/writer/writer.cpp src/writer/writer_json.cpp)

# --------------- TOP_LEVEL -------------------
add_library(TOP_LEVEL SHARED src/atpg_top/atpg_top.cpp)
target_link_libraries(TOP_LEVEL PUBLIC READER CIRCUIT_TREE WRITER_TXT WRITER_JSON FAULT_API nlohmann_json::nlohmann_json)

# ---------------------------------------------
# ------- Declare and link main target --------
# ---------------------------------------------

add_executable(${PROJECT_NAME} src/main.cpp)    # Define the main executable target
target_link_libraries(${PROJECT_NAME} PUBLIC nlohmann_json::nlohmann_json Boost::program_options TOP_LEVEL)    # Link library to the main target

# ---------------------------------------------
# ------- Declare and link test targets -------
# ---------------------------------------------


set(TEST_SOURCES test/test_main.cpp test/test_yosys_json_parser.cpp)
add_executable(Test-ATPGK ${TEST_SOURCES})
target_link_libraries(Test-ATPGK PRIVATE GTest::gtest GTest::gtest_main nlohmann_json::nlohmann_json Boost::program_options PARSER_JSON CIRCUIT_TREE)
include(GoogleTest)
gtest_discover_tests(Test-ATPGK)