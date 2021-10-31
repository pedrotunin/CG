#include "openglwindow.hpp"

#include <imgui.h>

#include "abcg.hpp"

void OpenGLWindow::handleEvent(SDL_Event &event) {
  // Keyboard events
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_SPACE)
      m_gameData.m_input.set(static_cast<size_t>(Input::Fire));
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.set(static_cast<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.set(static_cast<size_t>(Input::Right));
  }
  if (event.type == SDL_KEYUP) {
    if (event.key.keysym.sym == SDLK_SPACE)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Fire));
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Right));
  }

  // Mouse events
  if (event.type == SDL_MOUSEBUTTONDOWN) {
    if (event.button.button == SDL_BUTTON_LEFT)
      m_gameData.m_input.set(static_cast<size_t>(Input::Fire));
  }
  if (event.type == SDL_MOUSEBUTTONUP) {
    if (event.button.button == SDL_BUTTON_LEFT)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Fire));
  }
}

void OpenGLWindow::initializeGL() {
  // Load a new font
  ImGuiIO &io{ImGui::GetIO()};
  auto filename{getAssetsPath() + "Inconsolata-Medium.ttf"};
  m_font = io.Fonts->AddFontFromFileTTF(filename.c_str(), 40.0f);
  if (m_font == nullptr) {
    throw abcg::Exception{abcg::Exception::Runtime("Cannot load font file")};
  }

  // Create program to render the other objects
  m_objectsProgram = createProgramFromFile(getAssetsPath() + "objects.vert",
                                           getAssetsPath() + "objects.frag");

  abcg::glClearColor(0, 0, 0, 1);

#if !defined(__EMSCRIPTEN__)
  abcg::glEnable(GL_PROGRAM_POINT_SIZE);
#endif

  // Start pseudo-random number generator
  m_randomEngine.seed(
      std::chrono::steady_clock::now().time_since_epoch().count());

  restart();
}

void OpenGLWindow::restart() {
  m_gameData.m_state = State::Playing;
  m_gameData.m_enemies_left = 30;

  m_ship.initializeGL(m_objectsProgram);
  m_enemies.initializeGL(m_objectsProgram, 30);
  m_bullets.initializeGL(m_objectsProgram);
}

void OpenGLWindow::update() {
  float deltaTime{static_cast<float>(getDeltaTime())};

  // Wait 5 seconds before restarting
  if (m_gameData.m_state != State::Playing &&
      m_restartWaitTimer.elapsed() > 5) {
    restart();
    return;
  }

  m_ship.update(m_gameData);
  m_enemies.update();
  m_bullets.update(m_ship, m_gameData, deltaTime);

  if (m_gameData.m_state == State::Playing) {
    checkCollisions();
    checkWinCondition();
  }
}

void OpenGLWindow::paintGL() {
  update();

  abcg::glClear(GL_COLOR_BUFFER_BIT);
  abcg::glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  m_enemies.paintGL();
  m_bullets.paintGL();
  m_ship.paintGL(m_gameData);
}

void OpenGLWindow::paintUI() {
  abcg::OpenGLWindow::paintUI();

  {

    ImGuiWindowFlags flags{ImGuiWindowFlags_NoBackground |
                            ImGuiWindowFlags_NoTitleBar |
                            ImGuiWindowFlags_NoInputs};

    if (m_gameData.m_state == State::Playing) {

      auto sizeDisplayEnemies{ImVec2(250, 65)};
      auto posDisplayEnemies{
          ImVec2(m_viewportWidth - sizeDisplayEnemies.x - 5,
                 m_viewportHeight - sizeDisplayEnemies.y - 5)};

      ImGui::SetNextWindowPos(posDisplayEnemies);
      ImGui::SetNextWindowSize(sizeDisplayEnemies);
      ImGui::Begin(" ", nullptr, flags);

      ImGui::PushFont(m_font);
      ImGui::Text("Enemies: %d", m_gameData.m_enemies_left);
      ImGui::PopFont();
      ImGui::End();
    } 
    else {

      const auto size{ImVec2(300, 85)};
      const auto position{ImVec2((m_viewportWidth - size.x) / 2.0f,
                               (m_viewportHeight - size.y) / 2.0f)};

      ImGui::SetNextWindowPos(position);
      ImGui::SetNextWindowSize(size);
      ImGuiWindowFlags flags{ImGuiWindowFlags_NoBackground |
                            ImGuiWindowFlags_NoTitleBar |
                            ImGuiWindowFlags_NoInputs};

      ImGui::Begin(" ", nullptr, flags);
      ImGui::PushFont(m_font);
      
      if (m_gameData.m_state == State::GameOver) {
        ImGui::Text("   Game Over!");
      } else if (m_gameData.m_state == State::Win) {
        ImGui::Text("   *You won!*");
      }
    
      ImGui::PopFont();
      ImGui::End();
    } 
  }
}

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;

  abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWindow::terminateGL() {
  abcg::glDeleteProgram(m_objectsProgram);

  m_enemies.terminateGL();
  m_bullets.terminateGL();
  m_ship.terminateGL();
}

void OpenGLWindow::checkCollisions() {
  // Check collision between ship and enemies
  for (const auto &enemy : m_enemies.m_enemies) {
    const auto enemyTranslation{enemy.m_translation};
    const auto distance{
        glm::distance(m_ship.m_translation, enemyTranslation)};

    if (distance < m_ship.m_scale * 0.9f + enemy.m_scale * 0.40f) {
      m_gameData.m_state = State::GameOver;
      m_restartWaitTimer.restart();
    }
  }

  // Check if enemies passed
  for (const auto &enemy : m_enemies.m_enemies) {
    const auto enemyTranslation{enemy.m_translation};

    if (enemyTranslation.y < -1.0f) {
      m_gameData.m_state = State::GameOver;
      m_restartWaitTimer.restart();
    }
  }

  // Check collision between bullets and enemies
  for (auto &bullet : m_bullets.m_bullets) {
    if (bullet.m_dead) continue;

    for (auto &enemy : m_enemies.m_enemies) {
      for (const auto i : {-2, 0, 2}) {
        for (const auto j : {-2, 0, 2}) {
          const auto enemyTranslation{enemy.m_translation +
                                         glm::vec2(i, j)};
          const auto distance{
              glm::distance(bullet.m_translation, enemyTranslation)};

          if (distance < m_bullets.m_scale + enemy.m_scale * 0.70f) {
            enemy.m_hit = true;
            bullet.m_dead = true;

            m_gameData.m_enemies_left -= 1;
          }
        }
      }
    }

    m_enemies.m_enemies.remove_if(
        [](const Enemies::Enemy &a) { return a.m_hit; });
  }
}

void OpenGLWindow::checkWinCondition() {
  if (m_enemies.m_enemies.empty()) {
    m_gameData.m_state = State::Win;
    m_restartWaitTimer.restart();
  }
}