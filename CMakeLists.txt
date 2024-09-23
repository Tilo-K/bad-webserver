cmake_minimum_required(VERSION 3.10)
project(ServerProject)

# Set compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O3 -flto")

# Add subdirectories for all modules
add_subdirectory(sc/socket)
add_subdirectory(sc/thread)
add_subdirectory(sc/mutex)
add_subdirectory(sc/queue)

# Add the executable for your project
add_executable(server server.c)

# Link the libraries created in the subdirectories
target_link_libraries(server PRIVATE sc_socket sc_thread sc_mutex sc_queue)

