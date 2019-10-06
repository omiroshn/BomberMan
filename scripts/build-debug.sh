#!/bin/sh
mkdir -p build
cd build
cmake -DDEBUG=1 -DCMAKE_BUILD_TYPE=Debug -DCREATE_PACK=OFF ..
cmake --build . -- -j8
