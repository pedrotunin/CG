#include "enemies.hpp"

#include <cppitertools/itertools.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

void Enemies::initializeGL(GLuint program, int quantity) {
  terminateGL();

  // Start pseudo-random number generator
  m_randomEngine.seed(
      std::chrono::steady_clock::now().time_since_epoch().count());

  m_program = program;
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  // Create enemies
  m_enemies.clear();
  m_enemies.resize(quantity);

  const float spaceX = 0.18f;
  float spaceY = 0.0f;

  int i = 1;

  for (auto &enemy : m_enemies) {
    enemy = createEnemy();
    enemy.m_translation = {-1.0f + spaceX * i, 1.0f - spaceY};
    if (i % 10 == 0) {
        i = 0;
        spaceY += 0.18f;
    }
    i++;
  }
}

void Enemies::paintGL() {
  abcg::glUseProgram(m_program);

  for (const auto &enemy : m_enemies) {
    abcg::glBindVertexArray(enemy.m_vao);

    abcg::glUniform4fv(m_colorLoc, 1, &enemy.m_color.r);
    abcg::glUniform1f(m_scaleLoc, enemy.m_scale);

    abcg::glUniform2f(m_translationLoc, enemy.m_translation.x,
                          enemy.m_translation.y);

    abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, enemy.m_polygonSides + 2);

    abcg::glBindVertexArray(0);
  }

  abcg::glUseProgram(0);
}

void Enemies::terminateGL() {
  for (auto enemy : m_enemies) {
    abcg::glDeleteBuffers(1, &enemy.m_vbo);
    abcg::glDeleteVertexArrays(1, &enemy.m_vao);
  }
}

void Enemies::update() {
  for (auto &enemy : m_enemies) {
    enemy.m_translation.y -= 0.00175f;
  }
}

Enemies::Enemy Enemies::createEnemy(glm::vec2 translation,
                                              float scale) {
  Enemy enemy;

  enemy.m_polygonSides = 10;

  enemy.m_color = glm::vec4{1.0f, 0.0f, 0.0f, 1.0f};

  enemy.m_color.a = 1.0f;
  enemy.m_scale = scale;
  enemy.m_translation = translation;

  std::array<glm::vec2, 9> positions{
      // Main body
      glm::vec2{-5.0f, +5.0f}, 
      glm::vec2{+5.0f, +5.0f}, 
      glm::vec2{+0.0f, -5.0f},

      //Rigth Wing
      glm::vec2{+5.0f, +5.0f}, 
      glm::vec2{+5.0f, -1.0f},
      glm::vec2{+0.0f, -1.0f}, 

      //Left Wing
      glm::vec2{-5.0f, +5.0f},
      glm::vec2{-5.0f, -1.0f},
      glm::vec2{0.0f, -1.0f}

  };

  for (auto &position : positions) {
    position /= glm::vec2{5.0f, 5.0f};
  }

  std::array indices{
    0, 1, 2,
    6, 7, 4
  };

  glGenBuffers(1, &enemy.m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, enemy.m_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenBuffers(1, &enemy.m_ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, enemy.m_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  GLint positionAttribute{glGetAttribLocation(m_program, "inPosition")};

  glGenVertexArrays(1, &enemy.m_vao);

  glBindVertexArray(enemy.m_vao);

  glEnableVertexAttribArray(positionAttribute);
  glBindBuffer(GL_ARRAY_BUFFER, enemy.m_vbo);
  glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, enemy.m_ebo);

  glBindVertexArray(0);

  return enemy;
}