#include "../include/MySet.h"

int MySet::hashFunction(std::vector<Pair>& elem)
{
	std::hash<int> hasher;
	int answer = 0;

	for (Pair pair : elem)
	{
		answer ^= hasher(pair.x) + hasher(pair.y) + 0x9e3779b9 + (answer << 6) + (answer >> 2);
	}
	return answer;
}

std::vector<std::vector<Pair>> MySet::at(std::vector<Pair>& elem)
{
	return container[hashFunction(elem)];
}

std::vector<std::vector<Pair>> MySet::elements()
{
	std::vector<std::vector<Pair>> elems;

	for (auto& it : container)
	{
		for (auto& elem : it.second)
		{
			if (elem.size() > 0)
			{
				elems.push_back(elem);
			}
		}
	}

	return elems;
}

void MySet::add(std::vector<Pair>& elem)
{
	if (container.find(hashFunction(elem)) == container.end())
	{
		container.emplace(hashFunction(elem), std::vector<std::vector<Pair>>());
	}
	auto& v = container.at(hashFunction(elem));
	if (std::find(v.begin(), v.end(), elem) == v.end())
	{
		v.push_back(elem);
	}
}

void MySet::remove(std::vector<Pair>& elem)
{
	if (container.find(hashFunction(elem)) == container.end())
	{
		return;
	}
	auto& v = container.at(hashFunction(elem));
	std::vector<std::vector<Pair>> w(v.size());
	std::remove_copy(v.begin(), v.end(), w.begin(), elem);
	container.erase(hashFunction(elem));
	container.emplace(hashFunction(elem), w);
}