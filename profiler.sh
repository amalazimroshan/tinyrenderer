 g++ -ggdb -g -pg -O0 -o main main.cpp tgaimage.cpp display.cpp graphics.cpp -lSDL2
 ./main

 gprof main gmon.out > analysis.txt
#  cat analysis.txt