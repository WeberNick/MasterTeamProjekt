#include "partition_manager.hh"

PartitionManager::PartitionManager() :
    _counterPartitionID(0),
    _partitions()
{}

PartitionManager::~PartitionManager()
{
    // iterate over map and delete every map item (PartitionBase)
    std::map<uint, PartitionBase*>::iterator it;
    for(it = _partitions.begin(); it != _partitions.end(); ++it) {
        delete it->second;
    }
}

PartitionFile* PartitionManager::createPartitionFileInstance(const std::string aPath, const std::string aName, const uint64_t aPartitionSize, const uint aPageSize, const uint aSegmentIndexPage, const uint aGrowthIndicator)
{
    PartitionFile* lPartition = new PartitionFile(aPath, aName, aPartitionSize, aPageSize, aSegmentIndexPage, aGrowthIndicator, _counterPartitionID++);
    _partitions[lPartition->getID()] = lPartition;
    return (PartitionFile*)_partitions.at(lPartition->getID());
}

PartitionRaw* PartitionManager::createPartitionRawInstance(const std::string aPath, const std::string aName, const uint64_t aPartitionSize, const uint aPageSize, const uint aSegmentIndexPage)
{
    PartitionRaw* lPartition = new PartitionRaw(aPath, aName, aPartitionSize, aPageSize, aSegmentIndexPage, _counterPartitionID++);
    _partitions[lPartition->getID()] = lPartition;
    return (PartitionRaw*)_partitions.at(lPartition->getID());
}

uint PartitionManager::getNoPartitions()
{
    return _partitions.size();
}

PartitionBase* PartitionManager::getPartition(const uint8_t aID)
{
    return _partitions.at(aID);
}