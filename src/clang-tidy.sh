#!/bin/bash

clang-tidy `find . -type f \( -name '*.hpp' -o -name '*.cpp' \) -not -path "./thirdparty/*" ` -config='' -p ../cmake-build-debug/ "$@" 
# -- -std=c++11 -stdlib=libc++ -I/usr/lib/llvm-8/include/c++/v1/ -x c++