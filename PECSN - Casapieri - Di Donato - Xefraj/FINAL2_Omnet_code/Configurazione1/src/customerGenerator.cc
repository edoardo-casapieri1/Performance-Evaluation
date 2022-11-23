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

#include "customerGenerator.h"

#include "Customer_m.h"
#include "Till.h"

Define_Module(ClientGenerator);

void ClientGenerator::initialize()
{
    //Set this variable to 1 to change the distribution of arrival
    //we get the interarrival rate from the network
    cModule* SuperM=getModuleByPath("SuperMarketConfigurazione1");
    initialInterArrival= SuperM->par("initialInterArrivalRate");
    wrappingProb=SuperM->par("wrappingProbability");
    numberOfTills= SuperM->par("numbOfTills");
    distribution = SuperM->par("distribution");

    //RNGs Indexes
    rngIdxInterArrival =SuperM-> par("rngIdxInterArrival");
    rngIdxWrapProb = SuperM-> par("rngIdxWrapProb");
    rngIdxTillSelection = SuperM-> par("rngIdxTillSelection");

    EV<<getName()<<": ready to send new client"<<endl;
    EV<<getName()<<"Inter arrival rate "<< initialInterArrival<<endl;
    EV<<getName()<<"Wrapping probability "<< wrappingProb<<endl;
    EV<<getName()<<"IDXInterArrival "<< rngIdxInterArrival<<endl;

    beep_= new cMessage("beep_");
    double timeC= selectDistribution(distribution,1);
    scheduleAt(simTime() + timeC, beep_);
}

/**
 * The handleMessage:
 *  - Generates a Customer
 *  - Set randomly if the customer will ask for the wrapping service
 *  - Send the customer to a Till (The till is selected at random)
 *  - Schedule a message to generate a new costumer
 */
void ClientGenerator::handleMessage(cMessage *msg)
{
    //NOT USED: For discourage arrival scenario
    /*cModule* SuperM=getModuleByPath("SuperMarketConfigurazione1");
    int actualNumberOfClients= SuperM->par("numbOfClientsToBeServed");
    actualNumberOfClients--;
    SuperM->par("numbOfClientsToBeServed")= actualNumberOfClients;
    //if there is someone in the shop*/
    //Set this variable to 1 to change the distribution of arrival

    Customer* msgClient=new Customer("Customer");

    double wrapProb= uniform(0,1,rngIdxWrapProb);
    //when a costumer is generated he as a probability p to request the gift-wrap service
    if(wrapProb>(1-wrappingProb))
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
    //the customer is sent to the till


  // Send the customer to a till at random
    int tillToSendTo = intuniform(0, numberOfTills - 1, rngIdxTillSelection);
    send(msgClient, "o", tillToSendTo);


    //NOT USED: Discoraged arrival scenario
    //we get the number of customers in the till
    //and decrease the inter-arrival rate according to that
    //(explained in the documentation)
    //cModule* actualTill=getModuleByPath("^.tillCheckOutAndWrap");
    //Till* currentTill=check_and_cast<Till*>(actualTill);
    //int numberOfClient=currentTill->getNumberOfClientToBeServed();

    //EV<<numberOfClient <<endl;
    //numberOfClient++;
    double timeC=selectDistribution(distribution,1);

    scheduleAt(simTime() + timeC, beep_);

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
            timeC=exponential((1/initialInterArrival)*numberOfClient,
                    rngIdxInterArrival); //take as input the avg
        }
        if(distribution==1){
            timeC=
                    uniform((1/initialInterArrival)*numberOfClient - (1/initialInterArrival)*numberOfClient*9/10 ,
                            (1/initialInterArrival)*numberOfClient + (1/initialInterArrival)*numberOfClient*9/10,
                            rngIdxInterArrival);
        }
        if(distribution==2){
                    timeC=1/initialInterArrival;
                }

        return timeC;
}
