// Author: Gabriel Lagacé

#ifndef ORDERSLIST_H
#define ORDERSLIST_H

#include <list>
#include "Order.h"

class OrdersList
{
private:
    list<Order> Orders;

public:
    OrdersList();
    ~OrdersList();
    bool move(int, int);
    bool remove(int);
};

#endif