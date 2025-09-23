// Author: Gabriel Lagacé

#include <iostream>
#include <string>
#include <list>
// #include <bits/stdc++.h>
#include "OrdersList.h"

using namespace std;

OrdersList::OrdersList()
    : Orders{new list<Order>}
{
}

int OrdersList::add(Order o)
{
    Orders->insert(Orders->end(), o);

    return 0;
}

// // To be tested
// int OrdersList::move(int id, int toIndex)
// {
//     if (Orders.empty())
//         return 0;

//     // Find the iterator to the element with the given ID
//     auto fromIt = std::find_if(Orders.begin(), Orders.end(), [id](Order &o)
//                                { return o.getId() == id; });

//     if (fromIt == Orders.end())
//     {
//         // Order ID not in list
//         return 1;
//     }

//     // Find current index of the element
//     size_t fromIndex = std::distance(Orders.begin(), fromIt);

//     if (fromIndex == toIndex)
//     {
//         // Order already in specified position
//         return 0;
//     }

//     if (toIndex > Orders.size())
//     {
//         // Index out of range
//         return 2;
//     }

//     // Get iterator to the target position
//     auto toIt = Orders.begin();
//     std::advance(toIt, toIndex);

//     // If moving forward, need to increment target position because
//     // `fromIt` will be removed before insert point
//     if (fromIndex < toIndex)
//     {
//         ++toIt;
//     }

//     Orders.splice(toIt, Orders, fromIt);
// }