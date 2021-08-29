#ifndef FUNCTIONS_RANDOM_HPP
#define FUNCTIONS_RANDOM_HPP

#include "classes_sir.hpp"
#include "space_random.hpp"

#include <cassert>
#include <iomanip>
#include <iostream>
#include <thread>

//@concept stampa a video dell'andamento dell'epidemia nell'automa
inline void Print_distribution_Trend(sir::Random_Evo const &statistics,
                                     random_evo::World const &now) {
  std::cout << " ---------------------------------------------------- \n"
            << "                       ->Trend<-                      \n";
  std::cout << " Popolazione sana:       " << std::setw(5) << now.Healthy()
            << "-> " << std::setw(10);
  if (statistics.Size() > 1)
    switch (statistics.Trend(statistics.Size() - 1,
                             sir::Data::GROUP::Susceptible)) {
    case 0:
      std::cout << FOREGROUND_GREEN << " in decrescita " << RESET_COLOR;
      break;
    case 1:
      std::cout << FOREGROUND_MAGENTA << " costante " << RESET_COLOR;
      break;
    default:
      std::cout << FOREGROUND_RED << " in crescita " << RESET_COLOR;
      break;
    }
  std::cout << "\n";
  std::cout << " Popolazione infetta:    " << std::setw(5) << now.Infected()
            << "-> " << std::setw(10);
  if (statistics.Size() > 1)
    switch (
        statistics.Trend(statistics.Size() - 1, sir::Data::GROUP::Infected)) {
    case 0:
      std::cout << FOREGROUND_GREEN << " in decrescita " << RESET_COLOR;
      break;
    case 1:
      std::cout << FOREGROUND_MAGENTA << " costante " << RESET_COLOR;
      break;
    default:
      std::cout << FOREGROUND_RED << " in crescita " << RESET_COLOR;
      break;
    }
  std::cout << "\n";

  std::cout << " Popolazione rimossa:    " << std::setw(5) << now.Removed()
            << "-> " << std::setw(10);
  if (statistics.Size() > 1)
    switch (
        statistics.Trend(statistics.Size() - 1, sir::Data::GROUP::Removed)) {
    case 0:
      std::cout << FOREGROUND_GREEN << " in decrescita " << RESET_COLOR;
      break;
    case 1:
      std::cout << FOREGROUND_MAGENTA << " costante " << RESET_COLOR;
      break;
    default:
      std::cout << FOREGROUND_RED << " in crescita " << RESET_COLOR;
      break;
    }
  std::cout << "\n";
  std::cout << " ---------------------------------------------------- \n";
}

//@concept sviluppo dell'automa
inline bool Simulator(sir::Random_Evo &dates, random_evo::World &space) {
  try {
    assert(dates.Return_T() != 0);
    char g;
    std::cout << FOREGROUND_CYAN
              << "\n\n Si vuole visualizzare la griglia? (y/n) ";
    std::cin >> g;
    std::cout << RESET_COLOR << CLEAR_TERMINAL;
    for (int i = 0; i <= dates.Return_T(); i++) {
      if (g != 'n' && g != 'N') {
        random_evo::Print(space);

        std::cout << " >----------------------------------<\n"
                  << " |           ->LEGENDA<-            |              "
                     "-----------------\n"
                  << " |   " << random_evo::STATE_CELL::Healthy
                  << " -> Sani        " << random_evo::STATE_CELL::Recovered
                  << " -> Guariti  |              | GIORNO: " << std::setw(5)
                  << i << " |\n"
                  << " |   " << random_evo::STATE_CELL::Infected
                  << " -> Infetti     " << random_evo::STATE_CELL::Dead
                  << " -> Morti    |              -----------------\n"
                  << " >----------------------------------<\n";
        Print_distribution_Trend(dates, space);
      } else {
        if (i % 5 == 0)
          std::cout << FOREGROUND_RED << "\n Elaborazione... " << i << "\\"
                    << dates.Return_T() << RESET_COLOR;
      }
      dates.Add_Day(space.Healthy(), space.Infected(), space.Removed());
      space = random_evo::Evolve(space, dates.Return_gamma(),
                                 dates.Return_beta(), dates.Return_mortality());
      random_evo::Move(space);
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    std::cout << CLEAR_TERMINAL;
  } catch (std::runtime_error const &e) {
    std::cerr << e.what() << '\n';
    return false;
  }
  assert(dates.Size() != 0);
  return true;
}

//@concept  richiesta in input delle dimensioni della griglia
inline void Size(std::array<int, 2> &dim) {
  std::cout << '\n'
            << FOREGROUND_GREEN
            << "    |-> Inserire la larghezza dello spazio di movimento:  ";
  std::cin >> dim[0];
  std::cout << '\n'
            << "    |-> Inserire l'altezza dello spazio di movimento:  ";
  std::cin >> dim[1];
  std::cout << RESET_COLOR;
}

inline std::array<int, 2> menu_random() {
  std::array<int, 2> dim;
  std::cout << CLEAR_TERMINAL;
  std::cout << FOREGROUND_CYAN << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
            << "BENVENUTO!" << FOREGROUND_CYAN
            << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << RESET_COLOR
            << "\n\n";
  std::cout << FOREGROUND_RED << "------------------------------------------\n"
            << "|                 INFO                    |\n"
            << "| Visualizzazione grafica dello sviluppo  |\n"
            << "| di una epidemia inserite le probabilita | \n"
            << "| di contagio, guarigione e mortalita'.   |\n"
            << "| Lo sviluppo avviene tramite generazione |\n"
            << "| di numeri casuali per il movimento della|\n"
            << "| popolazione sana, infetta o guarita     |\n"
            << "-------------------------------------------\n\n\n"
            << RESET_COLOR;
  Size(dim);
  return dim;
}

#endif