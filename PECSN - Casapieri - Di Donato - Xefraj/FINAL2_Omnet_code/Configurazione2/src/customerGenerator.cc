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

#include "Customer_m.h"
#include "Till_Checkout.h"
#include "Till_Wrap.h"
#include <string>
#include "customerGenerator.h"

Define_Module(ClientGenerator);

void ClientGenerator::initialize()
{
    cModule* SuperM=getModuleByPath("Try");
    this-> initialInterArrivalRate= SuperM->par("initialInterArrivalRate");
    this-> wrappingProb=SuperM->par("wrappingProbability");
    int distribution=SuperM->par("distribution");

    rngIdxInterArrival =SuperM-> par("rngIdxInterArrival");
    rngIdxWrapProb = SuperM-> par("rngIdxWrapProb");
    rngIdxTillSelectionCheck = SuperM-> par("rngIdxTillSelectionCheck");

    EV<<getName()<<": ready to send new client "<< initialInterArrivalRate<<" "<<wrappingProb<<endl;
    beep_= new cMessage("beep_");
    double timeC=selectDistribution(distribution,1);
    scheduleAt(simTime() + timeC, beep_);
}

/**
 * The handleMessage:
 *  - Generates a Customer
 *  - Set randomly if the customer will ask for the wrapping service
 *  - Send the customer to a checkout Till (The till is selected at random)
 *  - Schedule a message to generate a new costumer
 */
void ClientGenerator::handleMessage(cMessage *msg)
{
    cModule* SuperM=getModuleByPath("Try");
    int NumberTillsCheckout= SuperM->par("NCheck");
    int distribution=SuperM->par("distribution");
    Customer* msgClient=new Customer("Customer");
    double wrapProb= uniform(0,1, rngIdxWrapProb );
    //We generate a wrapping customer with a certain probability
    if(wrapProb>(1 - wrappingProb))
    {
        EV<<"Sicuramente impacchetto"<<endl;
        msgClient->setServicesRequested(2);
    }
    else
    {
        EV<<"IO NON IMPACCHETTO"<<endl;
        msgClient->setServicesRequested(1);
    }
    msgClient->setServicesDone(0);

    //choose to which check out till the costumer will be sent
      int tillToSendTo = intuniform(0, NumberTillsCheckout - 1, rngIdxTillSelectionCheck);
      send(msgClient, "o", tillToSendTo);

    //NOT USED CODE:
    // Was written For Discouraged arrival purposes
      /*cModule* actualTill=getModuleByPath("^.tillC");
      cModule* actualTill=getModuleByPath("SuperMarketConfigurazione1.openTills[0].tillA");
      cModule* actualTill=getModuleByPath("^.tillA");
      Till_Checkout* currentTill=check_and_cast<Till_Checkout*>(actualTill);
      int numberOfClient=currentTill->getNumberOfClientToBeServed();
      numberOfClient++;
      EV<<numberOfClient<<endl;

      int numberOfCustomerWrap;
      numberOfCustomerWrap=getWaitingWrapCustomers();
      numberOfClient+=numberOfCustomerWrap;
      EV<<numberOfClient<<endl;
      EV<<"Num customer  "<<numberOfClient<<endl;
    */
    double timeC=selectDistribution(distribution,1);

    scheduleAt(simTime() + timeC, beep_);
}

/** NOT USED: getWaitingWrapCustomer
 * Get the number of customer in the waiting till
 * for discouraged arrivals
 */
int ClientGenerator:: getWaitingWrapCustomers()
{
    int numberOfCustomers=0;
    std::string Path("Try.TillWrap");
    cModule* SuperM=getModuleByPath("Try");
    int NumberWrap= SuperM->par("NWrap");
    int NumberCheckout= SuperM->par("NCheck");
    EV<<"Ci sono Casse di wrap: "<<NumberWrap<<endl;
    EV<<"Ci sono Casse di check out: "<<NumberCheckout<<endl;
    for(int i = 0; i < NumberWrap; i++){
        std::string CompletePath;
        CompletePath = Path + "[" + std::to_string(i) + "]";
        cModule* actualTill=getModuleByPath(CompletePath.c_str());
        Till_Wrap* currentTill=check_and_cast<Till_Wrap*>(actualTill);
        numberOfCustomers+=currentTill->getNumberOfClientToBeServed();
    }
    EV<<"NUMERO CLIENTI IN WRAP"<<numberOfCustomers<<endl;
    numberOfCustomers=ceil( numberOfCustomers/NumberCheckout);

    EV<<"NUMERO MEDIO DI CLIENTI IN WRAP"<<numberOfCustomers<<endl;
    return numberOfCustomers;
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
double ClientGenerator::selectDistribution(int distribution, int numberOfClient){
        double timeC;
        if(distribution==0){
            timeC=exponential((1/initialInterArrivalRate)*numberOfClient,
                    rngIdxInterArrival); //take as input the avg
        }
        if(distribution==1){
            timeC=
                    uniform((1/initialInterArrivalRate)*numberOfClient - (1/initialInterArrivalRate)*numberOfClient*9/10 ,
                            (1/initialInterArrivalRate)*numberOfClient + (1/initialInterArrivalRate)*numberOfClient*9/10,
                            rngIdxInterArrival);
        }
        //for validation purposes
        if(distribution==2){
            timeC=1/initialInterArrivalRate;
        }
        return timeC;
}
