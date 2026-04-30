file(GLOB module_files "${CMAKE_CURRENT_SOURCE_DIR}/*.cppm")
add_library(module STATIC)

target_sources(module
        PUBLIC
        FILE_SET cxx_modules TYPE CXX_MODULES FILES
        ${module_files}
)