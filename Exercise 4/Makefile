CC=g++
CXXFLAGS= -Wall -Wvla -Wextra -Werror -g -std=c++14
LDFLAGS= -lm
HEADERS= Matrix.h Activation.h Dense.h MlpNetwork.h
HEADERS_TEST= test.h Matrix.h Activation.h Dense.h test_activation.h test_dense.h test_matrix.h
OBJS= Matrix.o Activation.o Dense.o MlpNetwork.o main.o
OBJS_TEST= test.o test_activation.o test_dense.o test_matrix.o Matrix.o Activation.o Dense.o

%.o : %.c


mlpnetwork: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

test: $(OBJS_TEST)
	$(CC) $(LDFLAGS) -o $@ $^
	./test

$(OBJS) : $(HEADERS)
$(OBJS_TEST) : $(HEADERS_TEST)

.PHONY: clean clean_test
clean:
	rm -rf *.o
	rm -rf mlpnetwork

clean_test:
	rm -rf *.o
	rm -rf test




