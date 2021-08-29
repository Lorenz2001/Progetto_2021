#include "TApplication.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "graphs.hpp"

int main(int argc, char *argv[]) {

  TApplication theApp("App", &argc, argv);
  char x = 'n';
  while (x != 'Y' && x != 'y') {
    std::vector<int> sim;
    int c;
    std::cout
        << "\n\n INSERIRE I NUMERI IDENTIFICATIVI DELLE SIMULAZIONI (INSERIRE "
           "0 PER TERMINARE): ";
    std::cin >> c;
    while (c != 0) {
      sim.push_back(c);
      std::cout << "                                                           "
                   "        "
                   "             ";
      std::cin >> c;
    }
    for (int i = 0; i < static_cast<int>(sim.size()); i++) {
      std::cout << "Numero simulazione: " << sim[i] << "\n";

      Graphs(sim[i]);
      std::cout << "##############################################\n\n";
      if (i != static_cast<int>(sim.size()) - 1) {
        std::cout << " \n Visualizzare la simulazione successiva? (y/n->Exit) ";
        std::cin >> x;
        if ((c == 'N' || c == 'n') && i != (static_cast<int>(sim.size()) - 1))
          break;
      }
    }

    std::cout << "\n\n Terminare il programma?(y/n)";
    std::cin >> x;
  }
}