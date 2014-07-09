#include <boost/lexical_cast.hpp>
#include <iostream>

struct options {
  unsigned int seed, sweeps;
  bool valid;
  options(unsigned int argc, char *argv[], bool print = true) :
    seed(29833), sweeps(1 << 12), valid(true) {
    for (unsigned int i = 1; i < argc; ++i) {
      switch (argv[i][0]) {
      case '-' :
        switch (argv[i][1]) {
        case 's' :
          if (++i == argc) { usage(print); return; }
          seed = boost::lexical_cast<unsigned int>(argv[i]); break;
        case 'n' :
          if (++i == argc) { usage(print); return; }
          sweeps = boost::lexical_cast<unsigned int>(argv[i]); break;
        case 'h' :
          usage(print, std::cout); return;
        default :
          usage(print); return;
        }
        break;
      default :
        usage(print); return;
      }
    }
    if (sweeps <= 0) {
      std::cerr << "invalid parameter(s)\n"; usage(print); return;
    }
    if (print) {
       std::cout << "Seed of RNG            = " << seed << std::endl
                 << "Number of Points       = " << sweeps << std::endl;
     }
   }
   void usage(bool print, std::ostream& os = std::cerr) {
     if (print)
       os << "[command line options]\n\n"
          << "  -s int    Seed of RNG\n"
          << "  -n int    Number of Points\n"
          << "  -h        this help\n\n";
     valid = false;
   }
 };
