#!/bin/bash
echo generating...

mkdir -p build/debug
cd build/debug
cmake -DCMAKE_BUILD_TYPE=Debug ../..
cd -

mkdir -p build/release
cd build/release
cmake -DCMAKE_BUILD_TYPE=Release ../..
cd -

echo "done..."
