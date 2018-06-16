#include "tuples.hh"

Partition_T::Partition_T() : 
    _size(0), _pID(0), _pName(""), _pPath(""), _pType(0), _pGrowth(0)
{}


Partition_T::Partition_T(const uint8_t aPID, const std::string& aName, const std::string& aPath, const uint8_t aType, const uint16_t aGrowth) : 
    _size(0), _pID(aPID), _pName(aName), _pPath(aPath), _pType(aType), _pGrowth(aGrowth)
{
    _size = sizeof(_pID) + (_pName.size() + 1) + (_pPath.size() + 1) + sizeof(_pType) + sizeof(_pGrowth); //+1 for each string for \0
}

Partition_T::Partition_T(const Partition_T& aPartitionTuple) :
    _size(aPartitionTuple.size()),
    _pID(aPartitionTuple.ID()),
    _pName(aPartitionTuple.name()),
    _pPath(aPartitionTuple.path()),
    _pType(aPartitionTuple.type()),
    _pGrowth(aPartitionTuple.growth())
{}

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
}

std::ostream& operator<< (std::ostream& stream, const Partition_T& aPartTuple)
{
    stream << "Size: " << aPartTuple.size()
        << ", PID: " << aPartTuple.ID()
        << ", Name: " << aPartTuple.name()
        << ", Path: " << aPartTuple.path()
        << ", Type: " << aPartTuple.type()
        << ", Growth: " << aPartTuple.growth();
    return stream;
}


Segment_T::Segment_T() : 
    _size(0), _sPID(0), _sID(0), _sName(""), _sType(0), _sFirstPage(0)
{}


Segment_T::Segment_T(const uint8_t aPID, const uint16_t aSID, const std::string& aName, const uint8_t aType, const uint32_t aFirstPage) : 
    _size(0), _sPID(aPID), _sID(aSID), _sName(aName), _sType(aType), _sFirstPage(aFirstPage)
{
    _size = sizeof(_sPID) + sizeof(_sID) + (_sName.size() + 1) + sizeof(_sType) + sizeof(_sFirstPage); //+1 for each string for \0
   // std::cout << "Size of class: " << _size << std::endl;
}

Segment_T::Segment_T(const Segment_T& aSegmentTuple) :
    _size(aSegmentTuple.size()),
    _sPID(aSegmentTuple.partID()),
    _sID(aSegmentTuple.ID()),
    _sName(aSegmentTuple.name()),
    _sType(aSegmentTuple.type()),
    _sFirstPage(aSegmentTuple.firstPage())
{}

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
}

std::ostream& operator<< (std::ostream& stream, const Segment_T& aSegmentTuple)
{
    stream << "Size: " << aSegmentTuple.size()
        << ", PID: " << aSegmentTuple.partID()
        << ", SID: " << aSegmentTuple.ID()
        << ", Name: " << aSegmentTuple.name()
        << ", Type: " << aSegmentTuple.type()
        << ", First Page: " << aSegmentTuple.firstPage();
    return stream;
}

Employee_T::Employee_T() : 
    _size(0), _pAge(0), _pName(""), _pSalary(0)
{}


Employee_T::Employee_T(const uint8_t aAge, const std::string& aName, const uint16_t aSalary) : 
    _size(0), _pAge(aAge), _pName(aName), _pSalary(aSalary)
{
    _size = sizeof(_pAge) + (_pName.size() + 1) + sizeof(_pSalary) ; //+1 for each string for \0
}

Employee_T::Employee_T(const Employee_T& aEmployeeTuple) :
    _size(aEmployeeTuple.size()),
    _pAge(aEmployeeTuple.age()),
    _pName(aEmployeeTuple.name()),
    _pSalary(aEmployeeTuple.salary())
{}

Employee_T& Employee_T::operator=(const Employee_T& aEmployeeTuple)
{
    if(this != &aEmployeeTuple)
    {
        //exception safe copy assignment with swap would be overkill
        _size    = aEmployeeTuple.size();
        _pAge     = aEmployeeTuple.age();
        _pName   = aEmployeeTuple.name();
        _pSalary   = aEmployeeTuple.salary();
    }
    return *this;
}

void Employee_T::init(const uint8_t aAge, const std::string& aName, const uint16_t aSalary) noexcept
{
    _pAge = aAge;
    _pName = aName;
    _pSalary = aSalary;
    _size = sizeof(_pAge) + (_pName.size() + 1) + sizeof(_pSalary);
}

void Employee_T::toDisk(byte* aPtr) const noexcept
{
    *(uint16_t*)aPtr = _pSalary;
    aPtr += sizeof(_pSalary);

    *(uint8_t*)aPtr = _pAge;
    aPtr += sizeof(_pAge);

    for(size_t i = 0; i < _pName.size() + 1; ++i)
    {
        *(char*)aPtr = _pName.c_str()[i];
        ++aPtr;
    }
}

void Employee_T::toDisk(byte* aPtr) noexcept
{
    static_cast<const Employee_T&>(*this).toDisk(aPtr);
}


void Employee_T::toMemory(byte* aPtr) noexcept
{
    _pSalary = *(uint16_t*)aPtr;
    aPtr += sizeof(_pSalary);

    _pAge = *(uint8_t*)aPtr;
    aPtr += sizeof(_pAge);

    _pName = std::string((const char*)aPtr);
    aPtr += _pName.size() + 1;
    _size = sizeof(_pAge) + (_pName.size() + 1) + sizeof(_pSalary); //+1 for each string for \0
}

std::ostream& operator<< (std::ostream& stream, const Employee_T& aEmployeeTuple)
{
    stream << "Size: " << aEmployeeTuple.size()
        << ", Age: " << aEmployeeTuple.age()
        << ", Name: " << aEmployeeTuple.name()
        << ", Salary: " << aEmployeeTuple.salary();
    return stream;
}
