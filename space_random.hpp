#ifndef RANDOM_EVOLUTION_HPP
#define RANDOM_EVOLUTION_HPP

#include "consts.hpp"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace random_evo {

enum class STATE_CELL {
  Empty,
  Healthy,
  Infected,
  Recovered,
  Dead
}; //@context stato della cella nell'automa

class World { //@concept classe dello spazio dell'automa: ogni class Word è "un
              //giorno"

private:
  using Row = std::vector<STATE_CELL>;
  int n_rows;
  int n_columns;
  std::vector<Row> space; //@context SPAZIO DELL'AUTOMA
  void
  Generate_world(int const n,
                 int const i0); //@concept genera da zero l'automa inserendo
                                //@concept popolazione totale e infetti iniziali
  int Counter(STATE_CELL const &state) const;

public:
  World(int rows, int columns) //@concept costruttore spazio vuoto
      : n_rows(rows), n_columns(columns), space(rows, [columns] {
          Row row0(columns, STATE_CELL::Empty);
          return row0;
        }()) {}

  World(int rows, int columns, int n,
        int i0) //@concept costruttore di base: genera da zero lo spazio
                //dell'automa
      : World(rows, columns) {
    if (n < i0)
      throw std::runtime_error{"Il numero di infetti è superiore al numero di "
                               "persone presenti nella popolazione"};
    assert(n > i0);
    Generate_world(n, i0);
  }

  //@context Return info
  int Rows() const { return n_rows; };
  int Columns() const { return n_columns; };

  STATE_CELL Space(int line,
                   int col) const { //@concept ritorna il valore della cella
    assert(line < n_rows);
    assert(col < n_columns);
    return space[line][col];
  };
  void Space(int line, int col, STATE_CELL change) {
    space[line][col] = change;
  }

  int Infected() const { return Counter(STATE_CELL::Infected); };
  int Healthy() const { return Counter(STATE_CELL::Healthy); };
  int Recovered() const { return Counter(STATE_CELL::Recovered); };
  int Dead() const { return Counter(STATE_CELL::Dead); };
  int Removed() const {
    return (Counter(STATE_CELL::Recovered) + Counter(STATE_CELL::Dead));
  };

  friend void Print(World const &to_print);
};

inline std::ostream &operator<<(std::ostream &os, STATE_CELL const &s) {
  switch (s) {
  case STATE_CELL::Empty:
    os << " ";
    break;
  case STATE_CELL::Healthy:
    os << FOREGROUND_GREEN << "\u03A8" << RESET_COLOR; //Ψ
    break;
  case STATE_CELL::Infected:
    os << FOREGROUND_RED << "\u0398" << RESET_COLOR; //Θ
    break;
  case STATE_CELL::Recovered:
    os << FOREGROUND_MAGENTA << "\u039E" << RESET_COLOR; //Ξ
    break;
  case STATE_CELL::Dead:
    os << FOREGROUND_YELLOW << "\u03A6" << RESET_COLOR; //Φ
    break;

  default:
    os.setstate(std::ios_base::failbit);
  }
  return os;
}

int Check_neighbors(World const &analyse, int const line, int const col,
                    STATE_CELL state_to_search);
STATE_CELL New_State(World const &now, STATE_CELL to_change, int i, int j,
                     double gamma, double beta, double mortality);
World Evolve(World old, double gamma, double beta, double mortality);
void Move(World &old);
void Print(World const &to_print);

} // namespace random_evo
#endif