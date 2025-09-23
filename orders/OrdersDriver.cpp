// Author: Gabriel Lagacé

#include <iostream>
#include "Order.h"
#include "OrdersList.h"

using namespace std;

void testOrdersList()
{
    // ------------------- WIP --------------------
    OrdersList *testList = new OrdersList();

    Order *order1 = new Order(1);
    Order *order2 = new Order(2);
    Order *order3 = new Order(3);

    testList->add(*order1);
    testList->add(*order2);
    testList->add(*order3);

    delete order1;
    delete order2;
    delete order3;
    delete testList;
}

int main()
{
    testOrdersList();

    return 0;
}