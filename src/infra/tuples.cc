#include "tuples.hh"

Partition_T::Partition_T() : 
    _size(0), _pID(0), _pName(""), _pPath(""), _pType(0), _pGrowth(0)
{
    TRACE("'Partition_T' object was default-constructed: " + to_string());
}


Partition_T::Partition_T(const uint8_t aPID, const std::string& aName, const std::string& aPath, const uint8_t aType, const uint16_t aGrowth) : 
    _size(0), _pID(aPID), _pName(aName), _pPath(aPath), _pType(aType), _pGrowth(aGrowth)
{
    _size = sizeof(_pID) + (_pName.size() + 1) + (_pPath.size() + 1) + sizeof(_pType) + sizeof(_pGrowth); //+1 for each string for \0
    TRACE("'Partition_T' object was constructed: " + to_string());
}

Partition_T::Partition_T(const Partition_T& aPartitionTuple) :
    _size(aPartitionTuple.size()),
    _pID(aPartitionTuple.ID()),
    _pName(aPartitionTuple.name()),
    _pPath(aPartitionTuple.path()),
    _pType(aPartitionTuple.type()),
    _pGrowth(aPartitionTuple.growth())
{
    TRACE("'Partition_T' object was copy-constructed: " + to_string());
}

Partition_T& Partition_T::operator=(const Partition_T& aPartitionTuple)
{
    if(this != &aPartitionTuple)
    {
        //exception safe copy assignment with swap would be overkill
        _size    = aPartitionTuple.size();
        _pID     = aPartitionTuple.ID();
        _pName   = aPartitionTuple.name();
        _pPath   = aPartitionTuple.path();
        _pType   = aPartitionTuple.type();
        _pGrowth = aPartitionTuple.growth();
    }
    return *this;
}

void Partition_T::init(const uint8_t aPID, const std::string& aName, const std::string& aPath, const uint8_t aType, const uint16_t aGrowth) noexcept
{
    _pID = aPID;
    _pName = aName;
    _pPath = aPath;
    _pType = aType;
    _pGrowth = aGrowth;
    _size = sizeof(_pID) + (_pName.size() + 1) + (_pPath.size() + 1) + sizeof(_pType) + sizeof(_pGrowth); //+1 for each string for \0
    TRACE("'Partition_T' object was initialized: " + to_string());
}

void Partition_T::toDisk(byte* aPtr) const noexcept
{
    *(uint8_t*)aPtr = _pID;
    aPtr += sizeof(_pID);
    for(size_t i = 0; i < _pName.size() + 1; ++i)
    {
        *(char*)aPtr = _pName.c_str()[i];
        ++aPtr;
    }

    for(size_t i = 0; i < _pPath.size() + 1; ++i)
    {
        *(char*)aPtr = _pPath.c_str()[i];
        ++aPtr;
    }
    *(uint8_t*)aPtr = _pType;
    aPtr += sizeof(_pType);
    *(uint16_t*)aPtr = _pGrowth;
    aPtr += sizeof(_pGrowth);
    TRACE("Transformed 'Partition_T' object (" + to_string() + ") to its disk representation");
}

void Partition_T::toDisk(byte* aPtr) noexcept
{
    static_cast<const Partition_T&>(*this).toDisk(aPtr);
}


void Partition_T::toMemory(byte* aPtr) noexcept
{
    _pID = *(uint8_t*)aPtr;
    aPtr += sizeof(_pID);
    _pName = std::string((const char*)aPtr);
    aPtr += _pName.size() + 1;
    _pPath = std::string((const char*)aPtr);
    aPtr += _pPath.size() + 1;
    _pType = *(uint8_t*)aPtr;
    aPtr += sizeof(_pType);
    _pGrowth = *(uint16_t*)aPtr;
    aPtr += sizeof(_pGrowth);
    _size = sizeof(_pID) + (_pName.size() + 1) + (_pPath.size() + 1) + sizeof(_pType) + sizeof(_pGrowth); //+1 for each string for \0
    TRACE("Transformed 'Partition_T' object (" + to_string() + ") to its memory representation");
}

std::ostream& operator<< (std::ostream& stream, const Partition_T& aPartTuple)
{
    stream << aPartTuple.to_string();
    return stream;
}


Segment_T::Segment_T() : 
    _size(0), _sPID(0), _sID(0), _sName(""), _sType(0), _sFirstPage(0)
{
    TRACE("'Segment_T' object was default-constructed: " + to_string());
}


Segment_T::Segment_T(const uint8_t aPID, const uint16_t aSID, const std::string& aName, const uint8_t aType, const uint32_t aFirstPage) : 
    _size(0), _sPID(aPID), _sID(aSID), _sName(aName), _sType(aType), _sFirstPage(aFirstPage)
{
    _size = sizeof(_sPID) + sizeof(_sID) + (_sName.size() + 1) + sizeof(_sType) + sizeof(_sFirstPage); //+1 for each string for \0
    TRACE("'Segment_T' object was constructed: " + to_string());
}

Segment_T::Segment_T(const Segment_T& aSegmentTuple) :
    _size(aSegmentTuple.size()),
    _sPID(aSegmentTuple.partID()),
    _sID(aSegmentTuple.ID()),
    _sName(aSegmentTuple.name()),
    _sType(aSegmentTuple.type()),
    _sFirstPage(aSegmentTuple.firstPage())
{
    TRACE("'Segment_T' object was copy-constructed: " + to_string());
}

Segment_T& Segment_T::operator=(const Segment_T& aSegmentTuple)
{
    if(this != &aSegmentTuple)
    {
        //exception safe copy assignment with swap would be overkill
        _size       = aSegmentTuple.size();
        _sPID       = aSegmentTuple.partID();
        _sID        = aSegmentTuple.ID();
        _sName      = aSegmentTuple.name();
        _sType      = aSegmentTuple.type();
        _sFirstPage = aSegmentTuple.firstPage();
    }
    return *this;
}

void Segment_T::init(const uint8_t aPID, const uint16_t aSID, const std::string& aName, const uint8_t aType, const uint32_t aFirstPage) noexcept
{
    _sPID = aPID;
    _sID = aSID;
    _sName = aName;
    _sType = aType;
    _sFirstPage = aFirstPage;
    _size = sizeof(_sPID) + sizeof(_sID) + (_sName.size() + 1) + sizeof(_sType) + sizeof(_sFirstPage); //+1 for each string for \0
    TRACE("'Segment_T' object was initialized: " + to_string());
}

void Segment_T::toDisk(byte* aPtr) const noexcept
{
    *(uint8_t*)aPtr = _sPID;
    aPtr += sizeof(_sPID);
    *(uint16_t*)aPtr = _sID;
    aPtr += sizeof(_sID);
    for(size_t i = 0; i < _sName.size() + 1; ++i)
    {
        *(char*)aPtr = _sName.c_str()[i];
        ++aPtr;
    }

    *(uint8_t*)aPtr = _sType;
    aPtr += sizeof(_sType);
    *(uint32_t*)aPtr = _sFirstPage;
    aPtr += sizeof(_sFirstPage);
    TRACE("Transformed 'Segment_T' object (" + to_string() + ") to its disk representation");
}

void Segment_T::toDisk(byte* aPtr) noexcept
{
    static_cast<const Segment_T&>(*this).toDisk(aPtr);
}

void Segment_T::toMemory(byte* aPtr) noexcept
{
    _sPID = *(uint8_t*)aPtr;
    aPtr += sizeof(_sPID);
    _sID = *(uint16_t*)aPtr;
    aPtr += sizeof(_sID);
    _sName = std::string((const char*)aPtr);
    aPtr += _sName.size() + 1;
    _sType = *(uint8_t*)aPtr;
    aPtr += sizeof(_sType);
    _sFirstPage = *(uint32_t*)aPtr;
    _size = sizeof(_sPID) + sizeof(_sID) + (_sName.size() + 1) + sizeof(_sType) + sizeof(_sFirstPage); //+1 for each string for \0
    TRACE("Transformed 'Segment_T' object (" + to_string() + ") to its memory representation");
}

std::ostream& operator<< (std::ostream& stream, const Segment_T& aSegmentTuple)
{
    stream << aSegmentTuple.to_string();
    return stream;
}

uint64_t Employee_T::_idCount = 1;

Employee_T::Employee_T() : 
    _size(0), _id(0), _salary(0.0), _age(0), _name("")
{
    TRACE("'Employee_T' object was default constructed: " + to_string());
}


Employee_T::Employee_T(const std::string& aName, const double aSalary, const uint8_t aAge) : 
    _size(0), _id(_idCount++), _salary(aSalary), _age(aAge), _name(aName)
{
    _size = sizeof(_id) + sizeof(_salary) + sizeof(_age) + (_name.size() + 1) ; //+1 for each string for \0
    TRACE("'Employee_T' object was constructed: " + to_string());
}

Employee_T::Employee_T(const Employee_T& aEmployeeTuple) :
    _size(aEmployeeTuple.size()),
    _id(aEmployeeTuple.ID()),
    _salary(aEmployeeTuple.salary()),
    _age(aEmployeeTuple.age()),
    _name(aEmployeeTuple.name())
{
    TRACE("'Employee_T' object was copy-constructed: " + to_string());
}

Employee_T& Employee_T::operator=(const Employee_T& aEmployeeTuple)
{
    if(this != &aEmployeeTuple)
    {
        //exception safe copy assignment with swap would be overkill
        _size    = aEmployeeTuple.size();
        _id      = aEmployeeTuple.ID();
        _salary  = aEmployeeTuple.salary();
        _age     = aEmployeeTuple.age();
        _name    = aEmployeeTuple.name();
    }
    return *this;
}

void Employee_T::init(const std::string& aName, const double aSalary, const uint8_t aAge) noexcept
{
    _id = _idCount++;
    _salary = aSalary;
    _age = aAge;
    _name = aName;
    _size = sizeof(_id) + sizeof(_salary) + sizeof(_age) + (_name.size() + 1) ; //+1 for each string for \0
    TRACE("'Employee_T' object was initialized: " + to_string());
}

void Employee_T::toDisk(byte* aPtr) const noexcept
{
    *(uint64_t*)aPtr = _id;
    aPtr += sizeof(_id);

    *(double*)aPtr = _salary;
    aPtr += sizeof(_salary);

    *(uint8_t*)aPtr = _age;
    aPtr += sizeof(_age);

    for(size_t i = 0; i < _name.size() + 1; ++i)
    {
        *(char*)aPtr = _name.c_str()[i];
        ++aPtr;
    }
    TRACE("Transformed 'Employee_T' object (" + to_string() + ") to its disk representation");
}

void Employee_T::toDisk(byte* aPtr) noexcept
{
    static_cast<const Employee_T&>(*this).toDisk(aPtr);
}


void Employee_T::toMemory(byte* aPtr) noexcept
{
    _id = *(uint64_t*)aPtr;
    aPtr += sizeof(_id);

    _salary = *(double*)aPtr;
    aPtr += sizeof(_salary);

    _age = *(uint8_t*)aPtr;
    aPtr += sizeof(_age);

    _name = std::string((const char*)aPtr);
    aPtr += _name.size() + 1;
    _size = sizeof(_id) + sizeof(_salary) + sizeof(_age) + (_name.size() + 1) ; //+1 for each string for \0
    TRACE("Transformed 'Employee_T' object (" + to_string() + ") to its memory representation");
}

std::ostream& operator<< (std::ostream& stream, const Employee_T& aEmployeeTuple)
{
    stream << aEmployeeTuple.to_string();
    return stream;
}
