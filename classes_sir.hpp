#ifndef CLASSES_SIR_HPP
#define CLASSES_SIR_HPP

#include "consts.hpp"

#include <map>
#include <stdexcept>
#include <vector>

namespace sir {

int const DEFAULT_N = 50000;
int const DEFAULT_T = 50;
int const DEFAULT_d_T = 1;
int const DEFAULT_I0 = 1000;
double const DEFAULT_beta = 0.3;
double const DEFAULT_gamma = 0.1;

struct Data {
  enum class GROUP { Susceptible, Infected, Removed };
  double s; // numero suscettibili
  double i; // numero infetti
  double r; // numero rimossi
};

enum class OPTION { Default, User };

class Population {

private:
  static int counter;
  Data fct_new(Data &old);
  int Choice();

protected:
  std::vector<Data> world;
  int N;
  int T;
  int d_T;
  int I0;
  double gamma;
  double beta;
  double original_gamma;
  double original_beta;
  int id = 0;
  bool evolved = false;
  int time_extinction = 0;
  int change;

public:
  //@context COSTRUCTORS
  Population(OPTION o) {
    if (o != OPTION::Default)
      throw std::runtime_error{"OPZIONE NON RICONOSCIUTA"};
    counter++;
    id = counter;
    time_extinction = 0;
    evolved = false;
    N = DEFAULT_N;
    T = DEFAULT_T;
    d_T = DEFAULT_d_T;
    I0 = DEFAULT_I0;
    gamma = DEFAULT_gamma;
    beta = DEFAULT_beta;
    original_beta = DEFAULT_beta;
    original_gamma = DEFAULT_gamma;
  }

  Population(int n, int t, int i0, double g, double b)
      : N{n}, T{t}, I0{i0}, gamma{g}, beta{b} {
    counter++;
    id = counter;
    d_T = 1;
    time_extinction = 0;
    evolved = false;
    original_beta = b;
    original_gamma = g;
  }

  Population() {
    counter++;
    id = counter;
  }

  int Evolution(bool q);

  void Print() const;
  void Print(int start, int end) const;
  void Print_Info();
  void Print_on_file() const;
  void Print_rand() const;

  bool Abort(bool control);

  int Return_id() const { return id; };
  int Return_T() const { return T; };
  int Return_N() const { return N; };
  int Return_I0() const { return I0; };
  int Return_d_T() const { return d_T; };
  double Return_gamma() const { return gamma; };
  double Return_beta() const { return beta; };
  double Return_original_gamma() const { return original_gamma; };
  double Return_original_beta() const { return original_beta; };
  int Size() const { return world.size(); }
  int Trend(int day, Data::GROUP type_group) const;

  friend bool operator>(const Population &a, const Population &b) noexcept;
  friend bool operator<(const Population &a, const Population &b) noexcept;
  friend bool operator==(const Population &a, const Population &b) noexcept;
  friend Population Upload_SIR();
  friend void Print(std::map<Population, int> &x);
  friend class Random_Evo;
};

class Random_Evo : public Population {
private:
  double mortality;

public:
  Random_Evo(int n, int i0, double g, double b, double m, int t)
      : Population{}, mortality{m} {
    N = n;
    I0 = i0;
    gamma = g;
    beta = b;
    original_beta = b;
    original_gamma = g;
    T = t;
  };
  Random_Evo() : Population{} {};

  double Return_mortality() const { return mortality; }
  void Add_Day(double s, double i, double r);
  void Transfer(Population &a);

  void Print_r() const;

  friend Random_Evo Upload_Rand();
};

bool operator<(const Population &a, const Population &b) noexcept;
bool operator>(const Population &a, const Population &b) noexcept;
bool operator==(const Population &a, const Population &b) noexcept;

void Print(std::map<Population, int> &x);

Population Upload_SIR();

Random_Evo Upload_Rand();

} // namespace sir

#endif