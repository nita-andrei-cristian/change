mkdir debug
cd debug
clear
cmake -DCMAKE_BUILD_TYPE=Release -DGGML_CUDA=ON -G Ninja ..

cd ..
cmake --build debug --config Release -j

cd debug

# This helps clang detect the file
mv compile_commands.json ../compile_commands.json

./change
