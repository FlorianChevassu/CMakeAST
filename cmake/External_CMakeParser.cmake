ExternalProject_Add(CMakeParser
  GIT_REPOSITORY https://github.com/FlorianChevassu/CMakeParser.git
  GIT_TAG master
  INSTALL_COMMAND "" # installation is done via the superbuild.
  CMAKE_CACHE_ARGS
    -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/CMakeParser-install
    -DBUILD_TESTING:BOOL=OFF
)
set(CMakeParser_DIR ${CMAKE_CURRENT_BINARY_DIR}/CMakeParser-install/lib/cmake/CMakeParser)
