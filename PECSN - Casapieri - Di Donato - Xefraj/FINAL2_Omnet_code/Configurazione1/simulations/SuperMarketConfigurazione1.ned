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

package configurazione1.simulations;

import configurazione1.ClientGenerator;
import configurazione1.Till;

network SuperMarketConfigurazione1
{
    parameters:
        int numbOfTills;
        int distribution;
        double initialInterArrivalRate;
        double checkoutRate;
        double wrappingRate;
        double wrappingProbability;
        
        int rngIdxWrapProb;
        int rngIdxInterArrival;
        int rngIdxTillSelection;
        int rngIdxCheckRate;
        int rngIdxWrapRate;
        @display("bgb=866,702");
    submodules:
        custGenerator: ClientGenerator {
            @display("i=abstract/people2;p=145,148");
        }
        openTills[numbOfTills]: Till {
            @display("p=244,43,c,35");
        }
    connections:
        for i=0..(numbOfTills-1) {
            custGenerator.o++ --> openTills[i].in;
        }
}
