CC = gcc
TST = test/
FLAGS1 = -std=c11 -ggdb3
UNAME := $(shell uname)

.PHONY: all clean rebuild test check
default: all

all: clean s21_cat s21_grep  check test

s21_cat:
	$(CC) $(FLAGS1) ../cat/s21_cat.c -o ../cat/s21_cat

s21_grep:
	$(CC) $(FLAGS1) s21_grep.c -o s21_grep

clean:
	rm -rf ../cat/*.o *.o ../cat/*.a *.a  ../cat/*.cfg *.cfg *.gc* *.info a.out ../cat/s21_cat s21_grep

test: s21_cat s21_grep
	bash ../test/Test.sh


rebuild:
	$(MAKE) clean
	$(MAKE) all

check:
	cppcheck --enable=all --suppress=missingIncludeSystem *.c ../cat/*.c
	cp ../../materials/linters/CPPLINT.cfg CPPLINT.cfg
	cp ../../materials/linters/CPPLINT.cfg ../cat/CPPLINT.cfg
	python3 ../../materials/linters/cpplint.py --extension=c *.c ../cat/*.c
ifeq ($(UNAME), Darwin)
	CK_FORK=no leaks --atExit -- ../cat/s21_cat -b -n -s -et ../test/text/test3.txt
	CK_FORK=no leaks --atExit -- ./s21_grep -e you -i ../test/text/test3.txt
endif
ifeq ($(UNAME), Linux)
	valgrind --track-origins=yes -s -q ../cat/s21_cat -b -n -s -et ../test/text/test3.txt
	valgrind --track-origins=yes -s -q ./s21_grep -e you -i ../test/text/test3.txt
endif

check_pyt:
	cp ../../materials/linters/CPPLINT.cfg CPPLINT.cfg
	cp ../../materials/linters/CPPLINT.cfg ../cat/CPPLINT.cfg
	python3 ../../materials/linters/cpplint.py --extension=c *.c ../cat/*.c
