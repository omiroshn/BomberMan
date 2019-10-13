#!/bin/sh
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCREATE_PACK=ON ..
cmake --build . -- -j8
cp -Rp bin/bomberman.app ../bomberman_release.app