cmake_minimum_required(VERSION 3.10)
project(GenericIO)

set(SOURCES
		main.cpp
		examples/single_type_example.cpp
		examples/aggregated_type_example.cpp
		examples/xyz_format_example.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})