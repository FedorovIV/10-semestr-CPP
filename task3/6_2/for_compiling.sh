rm -rf gcm.cache app *.o

# header units std (разово)
g++-14 -std=c++20 -fmodules-ts -xc++-system-header -c iostream cassert numeric stdexcept

# модульный интерфейс
g++-14 -std=c++20 -fmodules-ts -x c++ -c rational.ixx  -o rational.ifc.o

# модульная реализация
g++-14 -std=c++20 -fmodules-ts -x c++ -c rational.cppm -o rational.impl.o

# main
g++-14 -std=c++20 -fmodules-ts -x c++ -c main.cpp -o main.o

# линковка
g++-14 -std=c++20 -fmodules-ts main.o rational.ifc.o rational.impl.o -o app

./app
