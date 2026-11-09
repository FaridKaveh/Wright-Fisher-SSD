#ifndef TWO_DEME_WF
#define TWO_DEME_WF

#include <array>
#include <functional>
#include <random>
#include <vector>

class TwoDemeWF {

private:
  int resources;
  double size_param;
  double migration_prob;
  std::function<std::array<double, 2>(double, double, double, double, double,
                                      double)>
      migration_func;
  std::random_device rd; // true random seed
  // std::default_random_engine engine(rd()); // pseudo random generator

  // state = {type0_deme1, type1_deme1, type0_deme2, type1_deme2}
  std::array<int, 4> state;

  void set_state_initial(const double &x1, const double &x2);
  void refresh_state(const int &pop1, const int &pop2, const int &type0_deme1,
                     const int &type0_deme2) {
    this->state = {type0_deme1, pop1 - type0_deme1, type0_deme2,
                   pop2 - type0_deme2};
  }

  void fill_deme(const int &deme, std::bernoulli_distribution &own_dist,
                 std::bernoulli_distribution &other_dist,
                 std::bernoulli_distribution &migration_dist,
                 std::array<int, 4> &new_state,
                 std::default_random_engine &engine);

public:
  TwoDemeWF(const int &res, const double &size, const double &prob,
            const std::function<std::array<double, 2>(
                double, double, double, double, double, double)> &func)
      : resources(res), size_param(size), migration_prob(prob),
        migration_func(func) {}

  std::vector<std::array<int, 4>>
  generate_sample(const double &x1, const double &x2, const int &max_gen);

  bool check_fixation();
};

#endif
