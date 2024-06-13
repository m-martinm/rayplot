MAKEFILE_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST)))) # Windows?
INCLUDE := -Iraylib/src/ -I./
LIBS := -Lraylib/src/ -lraylib -lm -lGL -lm -lpthread -ldl -lrt -lX11
# For Windows link the followings instead: -lraylib -lgdi32 -lwinmm

all: examples test

test: test.c rplot.h
	gcc test.c $(INCLUDE) $(LIBS) -o build/test

examples: line scatter heatmap

line: examples/line.c rplot.h
	gcc examples/line.c $(INCLUDE) $(LIBS) -o build/line

scatter: examples/scatter.c rplot.h
	gcc examples/scatter.c $(INCLUDE) $(LIBS) -o build/scatter

heatmap: examples/heatmap.c rplot.h
	gcc examples/heatmap.c $(INCLUDE) $(LIBS) -o build/heatmap

clean:
	rm build/line build/scatter build/heatmap build/test

runall:
	./build/heatmap &
	./build/scatter &
	./build/line &