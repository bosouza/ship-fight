linkOpenGL := -lglew32 -lglfw3 -lopengl32 -lgdi32

.PHONY: build
build: game.exe

bin/%.o: src/%.cpp
	g++ $< -std=c++11 -c -Iinc -o $@

game.exe: $(patsubst src/%.cpp,bin/%.o,$(wildcard src/*.cpp))
	g++ $^ -o $@ -std=c++11 $(linkOpenGL)