find_package(Catch2 REQUIRED)

add_executable(UnitTests
  main.cpp
  ASTBuilder_tests.cpp
)
target_link_libraries(UnitTests PRIVATE CMakeAST Catch2::Catch2)