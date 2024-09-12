#include <cstdlib>
#include <filesystem>
#include <iostream>

#include "builder.hpp"

// Using declarations
using std::cerr;
using std::cin;
using std::cout;
using std::getline;
using std::string;
using std::to_string;
using std::vector;
using std::filesystem::absolute;
using std::filesystem::exists;
using std::filesystem::path;

namespace ew {

// Default constructor
builder::builder()
{
  // Basic description
  app_description.push_back(string("This is a simple mpv playlist builder \n"));
  app_description.push_back(string("*** NOTHING ELSE FOLLOWS *** \n"));
}

// This is the main that handles parameters
auto builder::main(vector<string> &params) -> int
{
  // Somewhere to place unknown command line parameters
  vector<string> dump;

  // iterate through params to remove the -- from the text
  for (vector<string>::const_iterator i = params.begin(); i != params.end(); ++i) {
    if (*i == "--help" || *i == "-h") {
      help();
      return EXIT_SUCCESS;
    } else if (*i == "--verbose" || *i == "-v") {
      verbose_output = verbosity::info;
      cout << "verbose_output: " << verbose_output << '\n';
    } else if (*i == "--version" || *i == "-V") {
      cout << app_name << " " << 00 << "." << 00 << "." << 01 << '\n';
      return EXIT_SUCCESS;
    } else if (*i == "--playlist" || *i == "-p") {
      // used to verify that the file does not already exist
      // and if so asks weather its ok to overwrite

      // iterate to the file
      ++i;

      if (i[ 0 ][ 0 ] == '-') {
        cerr << "\tError: Filename can not begin with - \n";
        return EXIT_FAILURE;
      }

      // Save file with the absolute path
      file          = absolute(*i);

      // Get the file name stem and extension
      file_name     = file.stem();
      file_ext      = file.extension();

      // Absolute path to the file locations
      absolute_path = file;

      // Strip off the file name from the path
      if (absolute_path.has_filename()) {
        absolute_path.remove_filename();
      }

      // Test if the root path exists
      if (!exists(absolute_path)) {
        cout << "\tDirectory " << absolute_path << " does not exist\n";
        cout << "\t please select an existing directory to save the playlist file to.\n";
        return EXIT_FAILURE;
      }

      if (verbose_output == verbosity::info) {
        // Some basic info
        cout << "File Name: \t" << file_name << '\n';
        cout << "File Extension: \t" << file_ext << '\n';
      }

      // Test to see if the file already exists.
      if (exists(path(absolute_path / (file_name + file_ext)))) {
        string answer = "Y";
        cout << "File " << absolute_path / (file_name + file_ext)
             << " already exists.\nDo you want to overwrite? [Y/n] ";  // TODO: Find a way to get the default
                                                                       // answer, set answer = "yes" ?
        // get input from the user on whether or not overwrite pre-existing file
        while (getline(cin, answer)) {
          if (answer == "n" || answer == "N" || answer == "no") {
            cerr << "Please rerun the app with the file name you want.\n";
            return EXIT_FAILURE;
          } else if (answer == "y" || answer == "Y" || answer == "yes" || answer.empty()) {
            break;
          } else {
            cout << "Sorry unrecognized answer, please choose yes(y, default) or no(n).\n";
            cout << "File already exists.\nDo you want to overwrite? [Y/n] ";
            continue;  // Somehow this works?
          }
        }

      }  // End test to see if file already exists.
    } else if (*i == "--files" || *i == "-f") {
      // parse list of files here.
      // for the time being must be at end of parm list before the actually files

      // Iterate to the beginning of the list of files
      ++i;

      // Iterate through the list of files passed in
      for (vector<string>::const_iterator k = i; k != params.end(); ++k) {
        // Break on next parameter or invalid file name
        if (k[ 0 ][ 0 ] == '-') {
          break;
        }

        // TODO: Determine if the file exists first before adding to the list
        // SIDE-NOTE: Do we want to exit without writing the playlist if one of the files passed in doesn't exist?
        file_list.push_back(*k);

        if (verbose_output == verbosity::info) {
          cout << "File: " << *k << " >> Entry Number:" << number_entries << '\n';
        }
        ++number_entries;
      }
    } else {  // catch all to make sure there are no invalid parameters
      dump.push_back(*i);
    }
  }

  // Write the playlist file
  buildPlaylist();
  return EXIT_SUCCESS;
}

// This main converts c style parameters to c++ strings
// then passes it to main that handles the actual parameters.
auto builder::main(int argv, char *argc[]) -> int
{
  // Create a temp param list
  vector<string> params;

  // Set the application name
  setAppName(argc[ 0 ]);

  // C string to C++ std::string conversion
  for (int i = 1; i != argv; ++i) {
    params.push_back(argc[ i ]);
  }

  // Return the main(std::vector<std::string>) method
  return main(params);
}

// method to write the playlist
auto builder::buildPlaylist() -> int
{
  // Decide on which format to write
  if (file_ext == ".pls") {
    buildPLS();
  } else if (file_ext == ".m3u") {
    buildM3U();
  }

  return EXIT_SUCCESS;
}

// M3U playlist file builder
int builder::buildM3U()
{
  // File contents
  string fileContents = "#EXTM3U\n";
  fileContents += "#Playlist created by " + app_name + " " + "0.0.0.1\n";
  for (auto &cfile: file_list) {
    fileContents += "#EXTINF:000," + cfile + '\n';  // TODO: Get stream length in seconds
    fileContents += cfile + '\n';
  }

  // NOTE: Testing purposes only
  for (auto &contents: fileContents) {
    cout << contents;
  }

  if (!file_name.empty()) {
    return writeFile(absolute_path / (file_name + file_ext), fileContents);
  } else {
    cerr << "Currently M3U file format is not supported\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

// PLS playlist file builder
int builder::buildPLS()
{
  // Contents of the file
  string fileContents = "[playlist] \n";

  // Number of files listed in the playlist
  int    fileNumber   = 1;

  // Start contests here
  fileContents += "\n";
  for (auto &cfile: file_list) {
    // TODO: test to see if its a stream or file and add the correctly =file:// =stream option
    fileContents += "File" + to_string(fileNumber) + "=" + cfile + " \n";
    fileContents += "\n";
    ++fileNumber;
  }

  fileContents += "NumberOfEntries=" + to_string(number_entries) + '\n';
  fileContents += "Version=2\n";
  // End contents here

  // Write the file
  if (!file_name.empty()) {
    return writeFile(absolute_path / (file_name + file_ext), fileContents);
  } else {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

// Help display
auto builder::help() -> void
{
  cout << "Usage: " << app_name << " [options] files...\n\n";
  cout << "Options: \n";
  cout << " -h, --help \t\t Print this help message and exit the program.\n";
  cout << " -v, --verbose \t\t Print out all the valid command line parameters\n";  // TODO: Change this
  cout << " \t\t\t passed to the program.\n";
  cout << " -p, --playlist <file>.<ext> \t The name of the playlist <file> and <ext> being either pls or m3u.\n";
  cout << " -f, --files <files> \t The list of <files> saved to the playlist file, must be last switch.\n";
  cout << " -V, --version \t\t Print the version and exit.\n";
}

auto builder::setAppName(string name) -> void
{
  app_name = name;
}

}  // namespace ew
