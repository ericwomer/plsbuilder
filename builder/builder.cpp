#include "builder.h"

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

builder::builder() :
    app_name("PlsBuilder")
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

        if (*i == "--help" || *i == "-H") {
            actions.push_back("help");
        } else if (*i == "--verbose" || *i == "-V") {
            actions.push_back("verbose");
        } else if (*i == "--version") {
            actions.push_back("version");
        }  else if (*i == "--playlist") {
            // the playlist file to save to
            // actions.push_back("playlist");
            ++i;
            file_name.append(*i);
        }  else if (*i == "--files") {
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
            std::cout << "Help for " << app_name << std::endl;
            std::cout << "--help -H : print this help message and exit the program" << std::endl;
            std::cout << "--verbose -V : print out all the valid command line parameters" << std::endl;
            std::cout << "               passed to the program" << std::endl;
            return 0; // exit the app

        } else if (c == "verbose") {

            for (auto &d : actions){
                std::cout << "--" << d << " ";
            }
            std::cout << std::endl;
            return 0;
        } else if (c == "version") {
            std::cout << 00 << "." << 00 << "." << 01 << std::endl; // create a version builder class??
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

    fileContents += "Version=2";

    if(!file_name.empty()){
        return writeFile(file_name, fileContents);
    } else {
        return 0;
    }

    return 0;
}


} // rake
