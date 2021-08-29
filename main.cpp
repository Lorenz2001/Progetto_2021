#include "functions_mains.hpp"

#include <iostream>
#include <map>

int main() {
  if (menu1() == false)
    return 0; //@concept il programma termina essendo stata scelta la
              //@concept simulazione unica

  using Sims = std::map<sir::Population, int>;

  Sims simulations;
  Sims simulations_evolved;
  Sims simulations_not_evolved;

  while (1) {

    Multiple_simulations(simulations, simulations_evolved,
                         simulations_not_evolved);

    //@context chiusura programma (ad ogni scelta)
    std::cout << "\n\nChiudere il programma? (y/n) ";
    char w;
    std::cin >> w;
    if (w == 'N' || w == 'n')
      continue;
    if (w == 'y' || w == 'Y')
      return 0;
  }
  return 0;
}