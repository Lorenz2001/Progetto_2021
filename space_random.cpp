#include "space_random.hpp"
#include "consts.hpp"

#include <iomanip>
#include <random>

namespace random_evo {

void World::Generate_world(int const n, int const i0) {
  if (n > 500)
    throw std::runtime_error{
        "La popolazione è troppo grande per esser gestita"};
  std::default_random_engine gen{std::random_device{}()};
  std::uniform_int_distribution<int> LIN{0, n_rows - 1};
  std::uniform_int_distribution<int> COL{0, n_columns - 1};
  for (int i = 0; i < (n - i0); i++) //@concept inserimento persone sane
  {
    auto line = LIN(gen);
    auto column = COL(gen);
    for (; space[line][column] != STATE_CELL::Empty;
         line = LIN(gen), column = COL(gen))
      ;
    space[line][column] = STATE_CELL::Healthy;
  }
  for (int i = 0; i < i0; i++) //@concept inserimento persone malate
  {
    auto line = LIN(gen);
    auto column = COL(gen);
    for (; space[line][column] != STATE_CELL::Empty;
         line = LIN(gen), column = COL(gen))
      ;
    space[line][column] = STATE_CELL::Infected;
  }
}

int World::Counter(STATE_CELL const &state)
    const { //@concept conta quanti elementi con lo stato state sono presenti
            //@concept nello spazio dell'automa
  assert(state != STATE_CELL::Empty);
  int c = 0;
  for (int i = 0; i < n_rows; i++)
    for (int j = 0; j < n_columns; j++)
      if (space[i][j] == state)
        c++;
  return c;
}

int Check_neighbors(World const &analyse, int const line, int const col,
                    STATE_CELL state_to_search) {
  int counter = 0;
  auto const max_x = analyse.Columns() - 1;
  auto const max_y = analyse.Rows() - 1;
  auto f = [](int const a, int const &max)
      -> std::vector<int> { //@context funzione per individuare le possibili
                            //@context celle vicine visualizzabili
    std::vector<int> ret(2);
    if (a == max) {
      ret[0] = -1;
      ret[1] = 0;
      return ret;
    } else if (a == 0) {
      ret[0] = 0;
      ret[1] = 1;
      return ret;
    } else {
      ret[0] = -1;
      ret[1] = 0;
      ret.push_back(1);
      return ret;
    }
  };
  for (auto const &i : f(line, max_y))
    for (auto const &j : f(col, max_x))
      if (analyse.Space(line + i, col + j) == state_to_search &&
          !(i == 0 && j == 0))
        counter++;

  return (analyse.Space(line, col) == state_to_search) ? counter-- : counter;
}

STATE_CELL New_State(World const &now, STATE_CELL to_change, int i, int j,
                     double gamma, double beta, double mortality) {
  std::default_random_engine ran_change{std::random_device{}()};
  int neigh_infect = Check_neighbors(now, i, j, STATE_CELL::Infected);
  switch (to_change) {
  case STATE_CELL::Infected: //@context probabile guarigione-morte
  {
    std::uniform_real_distribution<double> recover(0, 1);
    if (recover(ran_change) <= gamma) {
      std::uniform_real_distribution<double> dead(0, 1);
      return (dead(ran_change) <= mortality) ? STATE_CELL::Dead
                                             : STATE_CELL::Recovered;
    }

  } break;
  case STATE_CELL::Healthy: //@context probabile infezione
  {
    if (neigh_infect >= 1) {
      std::uniform_real_distribution<double> reco{0, 1};
      auto rec = reco(ran_change);
      for (int i = 0; rec > beta && i < neigh_infect;
           i++, rec = reco(ran_change))
        ;
      if (rec <= beta)
        return STATE_CELL::Infected;
      else
        return to_change;
    } else
      return to_change;
  }
  default:
    return to_change;
    break;
  }
  return to_change;
}

World Evolve(World old, double gamma, double beta, double mortality) {
  for (int i = 0; i < old.Rows(); i++)
    for (int j = 0; j < old.Columns(); j++)
      old.Space(i, j,
                New_State(old, old.Space(i, j), i, j, gamma, beta, mortality));
  return old;
}

void Move(World &old) {
  if (old.Healthy() + old.Removed() + old.Infected() > 500)
    throw std::runtime_error{
        "La popolazione è troppo grande per calcolare gli spostamenti"};

  std::default_random_engine eng{std::random_device{}()};
  World New(old.Rows(), old.Columns());
  for (int i = 0; i < old.Rows(); i++)
    for (int j = 0; j < old.Columns(); j++) {
      auto x = j;
      auto y = i;
      if (old.Space(i, j) != STATE_CELL::Dead) { //@concept vengono "spostate"
                                                 // solo le persone "vive"
        while (old.Space(y, x) !=
                   STATE_CELL::Empty || //@concept se la cella (x,y) non è vuota
                                        // nella griglia old si sposta il suo
                                        // valore altrove nella griglia new
               New.Space(y, x) !=
                   STATE_CELL::Empty) { //@concept se la cella (x,y) non è vuota
                                        // nella griglia new si ricerca una
                                        // nuova posizione
          if (j == old.Columns() - 1) {
            std::uniform_int_distribution<int> motion1(-1, 0);
            x = motion1(eng);
          } else if (j == 0) {
            std::uniform_int_distribution<int> motion1(0, 1);
            x = motion1(eng);
          } else {
            std::uniform_int_distribution<int> motion1(-1, 1);
            x = motion1(eng);
          }

          if (i == old.Rows() - 1) {
            std::uniform_int_distribution<int> motion2(-1, 0);
            y = motion2(eng);
          } else if (i == 0) {
            std::uniform_int_distribution<int> motion2(0, 1);
            y = motion2(eng);
          } else {
            std::uniform_int_distribution<int> motion2(-1, 1);
            y = motion2(eng);
          }
          //@concept (x,y) = posizione nella nuova griglia
          x += j;
          y += i;
        }
      }
      New.Space(y, x, old.Space(i, j));
    }
  old = New;
}

void Print(World const &to_print) {
  std::cout << CLEAR_TERMINAL;
  std::cout << BACKGROUND_WHITE << "  ";
  for (int i = 0; i < to_print.n_columns; i++)
    std::cout << " ";
  std::cout << "  \n" << RESET_COLOR;
  for (int i = 0; i < to_print.n_rows; i++) {
    std::cout << BACKGROUND_WHITE << "  " << RESET_COLOR;
    for (int j = 0; j < to_print.n_columns; j++) {
      std::cout << to_print.space[i][j];
    }
    std::cout << BACKGROUND_WHITE << "  " << RESET_COLOR << "\n";
  }
  std::cout << BACKGROUND_WHITE << "  ";
  for (int i = 0; i < to_print.n_columns; i++)
    std::cout << " ";
  std::cout << "  \n" << RESET_COLOR;
}

} // namespace random_evo