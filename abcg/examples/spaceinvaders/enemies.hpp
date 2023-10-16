#ifndef ENEMIES_HPP_
#define ENEMIES_HPP_

#include <list>
#include <random>

#include "abcgOpenGL.hpp"

#include "gamedata.hpp"
#include "ship.hpp"

class Enemies {
public:
  void create(GLuint program, int quantity);
  void paint(GameData const &gameData);
  void destroy();
  void update(float deltaTime);

  struct Enemy {
    glm::vec4 m_color{1};
    bool m_hit{};
    float m_scale{};
    glm::vec2 m_translation{};
    glm::vec2 m_velocity{};
  };

  std::list<Enemy> m_enemies;

  int qtd_enemies{};

  float m_direction{-1.0f};

  abcg::Timer m_updateY;

  Enemy makeEnemy(glm::vec2 translation = {}, float scale = 0.25f);

private:
  GLuint m_program{};
  GLint m_colorLoc{};
  GLint m_translationLoc{};
  GLint m_scaleLoc{};

  std::default_random_engine m_randomEngine;
  std::uniform_real_distribution<float> m_randomDist{-1.0f, 1.0f};

  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};
};

#endif