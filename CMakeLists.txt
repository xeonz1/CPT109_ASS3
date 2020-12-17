cmake_minimum_required(VERSION 3.0.0)
project(multi_index VERSION 0.1.0)

add_compile_options("-std=c89")

add_compile_options("--pedantic-errors")

file(GLOB srcfile "*.c" "data_index/*.c" "utilities/*.c" "hotel_struct/*.c" "interface/*.c")
add_executable(multi_index ${srcfile})

target_include_directories(multi_index PUBLIC ${PROJECT_SOURCE_DIR})
target_include_directories(multi_index PUBLIC "${PROJECT_SOURCE_DIR}/hotel_data")
target_include_directories(multi_index PUBLIC "${PROJECT_SOURCE_DIR}/utilities")
target_include_directories(multi_index PUBLIC "${PROJECT_SOURCE_DIR}/data_index")
target_include_directories(multi_index PUBLIC "${PROJECT_SOURCE_DIR}/hotel_struct")
target_include_directories(multi_index PUBLIC "${PROJECT_SOURCE_DIR}/interface")
