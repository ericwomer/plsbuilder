#ifndef BUILDER_H
#define BUILDER_H

#include <string>
#include <vector>

#include <iostream>
#include <fstream>

#include "base/base.h"

namespace rake {
class builder : public base
{
public: // methods
    builder();
    virtual int main(void);
    virtual int main(int argv, char *argc[]);
    virtual int main(std::vector<std::string> &params);
    virtual int size(void) { return sizeof(this); };
    virtual std::string name(void) { return app_name; };
    virtual void help(void);
    virtual void setAppName(std::string name);
private: // methods
    int writePlaylist();
private: // vars
    std::vector<std::string> actions;

    std::string app_name;
    std::vector<std::string> app_description;

    std::vector<std::string> file_list;
    std::string file_name;

    int number_entries = 0;
};

} // rake
#endif // BUILDER_H
