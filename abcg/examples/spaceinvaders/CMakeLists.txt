project(spaceinvaders)
add_executable(${PROJECT_NAME} main.cpp window.cpp enemies.cpp bullets.cpp
                               ship.cpp)
enable_abcg(${PROJECT_NAME})