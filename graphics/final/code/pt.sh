rm build/final
cmake -B build
cmake --build build

# build/final tc2/light.txt output/light.bmp
# build/final tc2/mesh.txt output/mesh.bmp
build/final tc2/whitdpt.txt output/whitdpt.bmp
# build/final tc2/glossy.txt output/glossy.bmp
# build/final tc2/testwl.txt output/testwl.bmp
# build/final tc2/fenier.txt output/fenier.bmp
# build/final tc2/focuslight.txt output/focuslight.bmp
# build/final tc2/wlfl.txt output/wlfl.bmp
# build/final tc2/norm.txt output/norm.bmp
# build/final tc2/fenier.txt output/fenier.bmp
# build/final tc2/0incident.txt output/0incident.bmp