#include "functions_random.hpp"

#include <array>
#include <fstream>
#include <iostream>

int main(int argc, char *argv[]) {
  if (argc == 1) {
    while (1) {
      std::array<int, 2> const size = menu_random();
      std::cout << "\n\n";
      sir::Random_Evo evolution{sir::Upload_Rand()};
      random_evo::World simulation(size[1], size[0], evolution.Return_N(),
                                   evolution.Return_I0());
      bool control = Simulator(evolution, simulation);
      assert(control == true);
      std::cout << CLEAR_TERMINAL;
      evolution.Print_r();
      std::cout << FOREGROUND_GREEN
                << "\n\n\n RIPETERE UNA SIMULAZIONE? (y/n) ";
      char c;
      std::cin >> c;
      std::cout << RESET_COLOR;
      if (c != 'y' && c != 'Y')
        break;
    }

  } else //@concept lettura parametri da file
  {
    std::ifstream in;
    for (int j = 1; j < argc; j++) {
      std::string name_file = argv[j];
      std::cout << CLEAR_TERMINAL << FOREGROUND_GREEN
                << "\n-------------SIMULAZIONE N° " << j << ": " << name_file
                << "-------------------/n/n" << RESET_COLOR;
      in.open(name_file);
      std::vector<double> x(6);
      std::array<int, 2> dim;
      /*
      [0]=popolazione totale
      [1]=infetti iniziali
      [2]=gamma
      [3]=beta
      [4]=mortalità
      [5]=Tempo
      */
      for (int i = 0; i < 6; i++) {
        in >> x[i];
      }
      in.close();
      sir::Random_Evo evolution(x[0], x[1], x[2], x[3], x[4], x[5]);
      evolution.Print_Info();
      Size(dim);
      random_evo::World simulation(dim[1], dim[0], evolution.Return_N(),
                                   evolution.Return_I0());
      assert(Simulator(evolution, simulation) == true);
      std::cout << CLEAR_TERMINAL;
      evolution.Print_r();
      char c;
      if (j != argc - 1) {
        std::cout
            << "\n\n Visualizzare la simulazione successiva? (y/n->Exit) ";
        std::cin >> c;
        if (c == 'N' || c == 'n')
          break;
      }
    }
  }
}