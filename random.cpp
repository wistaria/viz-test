// Random number test

#include <boost/random.hpp>
#include <cmath>
#include <iostream>
#include "random_options.hpp"

int main(int argc, char* argv[]) {
  options p(argc, argv, false);
  if (!p.valid) std::exit(127);
  unsigned int sweeps = p.sweeps;

  // random number generators
  boost::random::linear_congruential_engine<uint32_t, 65539, 0, 2147483647> eng(p.seed);

  std::cout << "X,Y,Z,id" << std::endl;
  for (unsigned int mcs = 0; mcs < sweeps; ++mcs) {
    double x = eng() / 2147483647.;
    double y = eng() / 2147483647.;
    double z = eng() / 2147483647.;
    std::cout << x << ',' << y << ',' << z << ',' << mcs << std::endl;
  }
}
