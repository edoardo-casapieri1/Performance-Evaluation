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

#include "Till_Wrap.h"
#include "Customer_m.h"

Define_Module(Till_Wrap);

void Till_Wrap::initialize()
{
    cModule* SuperM=getModuleByPath("Try");
    this-> wrappingRate= SuperM->par("wrappingRate");
    this-> distribution = SuperM->par("distribution");

    this-> idxWrapRate = SuperM->par("rngIdxWrapRate");

    NumberOfClientToBeServed=0;
    serverEmpty=true;
    beep_= new cMessage("beep_");
    Client_=new cMessage("Client_");
    this -> sWaitingTime=registerSignal("waitingTimeCheckOutAndWrap");
    this-> sResponseTime=registerSignal("responseTimeCheckOutAndWrap");
}

void Till_Wrap::handleMessage(cMessage *msg)
{
    Customer* custMsg;
    custMsg=check_and_cast<Customer*>(msg);

    if(msg->isSelfMessage())
    {
        customerFinished(custMsg);
    }
    else
    {
        isNewCustomer(custMsg);
    }
}


/** customerFinished:
 * When  costumer finish the wrapping service is ready to leave the shop:
 *      - store his response time
 *      - Serve the next costumer in queue if exists (store its waiting time)
 */
void Till_Wrap::customerFinished(Customer* custMsg)
{
            EV<<getName()<<":Client served!!!!"<<endl;
            NumberOfClientToBeServed--;
            emit(this->sResponseTime,simTime()-custMsg->getEntersSystem());
            EV<<getName()<<simTime() - custMsg-> getEntersSystem()<<endl;
            if(OrdersQueue.empty())
            {
                EV<<"In ATTESA CODA VUOTA"<<endl;
                serverEmpty=true;
            }
            else
            {
                Customer c=OrdersQueue.front();
                storedCust= new Customer(c);
                OrdersQueue.pop();
                //A customer has ended the queue so he is ready to be served
                //the waiting time is the total time waited in the Checkout till and
                // Wrap till
                emit(this->sWaitingTime,((simTime()-storedCust->getEntersQueueWrap())+
                                        (storedCust->getEntersTillCheckout()-storedCust->getEntersSystem())));

                //OLD ERROR
                //emit(this->sWaitingTime,((simTime()-custMsg->getEntersQueueWrap())+
                //        (custMsg->getEntersTillCheckout()-custMsg->getEntersSystem())));
                requestedWrap(storedCust);

            }

}

/**
 * isNewCostumer:
 *  start serving the costumer if the queue is empty (store its waiting time)
 *  else put the costumer in queue (store the wrap queue entering time)
 */
void Till_Wrap::isNewCustomer(Customer* custMsg)
{

    NumberOfClientToBeServed++;
    custMsg->setEntersQueueWrap(simTime());
    //if the server is Empty the customer is served
    if(serverEmpty==true)
    {
        //If the customer enters in an empty till the total waiting time
        //is the time he waited in the checkout till
        emit(this->sWaitingTime,custMsg->getEntersTillCheckout()-custMsg->getEntersSystem());
        serverEmpty=false;
        requestedWrap(custMsg);
    }
    else
    {
        custMsg->setEntersQueueWrap(simTime());
        OrdersQueue.push(*custMsg);
        EV<<getName()<<":New Client joined the Queue!"<<OrdersQueue.size()<<endl;
    }
}

void Till_Wrap::requestedWrap(Customer* custMsg)
{
    EV<<getName()<<"Start check-out customer"<<endl;
    double timeC=selectDistribution(distribution,1);
    scheduleAt(simTime() + timeC, custMsg);
}


int Till_Wrap::getNumberOfClientToBeServed()
{
    return NumberOfClientToBeServed;
}

/**
 * selectDistribution
 * INPUT:
 *      - distribution:
 *          0-for exponential; 1-for uniform; 2-for deterministic
 *      - numberOfClient:
 *          This parameter is always set to 1;
 *          Has been introduced to allow a discouraged arrival study
 *          (not performed in this project)
 *
 * The mean value of each distribution is taken from the ini file.
 * Since the Uniform distribution needs the interval [a,b] the values
 * a=1/10mean b=19/10mean
 */
double Till_Wrap::selectDistribution(int distribution, int numberOfClient){
        double timeC;
        if(distribution==0){
            timeC=exponential((1/wrappingRate)*numberOfClient,
                    idxWrapRate); //take as input the avg
        }
        if(distribution==1){
            timeC=
                    uniform((1/wrappingRate)*numberOfClient - (1/wrappingRate)*numberOfClient*9/10 ,
                            (1/wrappingRate)*numberOfClient + (1/wrappingRate)*numberOfClient*9/10,
                            idxWrapRate);
        }
        if(distribution==2){
                    timeC=1/wrappingRate;
                }
        return timeC;
}

