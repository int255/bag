#include <stdio.h>
#include <vector>
#include <string>
#include <algorithm>

struct item_t
{
	int weight;
	int value; 
};

struct imp_t : public item_t
{
	bool can_choose;
	bool chosen;
};

static int sLevel = 0;
int fill(int capacity, std::vector<imp_t> & imps)
{
	++sLevel;

	int value = 0;

	auto it = std::find_if(imps.begin(), imps.end(), [](const imp_t & imp){
		return imp.can_choose;
	});

	if ( imps.end() == it)
	{
		value = 0;
	} else {

		int v = 0;
		{
			// skip item
			it->can_choose = false;
			int cap1 = capacity;
			int v1 = fill(cap1, imps);

			it->chosen = true;
			v = it->value;
			int cap2 = capacity - it->weight;
			int v2 = 0;
			if (cap2 >= 0)
			{
				v2 = fill(cap2, imps);
			}
			
			bool skip = (v2 + it->value) < v1;
			if ( skip )
			{
				it->can_choose = false;
				it->chosen = false;
				value += v1;
			} else {
				it->chosen = true;
				it->can_choose = false;
				value += v2;
			}
		}
	}
	--sLevel;

	std::string str;
	char buf[256];
	for (int i = 0; i<imps.size(); ++i)
	{
		if (imps[i].chosen)
		{
			sprintf(buf, "(i:%d, v:%d, w: %d),", i, imps[i].value, imps[i].weight);
			str.append(buf);
		}
	}
	printf("lvl: %d, total_value:%d, chosen:%s\n", sLevel, value, str.c_str());
	return value;
}

void fill_bag(int capacity, const std::vector<item_t> & items)
{
	std::vector<imp_t> imps;
	for ( auto & item : items)
	{
		imp_t imp;
		imp.weight = item.weight;
		imp.value = item.value;
		imp.can_choose = true;
		imp.chosen = false;
		imps.emplace_back(std::move(imp));
	}
	fill(capacity, imps);
}

int main()
{
	std::vector<item_t> items = {
		{1, 1},
		{2, 3},
		{10, 6},
		{5, 8},
		{3, 5},
	};

	int capacity = 15;
	fill_bag(capacity, items);
	return 0;
}
