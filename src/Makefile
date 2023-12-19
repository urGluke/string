NAME = s21_string.a
CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
SRCS= *.c 
TEST_SRC= tests/test.c tests/*.c
TEST_FLAGS_LINUX= -lcheck -lm -lsubunit -lpthread
TEST_FLAGS= -lcheck -lm -lpthread
GCOV_FLAGS = -ftest-coverage -fprofile-arcs
STYLE=clang-format --style=google
FORMAT= *.c

all: test

${NAME}:
	$(CC) -c ${SRCS}
	ar rc ${NAME} *.o
	ranlib ${NAME}
	rm *.o

test: ${NAME}
	$(CC) -c ${TEST_SRC} 
	$(CC) *.o ${NAME} ${TEST_FLAGS} -o test
	./test

test_linux: rebuild
	$(CC) -c ${TEST_SRC} 
	$(CC) *.o ${NAME} ${TEST_FLAGS_LINUX} -o test_linux

gcov_report: $(NAME)
	$(CC) ${GCOV_FLAGS} -c ${TEST_SRC}
	$(CC) $(GCOV_FLAGS) *.o ${NAME} ${TEST_FLAGS} -o test
	./test
	lcov -t "test" -o test.info -c -d .
	genhtml -o report test.info
	open report/index.html

gcov_report_linux:
	$(CC) ${GCOV_FLAGS} -c ${TEST_SRC}
	$(CC) $(GCOV_FLAGS) *.o $(TEST_FLAGS_LINUX) -o test
	./test
	lcov -t "test" -o test.info -c -d .
	genhtml -o report test.info
	open report/index.html

check_style:
	$(STYLE) -n $(FORMAT) $(TEST_SRC)

style:
	$(STYLE) -i $(FORMAT) $(TEST_SRC)

clean:
	rm -rf *.o *.a test test_linux *.gcno *.gcda *.info report

rebuild : clean all