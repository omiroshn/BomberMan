#!/bin/sh
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCREATE_PACK=ON ..
cmake --build . -- -j8
cpack -G DragNDrop
DMG_FINENAME=$(find . -maxdepth 1 -type f -print | egrep -i '\.dmg$')
cp -Rp ${DMG_FINENAME} ../bomberman.dmg