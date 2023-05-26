C = gcc
CFLAGC =  -Wall -Werror -Wextra  -std=c11 
SOURCES = parser/s21_parcer.c parser/affine.c parser/check.c
OBJECTS = s21_parcer.o


all: build static test

build:
	$(CC) $(CFLAGC) -c $(SOURCES)

static:
	ar rc s21_parcer.a $(OBJECTS)
	ranlib s21_parcer.a

# for linux: -lcheck -lpthread -pthread -lrt -lm -lsubunit
test: clean
	$(CC) $(CFLAGC) -lcheck  parser/*.c -o test
	./test

gcov_report: clean
	gcc $(WWW) -lcheck --coverage parser/*.c -o gcov_report
	./gcov_report
	lcov -t "./gcov_report" -o gcov_report.info -c -d .
	genhtml -o report gcov_report.info
	rm -rf *.gcda
	rm -rf *.gcno
	rm -rf *.info
	open ./report/index.html

install:
	make clean
	mkdir build
	cd 3D_Viewer && qmake && make && make clean && rm Makefile && cd ../ && mv 3D_Viewer/3D_Viewer.app build
	

uninstall:
	rm -rf build*

dvi:
	open dvi.txt

dist:
	rm -rf Archive_3D_Viewer/
	mkdir Archive_3D_Viewer/
	mkdir Archive_3D_Viewer/src
	mv ./build/3D_Viewer.app Archive_3D_Viewer/src/
	tar cvzf Archive_3D_Viewer.tgz Archive_3D_Viewer/
	rm -rf Archive_3D_Viewer/

clean:
	rm -rf *.{a,o,dSYM,out}
	rm -rf $(EXECUTABLE)
	rm -rf gcov_report
	rm -rf report
	rm -rf CPPLINT.cfg
	rm -rf *.gcda
	rm -rf *.gcno
	rm -rf *.info
	rm -rf test
	rm -rf Archive_3D_Viewer
	rm -rf *tgz
	rm -rf build

rebuild: clean all

linter:
		cp ../materials/linters/.clang-format ..
		clang-format -i parser/*.c parser/*.h 3D_Viewer/*.cpp 3D_Viewer/*.h
		rm -rf ../.clang-format

leak:
		CK_FORK=no leaks --atExit -- ./test
style:
		cp ../materials/linters/.clang-format ..
		clang-format -n parser/*.c parser/*.h 3D_Viewer/*.cpp 3D_Viewer/*.h
		rm -rf ../.clang-format
