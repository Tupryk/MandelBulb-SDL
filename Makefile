all:
	g++ -std=c++11 -Wall -Iinclude src/main.cpp -o p -F/Library/Frameworks -framework SDL2 src/App.cpp

clean:
	rm p