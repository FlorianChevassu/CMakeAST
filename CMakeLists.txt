cmake_minimum_required(VERSION 3.14)

project(CMakeAST LANGUAGES CXX)

option(BUILD_SUPERBUILD "Build CMakeAST using the Superbuild." ON)
if(BUILD_SUPERBUILD)
  include(cmake/Superbuild.cmake)
  return()
endif()

add_subdirectory(src)

option(BUILD_TESTING "Build tests" ON)
if (BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()

export(TARGETS CMakeAST NAMESPACE CMakeAST:: FILE CMakeASTTargets.cmake)

install(TARGETS CMakeAST
        EXPORT CMakeASTTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

install(EXPORT CMakeASTTargets
        FILE CMakeASTTargets.cmake
        NAMESPACE CMakeAST::
        DESTINATION lib/cmake/CMakeAST
)

configure_file(${CMAKE_CURRENT_LIST_DIR}/cmake/CMakeASTConfig.cmake.in CMakeASTConfig.cmake @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/CMakeASTConfig.cmake"
        DESTINATION lib/cmake/CMakeAST
)
