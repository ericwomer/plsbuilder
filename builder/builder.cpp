#include "builder.h"
/*
 * TODO: Put the files in a singleton class(?) and/or have it auto close if open when leaving scope(?)
 * or watch more Bjourne Strousup (I hope I spelled his name correctly) and Bo Qain videos.
 *
 * TODO: Either get rid off -f, --files altogether and make it defaulet that the last options are files
 * or find a way to add files in the middle of a parameter list.
 */

namespace rake {

int writeFile(std::string &filename, std::string &contents) {
    std::ofstream out;

    // Close it incase it was left open.
    if( out.is_open() ) {
        out.close();
    }
    out.open(filename);

    if (!out) {
        std::cerr << "Could not open " << filename;
        return -1;
    }

    out << contents;
    out.close();

    return 0;
}

builder::builder()
{
    app_description.push_back(std::string("This is a simple mpv playlist builder \n"));
    app_description.push_back(std::string("** nothing else follows ** \n"));
}

// All of the application starts here
// if any parameters are used they are handled
// with member vars.
int builder::main (void) {
    // Start Application Here.
    writePlaylist();
    return 0;
}

// This is the main that handles parameters
int builder::main (std::vector<std::string> &params) {

    // std::vector<std::string> actions;
    std::vector<std::string> dump;

    // iterate through params to remove the -- from the text
    for (std::vector<std::string>::const_iterator i = params.begin() ; i != params.end(); ++i) {

        if (*i == "--help" || *i == "-h") {
            actions.push_back("help");
        } else if (*i == "--verbose" || *i == "-v") {
            actions.push_back("verbose");
        } else if (*i == "--version" || *i == "-V") {
            actions.push_back("version");
        }  else if (*i == "--playlist" || *i == "-f") {

            // used to verify that the file does not already exist
            // and if so asks weather its ok to overwrite
            std::ifstream file_test;

            // the playlist file to save to
            // actions.push_back("playlist");
            ++i;

            // Incase some how the file is open before we opened it?
            if(file_test.is_open()){
                file_test.close();
            }

            file_name.append(*i);

            // Test to see if the file already exists.
            file_test.open(file_name);

            if(file_test.is_open()){

                std::string answer = "";
                std::cout << "File already exists.\nDo you want to overwrite? [Y/n] ";
                while(std::getline(std::cin, answer)){
                    if(answer == "n" || answer == "N" || answer == "no"){
                        std::cerr << "Please rerun the app with the file name you want." << std::endl;
                        file_test.close();
                        return -1;
                    }
                    else if(answer == "y" || answer == "Y" || answer == "yes" || answer.empty()){
                        break;
                    }
                    else{
                        std::cout << "Sorry unrecognized answer, please choose yes(y, default) or no(n)." << std::endl;
                        std::cout << "File already exists.\nDo you want to overwrite? [Y/n] ";
                        continue; // Somehow this works?
                    }
                }

                file_test.close();
            } // End test to see if file already exists.

            // Close the file.
            file_test.close();

        }  else if (*i == "--files" || *i == "-f") {
            // parse list of files here.
            // for the time being must be at end of parmlist before the actually files
            ++i;
            for(std::vector<std::string>::const_iterator k = i; k != params.end(); ++k){
                file_list.push_back(*k);
                std::cout << "File: " << *k << " >> Entry Number:" << number_entries << std::endl;
                ++number_entries;
            }
        } else { // catch all to make sure there are no invalid parameters
            dump.push_back(*i);
        }
    }

    for (auto &c : actions) { // handle all the prameters
        if (c == "help"){
            help();
            return 0; // exit the app

        } else if (c == "verbose") {

            for (auto &d : actions){
                std::cout << "--" << d << " ";
            }
            std::cout << std::endl;
            return 0;
        } else if (c == "version") {
            std::cout << app_name << " " << 00 << "." << 00 << "." << 01 << std::endl; // create a version builder class??
            return 0;
        }
        else {
            dump.push_back(c);
        }
    }

    return main();
}

// This main converts c style parameters to c++ strings
// then passes it to main that handles the actual parametrs.
int builder::main (int argv, char *argc[]) {
    // Start here if there are params
    std::vector<std::string> params;

    app_name = argc[0];

    for(int i = 1; i != argv; ++i){
        params.push_back(argc[i]);
    }

    return main(params);
}

int builder::writePlaylist(){

    std::string fileContents;

    fileContents += "[playlist] \n";
    fileContents += "NumberOfEntries=" + std::to_string(number_entries);

    int fileNumber = 1;
    fileContents += "\n";
    for(auto &c : file_list){
        fileContents += "File" + std::to_string(fileNumber) + "=file://" + c + " \n";
        fileContents += "Length" + std::to_string(fileNumber) + "=-1 \n";
        fileContents += "\n";
        ++fileNumber;
    }

    fileContents += "Version=2\n";

    if(!file_name.empty()){
        return writeFile(file_name, fileContents);
    } else {
        return 0;
    }

    return 0;
}

void builder::help(void) {
    std::cout << "Usage: " << app_name << " [options] files...\n\n";
    std::cout << "Options: \n";
    std::cout << " -h, --help \t\t Print this help message and exit the program.\n";
    std::cout << " -v, --verbose \t\t Print out all the valid command line parameters\n";
    std::cout << " \t\t\t passed to the program.\n";
    std::cout << " -p, --playlis <file> \t The name of the playlist <file>.\n";
    std::cout << " -f, --files <files> \t The list of <files> saved to the playlist file, must be last switch.\n";
    std::cout << " -V, --version \t\t Print the version and exit.\n";
}

void builder::setAppName(std::string name){
    app_name = name;
}

} // rake
