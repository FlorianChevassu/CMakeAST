find_package(Catch2 REQUIRED)

add_executable(UnitTests
  main.cpp
  ArgumentASTBuilder_tests.cpp
  ArgumentsASTBuilder_tests.cpp
  ASTBuilder_tests.cpp
  BracketCommentASTBuilder_tests.cpp
  CommandInvocationASTBuilder_tests.cpp
  FileASTBuilder_tests.cpp
  LineCommentASTBuilder_tests.cpp
  Visitor_tests.cpp
)
target_link_libraries(UnitTests PRIVATE CMakeAST Catch2::Catch2)
