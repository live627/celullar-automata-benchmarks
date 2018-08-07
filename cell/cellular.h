#pragma once

#include <vector>
#include <memory>
#include <time.h>

class Cellular2d
{
	std::vector<std::vector<bool>> map;
	int width, height;
public:
	Cellular2d(const int width, const int height)
		:
		width(width),
		height(height)
	{
		map.resize(height, std::vector<bool>(width));

		Generate();  //setting up the map grid
		for (int k = 0; k < 5; k++) //run sim a few times to smooth out map
			Smooth();
	}
	auto GetMap() const { return map; }

private:
	void Generate()
	{
		int chanceToStartAlive = 41;  //percentage of being a wall tile at start

		for (int h = 0; h < height; h++)
			for (int w = 0; w < width; w++) //initialising values here
				map[h][w] = rand() % 100 + 1 < chanceToStartAlive;
	}
	// Iterates through every tile in the map and decides if needs to be born, die, or remain unchanged
	void Smooth()
	{
		for (int h = 0; h < map.size(); h++)
		{
			for (int w = 0; w < map[0].size(); w++)
			{
				int newVal = countAliveNeighbours(w, h);
				if (map[h][w])
					map[h][w] = !(newVal < 3);
				else
					map[h][w] = newVal > 4;
			}
		}
	}
	// Counts the number of "alive" cells around the target cell, given x and y coords
	int8_t countAliveNeighbours(const int x, const int y) const
	{
		int8_t count = 0;
		for (int i = -1; i < 2; i++)
		{
			for (int j = -1; j < 2; j++)
			{
				int neighbour_x = x + i;
				int neighbour_y = y + j;

				/*
				Count the neighbour as "alive" if:
				- it is within the map boundaries
				- was already deemed "alive"
				- is not the target cell
				*/
				if ((IsInBounds(neighbour_x, neighbour_y) || map[neighbour_y][neighbour_x]) && !(i == 0 && j == 0))
					count++;
			}
		}
		return count;
	}
	// Determines whether a cell is within the map, given x and y coords
	bool IsInBounds(const int x, const int y) const
	{
		return x < 0 || y < 0 || x >= map[0].size() || y >= map.size();
	}
};

class Cellular1d
{
	std::vector<bool> map;
	int width, height;
	unsigned int seed;
public:
	Cellular1d(const int & width, const int & height, const unsigned int & seed = time(nullptr))
		:
		width(width),
		height(height),
		seed(seed)
	{
		map.resize(height * width);

		Generate();  //setting up the map grid
		for (int k = 0; k < 5; k++) //run sim a few times to smooth out map
			Smooth();
	}
	auto GetMap() const { return map; }

private:
	void Generate()
	{
		// Seed the random-number generator with the current time so that
		// the numbers will be different every time we run.
		//srand(seed);
		
		// Percentage of being a wall tile at start
		int chanceToStartAlive = 41; 

		for (int index = 0; index < map.size(); index++)
			map[index] = rand() % 100 + 1 < chanceToStartAlive;
	}
	// Iterates through every tile in the map and decides if needs to be born, die, or remain unchanged
	void Smooth()
	{
		for (int index = 0; index < map.size(); index++)
		{
			int newVal = countAliveNeighbours(index);
			if (map[index])
				map[index] = !(newVal < 3);
			else
				map[index] = newVal > 4;
		}
	}
	// Counts the number of "alive" cells around the target cell
	int8_t countAliveNeighbours(const int & index) const
	{
		int8_t count = 0;
		int x = index / width, y = index % width;
		for (int i = -1; i < 2; i++)
		{
			for (int j = -1; j < 2; j++)
			{
				int neighbour = (x + i) * width + (y + j);

				/*
				Count the neighbour as "alive" if:
				- it is within the map boundaries
				- was already deemed "alive"
				- is not the target cell
				*/
				if ((IsInBounds(neighbour) || map[neighbour]) && !(i == 0 && j == 0))
					count++;
			}
		}
		return count;
	}
	// Determines whether a cell is within the map
	bool IsInBounds(const int & index) const
	{
		return index < 0 || index >= map.size();
	}
};

class Cellular
{
	std::unique_ptr<bool[]> map;
	int width, height, size;
	unsigned int seed;
public:
	Cellular(const int & width, const int & height, const unsigned int & seed = time(nullptr))
		:
		width(width),
		height(height),
		size(height * width),
		seed(seed)
	{
		map = std::make_unique<bool[]>(size);

		Generate();  //setting up the map grid
		for (int k = 0; k < 5; k++) //run sim a few times to smooth out map
			Smooth();
	}
	auto GetMap() const { return map.get(); }

private:
	void Generate()
	{
		// Seed the random-number generator with the current time so that
		// the numbers will be different every time we run.
		srand(seed);
		
		// Percentage of being a wall tile at start
		int chanceToStartAlive = 41; 

		for (int index = 0; index < size; index++)
			map[index] = rand() % 100 + 1 < chanceToStartAlive;
	}
	// Iterates through every tile in the map and decides if needs to be born, die, or remain unchanged
	void Smooth()
	{
		for (int index = 0; index < size; index++)
		{
			int newVal = countAliveNeighbours(index);
			if (map[index])
				map[index] = !(newVal < 3);
			else
				map[index] = newVal > 4;
		}
	}
	// Counts the number of "alive" cells around the target cell
	int8_t countAliveNeighbours(const int & index) const
	{
		int8_t count = 0;
		int x = index / width, y = index % width;
		for (int i = -1; i < 2; i++)
		{
			for (int j = -1; j < 2; j++)
			{
				int neighbour = (x + i) * width + (y + j);

				/*
				Count the neighbour as "alive" if:
				- it is within the map boundaries
				- was already deemed "alive"
				- is not the target cell
				*/
				if ((IsInBounds(neighbour) || map[neighbour]) && !(i == 0 && j == 0))
					count++;
			}
		}
		return count;
	}
	// Determines whether a cell is within the map
	bool IsInBounds(const int & index) const
	{
		return index < 0 || index >= size;
	}
};