CC = gcc
SRCS = interface.c ./parser/s21_parsing_file.c ./parser/s21_affine_transformations.c
OS:=$(shell uname -s)

all:	s21_3DViewer

s21_3DViewer:
ifeq ($(OS), Darwin)
	$(CC) `pkg-config --cflags gtk4` -DGDK_VERSION_MIN_REQUIRED=GDK_VERSION_4_2 $(SRCS) -o s21_3DViewer `pkg-config --libs gtk4`
else
	$(CC) `pkg-config --cflags gtk4` -DGDK_VERSION_MIN_REQUIRED=GDK_VERSION_4_2 $(SRCS) -o s21_3DViewer`pkg-config --libs gtk4` $(LINUX_FLAGS)
endif

run:	s21_3DViewer
	./s21_3DViewer

dvi:
	open readme.html

tests:
	${CC} s21_viewer-test.c ./parser/s21_parsing_file.c ./parser/s21_affine_transformations.c -lcheck -o tests
	./tests


dist:
	@tar -cf s21_3DViewer_v1.0.tar *.c *.h Makefile readme.html gif obj parser picture


install:	uninstall s21_3DViewer
	@mkdir ~/3DViewer_v1.0
	@install ./s21_3DViewer ~/3DViewer_v1.0/
	@make clean

gcov_report:
	@gcc --coverage s21_viewer-test.c -lcheck ./parser/s21_parsing_file.c ./parser/s21_affine_transformations.c -o unit-test
	@./unit-test
	@lcov -t "test" -o test.info -c -d .
	@genhtml -o report test.info

uninstall:
	@rm -rf ~/3DViewer_v1.0

leaks:
ifeq ($(OS), Darwin)
	CK_FORK=no leaks --atExit -- ./s21_3DViewer
else
	G_SLICE=debug-blocks valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./s21_3DViewer $(LINUX_FLAGS) 
endif

style:
	python3 ../materials/linters/cpplint.py --extensions=c *.c *.h

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem . *.c

clean:
	@/bin/rm -rf *.o *.a *.log *.aux *.pdf *.tar test unit-test *.gcno *gcda report *.info leak_check s21_3DViewer ./picture/*.png ./picture/*.bmp ./picture/*.jpeg ./gif/*.gif ./gif/*.png *.txt tests

.PHONY:	all s21_3DViewer run install uninstall clean