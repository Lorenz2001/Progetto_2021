#include "classes_sir.hpp"
#include "consts.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>

//@context Declaration function namespace sir

namespace sir {

int Population::counter = 0;

//@context CLASS POPULATION

int Population::Choice() {
  if (T == 0) {
    std::cout
        << "\n Si vogliono stampare i dati elaborati fino alla decrescita "
           "della curva per scegliere un evento? (y/n) ";

  } else {
    std::cout
        << "\n Si vogliono stampare i dati elaborati fino a meta' simulazione? "
           "della curva per scegliere un evento? (y/n) ";
  }
  char w;
  std::cin >> w;
  if (w == 'y' || w == 'Y')
    Print();
  std::cout << "\n Scegliere un evento: \n\n";
  std::cout << "    0-Nessuna modifica\n";
  std::cout << FOREGROUND_GREEN;
  std::cout << "    1-Vengono messe in atto restrizioni\n";
  std::cout << "    2-Vengono trovate delle nuove cure\n";
  std::cout << "    3-Vengono sviluppati dei vaccini\n";
  std::cout << "    4-Vengono sviluppate sia delle cure che i vaccini\n";
  std::cout << FOREGROUND_RED;
  std::cout << "    5-Si sviluppano varianti più contagiose e mortali\n";
  std::cout
      << "    6-Nessuna legge impone restrizioni favorendo assembramenti\n";
  std::cout << "    7-Si sviluppano nuove varianti più contaggiose e mortali "
               "senza che i governi pongano limiti ad assembramenti\n\n";
  std::cout << RESET_COLOR;
  std::cout << "Scelta: ";
  std::cin >> change;
  switch (change) {
  case 1: {
    std::cout << "\n Hai scelto evento 1: beta cala del 20%";
    beta *= 0.80;
  } break;

  case 2: {
    std::cout << "\n Hai scelto evento 2: gamma aumenta del 20%";
    gamma *= 1.20;
  } break;

  case 3: {
    std::cout << "\n Hai scelto evento 3: beta diminuisce del 10%";
    beta *= 0.90;
  } break;

  case 4: {
    std::cout << "\n Hai scelto evento 6: gamma aumenta del 20% e beta "
                 "diminuisce del 10%";
    gamma *= 1.20;
    beta *= 0.90;
  } break;
  case 5: {
    std::cout << "\n Hai scelto evento 5: beta e gamma aumentano del 10%";
    gamma *= 1.10;
    beta *= 1.10;
  } break;
  case 6: {
    std::cout << "\n Hai scelto evento 4: beta aumenta del 10%";
    beta *= 1.10;
  } break;
  case 7: {
    std::cout << "\n Hai scelto evento 7: gamma aumenta del 10% e beta aumenta "
                 "del 30%";
    gamma *= 1.10;
    beta *= 1.30;
  } break;
  default: {
    std::cout << "\n Nessun evento ha influito sulla diffusione della "
                 "pandemia: beta e gamma rimangono stabili";
    change = 0;
  } break;
  }
  return change;
}

Data Population::fct_new(Data &old) {
  Data _new;
  _new.s = old.s * (1 - beta * old.i / N);
  _new.i = old.i * (1 + beta * old.s / N - gamma);
  _new.r = old.r + gamma * old.i;
  old = _new;
  return _new;
}

void Population::Print() const {
  if (evolved == false && world.size() == 0) {
    std::cout << FOREGROUND_RED << " Error on Population::Print(): Population "
              << id << " NOT EVOLVED " << RESET_COLOR << "\n";
    return;
  }
  std::cout << std::setw(8) << "\n Giorno "
            << " | " << std::setw(15) << " Suscettibili "
            << " | " << std::setw(10) << " Infetti  "
            << " | " << std::setw(10) << " Rimossi "
            << " | "
            << " Andamento contagio \n";
  int k = world.size();
  int c = (k < T || T == 0) ? world.size() : T;
  for (int i = 0; i < c; i += ((k > 500 && T == 0) || (T > 500)) ? 20 : 1) {
    std::cout << std::fixed << std::setw(8) << std::setprecision(0) << i * d_T
              << " | " << std::setw(15) << std::setprecision(0) << world[i].s
              << " | " << std::setprecision(0) << std::setw(10)
              << round(world[i].i) << " | " << std::setprecision(0)
              << std::setw(10) << world[i].r << " | " << std::setw(10)
              << std::setprecision(0);
    if (i == 0)
      std::cout << "\n";
    else {
      switch (Trend(i, Data::GROUP::Infected)) {
      case 0:
        std::cout << FOREGROUND_GREEN << " in decrescita " << RESET_COLOR
                  << "\n";
        break;
      case 1:
        std::cout << FOREGROUND_MAGENTA << " costante " << RESET_COLOR << "\n";
        break;
      default:
        std::cout << FOREGROUND_RED << " in crescita " << RESET_COLOR << "\n";
        break;
      }
    }
  }
}

void Population::Print(int start, int end) const {
  if (evolved == false) {
    std::cout << FOREGROUND_RED
              << " Error on Population::Print(int start, int end): Population"
              << id << "NOT EVOLVED " << RESET_COLOR << "\n";
    return;
  }
  std::cout << std::setw(8) << "\n Giorno "
            << " | " << std::setw(15) << " Suscettibili "
            << " | " << std::setw(10) << " Infetti  "
            << " | " << std::setw(10) << " Rimossi "
            << " | "
            << " Andamento contagio"
            << "\n";
  end = (end == 0) ? world.size() : end;
  for (int i = start; i < end; i++) {
    std::cout << std::fixed << std::setw(8) << std::setprecision(0) << i * d_T
              << " | " << std::setw(15) << std::setprecision(0) << world[i].s
              << " | " << std::setprecision(0) << std::setw(10)
              << round(world[i].i) << " | " << std::setprecision(0)
              << std::setw(10) << world[i].r << " | " << std::setw(10)
              << std::setprecision(0);
    if (i == 0)
      std::cout << "\n";
    else {
      switch (Trend(i, Data::GROUP::Infected)) {
      case 0:
        std::cout << FOREGROUND_GREEN << " in decrescita " << RESET_COLOR
                  << "\n";
        break;
      case 1:
        std::cout << FOREGROUND_MAGENTA << " costante " << RESET_COLOR << "\n";
        break;
      default:
        std::cout << FOREGROUND_RED << " in crescita " << RESET_COLOR << "\n";
        break;
      }
    }
  }
}

int Population::Evolution(bool q) {
  assert(gamma < beta);
  assert(I0 < N);
  if ((T == 0 && evolved == true) || (T != 0 && time_extinction != 0))
    return -1;
  Data start;
  if (evolved == false) {
    start.s = static_cast<double>(N - I0);
    start.i = static_cast<double>(I0);
    start.r = 0.;
    world.push_back(start);
  } else {
    start.s = world.at(world.size() - 1).s;
    start.i = world.at(world.size() - 1).i;
    start.r = world.at(world.size() - 1).r;
  }
  int c = 0;
  if (T == 0 || (evolved == true && start.i != 0)) {
    Data d = start;
    int j = T;
    while ((j != 0) ? (world[j].i > world[j - 1].i) : (1 != 0)) {

      world.push_back(fct_new(d));
      j++;
    }

    if (q == true && evolved == false) {
      c = Choice();
    }
    while (round(d.i) != 0) {
      world.push_back(fct_new(d));
      j++;
    }
    time_extinction = j;
    world.push_back(fct_new(d));
  } else {
    Data d = start;
    double stop = static_cast<int>(T / 2);
    assert(evolved == false);
    for (int j = 0; j < stop; j += d_T) {
      world.push_back(fct_new(d));
      assert(round(d.i + d.r + d.s) == N);
    }

    if (q == true) {
      c = Choice();
    }

    for (int i = stop; i < T; i += d_T) {
      world.push_back(fct_new(d));
      assert(round(d.i + d.r + d.s) == N);
    }
  }
  evolved = true;
  return c;
}

void Population::Print_Info() {
  std::cout << "\n" << std::defaultfloat;
  std::cout << BACKGROUND_WHITE << FOREGROUND_BLUE
            << " Identificativo simulazione: " << id << "  " << RESET_COLOR
            << "\n";
  std::cout << BACKGROUND_RED << " Stato originale:" << RESET_COLOR << "\n";
  std::cout << "   " << BACKGROUND_BLUE << "\u03B3: " << std::setw(6)
            << original_gamma << RESET_COLOR << "\n";
  std::cout << "   " << BACKGROUND_BLUE << "\u03B2: " << std::setw(6)
            << original_beta << RESET_COLOR << "\n";
  std::cout << "     " << BACKGROUND_GREEN << "Suscettibili: " << std::setw(8)
            << N - I0 << RESET_COLOR << "\n";
  std::cout << "     " << BACKGROUND_GREEN << "Infetti:      " << std::setw(8)
            << I0 << RESET_COLOR << "\n";
  std::cout << "     ---------------------- \n";
  std::cout << "     " << BACKGROUND_GREEN << "Totali:       " << std::setw(8)
            << N << RESET_COLOR << "\n";
  std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
  if (evolved == true) {
    std::cout << "\n" << std::defaultfloat;
    std::cout << BACKGROUND_RED << " Evoluzione epidemia:" << RESET_COLOR
              << "\n";
    if (original_beta != beta || original_gamma != gamma) {
      std::cout << "     " << BACKGROUND_BLUE << "\u03B3: " << std::setw(6)
                << std::fixed << std::setprecision(4) << gamma << RESET_COLOR
                << "\n";
      std::cout << "     " << BACKGROUND_BLUE << "\u03B2: " << std::setw(6)
                << std::fixed << std::setprecision(4) << beta << RESET_COLOR
                << "\n";
    }
    if (T != 0 && world.at(world.size() - 1).i != 0)
      Evolution(false);
    std::cout << "     " << BACKGROUND_MAGENTA
              << "Tempo di estinzione epidemia: " << time_extinction
              << " giorni" << RESET_COLOR << "\n";
    std::cout << "\n";
  }
}

void Population::Print_on_file() const {
  assert(evolved == true);
  std::ofstream foinf, forim, fosus;
  std::string name_file1 =
      "Simulations/" + std::to_string(id) + "_Evolution_INFECTED.dat";
  std::string name_file2 = "Simulations/" + std::to_string(id) + "_Evolution_REMOVED.dat";
  std::string name_file3 =
      "Simulations/" + std::to_string(id) + "_Evolution_SUSCEPTIBLE.dat";
  foinf.open(name_file1);
  forim.open(name_file2);
  fosus.open(name_file3);
  int h = (T == 0) ? world.size() : T;
  for (int i = 0; i < h; i++) {
    foinf << i << "   " << world[i].i << "\n";
    forim << i << "   " << world[i].r << "\n";
    fosus << i << "   " << world[i].s << "\n";
  }
  foinf.close();
  forim.close();
  fosus.close();
}

void Population::Print_rand() const {
  assert(world.empty() != true);
  std::cout << std::setw(8) << "\n Giorno "
            << " | " << std::setw(15) << " Suscettibili "
            << " | " << std::setw(10) << " Infetti  "
            << " | " << std::setw(10) << " Rimossi "
            << " | "
            << " Andamento contagio\n";
  for (int i = 0; i < static_cast<int>(world.size()); i++) {
    std::cout << std::fixed << std::setw(8) << std::setprecision(0) << i * d_T
              << " | " << std::setw(15) << std::setprecision(0) << world[i].s
              << " | " << std::setprecision(0) << std::setw(10)
              << round(world[i].i) << " | " << std::setprecision(0)
              << std::setw(10) << world[i].r << " | " << std::setw(10)
              << std::setprecision(0);
    if (i == 0)
      std::cout << "\n";
    else {
      switch (Trend(i, Data::GROUP::Infected)) {
      case 0:
        std::cout << FOREGROUND_GREEN << " in decrescita " << RESET_COLOR
                  << "\n";
        break;
      case 1:
        std::cout << FOREGROUND_MAGENTA << " costante " << RESET_COLOR << "\n";
        break;
      default:
        std::cout << FOREGROUND_RED << " in crescita " << RESET_COLOR << "\n";
        break;
      }
    }
  }
}

int Population::Trend(int day, Data::GROUP type_group) const {
  assert(day >= 0);
  // 0=diminuisce; 1=uguale; 2=cresce
  switch (type_group) {
  case Data::GROUP::Susceptible:
    return (world[day].s > world[day - 1].s)
               ? 2
               : (world[day].s == world[day - 1].s) ? 1 : 0;
    break;
  case Data::GROUP::Removed:
    return (world[day].r > world[day - 1].r)
               ? 2
               : (world[day].r == world[day - 1].r) ? 1 : 0;
    break;

  default:
    return (world[day].i > world[day - 1].i)
               ? 2
               : (world[day].i == world[day - 1].i) ? 1 : 0;
    break;
  }
}

bool Population::Abort(bool control) {
  if (control == true) {
    std::cout << "\n"
              << "Conferma annulamento della simulazione (y/n): ";
    char w;
    std::cin >> w;
    if (w == 'N' || w == 'n')
      return false;
    if (w == 'y' || w == 'Y') {
      id = 0;
      counter--;
      return true;
    }
    return false;
  } else {
    counter--;
    return true;
  }
}

//@context CLASS Random_Evo : public Population

void Random_Evo::Add_Day(double s, double i, double r) {
  Data add{s, i, r};
  world.push_back(add);
}

void Random_Evo::Print_r() const {
  assert(world.empty() != true);
  std::cout << std::setw(8) << "\n Giorno "
            << " | " << std::setw(15) << " Suscettibili "
            << " | " << std::setw(10) << " Infetti  "
            << " | " << std::setw(10) << " Rimossi "
            << " | "
            << " Andamento contagio"
            << "\n";
  for (int i = 0; i < static_cast<int>(world.size());
       i += (world.size() > 100) ? world.size() / 10 : 1) {
    std::cout << std::fixed << std::setw(8) << std::setprecision(0) << i
              << " | " << std::setw(15) << std::setprecision(0) << world[i].s
              << " | " << std::setprecision(0) << std::setw(10)
              << round(world[i].i) << " | " << std::setprecision(0)
              << std::setw(10) << world[i].r << " | " << std::setw(10)
              << std::setprecision(0);
    if (i == 0)
      std::cout << "\n";
    else {
      switch ((world.size() < 100)
                  ? Trend(i, Data::GROUP::Infected)
                  : (world[i].i > world[i - 10].i)
                        ? 2
                        : (world[i].i == world[i - 10].i) ? 1 : 0) {
      case 0:
        std::cout << FOREGROUND_GREEN << " in decrescita " << RESET_COLOR
                  << "\n";
        break;
      case 1:
        std::cout << FOREGROUND_MAGENTA << " costante " << RESET_COLOR << "\n";
        break;
      default:
        std::cout << FOREGROUND_RED << " in crescita " << RESET_COLOR << "\n";
        break;
      }
    }
  }
}

void Random_Evo::Transfer(Population &a) {
  a.world.clear();
  for (auto &i : world) {
    a.world.push_back(i);
  }
  a.Abort(false);
  a.evolved = (world.empty() != true) ? true : false;
  a.T = T - 1;
}

//@context altre funzioni

bool operator<(const Population &a, const Population &b) noexcept {
  return a.id < b.id;
}
bool operator>(const Population &a, const Population &b) noexcept {
  return a.id > b.id;
}
bool operator==(const Population &a, const Population &b) noexcept {
  return a.id == b.id;
}

Population Upload_SIR() {
  Population new_pop;
  while (1) {
    std::cout << "\n\n Inserire il numero della popolazione: ";
    std::cin >> new_pop.N;
    std::cout << "\n Inserire il numero iniziale di infetti: ";
    std::cin >> new_pop.I0;
    std::cout
        << "\n Inserire la durata del monitoraggio in giorni (Inserire '0' "
           "per eseguire una simulazione fino alla fine dell'epidemia "
           "(Infetti=0) ): ";
    std::cin >> new_pop.T;
    std::cout << "\n Inserire il valore di Gamma: ";
    std::cin >> new_pop.gamma;
    std::cout << "\n Inserire il valore di Beta: ";
    std::cin >> new_pop.beta;
    std::cout << "\n\n";
    new_pop.original_beta = new_pop.beta;
    new_pop.original_gamma = new_pop.gamma;
    new_pop.d_T = 1;
    std::vector<std::string> errors;
    if (new_pop.beta < 0 || new_pop.beta > 1) {
      errors.push_back("Il valore di beta inserito non è valido.");
    }
    if (new_pop.gamma < 0 || new_pop.gamma > 1) {
      errors.push_back("Il valore di gamma inserito non è valido.");
    }
    if (new_pop.beta / new_pop.gamma < 1) {
      errors.push_back("La combinazione di valori di gamma e beta inseriti non "
                       "permette l'evolversi di un epidemia.");
    }

    int lenght = errors.size();
    if (lenght == 0)
      break;
    else {
      std::cout << BACKGROUND_RED << " ERRORS:" << RESET_COLOR << "\n";
      for (auto i = 0; i < lenght; i++)
        std::cout << FOREGROUND_RED << "     " << errors[i] << RESET_COLOR
                  << "\n";

      while (1) {
        std::cout << "Si vogliono inserire altri dati? (y/n) ";
        char w;
        std::cin >> w;
        if (w == 'N' || w == 'n')
          exit(0);
        if (w == 'y' || w == 'Y')
          break;
      }
    }
  }
  return new_pop;
}

Random_Evo Upload_Rand() {
  Random_Evo new_rand;
  while (1) {
    std::cout << "\n Inserire il numero della popolazione: ";
    std::cin >> new_rand.N;
    std::cout << "\n Inserire il numero iniziale di infetti: ";
    std::cin >> new_rand.I0;
    std::cout << "\n Inserire la durata del monitoraggio in giorni ";
    std::cin >> new_rand.T;
    new_rand.d_T = 1;
    std::cout << "\n Inserire il valore di Gamma: ";
    std::cin >> new_rand.gamma;
    std::cout << "\n Inserire il valore di Beta: ";
    std::cin >> new_rand.beta;
    std::cout << "\nInserire il valore della mortalità: ";
    std::cin >> new_rand.mortality;
    new_rand.original_beta = new_rand.beta;
    new_rand.original_gamma = new_rand.gamma;
    std::cout << "\n";
    std::vector<std::string> errors;
    if (new_rand.beta < 0 || new_rand.beta > 1) {
      errors.push_back("Il valore di beta inserito non è valido.");
    }
    if (new_rand.gamma < 0 || new_rand.gamma > 1) {
      errors.push_back("Il valore di gamma inserito non è valido.");
    }
    if (new_rand.beta / new_rand.gamma < 1) {
      errors.push_back("La combinazione di valori di gamma e beta inseriti non "
                       "permette l'evolversi di un epidemia.");
    }

    int lenght = errors.size();
    if (lenght == 0)
      break;
    else {
      std::cout << BACKGROUND_RED << " ERRORS:" << RESET_COLOR << "\n";
      for (auto i = 0; i < lenght; i++)
        std::cout << FOREGROUND_RED << "     " << errors[i] << RESET_COLOR
                  << "\n";

      while (1) {
        std::cout << "Si vogliono inserire altri dati? (y/n) ";
        char w;
        std::cin >> w;
        if (w == 'N' || w == 'n')
          exit(0);
        if (w == 'y' || w == 'Y')
          break;
      }
    }
  }
  return new_rand;
}

void Print(std::map<Population, int> &x) {

  std::cout << "\nPopolazione |";
  std::for_each(x.begin(), x.end(), [](auto const &x) {
    std::cout << std::setw(17) << x.first.N << "        |";
  });
  std::cout << "\n   gamma    |";
  std::for_each(x.begin(), x.end(), [](auto const &x) {
    std::cout << std::setw(17) << x.first.gamma << "        |";
  });
  std::cout << "\n    beta    |";
  std::for_each(x.begin(), x.end(), [](auto const &x) {
    std::cout << std::setw(17) << x.first.beta << "        |";
  });

  int q =
      std::min_element(x.begin(), x.end(), [](auto const &a, auto const &b) {
        return a.second < b.second;
      })->second;
  std::cout << "\n";
  for (int j = 1; j < q; j++) {
    std::cout << std::setw(13) << j;
    std::map<Population, int>::iterator it = x.begin();
    while (it != x.end()) {
      std::cout << std::setw(35);
      double a = it->first.world.at(j).i;
      double b = it->first.world.at(j - 1).i;
      if (a > b)
        std::cout << FOREGROUND_RED << " in crescita " << RESET_COLOR;
      if (a < b)
        std::cout << FOREGROUND_GREEN << " in decrescita " << RESET_COLOR;
      if (a == b)
        std::cout << FOREGROUND_MAGENTA << " costante " << RESET_COLOR;
      it++;
    }

    std::cout << "\n";
  }
}

} // namespace sir