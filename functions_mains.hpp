#ifndef FUNCTIONS_MAINS_HPP
#define FUNCTIONS_MAINS_HPP

#include "functions_random.hpp"

#include <algorithm>
#include <array>

//@concept creazione simulazione
inline auto New_Simulation() {
  using sir::OPTION;
  char c;

  try {
    while (1) {
      std::cout << FOREGROUND_CYAN
                << "\n Scelta dell'inserimento dei dati dell'epidemia:"
                << FOREGROUND_GREEN
                << " 1-Default: vengono inseriti i dati scelti di default \n"
                << FOREGROUND_GREEN
                << "                                                 2-Utente: "
                   "tutti i dati vengono scelti dall'utente \n"
                << RESET_COLOR;
      while (1) {
        std::cout << "     Scelta: ";
        std::cin >> c;
        if (c != '1' && c != '2') {
          std::cout << "\n"
                    << BACKGROUND_RED
                    << " La scelta non è valida! (inserire 1 o 2)"
                    << RESET_COLOR << "\n";
          std::cout << "\n";
        } else
          break;
      }
      auto pop((c == '1') ? OPTION::Default : sir::Upload_SIR());

      pop.Print_Info();
      std::cout << "\nConfermare i dati? (y/n/A='Abort evolution') ";
      char w;
      std::cin >> w;
      if (w == 'y' || w == 'Y') {
        return pop;
      }
      if (w == 'a' || w == 'A') {
        if (pop.Abort(true) == true)
          return pop;
        else {
          pop.Abort(false);
          continue;
        }
      }
      if (w == 'N' || w == 'n') {
        pop.Abort(false);
        continue;
      } else
        std::cout << BACKGROUND_RED << " ERROR" << RESET_COLOR << "\n";
    }
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
  }
  sir::Population x;
  return x;
}

//@concept Menu iniziale
inline bool menu1() {
  char choice;
  std::cout << CLEAR_TERMINAL;
  std::cout << "\n"
            << FOREGROUND_CYAN << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
            << "BENVENUTO!" << FOREGROUND_CYAN
            << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << RESET_COLOR
            << "\n\n";
  while (1) {
    std::cout
        << FOREGROUND_CYAN
        << "Selezionare una delle due opzioni (inserire 1 oppure 2):\n"
        << RESET_COLOR << "                 " << FOREGROUND_YELLOW
        << "1 - Simulazione unica: il programma termina una volta stampati "
           "a schermo i risultati \n"
        << RESET_COLOR << "                 " << FOREGROUND_YELLOW
        << "2 - Più simulazioni \n"
        << RESET_COLOR << "     " << FOREGROUND_CYAN << "Scelta:" << RESET_COLOR
        << " ";
    std::cin >> choice;
    if (choice != '1' && choice != '2') {
      std::cout << "\n"
                << BACKGROUND_RED
                << " La scelta non è valida! (inserire 1 o 2)\n"
                << RESET_COLOR;
      std::cout << "\n";
    } else
      break;
  }

  if (choice == '1') {
    std::cout << CLEAR_TERMINAL;
    std::cout << "\n\n";
    std::cout << "\n Tipologia di simulazione: 1 - Simulazione secondo "
              << "il modello" << FOREGROUND_CYAN
              << " SIR\n                           " << RESET_COLOR
              << "2 - Simulazione con movimento casuale (" << FOREGROUND_MAGENTA
              << "automa" << RESET_COLOR << ")\n "
              << "      SCELTA: ";
    char x;
    std::cin >> x;
    if (x == '1') {
      auto pop{New_Simulation()};
      assert(pop.Return_id() != 0);
      pop.Evolution(true);
      std::cout << "\n";
      pop.Print();
    } else {
      sir::Random_Evo evolution{sir::Upload_Rand()};
      std::array<int, 2> dim;
      Size(dim);
      random_evo::World simulation(dim[1], dim[0], evolution.Return_N(),
                                   evolution.Return_I0());
      bool control = Simulator(evolution, simulation);
      assert(control == true);
      std::cout << CLEAR_TERMINAL;
      evolution.Print_r();
    }
    return false;
  }
  return true;
}

//@concept Menu principale
inline int menu2() {
  int choice;
  std::cout << CLEAR_TERMINAL;
  std::cout << FOREGROUND_BLUE << BACKGROUND_WHITE
            << "\n                  ->CREAZIONE ED EVOLUZIONE DI PIU' "
               "SIMULAZIONI<-                  \n\n"
            << RESET_COLOR;
  std::cout << "     1-Creazione simulazioni\n";
  std::cout << "     2-Evoluzione simulazioni\n";
  std::cout << "     3-Visualizza risultati simulazioni\n";
  std::cout << "     4-Visualizza confronto simulazioni\n";
  std::cout << "     5-Stampare simulazioni evolute su file \n\n";
  std::cout << "   Scelta: ";
  std::cin >> choice;
  return choice;
}

//@concept attuazione scelta menu principale
inline void
Multiple_simulations(std::map<sir::Population, int> &simulations,
                     std::map<sir::Population, int> &simulations_evolved,
                     std::map<sir::Population, int> &simulations_not_evolved) {
  using Sims = std::map<sir::Population, int>;
  switch (menu2()) {
  case 1: { //@concept creazione simulazioni
    std::cout << FOREGROUND_GREEN
              << "###########################################################"
                 "############################ \n\n"
              << RESET_COLOR;
    int n_sim;
    std::cout << "Quante simulazioni si vogliono creare? ";
    std::cin >> n_sim;
    std::cout << "\n";
    for (int i = 0; i < n_sim; i++) { //@context creazione simulazione
      std::cout << "\n\n\n"
                << FOREGROUND_GREEN << CLEAR_TERMINAL << " SIMULAZIONE NUMERO "
                << i + 1 << " di " << n_sim << RESET_COLOR << "\n";
      auto pop{New_Simulation()};

      if (pop.Return_id() != 0) {
        simulations[pop] = 0;
        std::cout
            << "\n\nSi vuole far evolvere l'epidemia? (y/n) {E' possibile "
               "eseguire questo comando in un secondo momento dal menu "
               "principale} ";
        char choice;
        std::cin >> choice;
        if (choice == 'y' || choice == 'Y') {
          pop.Evolution(true);
          simulations[pop] =
              (pop.Return_T() == 0) ? pop.Size() : pop.Return_T();
          simulations_evolved[pop] =
              (pop.Return_T() == 0) ? pop.Size() : pop.Return_T();
        } else
          simulations_not_evolved[pop] = 0;
      }
      char choice;
      if (i != n_sim - 1) {
        std::cout << "\n\n Continuare con le simulazioni? (y/n) ";
        std::cin >> choice;
        if (choice == 'N' || choice == 'n')
          break;
      }
    }
    std::cout << "\n"
              << "\n"
              << "###########################################################"
                 "############################";

  } break;
  case 2: {
    char choice;
    std::cout << FOREGROUND_GREEN
              << "###########################################################"
                 "############################ \n\n"
              << RESET_COLOR;
    if (simulations_not_evolved.empty() == true) {
      std::cout << FOREGROUND_RED
                << " TUTTE LE SIMULAZIONI CREATE SONO STATE "
                   "PROCESSATE "
                << RESET_COLOR << "\n";
      break;
    }
    std::cout << "\n"
              << "\n"
              << "Visualizzare le informazioni di tutte le simulazioni "
                 "ancora da far evolvere? (y/n)";
    std::cin >> choice;

    std::vector<int> values;
    std::for_each(simulations_not_evolved.begin(),
                  simulations_not_evolved.end(),
                  [&, choice](auto &to_print) -> void {
                    if (choice == 'y' || choice == 'Y') {
                      sir::Population x(to_print.first);
                      x.Print_Info();
                      std::cout << "\n"
                                << "\n";
                    }
                    values.push_back(to_print.first.Return_id());
                  });

    int c;
    std::vector<int> evo;
    std::cout
        << "Scrivere i numeri identificativi delle simulazioni premendo "
           "invio per ogni scelta. Per terminare l'inserimento inserire 0";
    std::cout << "\n"
              << " Scelte: ";
    std::cin >> c;
    while (c != 0) {
      std::vector<int>::iterator iv =
          std::find(values.begin(), values.end(), c);
      if (iv == values.end()) {
        std::cout << FOREGROUND_RED
                  << " ATTENZIONE: IL NUMERO INSERITO NON "
                     "CORRISPONDE A NESSUNA SIMULAZIONE "
                  << RESET_COLOR;
      } else
        evo.push_back(c);
      std::cout << "         ";
      std::cin >> c;
    }
    if (evo.empty() == false) {
      std::sort(evo.begin(), evo.end());
      Sims::iterator it = simulations_not_evolved.begin();
      int i = 0;
      int k = evo.size();
      while (i < k) {
        sir::Population h(it->first);
        if (h.Return_id() == evo[i]) {
          std::cout << "\n"
                    << FOREGROUND_GREEN
                    << "EVOLUZIONE DELLA SIMULAZIONE N°: " << h.Return_id()
                    << RESET_COLOR << "\n";

          //@context SCELTA DEL TIPO DI EVOLUZIONE
          std::cout << "\n Tipologia di simulazione: 1 - Simulazione secondo "
                    << "il modello SIR;\n                           2 - "
                    << "Simulazione con movimento casuale.\n "
                    << "      SCELTA: ";
          char x;
          std::cin >> x;
          if (x == '2') {
            auto to_evolve = std::make_unique<sir::Random_Evo>(
                h.Return_N(), h.Return_I0(), h.Return_gamma(), h.Return_beta(),
                0.3, (h.Return_T() == 0) ? 150 : h.Return_T());
            std::array<int, 2> dim;
            Size(dim);
            random_evo::World simulation(dim[1], dim[0], to_evolve->Return_N(),
                                         to_evolve->Return_I0());
            bool control = Simulator(*to_evolve, simulation);
            assert(control == true);
            to_evolve->Transfer(h);
          } else
            h.Evolution(true);

          simulations_evolved[h] =
              (h.Return_T() == 0) ? h.Size() : h.Return_T();
          simulations.erase(h);
          simulations[h] = (h.Return_T() == 0) ? h.Size() : h.Return_T();
          simulations_not_evolved.erase(it);
          i++;
        }
        it++;
        std::cout << "\n"
                  << "\n";
      }
    }
    std::cout << "\n###########################################################"
                 "############################";
  } break;
  case 3: {
    char choice;
    std::cout << FOREGROUND_GREEN
              << "###########################################################"
                 "############################ \n\n"
              << RESET_COLOR;
    if (simulations_evolved.empty() == true) {
      std::cout << FOREGROUND_RED << " NON CI SONO SIMULAZIONI DA VISUALIZZARE "
                << RESET_COLOR << "\n";
      break;
    }
    std::cout << "\n"
              << "\n"
              << "Visualizzare le informazioni di tutte le simulazioni già "
                 "evolute? (y/n)";
    std::cin >> choice;

    std::vector<int> values;
    std::for_each(simulations_evolved.begin(), simulations_evolved.end(),
                  [&, choice](auto &to_print) -> void {
                    if (choice == 'y' || choice == 'Y') {
                      sir::Population x(to_print.first);
                      x.Print_Info();
                      std::cout << "\n"
                                << "\n";
                    }
                    values.push_back(to_print.first.Return_id());
                  });

    int c;
    std::cout << "Scrivere il numero identificativo della simulazione da "
                 "visualizzare";
    std::cout << "\n"
              << " Scelta: ";
    std::cin >> c;
    while (c != 0) {
      std::vector<int>::iterator iv =
          std::find(values.begin(), values.end(), c);
      if (iv == values.end()) {
        std::cout << FOREGROUND_RED
                  << " ATTENZIONE: IL NUMERO INSERITO NON "
                     "CORRISPONDE A NESSUNA SIMULAZIONE "
                  << RESET_COLOR << "\n";
      } else
        break;
      std::cout << "\n"
                << " Scelta: ";
      std::cin >> c;
    }

    Sims::iterator it = simulations_evolved.begin();
    bool control = true;
    while (control == true) {
      sir::Population h(it->first);
      if (h.Return_id() == c) {
        std::cout << "\n"
                  << "SIMULAZIONE N°: " << h.Return_id() << "\n";
        h.Print_Info();
        h.Print();
        control = false;
      }
      it++;
      std::cout << "\n"
                << "\n";
    }
    std::cout << "\n"
              << "###########################################################"
                 "############################";

  } break;
  case 4: {
    char choice;
    std::cout << FOREGROUND_GREEN
              << "###########################################################"
                 "############################ \n\n"
              << RESET_COLOR;
    if (simulations_evolved.empty() == true) {
      std::cout << FOREGROUND_RED << " NON CI SONO SIMULAZIONI DA VISUALIZZARE "
                << RESET_COLOR << "\n";
      break;
    }
    std::cout << "\n"
              << "\n"
              << "Visualizzare le informazioni di tutte le simulazioni "
                 "confrontabili? (y/n)";
    std::cin >> choice;
    std::vector<int> values;
    std::for_each(simulations_evolved.begin(), simulations_evolved.end(),
                  [&, choice](auto &to_print) -> void {
                    if (choice == 'y' || choice == 'Y') {
                      sir::Population x(to_print.first);
                      x.Print_Info();
                      std::cout << "\n"
                                << "\n";
                    }
                    values.push_back(to_print.first.Return_id());
                  });
    int c;
    std::vector<int> evo;
    std::cout
        << "Scrivere i numeri identificativi delle simulazioni premendo "
           "invio per ogni scelta. Per terminare l'inserimento inserire 0";
    std::cout << "\n"
              << " Scelte: ";
    std::cin >> c;
    while (c != 0) {
      std::vector<int>::iterator iv =
          std::find(values.begin(), values.end(), c);
      if (iv == values.end()) {
        std::cout << FOREGROUND_RED
                  << " ATTENZIONE: IL NUMERO INSERITO NON "
                     "CORRISPONDE A NESSUNA SIMULAZIONE "
                  << RESET_COLOR << "\n";
      } else
        evo.push_back(c);
      std::cout << "         ";
      std::cin >> c;
    }
    if (evo.empty() != true) {
      std::sort(evo.begin(), evo.end());
      Sims comparison;
      long unsigned int i = 0;
      for (auto &sim : simulations_evolved) {
        auto h = std::make_unique<sir::Population>(sim.first);
        if (h->Return_id() == evo[i]) {
          comparison[*h] = (h->Return_T() == 0) ? h->Size() : h->Return_T();
          if (i < evo.size())
            i++;
          else
            break;
        }
      }
      Print(comparison);
    }
    std::cout << "###########################################################"
                 "############################";
  } break;
  case 5: {
    char choice;
    std::cout << FOREGROUND_GREEN
              << "###########################################################"
                 "############################ \n\n"
              << RESET_COLOR;
    if (simulations_evolved.empty() == true) {
      std::cout << FOREGROUND_RED
                << " NON CI SONO SIMULAZIONI DA STAMPARE SU FILE "
                   "(prima devono evolversi) \n"
                << RESET_COLOR;
      break;
    }
    std::cout << "\n"
              << "\n"
              << "Visualizzare le informazioni di tutte le simulazioni "
                 "stampabili? (y/n)";
    std::cin >> choice;

    std::vector<int> values;
    std::for_each(simulations_evolved.begin(), simulations_evolved.end(),
                  [&, choice](auto &to_print) -> void {
                    if (choice == 'y' || choice == 'Y') {
                      sir::Population x(to_print.first);
                      x.Print_Info();
                      std::cout << "\n\n";
                    }
                    values.push_back(to_print.first.Return_id());
                  });
    int c;
    std::vector<int> wtp;
    std::cout
        << "Scrivere i numeri identificativi delle simulazioni premendo "
           "invio per ogni scelta. Per terminare l'inserimento inserire 0";
    std::cout << "\n"
              << " Scelte: ";
    std::cin >> c;
    while (c != 0) {
      std::vector<int>::iterator iv =
          std::find(values.begin(), values.end(), c);
      if (iv == values.end()) {
        std::cout << FOREGROUND_RED
                  << " ATTENZIONE: IL NUMERO INSERITO NON "
                     "CORRISPONDE A NESSUNA SIMULAZIONE "
                  << RESET_COLOR << "\n";
      } else
        wtp.push_back(c);
      std::cout << "         ";
      std::cin >> c;
    }
    if (wtp.empty() == false) {
      std::cout << "\n"
                << "STAMPA SU FILE IN CORSO... "
                << "\n";
      std::sort(wtp.begin(), wtp.end());
      long unsigned int i = 0;
      for (auto &sim : simulations_evolved) {
        auto h = std::make_unique<sir::Population>(sim.first);
        if (h->Return_id() == wtp[i]) {
          h->Print_on_file();
          if (i < wtp.size())
            i++;
          else
            break;
        }
      }
    }
    std::cout << "\n"
              << "###########################################################"
                 "############################";
  } break;
  default: {
    std::cout << "\n"
              << FOREGROUND_RED << " ERRORE: La scelta inserita non è valida "
              << RESET_COLOR << "\n";
  } break;
  }

  return;
}
#endif