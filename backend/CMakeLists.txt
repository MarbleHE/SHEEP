cmake_minimum_required (VERSION 3.5)
project (Sheep)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

enable_testing()

include_directories(include)

# Flags for profiling
# set (CMAKE_C_FLAGS " -pg ")
# set (CMAKE_CXX_FLAGS " -pg ")

find_package(LP)
if (LP_FOUND)
  include_directories(SYSTEM ${LP_INCLUDE_DIRS})
  find_library(GMP_LIBRARIES NAMES gmp)
  add_definitions(-DHAVE_LP)
endif (LP_FOUND)

find_package(TFHE)
if (TFHE_FOUND)
    include_directories(SYSTEM ${TFHE_INCLUDE_DIRS})
    add_definitions(-DHAVE_TFHE)
endif (TFHE_FOUND)

find_package(HElib)
if (HElib_FOUND)
   include_directories(SYSTEM ${HElib_INCLUDE_DIRS})
   find_library(NTL_LIBRARIES NAMES libntl.a)
   find_library(GMP_LIBRARIES NAMES gmp)
   add_definitions(-DHAVE_HElib)
endif (HElib_FOUND)

find_package(TBB)
if (TBB_FOUND)
  link_libraries(tbb)
  include_directories(SYSTEM ${TBB_INCLUDE_DIRS})
  add_definitions(-DHAVE_TBB)
endif()

find_package(SEAL)
if (SEAL_FOUND)
  include_directories(SYSTEM ${SEAL_INCLUDE_DIRS})
  add_definitions(-DHAVE_SEAL_BFV)
  add_definitions(-DHAVE_SEAL_CKKS)
endif()

find_package(PALISADE)
if (PALISADE_FOUND)
  include_directories(SYSTEM ${PALISADE_INCLUDE_DIRS})
  link_libraries(PALISADEcore)
  link_libraries(PALISADEpke)
  add_definitions(-DHAVE_PALISADE)
endif()

link_libraries(pthread)


####################################################################################
## SHEEP
##
## SHEEP as a static library to compile against
####################################################################################

add_library(sheep
        ${Sheep_SOURCE_DIR}/src/circuit.cpp
        ${Sheep_SOURCE_DIR}/src/circuit-util.cpp
        ${Sheep_SOURCE_DIR}/src/simple-circuits.cpp)

if (HElib_FOUND)
    target_link_libraries(sheep ${HElib_LIBRARIES} ${NTL_LIBRARIES} ${GMP_LIBRARIES})
endif(HElib_FOUND)

if (TFHE_FOUND)
    target_link_libraries(sheep ${TFHE_LIBRARIES})
endif(TFHE_FOUND)

if (SEAL_FOUND)
    target_link_libraries(sheep ${SEAL_LIBRARIES})
endif(SEAL_FOUND)

if (LP_FOUND)
    target_link_libraries(sheep ${LP_LIBRARIES} ${GMP_LIBRARIES})
endif(LP_FOUND)

if (PALISADE_FOUND)
    target_link_libraries(sheep ${PALISADE_LIBRARIES})
endif(PALISADE_FOUND)

if (TBB_FOUND)
    target_link_libraries(sheep ${TBB_LIBRARIES})
endif(TBB_FOUND)

add_subdirectory(applications)
add_subdirectory(tests)
