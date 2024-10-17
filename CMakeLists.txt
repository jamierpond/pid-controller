cmake_minimum_required(VERSION 3.12)
project(PidControllerProject VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard to the latest available
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add executable
add_executable(pid_controller main.cpp)

# Enable warnings
if(MSVC)
    target_compile_options(pid_controller PRIVATE /W4 /WX)
else()
    target_compile_options(pid_controller PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Enable optimizations for Release builds
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

# Set output directory
set_target_properties(pid_controller PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Add include directories if needed
# target_include_directories(pid_controller PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Link libraries if needed
# target_link_libraries(pid_controller PRIVATE some_library)
