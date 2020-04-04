target_sources(CMakeAST PRIVATE
    Visitor.hpp
)

target_include_directories(CMakeAST PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

set_property(TARGET CMakeAST APPEND
            PROPERTY PUBLIC_HEADER 
            ${CMAKE_CURRENT_SOURCE_DIR}/Visitor.hpp)