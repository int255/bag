#include <stdio.h>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>
#include <type_traits>

struct item_t
{
    char id;
    int weight;
    int value;
};

//template <class T>
//void inspect(const std::vector<T> & imps, int * outTotalValue, int * usedCapacity, std::string * outStr = nullptr)
//{
//    int total_value = 0;
//    int used_capacity = 0;
//    char buf[256];
//    std::string str;
//    for (int i = 0; i<imps.size(); ++i)
//    {
//        if (imps[i].chosen)
//        {
//            total_value += imps[i].value;
//            used_capacity += imps[i].weight;
//            str.append("Y");
//        }
//        sprintf(buf, "(%c v:%d, w:%d)", imps[i].id, imps[i].value, imps[i].weight);
//        str.append(buf);
//        str.append(", ");
//    }
//
//    if (outTotalValue)
//        *outTotalValue = total_value;
//
//    if (usedCapacity)
//        *usedCapacity = used_capacity;
//
//    if (outStr)
//        *outStr = str;
//}

std::vector<char> init_tags()
{
    std::vector<char> tags;
    for ( char ch = 'A'-1; ch<='Z'; ++ch)
    {
        tags.push_back(ch);
    }
    return tags;
}

static int sLevel = 0;
static std::vector<char> sTags = init_tags();
static int k = 0;

class ScopedLevel
{
public:
    ScopedLevel(){ ++sLevel;}
    ~ScopedLevel() {--sLevel;}
private:
    ScopedLevel(const ScopedLevel& rhs);
    ScopedLevel& operator=(const ScopedLevel& rhs);

};

// return the max value
int fill(int capacity, const std::vector<item_t> & items, std::vector<char> & choices)
{
    ScopedLevel scoped_level;
    int ret = 0;
    while (1)
    {
        auto vi = items;
        // ran out of items
        if (items.empty())
        {
            ret = 0;
            break;
        }
        
        auto it = std::find_if(vi.begin(), vi.end(), [capacity](const item_t & item){
            return item.weight <= capacity;
        });
        
        // ran out of capacity
        if (it == vi.end())
        {
            ret = 0;
            break;
        }
        
        auto item = *it;
        assert(it!=vi.end());
        vi.erase(it);
        
        // take it
        std::vector<char> choices1;
        int value1 = fill(capacity - item.weight, vi, choices1);
        
        
        // don't take it
        std::vector<char> choices2;
        int value2 = fill(capacity, vi, choices2);
        
        if (value1 + item.value < value2 )
        {
            // honor 2
            choices = choices2;
            ret = value2;
            break;
        } else {
            // honor 1
            choices = choices1;
            choices.push_back(item.id);
            ret = value1 + item.value;
            break;
        }
    }
    std::string items_str;
    char buf[256];
    for (auto & item: items)
    {
        sprintf(buf, "%c ", item.id);
        items_str.append(buf);
    }
    std::string choices_str;
    for (auto & ch : choices)
    {
        sprintf(buf, "%c ", ch);
        choices_str.append(buf);
    }
    printf("Return: %d. fill(%3d, \"%s\", choices \"%s\")\n",
           ret,
           capacity,
           items_str.c_str(),
           choices_str.c_str());
    return ret;
}

void fill_bag(int capacity, const std::vector<item_t> & items)
{
    static_assert(std::is_pod<item_t>::value, "item_t must be pod");
    std::vector<char> choices;
    auto value = fill(capacity, items, choices);
    printf("max value: %d, choices:", value);
    std::for_each(choices.begin(), choices.end(), [](char ch){
        printf("%c ", ch);
    });
    printf("\n");

}

int main()
{
    std::vector<item_t> items = {
        {'a', 1, 1},
        {'b', 2, 6},
        {'c', 5, 18},
        {'d', 6, 22},
        {'e', 7, 28},
        {'f', 8, 31},
        {'g', 9, 35},
        {'h', 11, 40},
    };
    
    int capacity = 26;
    fill_bag(capacity, items);
    return 0;
}

