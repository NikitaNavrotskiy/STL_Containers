CC=gcc
CFLAGS=-Wall -Werror -Wextra --std=c++17 -lstdc++  -g

CK_FORK=valgrind --vgdb=no --leak-check=full --show-leak-kinds=all -s --track-origins=yes --verbose
TEST_EXEC=my_containers_test
TEST_FLAGS=-lgtest -lm 
TEST_SRC=test/test_main.cc test/test_map.cc test/test_set.cc test/test_stack.cc test/test_array.cc \
	test/test_list.cc test/test_queue.cc test/test_multiset.cc test/test_vector.cc

all: clean test


test_exec: 
	$(CC) $(TEST_SRC) -o $(TEST_EXEC) $(TEST_FLAGS) $(CFLAGS)

test: test_exec
	./$(TEST_EXEC)

leaks:
	$(CK_FORK) ./$(TEST_EXEC)


gcov_report:
	$(CC) --coverage $(TEST_SRC) -o $(TEST_EXEC) $(TEST_FLAGS) $(CFLAGS) -lgcov
	./$(TEST_EXEC)
	lcov -t "$(TEST_EXEC)" -o containers_gcov.info -c -d .
	genhtml -o report containers_gcov.info
	open report/index.html
	google-chrome report/index.html

docker:
	docker build -t cont .
	docker run --name app cont

clean_docker:
	docker rm app
	docker rmi cont

clean:
	rm -rf $(TEST_EXEC)
	rm -rf report
	rm -rf *gcov.info *gcda *gcno
