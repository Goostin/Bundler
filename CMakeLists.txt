cmake_minimum_required(VERSION 3.21)

project(
    "Product Bundle Optimization Challange"
    LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(bundler
    main.cpp
    src/product.cpp
    src/product_metrics.cpp
    src/purchase.cpp
    src/bundling.cpp
)

target_include_directories(bundler PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_compile_features(bundler PRIVATE cxx_std_17)
if(MSVC)
    target_compile_options(bundler PRIVATE /W4)
else()
    target_compile_options(bundler PRIVATE -Wall -Wextra -Wpedantic)
endif()

install(TARGETS bundler RUNTIME DESTINATION bin)

add_executable(proof_reduce
    tests/proof_reduce.cpp
)

target_compile_features(proof_reduce PRIVATE cxx_std_17)
if(MSVC)
    target_compile_options(proof_reduce PRIVATE /W4)
else()
    target_compile_options(proof_reduce PRIVATE -Wall -Wextra -Wpedantic)
endif()
