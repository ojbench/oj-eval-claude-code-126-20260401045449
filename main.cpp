#include <chrono>
#include <iostream>

#include "game_of_life.h"

void VisualizeTest() {
  Initialize();
  int round = 0;
  std::cin >> round;
  PrintGame();
  std::cout << 0 << " " << GetLiveCell() << std::endl;
  for (int i = 1; i <= round; ++i) {
    Tick();
    PrintGame();
    std::cout << i << " " << GetLiveCell() << std::endl;
  }
}

int main() {
  VisualizeTest();
  return 0;
}
