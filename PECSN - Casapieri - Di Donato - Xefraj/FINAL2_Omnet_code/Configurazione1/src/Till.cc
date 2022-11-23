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

#include "Till.h"
#include "Customer_m.h"

Define_Module(Till);
/**
 * initialzie() take all the parameters from the ini file
 * register the signals
 */
void Till::initialize()
{
    cModule* SuperM=getModuleByPath("SuperMarketConfigurazione1");
    this->checkoutRate= SuperM->par("checkoutRate");
    this->wrappingRate=SuperM->par("wrappingRate");
    this -> distribution= SuperM->par("distribution");

    this -> IdxCheckRate = SuperM->par("rngIdxCheckRate");
    this -> IdxWrapRate = SuperM->par("rngIdxWrapRate");

    NumberOfClientToBeServed=0;
    serverEmpty=true;
    beep_= new cMessage("beep_");
    Client_=new cMessage("Client_");
    this->sWaitingTimeCheckOutAndWrap=registerSignal("waitingTimeCheckOutAndWrap");
    this-> sWaitingTimeCheckOut = registerSignal("waitingTimeCheckOut");
    this-> sResponseTimeCheckOutAndWrap=registerSignal("responseTimeCheckOutAndWrap");
    this-> sResponseTimeCheckOut=registerSignal("responseTimeCheckOut");

}

/**
 * handleMessage:
 * If msg not self msg: A new customer entered the till
 * If msg is self message: A customer ended the checkout or wrap service
 */
void Till::handleMessage(cMessage *msg)
{
    int remainingSteps;
    int doneSteps;
    Customer* custMsg;
    custMsg=check_and_cast<Customer*>(msg);
    if(msg->isSelfMessage())
    {
        isSelfMessage(custMsg);
    }
    else
    {
        isNewCustomer(custMsg);
    }
}



/**
 * isSelfMessage: A customer finished the checkout or wrap service
 *  - If the customer requested services are
 *  different from the done ones - schedule a new message
 *  - else
 *      the customer has finished store is response time
 *      (Two different signals for checkout-only and checkout+wrap costumers)
 *      - If there is some customer in queue, start serving the first one
 */
void Till::isSelfMessage(Customer* custMsg)
{
    int doneSteps;
    int NeededSteps= custMsg->getServicesRequested();

    doneSteps=custMsg->getServicesDone();
    doneSteps++;
    custMsg->setServicesDone(doneSteps);

    //we check if the customer has ended all of the requested operations
    if(NeededSteps!=doneSteps)
    {
        EV<<getName()<<":Client requested Gift Wrap service!"<<endl;
        double timeC=selectDistribution(distribution,1,wrappingRate, IdxWrapRate);
        scheduleAt(simTime() + timeC, custMsg);
    }
    else
    {
        //The customer ended all his operations
        EV<<getName()<<":Client served!!!!"<<endl;
        NumberOfClientToBeServed--;
        //We check if the customer requested only check out
        //or check out + wrap
        if(NeededSteps==1)
        {
            emit(this->sResponseTimeCheckOut,simTime()- custMsg->getEntersQueue());
        }
        else
        {
            emit(this->sResponseTimeCheckOutAndWrap,simTime() - custMsg->getEntersQueue());
        }


        //we have served a customer so if the queue is empty we should wait a new customer
        //else serve the first customer in the queue (FIFO)
        if(OrdersQueue.empty())
        {
            serverEmpty=true;
        }
        else
        {
            Customer c=OrdersQueue.front();
            storedCust= new Customer(c);
            OrdersQueue.pop();
            EV<<getName()<<"Queue size decredes!"<<OrdersQueue.size()<<endl;
            //the waiting time of this customer has ended
            registerWaitingSignals(storedCust);
            requestedCheckOut(storedCust);
        }
    }
}

/**
 * isNewCustomer: called when a new customer enters the till
 *  - Increases the number of customer to be served
 *  - If the server is empty:
 *      Record the waiting time
 *      Start the checkout service (a new customer must pay before any other operation)
 *  else:
 *      put the customer in queue
 */
void Till:: isNewCustomer(Customer* custMsg)
{
    EV<<getName()<<": New customer enters Till"<<endl;

    NumberOfClientToBeServed++;
    custMsg->setEntersQueue(simTime());
    //When a new client arrives if the queue is empty will be served immediately
    //otherwise will join the queue
    if(serverEmpty==true)
    {

        registerWaitingSignals(custMsg);
        serverEmpty=false;
        requestedCheckOut(custMsg);
    }
    else
    {

        OrdersQueue.push(*custMsg);
        EV<<getName()<<":New Client joined the Queue!"<<OrdersQueue.size()<<endl;
    }
}


/**
 * requestedCheckOut:
 *  Schedule a self message to tell a costumer has finished the checkout service
 */
void Till:: requestedCheckOut(Customer* custMsg)
{
    EV<<getName()<<"Start check-out customer"<<endl;
    double timeC=selectDistribution(distribution,1,checkoutRate, IdxCheckRate);
    scheduleAt(simTime() + timeC, custMsg);
}

/**
 * registerWaitingSignals: called when a costumer exits the queue
 *  The checkout-only costumer informations and checkout+wrap
 *  are stored in different signals
 *
 */
void Till:: registerWaitingSignals(Customer* custMsg)
{
    if(custMsg->getServicesRequested()==1)
        emit(this-> sWaitingTimeCheckOut,simTime() - custMsg->getEntersQueue());
    else
        emit(this-> sWaitingTimeCheckOutAndWrap,simTime() - custMsg->getEntersQueue());
}

int Till::getNumberOfClientToBeServed()
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
 *      - rate:
 *          Since in policy1 the same till performs the checkout and wrap
 *          this parameter gives us the ability to customize the rate of the
 *          checkout and wrap distribution (different values)
 *      - rngIdx:
 *          Random generator index (different for checkout and wrap services)
 *
 * The mean value of each distribution is taken from the ini file.
 * Since the Uniform distribution needs the interval [a,b] the values
 * a=1/10mean b=19/10mean
 */
double Till::selectDistribution(int distribution, int numberOfClient, double rate, int rngIdx){
        double timeC;
        if(distribution==0){
            timeC=exponential((1/rate)*numberOfClient, rngIdx); //take as input the avg
        }
        if(distribution==1){
            timeC=
                    uniform((1/rate)*numberOfClient - (1/rate)*numberOfClient*9/10 ,
                            (1/rate)*numberOfClient + (1/rate)*numberOfClient*9/10,
                            rngIdx);
        }
        if(distribution==2){
            timeC=1/rate;
        }
        return timeC;
}


