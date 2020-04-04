target_sources(CMakeAST PRIVATE
    Context.hpp
    Node.hpp
    Node.tpp
    NodeFactory.hpp
    NodeFactory.cpp
)

target_include_directories(CMakeAST PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

set_property(TARGET CMakeAST APPEND
            PROPERTY PUBLIC_HEADER 
            ${CMAKE_CURRENT_SOURCE_DIR}/Context.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/Node.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/Node.tpp)