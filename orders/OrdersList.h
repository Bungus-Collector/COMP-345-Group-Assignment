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
    // Constructors and destructor
    OrdersList();
    OrdersList(const OrdersList &);
    ~OrdersList();

    // Stream insertion operator
    friend std::ostream &operator<<(std::ostream &os, const OrdersList &ordersList);

    // Overloading assignment operator
    OrdersList &operator=(const OrdersList &);

    // Getter method
    list<Order *> *getOrders();

    // Setter method
    void setOrders(list<Order *> *);

    /**
     * @brief Adds an order to the list.
     *
     * @param Order to be added
     * @return 0 for success, error code otherwise
     */
    int add(Order *);

    /**
     * @brief Moves an order to a new position in the list.
     *
     * @param Order to be moved
     * @param int position in list
     * @return 0 for success, error code otherwise
     */
    int move(Order *, int);

    /**
     * @brief Removes an order by ID.
     *
     * @param Order to be moved
     * @param int new position
     * @return 0 for success, error code otherwise
     */
    int remove(int); // Remove by ID

    /**
     * @brief Removes an order by pointer.
     *
     * @param Order to be removed
     * @return 0 for success, error code otherwise
     */
    int remove(Order *); // Remove by pointer
};

#endif