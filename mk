g++ main.c barcode.cpp mqClient.cpp boxconfig.cpp -Wl,-rpath=./ -L. -liPort3API -lpaho-mqtt3as -o barcode -DJSON_IS_AMALGAMATION -std=c++11
