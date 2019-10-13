#!/bin/sh
mkdir -p build
cd build
cmake -DDEBUG=1 -DCMAKE_BUILD_TYPE=Debug -DCREATE_PACK=OFF ..
# cmake -DCMAKE_BUILD_TYPE=Release -DCREATE_PACK=ON ..
cmake --build . -- -j8
cpack -G DragNDrop
DMG_FINENAME=$(find . -maxdepth 1 -type f -print | egrep -i '\.dmg$')
cp -Rp ${DMG_FINENAME} ../bomberman.dmg