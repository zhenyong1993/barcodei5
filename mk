g++ main.c barcode.cpp mqClient.cpp boxconfig.cpp -Wl,-rpath=./ -L. -ljsoncpp -lpaho-mqtt3as -lcrypto -lssl -o barcode -DJSON_IS_AMALGAMATION -std=c++11
