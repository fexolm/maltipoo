find_package(SDL2 CONFIG REQUIRED)

add_executable(teapot main.cpp)

target_link_libraries(teapot VulkanGPU SDL2::SDL2)
