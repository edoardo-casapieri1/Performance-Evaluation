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

#include "Till_Checkout.h"
#include "Till_Wrap.h"
#include <string>

Define_Module(Till_Checkout);

void Till_Checkout::initialize()
{
    cModule* SuperM=getModuleByPath("Try");
    this-> checkoutRate= SuperM->par("checkoutRate");

    //RNG indexes
    this -> IdxCheckRate = SuperM->par("rngIdxCheckRate");
    this -> IdxWrapSelection = SuperM -> par("rngIdxTillSelectionWrap");

    NumberOfClientToBeServed=0;
    serverEmpty=true;
    beep_= new cMessage("beep_");
    Client_=new cMessage("Client_");
    this -> NumberTillsWrap= SuperM->par("NWrap");
    this -> distribution = SuperM->par("distribution");
    this -> sWaitingTime=registerSignal("waitingTimeCheckOut");
    this -> sResponseTime=registerSignal("responseTimeCheckOut");
}

void Till_Checkout::handleMessage(cMessage *msg)
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



int Till_Checkout::getNumberOfClientToBeServed()
{
    return NumberOfClientToBeServed;
}

/**
 * When a new customer arrives if the server is empty the customer is
 * served immediately and its waiting time is registered otherwise joins the queue
 */
void Till_Checkout::isNewCustomer(Customer* custMsg)
{
    NumberOfClientToBeServed++;
    custMsg->setEntersSystem(simTime());
    if(serverEmpty==true)
    {
        custMsg->setEntersTillCheckout(simTime());
        /* ******************************************************************************** */
        if(custMsg->getServicesRequested()==1)
            emit(this->sWaitingTime,custMsg->getEntersTillCheckout() - custMsg->getEntersSystem());
        /* ******************************************************************************** */
        EV<<getName()<<"Sono il Primo evviva!"<<endl;
        serverEmpty=false;
        requestedCheckout(custMsg);
    }
    else
    {
        OrdersQueue.push(*custMsg);
        EV<<getName()<<":New Client joined the Queue!"<<OrdersQueue.size()<<endl;
    }
}

/**
 * When a customer finishes the check-out service\
 * - If is a checkout-only costumer: register its response time
 * - If wrap service is requested - send the costumer to a wrapping till
 * - If there is a costumer in queue start it's service
 */
void Till_Checkout:: customerFinished(Customer* custMsg)
{
    int remainingSteps;
    NumberOfClientToBeServed--;
    remainingSteps= custMsg->getServicesRequested();
    if(remainingSteps == 2)//if gift-wrap requested
    {
        custMsg->setExitCheckout(simTime());
        //NOT USED:
        //  Select the wrapping till with less costumers
        //index_min is the index of the wrapping till
        //with the minimum number of customer waiting

        //**************************************************
        //int index_min=getIndexWrapTill();
        //**************************************************

        int tillToSendTo = intuniform(0, NumberTillsWrap - 1, IdxWrapSelection);
        send(custMsg, "toWrap", tillToSendTo);

    }
    else
    {
        //the customer has leaved our shop so we can save it's waiting time and
        //response time
        //Waiting Time=When he entered the till - when he entered the system
        //emit(this->sWaitingTime,custMsg->getEntersTillCheckout() - custMsg->getEntersSystem());
        //Response Time=When he exit the till - when he entered the system
        emit(this->sResponseTime,simTime() - custMsg->getEntersSystem());
    }
    EV<<getName()<<":Client served!!!!"<<endl;
    //check if someone is waiting in the queue
    serveNextCustomerInQueue();
}

void Till_Checkout::requestedCheckout(Customer* custMsg)
{
    EV<<getName()<<"Start check-out customer"<<endl;
    double timeC=selectDistribution(distribution,1);
    scheduleAt(simTime() + timeC, custMsg);
}


/** NOT_USED:
 * This function check for the wrap till with the minimum amount of
 * customer in the till (including the one currently in service)
 * in case two or more tills have the minimum number of customers
 * is selected the one with the minimum index
 */
int Till_Checkout::getIndexWrapTill()
{
    int numberOfClient;
    int min;
    int index_min;
    std::string Path("Try.TillWrap");
    cModule* SuperM=getModuleByPath("Try");
    int NumberWrap= SuperM->par("NWrap");

    for(int i = 0; i < NumberWrap; i++){
        std::string CompletePath;
        CompletePath = Path + "[" + std::to_string(i) + "]";
        cModule* actualTill=getModuleByPath(CompletePath.c_str());
        Till_Wrap* currentTill=check_and_cast<Till_Wrap*>(actualTill);
        numberOfClient=currentTill->getNumberOfClientToBeServed();
        if(i == 0){
            min = numberOfClient;
            index_min = i;
        }else if(numberOfClient < min){
            min = numberOfClient;
            index_min = i;
        }
    }
    return index_min;
}

/**
 * Check if there is a costumer in queue and start serving it
 */
void Till_Checkout:: serveNextCustomerInQueue()
{
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
        EV<<getName()<<"Queue size decredes!"<<OrdersQueue.size()<<endl;
        //we save the time when the customer in queue ended is waiting
        storedCust->setEntersTillCheckout(simTime());
        if(storedCust->getServicesRequested()==1)
            emit(this->sWaitingTime,storedCust->getEntersTillCheckout() - storedCust->getEntersSystem());
        requestedCheckout(storedCust);
    }
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
double Till_Checkout::selectDistribution(int distribution, int numberOfClient){
        double timeC;
        if(distribution==0){
            timeC=exponential((1/checkoutRate)*numberOfClient,
                    IdxCheckRate); //take as input the avg
        }
        if(distribution==1){
            timeC=
                    uniform((1/checkoutRate)*numberOfClient - (1/checkoutRate)*numberOfClient*9/10 ,
                            (1/checkoutRate)*numberOfClient + (1/checkoutRate)*numberOfClient*9/10,
                            IdxCheckRate);
        }
        if(distribution==2){
                    timeC=1/checkoutRate;
                }
        return timeC;
}

