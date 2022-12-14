//
// Generated file, do not edit! Created by nedtool 5.5 from Customer.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include "Customer_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp


// forward
template<typename T, typename A>
std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec);

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// operator<< for std::vector<T>
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

Register_Class(Customer)

Customer::Customer(const char *name, short kind) : ::omnetpp::cPacket(name,kind)
{
    this->servicesRequested = 0;
    this->servicesDone = 0;
    this->entersSystem = 0;
    this->entersTillCheckout = 0;
    this->exitCheckout = 0;
    this->entersQueueWrap = 0;
    this->entersWrap = 0;
}

Customer::Customer(const Customer& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

Customer::~Customer()
{
}

Customer& Customer::operator=(const Customer& other)
{
    if (this==&other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void Customer::copy(const Customer& other)
{
    this->servicesRequested = other.servicesRequested;
    this->servicesDone = other.servicesDone;
    this->entersSystem = other.entersSystem;
    this->entersTillCheckout = other.entersTillCheckout;
    this->exitCheckout = other.exitCheckout;
    this->entersQueueWrap = other.entersQueueWrap;
    this->entersWrap = other.entersWrap;
}

void Customer::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->servicesRequested);
    doParsimPacking(b,this->servicesDone);
    doParsimPacking(b,this->entersSystem);
    doParsimPacking(b,this->entersTillCheckout);
    doParsimPacking(b,this->exitCheckout);
    doParsimPacking(b,this->entersQueueWrap);
    doParsimPacking(b,this->entersWrap);
}

void Customer::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->servicesRequested);
    doParsimUnpacking(b,this->servicesDone);
    doParsimUnpacking(b,this->entersSystem);
    doParsimUnpacking(b,this->entersTillCheckout);
    doParsimUnpacking(b,this->exitCheckout);
    doParsimUnpacking(b,this->entersQueueWrap);
    doParsimUnpacking(b,this->entersWrap);
}

int Customer::getServicesRequested() const
{
    return this->servicesRequested;
}

void Customer::setServicesRequested(int servicesRequested)
{
    this->servicesRequested = servicesRequested;
}

int Customer::getServicesDone() const
{
    return this->servicesDone;
}

void Customer::setServicesDone(int servicesDone)
{
    this->servicesDone = servicesDone;
}

::omnetpp::simtime_t Customer::getEntersSystem() const
{
    return this->entersSystem;
}

void Customer::setEntersSystem(::omnetpp::simtime_t entersSystem)
{
    this->entersSystem = entersSystem;
}

::omnetpp::simtime_t Customer::getEntersTillCheckout() const
{
    return this->entersTillCheckout;
}

void Customer::setEntersTillCheckout(::omnetpp::simtime_t entersTillCheckout)
{
    this->entersTillCheckout = entersTillCheckout;
}

::omnetpp::simtime_t Customer::getExitCheckout() const
{
    return this->exitCheckout;
}

void Customer::setExitCheckout(::omnetpp::simtime_t exitCheckout)
{
    this->exitCheckout = exitCheckout;
}

::omnetpp::simtime_t Customer::getEntersQueueWrap() const
{
    return this->entersQueueWrap;
}

void Customer::setEntersQueueWrap(::omnetpp::simtime_t entersQueueWrap)
{
    this->entersQueueWrap = entersQueueWrap;
}

::omnetpp::simtime_t Customer::getEntersWrap() const
{
    return this->entersWrap;
}

void Customer::setEntersWrap(::omnetpp::simtime_t entersWrap)
{
    this->entersWrap = entersWrap;
}

class CustomerDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    CustomerDescriptor();
    virtual ~CustomerDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(CustomerDescriptor)

CustomerDescriptor::CustomerDescriptor() : omnetpp::cClassDescriptor("Customer", "omnetpp::cPacket")
{
    propertynames = nullptr;
}

CustomerDescriptor::~CustomerDescriptor()
{
    delete[] propertynames;
}

bool CustomerDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<Customer *>(obj)!=nullptr;
}

const char **CustomerDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *CustomerDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int CustomerDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 7+basedesc->getFieldCount() : 7;
}

unsigned int CustomerDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<7) ? fieldTypeFlags[field] : 0;
}

const char *CustomerDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "servicesRequested",
        "servicesDone",
        "entersSystem",
        "entersTillCheckout",
        "exitCheckout",
        "entersQueueWrap",
        "entersWrap",
    };
    return (field>=0 && field<7) ? fieldNames[field] : nullptr;
}

int CustomerDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "servicesRequested")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "servicesDone")==0) return base+1;
    if (fieldName[0]=='e' && strcmp(fieldName, "entersSystem")==0) return base+2;
    if (fieldName[0]=='e' && strcmp(fieldName, "entersTillCheckout")==0) return base+3;
    if (fieldName[0]=='e' && strcmp(fieldName, "exitCheckout")==0) return base+4;
    if (fieldName[0]=='e' && strcmp(fieldName, "entersQueueWrap")==0) return base+5;
    if (fieldName[0]=='e' && strcmp(fieldName, "entersWrap")==0) return base+6;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *CustomerDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "simtime_t",
        "simtime_t",
        "simtime_t",
        "simtime_t",
        "simtime_t",
    };
    return (field>=0 && field<7) ? fieldTypeStrings[field] : nullptr;
}

const char **CustomerDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *CustomerDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int CustomerDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    Customer *pp = (Customer *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *CustomerDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    Customer *pp = (Customer *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string CustomerDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    Customer *pp = (Customer *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getServicesRequested());
        case 1: return long2string(pp->getServicesDone());
        case 2: return simtime2string(pp->getEntersSystem());
        case 3: return simtime2string(pp->getEntersTillCheckout());
        case 4: return simtime2string(pp->getExitCheckout());
        case 5: return simtime2string(pp->getEntersQueueWrap());
        case 6: return simtime2string(pp->getEntersWrap());
        default: return "";
    }
}

bool CustomerDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    Customer *pp = (Customer *)object; (void)pp;
    switch (field) {
        case 0: pp->setServicesRequested(string2long(value)); return true;
        case 1: pp->setServicesDone(string2long(value)); return true;
        case 2: pp->setEntersSystem(string2simtime(value)); return true;
        case 3: pp->setEntersTillCheckout(string2simtime(value)); return true;
        case 4: pp->setExitCheckout(string2simtime(value)); return true;
        case 5: pp->setEntersQueueWrap(string2simtime(value)); return true;
        case 6: pp->setEntersWrap(string2simtime(value)); return true;
        default: return false;
    }
}

const char *CustomerDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *CustomerDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    Customer *pp = (Customer *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}


