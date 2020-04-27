cmake_minimum_required (VERSION 3.9...3.17)

# For older versions that do not automatically use highest allowed policy
if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project (Sheep  VERSION 0.1
                DESCRIPTION "SHEEP is a Homomorphic Encryption Evaluation Platform"
                LANGUAGES CXX)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find all (partially optional) dependencies
find_package(HElib)
find_package(TFHE)
find_package(SEAL)
find_package(LP)
find_package(PALISADE)
find_package(TBB)

add_subdirectory(src)
add_subdirectory(applications)
add_subdirectory(tests)