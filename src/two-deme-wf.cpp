#include "/home/farid/Documents/git/Wright-Fisher-SSD/src/two-deme-wf.h"
#include <array>
#include <cmath>
#include <iostream>
#include <random> //random generation tools
#include <vector>

void TwoDemeWF::set_state_initial(const double &x1, const double &x2) {
  double size_param = this->size_param;

  int pop1 = std::floor((this->resources) / (1 - (1 - size_param) * x1));
  int pop2 = std::floor((this->resources) / (1 - (1 - size_param) * x2));

  int type0_deme1 = std::floor(pop1 * x1);
  int type0_deme2 = std::floor(pop2 * x2);

  this->state = {type0_deme1, pop1 - type0_deme1, type0_deme2,
                 pop2 - type0_deme2};
}

bool TwoDemeWF::check_fixation() {
  std::array<int, 4> current_state(this->state);
  bool type0_fixed = current_state[1] == 0 && current_state[3] == 0;
  bool type1_fixed = current_state[0] == 0 && current_state[2] == 0;
  return (type0_fixed || type1_fixed);
}

void TwoDemeWF::fill_deme(const int &deme,
                          std::bernoulli_distribution &own_dist,
                          std::bernoulli_distribution &other_dist,
                          std::bernoulli_distribution &migration_dist,
                          std::array<int, 4> &new_state,
                          std::default_random_engine &engine) {
  int offset = deme == 1 ? 0 : 2;
  double usage = 0;

  while (usage < this->resources) {
    if (migration_dist(engine)) {
      int type = other_dist(engine) ? 1 : 0;
      double type_cost = type == 0 ? this->size_param : 1;

      usage += type_cost;

      new_state[type + offset] = usage <= this->resources
                                     ? new_state[type + offset] + 1
                                     : new_state[type + offset];

    } else {
      int type = own_dist(engine) ? 1 : 0;
      double type_cost = type == 0 ? this->size_param : 1;

      usage += type_cost;

      new_state[type + offset] = usage <= this->resources
                                     ? new_state[type + offset] + 1
                                     : new_state[type + offset];
    }
  }
}

std::vector<std::array<int, 4>> TwoDemeWF::generate_sample(const double &x1,
                                                           const double &x2,
                                                           const int &max_gen) {

  std::vector<std::array<int, 4>> sample_trajectory;

  this->set_state_initial(x1, x2);

  std::default_random_engine engine(this->rd()); // pseudo random generator

  // std::bernoulli_distribution migration_dist(this->migration_prob);
  int gen = 0;
  do {
    // while neither type is fixed

    std::array<int, 4> current_state(this->state);
    std::array<int, 4> new_state({0, 0, 0, 0});

    double type1_freq_deme1 = static_cast<double>(current_state[1]) /
                              (current_state[0] + current_state[1]);

    std::bernoulli_distribution sampling_dist_deme1(type1_freq_deme1);

    double type1_freq_deme2 = static_cast<double>(current_state[3]) /
                              (current_state[2] + current_state[3]);

    std::bernoulli_distribution sampling_dist_deme2(type1_freq_deme2);

    std::array<double, 2> back_migration_rates(migration_func(
        type1_freq_deme1, type1_freq_deme2, this->resources, this->size_param,
        this->migration_prob, this->migration_prob));
    std::bernoulli_distribution migration_two_to_one(back_migration_rates[0]);
    std::bernoulli_distribution migration_one_to_two(back_migration_rates[1]);

    this->fill_deme(1, sampling_dist_deme1, sampling_dist_deme2,
                    migration_two_to_one, new_state, engine);
    this->fill_deme(2, sampling_dist_deme2, sampling_dist_deme1,
                    migration_one_to_two, new_state, engine);

    this->state = new_state;
    sample_trajectory.push_back(this->state);
    ++gen;
  } while (!(this->check_fixation()) && gen <= max_gen);

  return sample_trajectory;
}
