cd /mnt/h/code/collective_effects_cpp/build
rm -rf *
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release -j4
cd ..