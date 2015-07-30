#include <iostream>

#include "builder/builder.h"

rake::builder PlsBuilder;

int main(int argv, char *argc[]) {

    if(argv == 1) {
        std::cerr << "Error: At least one paramater needs to be specifide." << std::endl;
        PlsBuilder.setAppName(argc[0]);
        PlsBuilder.help();
        return -1;
    } else {
        PlsBuilder.main(argv, argc);
    }

    return 0;
}
