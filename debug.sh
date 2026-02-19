mkdir debug
cd debug
clear
cmake -DCMAKE_BUILD_TYPE=Debug ..

cd ..
cmake --build debug

cd debug

./change
