g++ main.c barcode.cpp mqClient.cpp boxconfig.cpp -Wl,-rpath=./ libiPort3API.so -lpaho-mqtt3as -o barcode -DJSON_IS_AMALGAMATION -std=c++11
