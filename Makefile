DungeonCrawler: main.cpp
	g++ -o DungeonCrawler src/*.cpp main.cpp -I SFML-2.5.1/include -L SFML-2.5.1/lib -lsfml-graphics -lsfml-window -lsfml-system

.PHONY: test clean

test: DungeonCrawler
	export LD_LIBRARY_PATH=SFML-2.5.1/lib && ./DungeonCrawler

clean:
	rm -f DungeonCrawler
