#include "/home/farid/Documents/git/Wright-Fisher-SSD/src/migration_funcs.h"
#include "/home/farid/Documents/git/Wright-Fisher-SSD/src/two-deme-wf.h"
#include "/home/farid/Documents/git/Wright-Fisher-SSD/src/utils.h"
#include <array>
#include <iostream>
#include <ostream>
#include <random>
#include <vector>

// std::array<double, 2> calc_fixed_back_migration_rates(double x1, double x2,
//                                                       double resources,
//                                                       double size_param,
//                                                       double gamma1 = 0.1,
//                                                       double gamma2 = 0.1) {
//   double N1 = resources / (x1 + (1 - x1) * size_param);
//   double N2 = resources / (x2 + (1 - x2) * size_param);

//   double m_two_to_one =
//       gamma2 * ((1 - gamma1) * N2 - gamma1 * N1) / ((1 - gamma1 - gamma2) *
//       N2);
//   double m_one_to_two =
//       gamma1 * ((1 - gamma2) * N1 - gamma2 * N2) / ((1 - gamma1 - gamma2) *
//       N1);
//   std::array<double, 2> back_migration_rates = {m_two_to_one, m_one_to_two};

//   return back_migration_rates;
// }

std::array<double, 2> calc_generic_migration(double x1, double x2,
                                             double resources,
                                             double size_param,
                                             double gamma1 = 0.1,
                                             double gamma2 = 0.1) {
  double N1 = 1 / (x1 + (1 - x1) * size_param);
  double N2 = 1 / (x2 + (1 - x2) * size_param);

  double m_two_to_one = gamma2 * N2 / (N1 + N2);
  double m_one_to_two = gamma1 * N1 / (N1 + N2);

  std::array<double, 2> back_migration_rates = {m_two_to_one, m_one_to_two};

  return back_migration_rates;
}
int main(int argc, char *argv[]) {

  if (argc != 7) {
    std::cout << "Incorrect usage." << std::endl;
    std::cout << "Usage: ./wf-ssd resources size_parameter migration_prob "
                 "no_samples max_gen migration_struct"
              << std::endl;

    return 1;
  }

  std::string file = static_cast<std::string>("data") + "R" +
                     static_cast<std::string>(argv[1]) + "size" +
                     static_cast<std::string>(argv[2]) + "migration" +
                     static_cast<std::string>(argv[3]) + ".txt";

  int resources = std::stoi(argv[1], nullptr, 10);
  double size = std::stod(argv[2], nullptr);
  double migration = std::stod(argv[3], nullptr);
  int samples = std::stoi(argv[4], nullptr, 10);
  int max_gen = std::stoi(argv[5], nullptr, 10);
  int migration_struct = std::stoi(argv[6], nullptr, 10);
  std::cout << migration_struct << std::endl;

  int type0_fixed = 0;
  int type1_fixed = 0;
  std::function<std::array<double, 2>(double, double, double, double, double,
                                      double)>
      f;
  switch (migration_struct) {
  case 1:
    f = calc_generic_migration;

  case 2:
    f = calc_pop_grad_migration_rates;
  default:

    f = calc_const_migration_rates;
  }

  TwoDemeWF process(resources, size, migration, f);
  int unfixed_runs = 0;
  for (int i = 0; i < samples; ++i) {
    std::vector<std::array<int, 4>> trajectory =
        process.generate_sample(0.5, 0.5, max_gen);

    if (process.check_fixation()) {
      if (trajectory.back()[1] == 0)
        ++type0_fixed;
      else
        ++type1_fixed;
    }

    else
      ++unfixed_runs;

    std::vector<std::vector<int>> data;
    data.reserve(trajectory.size());
    for (auto elem : trajectory) {
      std::vector<int> data_point(elem.begin(), elem.end());
      data.push_back(data_point);
    };

    // writeVecOfVecsToCSV(data, file, "app");
  }

  std::cout << "type0_fixation_prob = "
            << static_cast<double>(type0_fixed) / (type1_fixed + type0_fixed)
            << std::endl;
  std::cout << "unfixed_runs = " << unfixed_runs << std::endl;
  return 0;
}
