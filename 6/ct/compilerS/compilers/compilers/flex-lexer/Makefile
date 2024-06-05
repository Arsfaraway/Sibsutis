DIRGUARD = @mkdir -p $(@D)

BIN = bin
OBJ = obj
SRC = src

HEADERSDIR = $(SRC)
CFLAGS = -Wall
CXX = g++
FLEXXX = /usr/bin/flex++

LEXERFLEX = $(SRC)/CoolLexer.flex
DRIVER = $(SRC)/driver.cpp

LEXERCPP = $(OBJ)/CoolLexer.cpp
EXE = $(BIN)/driver

.PHONY: all
all: $(EXE)
$(EXE): $(DRIVER) $(LEXERCPP)
	$(DIRGUARD)
	$(CXX) $(CFLAGS) $^ -I$(HEADERSDIR) -o $@ -Isrc

.PHONY: $(LEXERCPP)
$(LEXERCPP): $(LEXERFLEX)
	$(DIRGUARD)
	$(FLEXXX) -o $@ $<

.PHONY: clean
.SILENT: clean
clean:
	rm -rf $(BIN) $(OBJ)