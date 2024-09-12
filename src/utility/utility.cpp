#include <iostream>
#include <fstream>

#include "utility.hpp"

using std::cerr;
using std::cout;
using std::ofstream;
using std::string;
using std::filesystem::path;

namespace ew {

int writeFile(path filename, string &contents, verbosity verbose_output)
{
  ofstream out;

  if (verbose_output == verbosity::developer) {
    std::cout << "filename: " << filename << '\n';
  }

  // Close it incase it was left open.
  if (out.is_open()) {
    out.close();
  }

  if (verbose_output == verbosity::developer) {
    cout << "Opening " << filename << " for writing\n";
  }
  out.open(filename);

  if (!out) {
    cerr << "Could not open " << filename << '\n';
    return EXIT_FAILURE;
  }

  if (verbose_output == verbosity::developer) {
    cout << "Writing to " << filename << '\n';
  }
  out << contents;

  if (verbose_output == verbosity::developer) {
    cout << "Closing " << filename << '\n';
  }
  out.close();

  return 0;
}
}  // namespace ew