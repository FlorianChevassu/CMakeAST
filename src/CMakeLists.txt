find_package(CMakeParser REQUIRED)
find_package(polymorphic_value REQUIRED)

add_library(CMakeAST STATIC)

target_include_directories(CMakeAST PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

target_compile_features(CMakeAST PUBLIC cxx_std_17)

target_link_libraries(CMakeAST
  PUBLIC
    CMakeParser::CMakeParser
    polymorphic_value::polymorphic_value
)

add_subdirectory(ast)
add_subdirectory(builder)
add_subdirectory(visitor)
