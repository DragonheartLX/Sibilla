# Sibilla Bot

```cmd

# Install for linux
sudo apt install zip linux-libc-dev pkg-config

git clone https://github.com/DragonheartLX/Sibilla.git
cd Sibilla

# Install vcpkg
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
cd ..

mkdir build
cd build

# make sure your compiler support c++23
cmake .. -DCMAKE_TOOLCHAIN_FILE=./vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build .
```
