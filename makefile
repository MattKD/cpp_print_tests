CC = g++
CFLAGS = -Wall -pedantic -std=c++14 -O3

ODIR = obj
_OBJ = print_tests.o console_tests.o file_tests.o string_tests.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

print-tests: $(ODIR) $(OBJ)
	$(CC) -o $@ $(OBJ) $(CFLAGS)

$(ODIR):
	mkdir $@

$(ODIR)/print_tests.o: print_tests.cpp print_tests.h console_tests.h \
	file_tests.h string_tests.h
	$(CC) -o $@ -c $< $(CFLAGS)

$(ODIR)/console_tests.o: console_tests.cpp print_tests.h template_fprintf.h
	$(CC) -o $@ -c $< $(CFLAGS)

$(ODIR)/file_tests.o: file_tests.cpp print_tests.h template_fprintf.h
	$(CC) -o $@ -c $< $(CFLAGS)

$(ODIR)/string_tests.o: string_tests.cpp print_tests.h template_fprintf.h \
	string.h
	$(CC) -o $@ -c $< $(CFLAGS)
