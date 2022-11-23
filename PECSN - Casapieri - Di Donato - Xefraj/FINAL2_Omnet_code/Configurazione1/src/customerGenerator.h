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

#ifndef __CONFIGURAZIONE1_CLIENTGENERATOR_H_
#define __CONFIGURAZIONE1_CLIENTGENERATOR_H_

#include <omnetpp.h>
#include "Customer_m.h"

using namespace omnetpp;

/**
 * TODO - Generated class
 */
class ClientGenerator : public cSimpleModule
{
 private:
   cMessage* beep_;
   Customer* Customer_;
   double initialInterArrival;
   double wrappingProb;
   int numberOfTills;
   int distribution;
   int rngIdxInterArrival;
   int rngIdxWrapProb;
   int rngIdxTillSelection;

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual double selectDistribution(int distribution, int numberOfClient);
};

#endif
