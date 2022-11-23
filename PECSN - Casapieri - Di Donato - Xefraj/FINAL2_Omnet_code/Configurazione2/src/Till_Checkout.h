//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef __CONFIGURAZIONE2_TILL_CHECKOUT_H_
#define __CONFIGURAZIONE2_TILL_CHECKOUT_H_

#include <omnetpp.h>
#include <queue>
#include "Customer_m.h"

using namespace omnetpp;

/**
 * TODO - Generated class
 */
class Till_Checkout : public cSimpleModule
{
private:
    cMessage* beep_;
    cMessage* Client_;
    Customer* storedCust;
    std::queue<Customer> OrdersQueue;
    bool serverEmpty;
    double checkoutRate;
    int NumberOfClientToBeServed;
    int NumberTillsWrap;
    int distribution;

    int IdxCheckRate;
    int IdxWrapSelection;

    simsignal_t sWaitingTime;
    simsignal_t sResponseTime;
protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void isNewCustomer(Customer* custMsg);
    virtual void customerFinished(Customer* custMsg);
    virtual void requestedCheckout(Customer* custMsg);
    virtual int getIndexWrapTill();
    virtual void serveNextCustomerInQueue();
    virtual double selectDistribution(int distribution, int numberOfClient);

public:
    virtual int getNumberOfClientToBeServed();
};

#endif
