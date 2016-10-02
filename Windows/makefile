CXX ?= g++
CFLAGS = -Wall -pedantic -std=c++14 -O2

SRCDIR := ../src
SRCS := $(patsubst $(SRCDIR)/%,%,$(wildcard $(SRCDIR)/*.cpp))

DEPDIR := deps
DEPS := $(patsubst %.cpp,$(DEPDIR)/%.makefile,$(SRCS))

ODIR := build
OBJS := $(patsubst %.cpp,$(ODIR)/%.o,$(SRCS))

print-tests: $(OBJS)
	$(CXX) -o $@ $(OBJS) $(CFLAGS)

$(DEPDIR):
	mkdir $@

$(ODIR):
	mkdir $@

-include $(DEPS)

$(DEPDIR)/%.makefile: | $(DEPDIR)
	$(CXX) $(CFLAGS) -MM $(SRCDIR)/$*.cpp -MT "$(ODIR)/$*.o $@" > $@
	
$(ODIR)/%.o: | $(ODIR)
	$(CXX) -o $@ -c $(SRCDIR)/$*.cpp $(CFLAGS)
		
.PHONY: clean
clean:
	cmd /c if exist $(DEPDIR) rmdir /s /q $(DEPDIR)
	cmd /c if exist $(ODIR) rmdir /s /q $(ODIR)
	cmd /c if exist print-tests.exe del /F print-tests.exe
