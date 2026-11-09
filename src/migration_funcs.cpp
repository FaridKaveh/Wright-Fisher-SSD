#include <array>
#include <vector>

std::array<double, 2> calc_fixed_back_migration_rates(double x1, double x2,
                                                      double resources,
                                                      double size_param,
                                                      double gamma1 = 0.1,
                                                      double gamma2 = 0.1) {
  double N1 = resources / (x1 + (1 - x1) * size_param);
  double N2 = resources / (x2 + (1 - x2) * size_param);

  double m_two_to_one =
      gamma2 * ((1 - gamma1) * N2 - gamma1 * N1) / ((1 - gamma1 - gamma2) * N2);
  double m_one_to_two =
      gamma1 * ((1 - gamma2) * N1 - gamma2 * N2) / ((1 - gamma1 - gamma2) * N1);
  std::array<double, 2> back_migration_rates = {m_two_to_one, m_one_to_two};

  return back_migration_rates;
}

std::array<double, 2> calc_generic_migration(double x1, double x2,
                                             double resources,
                                             double size_param,
                                             double gamma1 = 0.1,
                                             double gamma2 = 0.1) {
  double N1 = resources / (x1 + (1 - x1) * size_param);
  double N2 = resources / (x2 + (1 - x2) * size_param);

  double m_two_to_one = gamma2 * N2 / (N1 + N2);
  double m_one_to_two = gamma1 * N1 / (N1 + N2);

  std::array<double, 2> back_migration_rates = {m_two_to_one, m_one_to_two};

  return back_migration_rates;
}

std::array<double, 2>
calc_net_zero_offspring_migration_rates(double x1, double x2, double resources,
                                        double size_param, double gamma1 = 0.1,
                                        double gamma2 = 0.1) {
  double fac1 = 1 - (1 - size_param) * (1 - x1);
  double fac2 = 1 - (1 - size_param) * (1 - x2);

  double m_two_to_one = fac1 * gamma2;
  double m_one_to_two = fac2 * gamma1;

  std::array<double, 2> back_migration_rates = {m_two_to_one, m_one_to_two};

  return back_migration_rates;
}
