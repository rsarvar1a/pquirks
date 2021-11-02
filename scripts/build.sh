cd build/

cmake -DCMAKE_BUILD_TYPE=Debug -G 'Ninja' ../src
cp compile_commands.json ..

ninja
cp pquirks ../bin/pquirks

