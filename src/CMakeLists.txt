find_package(CMakeParser REQUIRED)
find_package(polymorphic_value REQUIRED)

add_library(CMakeAST STATIC)

target_sources(CMakeAST PRIVATE
    ArgumentASTBuilder.hpp
    ArgumentASTBuilder.cpp
    ArgumentsASTBuilder.hpp
    ArgumentsASTBuilder.cpp
    BracketCommentASTBuilder.hpp
    BracketCommentASTBuilder.cpp
    CommandInvocationASTBuilder.hpp
    CommandInvocationASTBuilder.cpp
    FileASTBuilder.hpp
    FileASTBuilder.cpp
    FileElementASTBuilder.hpp
    FileElementASTBuilder.cpp
    LineCommentASTBuilder.hpp
    LineCommentASTBuilder.cpp
    ASTBuilder.hpp
    ASTBuilder.cpp
    Context.hpp
    Node.hpp
    NodeFactory.hpp
    NodeFactory.cpp
)

target_include_directories(CMakeAST PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

target_compile_features(CMakeAST PUBLIC cxx_std_17)

target_link_libraries(CMakeAST
  PUBLIC
    CMakeParser::CMakeParser
    polymorphic_value::polymorphic_value
)
