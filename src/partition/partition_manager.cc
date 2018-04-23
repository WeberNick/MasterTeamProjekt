#include "partition_manager.hh"
#include "../segment/segment_manager.hh"
#include "../segment/segment_fsm_sp.hh"


PartitionManager::PartitionManager() :
    _counterPartitionID(1),
    _partitions(),
    _partitionsByID(),
    _partitionsByName(),
    _partitionTuples(),
    _masterPartName("MasterPartition"),
    _masterSegPartName("PartitionMasterSegment"),
    _cb(nullptr),
    _init(false)
{}

PartitionManager::~PartitionManager()
{
    // iterate over map and delete every map item (PartitionBase)
    for(auto it = _partitions.begin(); it != _partitions.end(); ++it) {
        delete it->second;
    }
}

void PartitionManager::init(const CB& aControlBlock)
{
    if(!_init)
    {
        _cb = &aControlBlock;
        _init = true;
    }
}

void PartitionManager::load(part_vt& aTuples)
{
    _partitionTuples = aTuples;
    //fill internal data structure with all relevant info
    for(auto& partTuple : _partitionTuples)
    {
      _partitionsByID[partTuple._pID] = &partTuple;
      _partitionsByName[partTuple._pName] = &partTuple;
    }
}


PartitionFile* PartitionManager::createPartitionFileInstance(const std::string aPath, const std::string aName, const uint aGrowthIndicator)
{
    int pType = 1;
    PartitionFile* lPartition = new PartitionFile(aPath, aName, aGrowthIndicator, _counterPartitionID++, *_cb);
    _partitions[lPartition->getID()] = lPartition;

    part_t lp = {lPartition->getID(),aPath,aName,pType,aGrowthIndicator};
    createPartitionSub(lp);

    TRACE("File partition instance created.");
    return (PartitionFile*)_partitions.at(lPartition->getID());
}

PartitionRaw* PartitionManager::createPartitionRawInstance(const std::string aPath, const std::string aName)
{
    PartitionRaw* lPartition = new PartitionRaw(aPath, aName, _counterPartitionID++, *_cb);
    _partitions[lPartition->getID()] = lPartition;

    int pType = 0;

    part_t lp = {lPartition->getID(),aPath,aName,pType,0};
    createPartitionSub(lp);

    TRACE("Raw partition instance created.");
    return (PartitionRaw*)_partitions.at(lPartition->getID());
}

void PartitionManager::createPartitionSub(const part_t& aParT){
    SegmentManager& lSegMan = SegmentManager::getInstance();
    _partitionTuples.push_back(aParT);
    _partitionsByID[aParT._pID] = &_partitionTuples[_partitionTuples.size()-1];
    _partitionsByName[aParT._pName] = &_partitionTuples[_partitionTuples.size()-1];

    SegmentFSM_SP* lSeg = (SegmentFSM_SP*) lSegMan.getSegment(_masterSegPartName);
    lSeg->insertTuple((byte*) &aParT,sizeof(part_t));
}

PartitionBase* PartitionManager::getPartition(const uint8_t aID)
{
    //if the object has not been created before
    if (_partitions.find(aID)==_partitions.end()) {
         part_t lTuple = *_partitionsByID[aID];
        PartitionBase* s;
        switch(lTuple._pType){
            case 1://PartitionFile
            s = new PartitionFile(lTuple, *_cb);
            break;
            case 2: //PartitionRaw
            s = new PartitionRaw(lTuple, *_cb);
            break;

            default: return nullptr;
        }
        _partitions[lTuple._pID]=s;
    }
    return _partitions.at(aID);
}

PartitionBase* PartitionManager::getPartition(const std::string& aName){
    return getPartition(_partitionsByName[aName]->_pID);
}

void PartitionManager::deletePartition(const uint8_t aID){
    //delete object if exists
    auto lIter =  _partitions.find(aID);
    if( lIter !=_partitions.end()){
        delete lIter->second;
        _partitions.erase(lIter);
    }
    part_t* lpart = _partitionsByID[aID];
    //delete tuple on disk
    SegmentManager& lSegMan = SegmentManager::getInstance();
    lSegMan.deleteTupelPhysically(_masterSegPartName,aID,1);

    //delete tuple in memory
    _partitionsByName.erase(lpart->_pName);
    _partitionsByID.erase(aID);

    for (auto lIter2 = _partitionTuples.begin(); lIter2 <_partitionTuples.end(); ++lIter2){
        //as ID is unique, there is exactly one tuple fulfilling this condition. Makes check for identity easier.
        if (   lpart->_pID == lIter2->_pID /*&& 
        lpart->_pGrowth==segIter2->_pGrowth && 
        lpart->_pType == lIter2->_pType &&
        (lpart->_pName.compare(lIter2->_pName)==0) &&
        (lpart->_pPath.compare(lIter2->_pPath)==0)*/
        )
        {
            _partitionTuples.erase(lIter2);
            break;
        }
    }
    TRACE("Partition deleted successfully.");
}
void PartitionManager::deletePartition(const std::string& aName){
    deletePartition(_partitionsByName[aName]->_pID);
}

PartitionFile* PartitionManager::createMasterPartition(const part_t& aPart)
{
   return new PartitionFile(aPart, *_cb); 
}

PartitionFile* PartitionManager::createMasterPartition(const std::string& aPath, uint aGrowthIndicator, part_t& aMasterTuple){
     int pType = 1;
    PartitionFile* lPartition = new PartitionFile(aPath, _cb->mstrPart(), aGrowthIndicator, _counterPartitionID++, *_cb);
    _partitions[lPartition->getID()] = lPartition;

    part_t lp = {lPartition->getID(),_cb->mstrPart(),aPath,pType,aGrowthIndicator};
    aMasterTuple = lp;

    TRACE("Master partition created successfully.");
    return lPartition;
}

void PartitionManager::insertMasterPartitionTuple(const part_t& aMasterTuple){
    //insert Tuple in Segment
    createPartitionSub(aMasterTuple);
}
