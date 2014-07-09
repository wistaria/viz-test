// Local Flip MCMC for XY Model

#include <boost/random.hpp>
#include <cmath>
#include <complex>
#include <iostream>
#include <fstream>
#include <vector>
#include <alps/lattice.h>
#include "xy_options.hpp"

int main(int argc, char* argv[]) {
  options p(argc, argv, false);
  if (!p.valid) std::exit(127);
  double temperature = p.temperature;
  unsigned int sweeps = p.sweeps;

  // filename
  std::string fname1("spins");
  std::string fname2("argument");
  
  // lattice
  alps::Parameters params;
  params["LATTICE"] = "square lattice";
  params["L"] = p.length;
  typedef alps::graph_helper<> lattice_t;
  lattice_t lattice(params);

  // random number generators
  boost::mt19937 eng(p.seed);
  boost::variate_generator<boost::mt19937&, boost::uniform_real<> >
    uniform_01(eng, boost::uniform_real<>());

  // spin configuration
  std::vector<std::complex<double> >spins(lattice.num_sites());
  for (unsigned int s = 0; s < lattice.num_sites(); ++s) {
    double theta = 2 * M_PI * uniform_01();
    spins[s] = std::polar(1.0, theta);
  }

  for (unsigned int mcs = 0; mcs < sweeps; ++mcs) {
    for (unsigned int i = 0; i < lattice.num_sites(); ++i) {
      int s = uniform_01() * lattice.num_sites();
      std::complex<double> env = 0;
      lattice_t::neighbor_iterator itr, itr_end;
      for (boost::tie(itr, itr_end) = lattice.neighbors(s); itr != itr_end; ++itr)
        env += spins[*itr];
      double theta = 2 * M_PI * uniform_01();
      std::complex<double> trial = std::polar(1.0, theta);
      if (uniform_01() < std::exp(real((trial - spins[s]) * conj(env)) / temperature))
        spins[s] = trial;
    }

    {
      std::ofstream of((fname1 + boost::lexical_cast<std::string>(mcs) + ".vtk").c_str());
      of << "# vtk DataFile Version 2.0\n"
         << "XY spins\n"
         << "ASCII\n"
         << "DATASET STRUCTURED_POINTS\n"
         << "DIMENSIONS " << p.length << ' ' << p.length << " 1\n"
         << "ORIGIN 0.0 0.0 0.0\n"
         << "SPACING 1.0 1.0 1.0\n\n"
         << "POINT_DATA " << lattice.num_sites() << "\n\n";
      of << "VECTORS spin float\n";
      for (unsigned int s = 0; s < lattice.num_sites(); ++s)
        of << real(spins[s]) << ' ' << imag(spins[s]) << " 0\n";
    }
    {
      std::ofstream of((fname2 + boost::lexical_cast<std::string>(mcs) + ".vtk").c_str());
      of << "# vtk DataFile Version 2.0\n"
         << "XY spins\n"
         << "ASCII\n"
         << "DATASET STRUCTURED_POINTS\n"
         << "DIMENSIONS " << p.length << ' ' << p.length << " 1\n"
         << "ORIGIN 0.0 0.0 0.0\n"
         << "SPACING 1.0 1.0 1.0\n\n"
         << "POINT_DATA " << lattice.num_sites() << "\n\n";
      of << "VECTORS argument float\n";
      for (unsigned int s = 0; s < lattice.num_sites(); ++s)
        of << arg(spins[s]) << " 0 0\n";
    }
  }
}
