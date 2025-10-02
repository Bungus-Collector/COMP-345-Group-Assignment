// Author: Gabriel Lagacé

#include <iostream>
#include "Order.h"
#include "OrdersList.h"

using namespace std;

void testOrdersList()
{
    // ------------------- WIP --------------------
    OrdersList *testList1 = new OrdersList();

    Order *order1 = new Order(1);
    Order *order2 = new Order(2);
    Deploy *order3 = new Deploy(3, 10, new Territory(1, "Territory A", new Continent("Gabeland", 10), new int(10)));

    Order *order4 = new Order(*order1);
    Deploy *order5 = new Deploy(*order3);

    testList1->add(order1);
    testList1->add(order2);
    testList1->add(order3);
    testList1->add(order4);
    testList1->add(order5);

    testList1->move(order1, 2);

    cout << *order5->getTargetTerritory()->getArmies() << endl;
    order5->execute();
    cout << *order5->getTargetTerritory()->getArmies() << endl;

    return; // RETURN

    cout << *testList1 << endl;
    cout << "\n"
         << *order1 << endl;

    testList1->remove(order1);

    cout << *testList1 << endl;
    cout << "\n"
         << *order1 << endl;

    OrdersList testList2 = *testList1;

    list<Order *> *list1 = testList1->getOrders();
    list<Order *> *list2 = testList2.getOrders();

    cout << "list1:" << endl;
    for (Order *o : *list1)
    {
        cout << o << " ";
    }
    cout << endl;

    cout << "list2:" << endl;
    for (Order *o : *list2)
    {
        cout << o << " ";
    }
    cout << endl;

    Order order6 = *order1;

    cout << order1 << " " << &order6 << endl;
}

int main()
{
    testOrdersList();

    return 0;
}