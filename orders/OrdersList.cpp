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

OrdersList::OrdersList(const OrdersList &other)
{
    Orders = new list<Order *>();

    for (Order *o : *other.Orders)
    {
        if (o)
        {
            Orders->push_back(new Order(*o));
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

std::ostream &operator<<(ostream &os, const OrdersList &ordersList)
{
    os << "Orders List:\n";
    for (Order *o : *ordersList.Orders)
    {
        if (o)
        {
            os << *o << "\n";
        }
    }
    return os;
}

OrdersList &OrdersList::operator=(const OrdersList &other)
{
    if (this == &other)
    {
        return *this;
    }

    for (Order *o : *Orders)
    {
        delete o;
    }
    Orders->clear();

    for (Order *o : *other.Orders)
    {
        this->Orders->push_back(new Order(*o));
    }

    return *this;
}

list<Order *> *OrdersList::getOrders()
{
    return Orders;
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
            // delete *it;
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
        // delete *it;
        Orders->erase(it);
        return 0; // Success
    }

    return 1; // Not found
}