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

package configurazione1;

//
// TODO auto-generated module
//
simple Till
{
   	parameters:
        
        //Saves the waiting time
        @signal[waitingTimeCheckOutAndWrap](type="long");
        @statistic[signalWaitingTimeCheckOutAndWrap](source="waitingTimeCheckOutAndWrap";record=vector;);
        
        @signal[waitingTimeCheckOut](type="long");
        @statistic[signalWaitingTimeCheckOut](source="waitingTimeCheckOut";record=vector;);
        
        //Saves the number of Customers
        @signal[numbOfCust](type="int");
        @statistic[signalNumbOfCust](source="numbOfCust";record=vector;);
        
        //Salves Response Time 
        @signal[responseTimeCheckOutAndWrap](type="long");
        @statistic[signalResponseTimeCheckOutAndWrap](source="responseTimeCheckOutAndWrap";record=vector;);
        
        @signal[responseTimeCheckOut](type="long");
        @statistic[signalResponseTimeCheckOut](source="responseTimeCheckOut";record=vector;);
        //Salva tempo di impacchettamento
       // @signal[customerEndsWrap](type="long");
       // @statistic[signalCustomerEndsWrap](source="customerEndsWrap";record=vector;);
    
    gates:
        input in;
}
