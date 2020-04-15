CC = g++

SOURCE_DIR = src
OUTPUT_DIR = bin

HEADERS = $(wildcard $(SOURCE_DIR)/*.hpp)
SOURCES = $(wildcard $(SOURCE_DIR)/*.cpp)

.PHONY: clean

getScc: $(SOURCES) $(HEADERS)
	$(CC) -o $(OUTPUT_DIR)/$@ $(SOURCES)

clean:
	rm $(OUTPUT_DIR)/*
