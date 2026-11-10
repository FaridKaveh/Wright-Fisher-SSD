
#include <array>
#include <assert.h>
#include <cmath>
#include <vector>

std::array<double, 2>
calc_fixed_back_migration_rates(const double x1, const double x2,
                                const double resources, const double size_param,
                                const double gamma1 = 0.1,
                                const double gamma2 = 0.1) {
  double N1 = resources / (x1 + (1 - x1) * size_param);
  double N2 = resources / (x2 + (1 - x2) * size_param);

  double m_two_to_one =
      gamma2 * ((1 - gamma1) * N2 - gamma1 * N1) / ((1 - gamma1 - gamma2) * N2);
  double m_one_to_two =
      gamma1 * ((1 - gamma2) * N1 - gamma2 * N2) / ((1 - gamma1 - gamma2) * N1);
  std::array<double, 2> back_migration_rates = {m_two_to_one, m_one_to_two};

  return back_migration_rates;
}

std::array<double, 2> calc_pop_grad_migration_rates(const double x1,
                                                    const double x2,
                                                    const double resources,
                                                    const double size_param,
                                                    const double gamma1 = 0.1,
                                                    const double gamma2 = 0.1) {
  double type0_freq_deme1 = 1 - x1 > 0 ? 1 - x1 : 0;
  double type0_freq_deme2 = 1 - x2 > 0 ? 1 - x2 : 0;
  double delta = 1 - size_param;
  double beta = gamma1 * std::pow(size_param, 3) /
                ((2 * delta * (1 + std::pow(size_param, 3))));
  double grad1 = 2 * (type0_freq_deme1 - type0_freq_deme2) *
                 std::pow(delta, 2) /
                 (std::pow(1 - delta * type0_freq_deme1, 3) *
                  (1 - delta * type0_freq_deme1));
  double grad2 = 2 * (type0_freq_deme2 - type0_freq_deme1) *
                 std::pow(delta, 2) /
                 (std::pow(1 - delta * type0_freq_deme2, 3) *
                  (1 - delta * type0_freq_deme1));

  double m_two_to_one = gamma1 - beta * grad1;
  double m_one_to_two =
      gamma1 - beta * grad2; // both formulas are supposed to use gamma1

  assert(m_two_to_one >= 0 && m_two_to_one <= 1);
  assert(m_one_to_two >= 0 && m_one_to_two <= 1);
  std::array<double, 2> back_migration_rates = {m_two_to_one, m_one_to_two};

  return back_migration_rates;
}

std::array<double, 2> calc_const_migration_rates(const double x1,
                                                 const double x2,
                                                 const double resources,
                                                 const double size_param,
                                                 const double gamma1 = 0.1,
                                                 const double gamma2 = 0.1) {
  std::array<double, 2> back_migration_rates = {gamma1, gamma2};
  return back_migration_rates;
}
