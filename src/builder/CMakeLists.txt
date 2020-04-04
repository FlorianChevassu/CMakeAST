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
    LineCommentASTBuilder.hpp
    LineCommentASTBuilder.cpp
    ASTBuilder.hpp
    ASTBuilder.cpp
)

target_include_directories(CMakeAST PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

set_property(TARGET CMakeAST APPEND
            PROPERTY PUBLIC_HEADER 
            ${CMAKE_CURRENT_SOURCE_DIR}/ArgumentASTBuilder.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/ArgumentsASTBuilder.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/BracketCommentASTBuilder.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/CommandInvocationASTBuilder.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/FileASTBuilder.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/LineCommentASTBuilder.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/ASTBuilder.hpp)
            