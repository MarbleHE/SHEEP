####################################################################################
## SHEEP
##
## SHEEP as a static library to compile against
####################################################################################

set(SOURCE_FILES
        circuit.cpp
        circuit-util.cpp
        simple-circuits.cpp)

set(HEADERS
        ${PROJECT_SOURCE_DIR}/include/sheep/all_equal.hpp
        ${PROJECT_SOURCE_DIR}/include/sheep/bits.hpp
        ${PROJECT_SOURCE_DIR}/include/sheep/circuit.hpp
        ${PROJECT_SOURCE_DIR}/include/sheep/circuit-repo.hpp
        ${PROJECT_SOURCE_DIR}/include/sheep/circuit-util.hpp
        ${PROJECT_SOURCE_DIR}/include/sheep/context.hpp
        ${PROJECT_SOURCE_DIR}/include/sheep/context-clear.hpp
        ${PROJECT_SOURCE_DIR}/include/sheep/context-helib.hpp
        ${PROJECT_SOURCE_DIR}/include/sheep/context-lp.hpp
        ${PROJECT_SOURCE_DIR}/include/sheep/context-palisade.hpp
        ${PROJECT_SOURCE_DIR}/include/sheep/context-seal-bfv.hpp
        ${PROJECT_SOURCE_DIR}/include/sheep/context-seal-ckks.hpp
        ${PROJECT_SOURCE_DIR}/include/sheep/context-tfhe.hpp
        ${PROJECT_SOURCE_DIR}/include/sheep/context-tfhe-bool.hpp
        ${PROJECT_SOURCE_DIR}/include/sheep/context-tfhe-common.hpp
        ${PROJECT_SOURCE_DIR}/include/sheep/context-tfhe-integer.hpp
        ${PROJECT_SOURCE_DIR}/include/sheep/protect-eval.hpp
        ${PROJECT_SOURCE_DIR}/include/sheep/sheep-server.hpp
        ${PROJECT_SOURCE_DIR}/include/sheep/simple-circuits.hpp)

add_library(sheep ${SOURCE_FILES})
set_target_properties(sheep PROPERTIES PUBLIC_HEADER ${HEADERS})

add_library(Sheep::sheep ALIAS sheep)
target_include_directories(sheep PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)
target_compile_features(sheep PUBLIC cxx_std_17)

if (HElib_FOUND)
    target_include_directories(sheep SYSTEM PUBLIC ${HElib_INCLUDE_DIRS})
    find_library(NTL_LIBRARIES NAMES libntl.a)
    find_library(GMP_LIBRARIES NAMES gmp)
    target_link_libraries(sheep PUBLIC ${HElib_LIBRARIES} ${NTL_LIBRARIES} ${GMP_LIBRARIES})
    target_compile_definitions(sheep PUBLIC -DHAVE_HElib)
endif(HElib_FOUND)

if (TFHE_FOUND)
    target_include_directories(sheep SYSTEM PUBLIC ${TFHE_INCLUDE_DIRS})
    target_link_libraries(sheep PUBLIC ${TFHE_LIBRARIES})
    target_compile_definitions(sheep PUBLIC -DHAVE_TFHE)
endif(TFHE_FOUND)

if (SEAL_FOUND)
    target_include_directories(sheep SYSTEM PUBLIC ${SEAL_INCLUDE_DIRS})
    target_link_libraries(sheep PUBLIC ${SEAL_LIBRARIES})
    target_compile_definitions(sheep PUBLIC -DHAVE_SEAL_BFV)
    target_compile_definitions(sheep PUBLIC -DHAVE_SEAL_CKKS)
endif(SEAL_FOUND)

if (LP_FOUND)
    target_include_directories(sheep SYSTEM PUBLIC ${LP_INCLUDE_DIRS})
    target_link_libraries(sheep PUBLIC ${LP_LIBRARIES} ${GMP_LIBRARIES})
    target_compile_definitions(sheep PUBLIC  -DHAVE_LP)
endif(LP_FOUND)

if (PALISADE_FOUND)
    target_include_directories(sheep SYSTEM PUBLIC ${PALISADE_INCLUDE_DIRS})
    target_link_libraries(sheep PUBLIC ${PALISADE_LIBRARIES})
    target_compile_definitions(sheep PUBLIC -DHAVE_PALISADE)
endif(PALISADE_FOUND)

if (TBB_FOUND)
    target_include_directories(sheep SYSTEM PUBLIC ${TBB_INCLUDE_DIRS})
    target_link_libraries(sheep PUBLIC ${TBB_LIBRARIES})
    target_compile_definitions(sheep PUBLIC -DHAVE_TBB)
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(sheep PUBLIC Threads::Threads)



####################################################################################
## SHEEP installation
##
## SHEEP installed globally
####################################################################################
# Create a Version File
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        SheepConfigVersion.cmake
        VERSION ${PACKAGE_VERSION}
        COMPATIBILITY AnyNewerVersion
)

# Configure (i.e. generate) the SheepConfig.cmake file and copy both it and the version to install
configure_file(SheepConfig.cmake.in SheepConfig.cmake @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/SheepConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/SheepConfigVersion.cmake"
        DESTINATION lib/cmake/Sheep
        )

install(TARGETS sheep
        EXPORT SheepTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        PUBLIC_HEADER DESTINATION include
        INCLUDES DESTINATION include/sheep)

install(DIRECTORY "../include/"
       DESTINATION "include")

install(EXPORT SheepTargets
        FILE SheepTargets.cmake
        NAMESPACE Sheep::
        DESTINATION lib/cmake/Sheep
        )

