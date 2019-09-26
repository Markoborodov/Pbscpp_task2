#include <ctime>
#include <vector>
#include <assert.h>
#include <algorithm>

struct TItem {
    int value;
    time_t timestamp;

    TItem(int v) : value(v), timestamp(std::time(0)) {}
    // ...
};

using Items = std::vector<TItem>;

// My code --------------------------------------------------------

template<int Arg, int... Args>
Items MakeItemsSimple()
{
    Items items, other;
    items.push_back(Arg);
    if constexpr (sizeof...(Args) != 0)
        other = MakeItemsSimple<Args...>();
    else return items;
    items.reserve(items.size() + other.size());
    items.insert(items.end(), other.begin(), other.end());
    return items;
}

auto MakePredicate(const Items &items)
{
    auto f = [&items](int value) {
        auto res = std::find_if(items.begin(), items.end(),
                [value](const TItem &item) {
                            return item.value == value;
                        } );
        return res != items.end();
    };
    return f;
}

// End of my code ---------------------------------------------------------


int main() {
    Items items = MakeItemsSimple<0, 1, 4, 5, 6>();
    Items newItems = MakeItemsSimple<7, 15, 1>();

    // ....
    auto isFound = MakePredicate(items);
    auto isFoundNew = MakePredicate(newItems);
    assert(isFound(0) == true);
    assert(isFound(7) == false);
    assert(isFoundNew(7) == true);
    assert(isFoundNew(6) == false);
}