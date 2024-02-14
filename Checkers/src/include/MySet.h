#pragma once

#include "../Constants.h"
#include "Pair.h"

class MySet
{
public:
	std::unordered_map<int, std::vector<std::vector<Pair>>> container;

	MySet() = default;
	virtual ~MySet() = default;

	std::vector<std::vector<Pair>> at(std::vector<Pair>& elem);

	std::vector<std::vector<Pair>> elements();

	void add(std::vector<Pair>& elem);

	void remove(std::vector<Pair>& elem);

private:
	int hashFunction(std::vector<Pair>& elem);
};