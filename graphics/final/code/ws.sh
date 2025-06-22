rm build/final
cmake -B build
cmake --build build

build/final tc2/whitd.txt output/whitd.bmp
build/final tc2/whitdlight.txt output/whitdlight.bmp
build/final tc2/wl.txt output/wl.bmp
build/final tc2/wtdlt.txt output/wtdlt.bmp