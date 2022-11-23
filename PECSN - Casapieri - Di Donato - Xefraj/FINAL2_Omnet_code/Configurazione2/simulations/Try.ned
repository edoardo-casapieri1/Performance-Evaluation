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

package configurazione2.simulations;

import configurazione2.ClientGenerator;
import configurazione2.Till_Checkout;
import configurazione2.Till_Wrap;
import configurazione2.ClientGenerator;

//
// TODO auto-generated type
//
network Try
{
    parameters:
        int N;
        int NWrap;
        int NCheck;
        int distribution;
        double initialInterArrivalRate;
        double checkoutRate;
        double wrappingRate;
        double wrappingProbability;
        
        int rngIdxWrapProb;
        int rngIdxInterArrival;
        int rngIdxTillSelectionCheck;
        int rngIdxCheckRate;
        int rngIdxWrapRate;
        int rngIdxTillSelectionWrap;
        @display("bgb=492.25998,560.72");
    submodules:
        custGenerator: ClientGenerator {
            @display("i=abstract/people2;p=101.520004,320.35202");
        }
        TillCheckOut[NCheck]: Till_Checkout {
            @display("p=218.832,196.272,c,120;i=block/queue;is=l");
        }
        TillWrap[NWrap]: Till_Wrap {
            @display("p=363.216,196.272,c,120;i=block/queue;is=l");
        }
    connections:
        for i=0..(NCheck-1) {
            custGenerator.o++ --> TillCheckOut[i].in;
        }

        for i=0..(NWrap-1), for j=0..(NCheck-1) {
            TillCheckOut[j].toWrap++ --> TillWrap[i].in++;
        }
}
