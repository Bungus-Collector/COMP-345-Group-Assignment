// Author: Gabriel Lagacé

#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <algorithm>
#include "OrdersList.h"
#include "OrdersErrorCodes.h"

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
            Orders->push_back(o->clone());
        }
    }
}

OrdersList::~OrdersList()
{
    for (Order *o : *Orders)
    {
        delete o;
    }
    Orders->clear();

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
        this->Orders->push_back(o->clone());
    }

    return *this;
}

std::string OrdersList::stringToLog() {
    std::ostringstream oss;

    oss << "New order added to OrdersList: " << *(Orders->back());

    return oss.str();
}

list<Order *> *OrdersList::getOrders()
{
    return Orders;
}

void OrdersList::setOrders(list<Order *> *o)
{
    delete Orders;
    Orders = o;
}

int OrdersList::add(Order *o)
{
    if (!o)
    {
        return INVALID_INPUT;
    }

    Orders->insert(Orders->end(), o);

    notify(this);

    return SUCCESS;
}

int OrdersList::move(Order *o, int pos)
{
    if (!o || !Orders)
    {
        return INVALID_INPUT;
    }

    if (pos < 0 || pos > static_cast<int>(Orders->size()))
    {
        return OUT_OF_BOUNDS;
    }

    auto it = std::find(Orders->begin(), Orders->end(), o);
    if (it == Orders->end())
    {
        return NOT_FOUND;
    }

    Orders->erase(it);
    auto insertIt = Orders->begin();
    std::advance(insertIt, pos);
    Orders->insert(insertIt, o);

    return SUCCESS;
}

int OrdersList::remove(int i)
{
    for (auto it = Orders->begin(); it != Orders->end(); ++i)
    {
        if ((*it)->getId() == i)
        {
            Orders->erase(it);
            return SUCCESS;
        }
    }

    return NOT_FOUND;
}

int OrdersList::remove(Order *o)
{
    if (!o)
    {
        return INVALID_INPUT;
    }

    auto it = std::find(Orders->begin(), Orders->end(), o);
    if (it != Orders->end())
    {
        delete *it;
        Orders->erase(it);
        return SUCCESS;
    }

    return NOT_FOUND;
}