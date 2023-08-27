DungeonCrawler: main.cpp
	g++ -o DungeonCrawler main.cpp -lsfml-graphics -lsfml-window -lsfml-system

.PHONY: test clean

test: DungeonCrawler
	./DungeonCrawler

clean:
	rm -f DungeonCrawler
