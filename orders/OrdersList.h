// Author: Gabriel Lagacé

#ifndef ORDERSLIST_H
#define ORDERSLIST_H

#include <list>
#include "Order.h"

class OrdersList
{
public:
    list<Order> *Orders;

    OrdersList();
    int add(Order);
    int move(int, int);
    int remove(int);
};

#endif