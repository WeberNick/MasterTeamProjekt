#include "partition_manager.hh"
#include "../segment/segment_manager.hh"
#include "../segment/segment_fsm_sp.hh"


PartitionManager::PartitionManager() :
    _counterPartitionID(1),
    _partitions(),
    _partitionsByID(),
    _partitionsByName(),
    _masterPartName("MasterPartition"),
    _masterSegPartName("PartitionMasterSegment"),
    _cb(nullptr)
{
    TRACE("'PartitionManager' constructed");
}

PartitionManager::~PartitionManager()
{
    // iterate over map and delete every map item (PartitionBase)
    for(const auto& it : _partitions) {
        delete it.second;
    }
    TRACE("'PartitionManager' destructed");
}

void PartitionManager::init(const CB& aControlBlock)
{
    if(!_cb)
    {
        _cb = &aControlBlock;
        TRACE("'PartitionManager' initialized");
    }
}

void PartitionManager::load(const part_vt& aTuples)
{
    uint8_t maxCounter = 0;
    //fill internal data structure with all relevant info
    for(const auto& partTuple : aTuples)
    {
      _partitionsByID[partTuple.ID()] = partTuple;
      _partitionsByName[partTuple.name()] = partTuple.ID();
      TRACE(partTuple.to_string() + std::string(" (DELETE TRACE AFTER DEBUGGING)"));
      if(partTuple.ID() > maxCounter){
          maxCounter = partTuple.ID();
      } 
    }
    _counterPartitionID = maxCounter + 1;
}

PartitionFile* PartitionManager::createPartitionFileInstance(const std::string& aPath, const std::string& aName, const uint16_t aGrowthIndicator)
{
    if(!PartitionBase::exists(aPath))
    {
        PartitionFile* lPartition = new PartitionFile(aPath, aName, aGrowthIndicator, _counterPartitionID++, *_cb);
        _partitions[lPartition->getID()] = lPartition;
        const uint8_t pType = 1; //file
        Partition_T lPartTuple(lPartition->getID(), lPartition->getName(), lPartition->getPath(), pType, lPartition->getGrowthIndicator());
        createPartitionSub(lPartTuple);
        TRACE("File partition instance created.");
        return static_cast<PartitionFile*>(_partitions.at(lPartition->getID()));
    }
    else
    {
        for(const auto& parts : _partitionsByID)
        {
            if(parts.second.path() == aPath)
            {
                return static_cast<PartitionFile*>(getPartition(parts.first));
            }
        }
    }
    //if this line is reached, something went wrong
    //examples are a given path to existing file which is not a partition
    throw PartitionExistsException(FLF);
}

PartitionRaw* PartitionManager::createPartitionRawInstance(const std::string& aPath, const std::string& aName)
{
    //currently reformats the raw partition with every call.. need to use the getPartition procedure as above
    PartitionRaw* lPartition = new PartitionRaw(aPath, aName, _counterPartitionID++, *_cb);
    _partitions[lPartition->getID()] = lPartition;
    const uint pType = 0;
    Partition_T lPartTuple(lPartition->getID(), lPartition->getName(), lPartition->getPath(), pType, MAX16); //MAX16 = invalid value to indicate 'no growth'
    createPartitionSub(lPartTuple);
    TRACE("Raw partition instance created.");
    return static_cast<PartitionRaw*>(_partitions.at(lPartition->getID()));
}

void PartitionManager::createPartitionSub(const Partition_T& aParT)
{
    _partitionsByID[aParT.ID()] = aParT;
    _partitionsByName[aParT.name()] = aParT.ID();
    SegmentFSM_SP* lSeg = static_cast<SegmentFSM_SP*>(SegmentManager::getInstance().getSegment(_masterSegPartName));
    lSeg->insertTuple<Partition_T>(aParT);
}

PartitionBase* PartitionManager::getPartition(const uint8_t aID)
{
    TRACE("Searching for the partition with ID '" + std::to_string(aID) + "'");
    //if the object has not been created before
    if (_partitions.find(aID) == _partitions.end()) {
        TRACE("Partition instance with ID '" + std::to_string(aID) + "' not found in-memory. Looking on disk...");
        const Partition_T& lTuple = _partitionsByID.at(aID);
        if(lTuple.ID() != aID)
        {
            TRACE("Requested partition ID (" + std::to_string(aID) + ") does not match retrieved partition tuple ID (" + std::to_string(lTuple.ID()) + ") from disk");
            throw PartitionNotExistsException(FLF);
        }
        PartitionBase* s;
        switch(lTuple.type()){
            case 1://PartitionFile
                s = new PartitionFile(lTuple, *_cb);
                break;
            case 2: //PartitionRaw
                s = new PartitionRaw(lTuple, *_cb);
                break;
            default: ASSERT_MSG("Invalid default-case of switch statement reached");
        }
        _partitions[lTuple.ID()] = s;
    }
    TRACE("Partition instance with ID '" + std::to_string(aID) + "' found");
    try
    {
        return _partitions.at(aID);
    }
    catch(const std::out_of_range& oor)
    {
        TRACE("## Error ## The requested partition with ID '" + std::to_string(aID) + "' does not exist in map (" + std::string(oor.what()) + ")");
        throw PartitionNotExistsException(FLF);
    }
}

PartitionBase* PartitionManager::getPartition(const std::string& aName)
{
    try
    {
        return getPartition(_partitionsByName.at(aName));
    }
    catch(const std::out_of_range& oor)
    {
        TRACE("## Error ## The requested partition with name '" + aName + "' does not exist in map (" + std::string(oor.what()) + ")");
        throw PartitionNotExistsException(FLF);
    }
}

const Partition_T& PartitionManager::getPartitionT(const uint8_t aID) const
{
    try
    {
        return _partitionsByID.at(aID);
    }
    catch(const std::out_of_range& oor)
    {
        TRACE("## Error ## The requested partition tuple with ID '" + std::to_string(aID) + "' does not exist in map (" + std::string(oor.what()) + ")");
        throw PartitionNotExistsException(FLF);
    }
}

Partition_T& PartitionManager::getPartitionT(const uint8_t aID) 
{
    return const_cast<Partition_T&>(static_cast<const PartitionManager&>(*this).getPartitionT(aID));
}

const Partition_T& PartitionManager::getPartitionT(const std::string& aName) const
{
    try
    {
        return getPartitionT(_partitionsByName.at(aName));
    }
    catch(const std::out_of_range& oor)
    {
        TRACE("## Error ## The requested partition tuple with name '" + aName + "' does not exist in map (" + std::string(oor.what()) + ")");
        throw PartitionNotExistsException(FLF);
    }
}

Partition_T& PartitionManager::getPartitionT(const std::string& aName)
{
    return const_cast<Partition_T&>(static_cast<const PartitionManager&>(*this).getPartitionT(aName));
}

void PartitionManager::deletePartition(const uint8_t aID)
{
    TRACE("Deletion of partition with ID " + std::to_string(aID) + " starts...");
    // delete all Segments on that partition
    SegmentManager& lSegMan = SegmentManager::getInstance();
    lSegMan.deleteSegements(aID);

    // delete partition
    PartitionBase* lPart = getPartition(aID);
    lPart->remove();
    //delete object
    delete lPart; //## evtl raus
    _partitions.erase(aID);
    const Partition_T lpart(_partitionsByID.at(aID));
    // delete tuple on disk
    lSegMan.deleteTuplePhysically<Partition_T>(_masterSegPartName, aID);

    // delete tuple in memory
    _partitionsByName.erase(lpart.name());
    _partitionsByID.erase(aID);
    TRACE("Partition with ID " + std::to_string(aID) + " deleted successfully.");
}

uint8_t PartitionManager::getPartitionID(const std::string& aName)
{
    try
    {
        return _partitionsByName.at(aName);
    }
    catch(const std::out_of_range& oor)
    {
        TRACE("## Error ## The requested partition with name '" + aName + "' does not exist in map (" + std::string(oor.what()) + ")");
        throw PartitionNotExistsException(FLF);
    }
}

string_vt PartitionManager::getPartitionNames() noexcept
{
    string_vt names;
    for (const auto& element : _partitionsByName)
        names.push_back(element.first);
    return names;
}

std::string PartitionManager::getPartitionName(const uint8_t aID)
{
    for(const auto& part : _partitionsByName)
    {
        if (aID == part.second)
            return part.first;
    }
    throw PartitionNotExistsException(FLF);
}

void PartitionManager::deletePartition(const std::string& aName)
{
    deletePartition(_partitionsByName.at(aName));
}

PartitionFile* PartitionManager::createMasterPartition(const Partition_T& aPart)
{
   return new PartitionFile(aPart, *_cb); 
}

PartitionFile* PartitionManager::createMasterPartition(const std::string& aPath, const uint aGrowthIndicator, Partition_T& aMasterTuple){
     uint pType = 1;
    PartitionFile* lPartition = new PartitionFile(aPath, _masterPartName, aGrowthIndicator, _counterPartitionID++, *_cb);
    _partitions[lPartition->getID()] = lPartition;
    TRACE(" ");
    Partition_T* t = new Partition_T(lPartition->getID(),_masterPartName,aPath,pType,aGrowthIndicator);
    aMasterTuple = *t;

    TRACE("Master partition created successfully.");
    return lPartition;
}

void PartitionManager::insertMasterPartitionTuple(const Partition_T& aMasterTuple){
    //insert Tuple in Segment
    createPartitionSub(aMasterTuple);
}
