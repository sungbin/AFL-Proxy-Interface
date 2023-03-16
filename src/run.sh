rm a.out *.o .coverage
clang++ -g -fsanitize-coverage=trace-pc-guard ./target_program.c -c
clang++ -fsanitize=address trace-pc-guard.cc  ./target_program.o ./coverage.c
./a.out
