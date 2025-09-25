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
    Order *order3 = new Deploy(3, 10, new Territory(1, "Territory A", new Continent("Gabeland", 10)));

    testList->add(order1);
    testList->add(order2);
    testList->add(order3);

    cout << *order1 << endl;
    cout << *order3 << endl;
    cout << order1->getId() << endl;
}

int main()
{
    testOrdersList();

    return 0;
}