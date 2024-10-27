# Define the library target
add_library(glad STATIC
    glad.c
    glad.h
)

# Specify include directories for the library
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
