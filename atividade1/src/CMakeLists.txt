project(a1)
add_executable(${PROJECT_NAME} main.cpp openglwindow.cpp enemies.cpp
                               bullets.cpp ship.cpp)
enable_abcg(${PROJECT_NAME})