// Author: Gabriel Lagacé

#include <iostream>
#include <string>
#include <list>
#include <algorithm>
#include "OrdersList.h"

using namespace std;

// Overloading the stream insertion operator
std::ostream &operator<<(ostream &os, const OrdersList &ordersList)
{
    os << "Orders List:\n";
    for (Order *o : *ordersList.Orders)
    {
        if (o) // Check for null pointers
        {
            os << *o << "\n";
        }
        else
        {
            os << "NULL\n";
        }
    }
    return os;
}

OrdersList::OrdersList()
    : Orders{new list<Order *>}
{
}

OrdersList::OrdersList(const OrdersList &other)
{
    list<Order *> *newOrders = new list<Order *>();

    for (Order *o : *other.Orders)
    {
        if (o) // Check for null pointers
        {
            newOrders->push_back(new Order(*o));
        }
        else
        {
            newOrders->push_back(nullptr); // Preserve null pointers
        }
    }
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