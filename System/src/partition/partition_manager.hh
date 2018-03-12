/**
 *  @file   partition_manager.hh
 *  @author Nicolas Wipfler (nwipfler@mail.uni-mannheim.de),
 *          Nick Weber (nickwebe@pi3.informatik.uni-mannheim.de)
 *  @brief  Singleton class managing all partitions
 *  @bugs   Currently no bugs known
 *  @todos  Currently no todos
 *
 *  @section DESCRIPTION
 *  This class manages all partition objects associated with a physical partition (raw or file) on disk.
 *  If a new partition must be created, this is done through the single instance of this class by calling
 *  createPartition[Type]Instance(...). Because the partitions constructors are private, this is the only
 *  way to create a new partition.
 */

#ifndef PARTITION_MANAGER_HH
#define PARTITION_MANAGER_HH

#include "infra/types.hh"
#include "partition_base.hh"
#include "partition_file.hh"
#include "partition_raw.hh"

#include <map>
#include <string>

class PartitionManager
{    
    private:
        explicit PartitionManager();
        PartitionManager(const PartitionManager&) = delete;
        PartitionManager& operator=(const PartitionManager&) = delete;
        ~PartitionManager(); // delete all partitions

    public:
        /**
         *  @brief  This function is the only way to get access to the PartitionManager instance
         *
         *  @return reference to the only PartitionManager instance
         */
        static PartitionManager& getInstance()
        {
            static PartitionManager lPartitionManagerInstance;
            return lPartitionManagerInstance;
        }

    public:
        void load(part_vt& aTuples);

    public:
        /* creates instance of partition; creation of partition on disk happens in the respective partition class */
        PartitionFile* createPartitionFileInstance(const std::string aPath, const std::string aName, const uint aGrowthIndicator, const control_block_t& aControlBlock);
        PartitionRaw* createPartitionRawInstance(const std::string aPath, const std::string aName, const control_block_t& aControlBlock);
  
    public:
        inline size_t           getNoPartitions(){ return _partitions.size(); }
        inline PartitionBase*   getPartition(const uint8_t aID){ return _partitions.at(aID); }
		inline const part_vt&   getPartitionTuples(){ return _partitionTuples; }
  
    private:
        uint _counterPartitionID;
        std::map<uint, PartitionBase*> _partitions;
        std::map<uint16_t, part_t*> _partitionsByID;
		std::map<std::string, part_t*> _partitionsByName;
        part_vt _partitionTuples;
};

#endif
