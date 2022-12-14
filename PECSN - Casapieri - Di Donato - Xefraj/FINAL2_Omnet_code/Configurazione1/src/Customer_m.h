//
// Generated file, do not edit! Created by nedtool 5.5 from Customer.msg.
//

#ifndef __CUSTOMER_M_H
#define __CUSTOMER_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0505
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



/**
 * Class generated from <tt>Customer.msg:19</tt> by nedtool.
 * <pre>
 * //
 * // TODO generated message class
 * //
 * packet Customer
 * {
 *     int servicesRequested; //1= Only check-out; 2=check-out + wrap
 *     int servicesDone; //1= Payment Done; 2=wrap Done
 *     simtime_t entersQueue;
 *     //the entersTill is not used 
 *     simtime_t entersTill;
 * 
 * }
 * </pre>
 */
class Customer : public ::omnetpp::cPacket
{
  protected:
    int servicesRequested;
    int servicesDone;
    ::omnetpp::simtime_t entersQueue;
    ::omnetpp::simtime_t entersTill;

  private:
    void copy(const Customer& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const Customer&);

  public:
    Customer(const char *name=nullptr, short kind=0);
    Customer(const Customer& other);
    virtual ~Customer();
    Customer& operator=(const Customer& other);
    virtual Customer *dup() const override {return new Customer(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual int getServicesRequested() const;
    virtual void setServicesRequested(int servicesRequested);
    virtual int getServicesDone() const;
    virtual void setServicesDone(int servicesDone);
    virtual ::omnetpp::simtime_t getEntersQueue() const;
    virtual void setEntersQueue(::omnetpp::simtime_t entersQueue);
    virtual ::omnetpp::simtime_t getEntersTill() const;
    virtual void setEntersTill(::omnetpp::simtime_t entersTill);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const Customer& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, Customer& obj) {obj.parsimUnpack(b);}


#endif // ifndef __CUSTOMER_M_H

