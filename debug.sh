mkdir debug
cd debug
clear
cmake -DCMAKE_BUILD_TYPE=Debug ..

cd ..
cmake --build debug

cd debug

# This helps clang detect the file
mv compile_commands.json ../compile_commands.json

./change
