add_executable(triangle main.cpp)

find_package(SDL2 CONFIG REQUIRED)

target_link_libraries(triangle VulkanGPU SDL2::SDL2)