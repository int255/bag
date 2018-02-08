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



static int sLevel = 0;

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
    int used_capacity = 0;
    std::for_each(choices.begin(), choices.end(), [&items, &used_capacity](char ch){
        auto it = std::find_if(items.begin(), items.end(), [ch](const item_t & item) { return ch ==item.id;});
        if (it != items.end())
        {
            used_capacity += it->weight;
        }
        printf("%c ", ch);
    });
    printf(" Capacity: %d/%d\n", used_capacity, capacity);

}

void fill_bag_v2(int capacity, const std::vector<item_t> & items)
{
    auto C = capacity;
    int n = items.size();
    int width = C + 1;
    int height = n + 1;
    // create (n +1) x (Capacity +1) matrix
    std::vector<int> table(width * height, 0);
    
    
    int * p = table.data();
    // init table
    for ( int x = 0; x<width; ++x)
    {
        p[x] = 0;
    }
    
    int * py = p;
    for (int y = 0; y<height; ++y, py+=width)
    {
        *py = 0;
    }
    
    // y ==  index; x == capacity
    auto fn = [&table, items, width, height](int i, int c)->int{
        if ( i == 0 ) return 0;
        if ( c == 0 ) return 0;
        
        
        int ii = i-1;
        int idx1 = ii * width + c;
        auto & item = items[i-1];
        int cap2 = c - item.weight;
        if (cap2 < 0)
        {
            //cap2 = 0;
            return table[idx1];
        }
        int idx2 = ii * width + cap2;
        
        return std::max(table[idx1], table[idx2] + item.value);
    };
    
    for ( int i = 1; i<height; ++i)
    {
        for (int c = 1; c<width; ++c)
        {
            int idx = i * width + c;
            table[idx] = fn(i, c);
        }
        
    }
    
    
    // dump table
    for (int i = 0; i<height; ++i)
    {
        for (int c = 0; c<width; ++c)
        {
            int idx = i * width + c;
            printf("[%d][%d]:%2d; ", i, c, table[idx]);
        }
        printf("\n");
    }
    
}

int main()
{
    int capacity = 11;//26;
    std::vector<item_t> items = {
        {'a', 1, 1},
        {'b', 2, 6},
        {'c', 5, 18},
        {'d', 6, 22},
        {'e', 7, 28},
//        {'f', 8, 31},
//        {'g', 9, 35},
//        {'h', 11, 40},
    };
    
    fill_bag_v2(capacity, items);
    //fill_bag(capacity, items);
    
    auto items_2 = items;
    items.emplace_back(item_t{'f', 8, 31});
    items.emplace_back(item_t{'g', 9, 35});
    items.emplace_back(item_t{'h', 11, 40});
    int capacity_2 = 26;
    //fill_bag(capacity_2, items_2);
    return 0;
}

