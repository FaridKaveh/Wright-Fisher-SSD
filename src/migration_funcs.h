#ifndef MIGRATION_FUNCS
#define MIGRATION_FUNCS

#include <array>
#include <cmath>

std::array<double, 2>
calc_fixed_back_migration_rates(const double x1, const double x2,
                                const double resources, const double size_param,
                                const double gamma1 = 0.1,
                                const double gamma2 = 0.1);

std::array<double, 2> calc_pop_grad_migration_rates(const double x1,
                                                    const double x2,
                                                    const double resources,
                                                    const double size_param,
                                                    const double gamma1 = 0.1,
                                                    const double gamma2 = 0.1);

std::array<double, 2> calc_const_migration_rates(const double x1,
                                                 const double x2,
                                                 const double resources,
                                                 const double size_param,
                                                 const double gamma1 = 0.1,
                                                 const double gamma2 = 0.1);

#endif
