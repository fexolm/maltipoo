add_executable(main main.cpp)

find_package(SDL2 CONFIG REQUIRED)

target_link_libraries(main VulkanGPU SDL2::SDL2)