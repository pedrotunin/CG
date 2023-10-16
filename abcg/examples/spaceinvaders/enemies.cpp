#include "enemies.hpp"

#include <cmath>
#include <cppitertools/itertools.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

void Enemies::create(GLuint program, int quantity) {
  destroy();

  m_program = program;
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  // Create enemies
  m_enemies.clear();
  m_enemies.resize(quantity);

  const float spaceX{0.18f};
  float spaceY{0.0f};

  int i{1};

  for (auto &enemy : m_enemies) {
    glm::vec2 translation{-1.0f + spaceX * i, 0.9f - spaceY};
    enemy = makeEnemy(translation, 0.05f);
    if (i % 10 == 0) {
      i = 0;
      spaceY += 0.18f;
    }
    i++;
  }
}

void Enemies::paint(GameData const &gameData) {
  if (gameData.m_state != State::Playing)
    return;

  abcg::glUseProgram(m_program);

  for (const auto &enemy : m_enemies) {
    abcg::glBindVertexArray(m_VAO);

    abcg::glUniform4fv(m_colorLoc, 1, &enemy.m_color.r);
    abcg::glUniform1f(m_scaleLoc, enemy.m_scale);

    abcg::glUniform2f(m_translationLoc, enemy.m_translation.x,
                      enemy.m_translation.y);

    abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, 9);

    abcg::glBindVertexArray(0);
  }

  abcg::glUseProgram(0);
}

void Enemies::destroy() {
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

void Enemies::update(float deltaTime) {

  for (auto &enemy : m_enemies) {
    enemy.m_translation.x -=
        0.2f * deltaTime * m_direction * (log(50 / m_enemies.size()) + 1);
    if (m_updateY.elapsed() >= 900.0 / 1000.0 &&
        (enemy.m_translation.x <= -0.95f || enemy.m_translation.x >= 0.95f)) {

      for (auto &e : m_enemies)
        e.m_translation.y -= 0.05f;

      m_direction *= -1.0f;
      m_updateY.restart();
    }
  }
}

Enemies::Enemy Enemies::makeEnemy(glm::vec2 translation, float scale) {
  Enemy enemy;

  enemy.m_color = glm::vec4{1.0f, 1.0f, 1.0f, 1.0f};
  enemy.m_color.a = 1.0f;
  enemy.m_scale = scale;
  enemy.m_translation = translation;

  std::array<glm::vec2, 9> positions{
      // Main body
      glm::vec2{-5.0f, +5.0f}, glm::vec2{+5.0f, +5.0f}, glm::vec2{+0.0f, -5.0f},

      // Rigth Wing
      glm::vec2{+5.0f, +5.0f}, glm::vec2{+5.0f, -1.0f}, glm::vec2{+0.0f, -1.0f},

      // Left Wing
      glm::vec2{-5.0f, +5.0f}, glm::vec2{-5.0f, -1.0f}, glm::vec2{0.0f, -1.0f}

  };

  for (auto &position : positions) {
    position /= glm::vec2{5.0f, 5.0f};
  }

  std::array indices{0, 1, 2, 3, 4, 5, 0, 6, 7};

  // Generate VBO
  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate EBO
  abcg::glGenBuffers(1, &m_EBO);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  GLint positionAttribute{abcg::glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &m_VAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_VAO);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);

  return enemy;
}