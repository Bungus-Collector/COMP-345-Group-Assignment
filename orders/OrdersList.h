// Author: Gabriel Lagacé

#ifndef ORDERSLIST_H
#define ORDERSLIST_H

#include <list>
#include "Order.h"

class OrdersList
{
private:
    list<Order *> *Orders;

public:
    OrdersList();
    ~OrdersList();
    int add(Order *);
    int move(int, int);
    int remove(int);     // Remove by ID
    int remove(Order *); // Remove by pointer
};

#endif