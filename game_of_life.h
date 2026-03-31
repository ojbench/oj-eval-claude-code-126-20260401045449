/**
 * This is the student's part for Conway's Game of Life Simulator.
 * You need to finish all functions marked with "TODO" label.
 * When submitting, you should hand in THIS header file (i.e. game_of_life.h) to OJ to get correct mark.
 *
 * By the way, you SHOULD NOT try to read or write any extra information to the console.
 * Any attempt to hack the OJ or special judger WILL BE voided (and punished if it causes severe results).
 *
 * This is part of the 1st homework of Class A and 2nd homework of Class B. The workload isn't large.
 * We've also prepared a small visualizer for you. Enjoy coding ~
 */
#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

/**
 * Feel free to define/modify variables here.
 */
inline int row = -1, col = -1;
inline std::vector<std::vector<bool>> game_map;
inline std::vector<std::vector<bool>> next_map;

/**
 * This function is called at the beginning of every game.
 * Read the initial map of the game from stdin.
 * The format of the input is written in the problem description.
 *
 * TODO: Read the initial map from input and initialize your variables.
 */
inline void Initialize() {
  std::cin >> col >> row;
  std::cin.ignore(); // Ignore newline after row/col

  // Initialize maps
  game_map.assign(row, std::vector<bool>(col, false));
  next_map.assign(row, std::vector<bool>(col, false));

  // Read pattern until we find '!'
  std::string pattern_line;
  std::string full_pattern;
  while (std::getline(std::cin, pattern_line)) {
    full_pattern += pattern_line;
    if (pattern_line.find('!') != std::string::npos) {
      break;
    }
  }

  // Parse RLE pattern
  int current_row = 0;
  int current_col = 0;
  int num = 0;

  for (size_t i = 0; i < full_pattern.length(); i++) {
    char c = full_pattern[i];

    if (c == '!') {
      break;
    } else if (c >= '0' && c <= '9') {
      num = num * 10 + (c - '0');
    } else if (c == 'b') {
      // Dead cells
      int count = (num == 0) ? 1 : num;
      current_col += count;
      num = 0;
    } else if (c == 'o') {
      // Live cells
      int count = (num == 0) ? 1 : num;
      for (int j = 0; j < count; j++) {
        if (current_row < row && current_col < col) {
          game_map[current_row][current_col] = true;
        }
        current_col++;
      }
      num = 0;
    } else if (c == '$') {
      // New line
      int count = (num == 0) ? 1 : num;
      current_row += count;
      current_col = 0;
      num = 0;
    }
  }
}

/**
 * This function is called once in every round of the game.
 * You need to simulate a round of game in this program.
 *
 * Note: You don't need to input or output anything. Just change the variables you defined so that they're in the new round.
 *
 * TODO: Simulate a new round of the game.
 */
inline void Tick() {
  // Apply Conway's Game of Life rules
  for (int r = 0; r < row; r++) {
    for (int c = 0; c < col; c++) {
      // Count live neighbors
      int live_neighbors = 0;
      for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
          if (dr == 0 && dc == 0) continue;
          int nr = r + dr;
          int nc = c + dc;
          if (nr >= 0 && nr < row && nc >= 0 && nc < col) {
            if (game_map[nr][nc]) {
              live_neighbors++;
            }
          }
        }
      }

      // Apply rules
      if (game_map[r][c]) {
        // Live cell
        if (live_neighbors < 2 || live_neighbors > 3) {
          next_map[r][c] = false; // Dies
        } else {
          next_map[r][c] = true; // Stays alive
        }
      } else {
        // Dead cell
        if (live_neighbors == 3) {
          next_map[r][c] = true; // Becomes alive
        } else {
          next_map[r][c] = false; // Stays dead
        }
      }
    }
  }

  // Swap maps
  game_map.swap(next_map);
}

/**
 * This function may be called at any time during the game.
 * You need to print the map of the game in a certain format.
 * Output format is written in the problem description.
 *
 * Note: In this function, you just needs to print the map. DO NOT simulate a new round of game.
 *
 * TODO: Print the whole map.
 */
inline void PrintGame() {
  std::cout << col << " " << row << std::endl;

  std::ostringstream pattern;

  // Find last row with live cells
  int last_row_with_live = -1;
  for (int r = row - 1; r >= 0; r--) {
    for (int c = 0; c < col; c++) {
      if (game_map[r][c]) {
        last_row_with_live = r;
        break;
      }
    }
    if (last_row_with_live != -1) break;
  }

  for (int r = 0; r <= last_row_with_live; r++) {
    // Find last live cell in this row
    int last_live_col = -1;
    for (int c = col - 1; c >= 0; c--) {
      if (game_map[r][c]) {
        last_live_col = c;
        break;
      }
    }

    // Process this row up to the last live cell
    if (last_live_col >= 0) {
      int c = 0;
      while (c <= last_live_col) {
        bool is_alive = game_map[r][c];
        int count = 1;

        // Count consecutive cells of the same type
        while (c + count <= last_live_col && game_map[r][c + count] == is_alive) {
          count++;
        }

        // Output the run
        if (is_alive) {
          if (count == 1) {
            pattern << 'o';
          } else {
            pattern << count << 'o';
          }
        } else {
          if (count == 1) {
            pattern << 'b';
          } else {
            pattern << count << 'b';
          }
        }

        c += count;
      }
    }

    // Add line separator
    if (r < last_row_with_live) {
      // Check if next row is empty
      bool next_is_empty = true;
      for (int c = 0; c < col; c++) {
        if (game_map[r + 1][c]) {
          next_is_empty = false;
          break;
        }
      }

      if (next_is_empty) {
        // Count consecutive empty rows
        int empty_count = 0;
        int check_r = r + 1;
        while (check_r <= last_row_with_live) {
          bool is_empty = true;
          for (int c = 0; c < col; c++) {
            if (game_map[check_r][c]) {
              is_empty = false;
              break;
            }
          }
          if (!is_empty) break;
          empty_count++;
          check_r++;
        }

        if (empty_count == 1) {
          pattern << "2$";
        } else {
          pattern << (empty_count + 1) << '$';
        }
        r = check_r - 1; // Skip empty rows
      } else {
        pattern << '$';
      }
    }
  }

  pattern << '!';
  std::cout << pattern.str() << std::endl;
}

/**
 * This function may be called at any time during the game.
 * You need to return the number of live cells.
 *
 * @returns the number of live cells in the map.
 *
 * Note: In this function, you SHOULD NOT simulate a new round of game or print anything to stdout.
 *
 * TODO: Returns the number of live cells.
 */
inline int GetLiveCell() {
  int count = 0;
  for (int r = 0; r < row; r++) {
    for (int c = 0; c < col; c++) {
      if (game_map[r][c]) {
        count++;
      }
    }
  }
  return count;
}
