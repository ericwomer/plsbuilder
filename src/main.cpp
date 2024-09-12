#include <cstdlib>
#include <exception>
#include <iostream>

#include "builder/builder.hpp"

using std::cerr;
using std::endl;

ew::builder PlsBuilder;

// Main entry point
int         main(int argv, char *argc[])
{
  try {
    if (argv == 1) {
      cerr << "Error: At least one paramater needs to be specified." << endl;
      PlsBuilder.setAppName(argc[ 0 ]);
      PlsBuilder.help();
      return EXIT_FAILURE;
    } else {
      PlsBuilder.main(argv, argc);
    }
    return EXIT_SUCCESS;
  } catch (const std::exception &ex) {
    cerr << "\tApplication encountered an exception: " << ex.what() << '\n';
    return EXIT_FAILURE;
  }
}
