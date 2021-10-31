#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#include <bitset>

enum class Input { Right, Left, Fire };
enum class State { Playing, GameOver, Win };

struct GameData {
  int m_enemies_left{0};
  State m_state{State::Playing};
  std::bitset<3> m_input;  // [fire, left, right]
};

#endif