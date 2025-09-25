// Author: Gabriel Lagacé

#include <iostream>
#include <string>
#include <list>
#include <algorithm>
#include "OrdersList.h"

using namespace std;

OrdersList::OrdersList()
    : Orders{new list<Order *>}
{
}

OrdersList::~OrdersList()
{
    for (Order *o : *Orders)
    {
        delete o;
    }

    delete Orders;
    Orders = nullptr;
}

int OrdersList::add(Order *o)
{
    Orders->insert(Orders->end(), o);

    return 0;
}

int OrdersList::remove(int i)
{
    for (auto it = Orders->begin(); it != Orders->end(); ++i)
    {
        if ((*it)->getId() == i)
        {
            delete *it;
            Orders->erase(it);
            return 0; // Success
        }
    }

    return 1; // Not found
}

int OrdersList::remove(Order *o)
{
    auto it = std::find(Orders->begin(), Orders->end(), o);
    if (it != Orders->end())
    {
        delete *it;
        Orders->erase(it);
        return 0; // Success
    }

    return 1; // Not found
}