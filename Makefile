
CXX := g++-15
CXXFLAGS := -std=gnu++17 -O3 -Wall -Wextra -Wpedantic -fno-fast-math
INCLUDES := -Iinclude
LDFLAGS := 

SRC := \
	src/Csv.cpp \
	src/Date.cpp \
	src/Stats.cpp \
	src/BlackScholes.cpp \
	src/ImpliedVol.cpp \
	src/MarketData.cpp \
	src/Hedger.cpp \
	src/Simulator.cpp \
	src/ResultWriter.cpp \
	src/main.cpp

TEST_SRC := \
	src/Csv.cpp \
	src/Date.cpp \
	src/Stats.cpp \
	src/BlackScholes.cpp \
	src/ImpliedVol.cpp \
	src/MarketData.cpp \
	src/Hedger.cpp \
	src/Simulator.cpp \
	src/ResultWriter.cpp \
	src/tests_main.cpp \
	src/UnitTest.cpp

BIN := bin
RUN := $(BIN)/run
TEST := $(BIN)/tests

all: build

build:
	mkdir -p $(BIN)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRC) -o $(RUN) $(LDFLAGS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(TEST_SRC) -o $(TEST) $(LDFLAGS)

run-task1: build
	$(RUN) simulate --S0 100 --mu 0.05 --sigma 0.24 --r 0.025 --T 0.4 --N 100 --paths 1000 --out out/

run-task2: build
	$(RUN) hedge-real \
		--t0 2011-07-05 --tN 2011-07-29 \
		--K 500 --exdate 2011-09-17 \
		--sec data/sec_GOOG.csv \
		--opt data/op_GOOG.csv \
		--rf data/interest.csv \
		--out out/result.csv

test: build
	$(TEST)

clean:
	rm -rf $(BIN) out/*.csv
