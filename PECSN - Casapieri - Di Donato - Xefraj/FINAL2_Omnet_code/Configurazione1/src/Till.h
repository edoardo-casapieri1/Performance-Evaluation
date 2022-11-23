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

#ifndef __CONFIGURAZIONE1_TILL_H_
#define __CONFIGURAZIONE1_TILL_H_

#include <omnetpp.h>
#include <queue>
#include "Customer_m.h"

using namespace omnetpp;

/**
 * TODO - Generated class
 */
class Till : public cSimpleModule
{
  private:
    cMessage* beep_;
    cMessage* Client_;
    Customer* storedCust;
    std::queue<Customer> OrdersQueue;
    bool serverEmpty;
    int NumberOfClientToBeServed;
    int distribution;

    int IdxCheckRate;
    int IdxWrapRate;

    double checkoutRate;
    double wrappingRate;
    simsignal_t sWaitingTimeCheckOutAndWrap;
    simsignal_t sWaitingTimeCheckOut;
    simsignal_t sResponseTimeCheckOutAndWrap;
    simsignal_t sResponseTimeCheckOut;
    /*simsignal_t sWaitingTime;
    simsignal_t sNumbOfCust;
    simsignal_t sResponseTime;*/
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void isSelfMessage(Customer* custMsg);
    virtual void isNewCustomer(Customer* custMsg);
    virtual void requestedCheckOut(Customer* custMsg);
    virtual void registerWaitingSignals(Customer* custMsg);
    virtual double selectDistribution(int distribution, int numberOfClient, double rate, int rngIdx);

  public:
    virtual int getNumberOfClientToBeServed();
};

#endif
