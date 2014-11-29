#include <iostream>

#include "builder/builder.h"

rake::builder PlsBuilder;

int main(int argv, char *argc[]) {

    if(argv == 1) {
        PlsBuilder.main();
    } else {
        PlsBuilder.main(argv, argc);
    }

    return 0;
}
