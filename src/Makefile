CC = g++
TARGET = main
OBJS = main.o compiler.o lexer.o parser.o gen.o
CXXFLAGS = -Wall -Wextra -std=c++2b

.PHONY: all
all: $(TARGET)

.PHONY: clean
clean:
	rm -rf *.o $(TARGET)

.PHONY: run
run: $(TARGET)
	@./$(TARGET) $(FILE)

.PHONY: runc
runc: $(TARGET)
	@./$(TARGET) -c $(FILE)

$(TARGET): $(OBJS) Makefile
	$(CC) $(OBJS) $(LIBS) $(LDFLAGS) -o $@

%.o: %.cpp Makefile
	$(CC) $(CXXFLAGS) -c $<
