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
    Deploy *order3 = new Deploy(3, 10, new Territory(1, "Territory A", new Continent("Gabeland", 10)));

    Order *order4 = new Order(*order1);
    Deploy *order5 = new Deploy(*order3);

    testList->add(order1);
    testList->add(order2);
    testList->add(order3);
    testList->add(order4);
    testList->add(order5);

    cout << *testList << endl;

    // cout << *order1 << endl;
    // cout << *order3 << endl;
    // cout << order1->getId() << endl;
    // cout << *order4 << endl;
    // cout << *order5 << endl;

    // cout << order3 << " " << order5 << endl;
}

int main()
{
    testOrdersList();

    return 0;
}