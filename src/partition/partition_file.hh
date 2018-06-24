/**
 *  @file    partition_file.hh
 *  @author  Nick Weber (nickwebe@pi3.informatik.uni-mannheim.de)
 *  @brief   A class implementing the interface of a partition stored on disk (currently a file)
 *  @bugs    Currently no bugs known
 *  @todos   -Update LSN?
 *           -implement extend of file
 *  @section TBD
 */
#pragma once

#include "partition_base.hh"
#include "../infra/tuples.hh"
#include "../interpreter/interpreter_fsip.hh"

#include <string>
#include <cmath>
#include <cstdlib>

class PartitionFile : public PartitionBase
{
    private:
        friend class PartitionManager;
        PartitionFile(const std::string& aPath, const std::string& aName, const uint16_t aGrowthIndicator, const uint8_t aPartitionID, const CB& aControlBlock);
        PartitionFile(const Partition_T& aTuple, const CB& aControlBlock);
        PartitionFile(const PartitionFile&) = delete;
        PartitionFile &operator=(const PartitionFile&) = delete;
    public:
        ~PartitionFile();

    public:
        /**
         *  @brief  Wrapper for call to allocPage in PartitonBase (this handles specific behaviour)
         *  @return an index to the allocated page
         *  @see    partition_base.hh
         */
        virtual uint32_t allocPage();

        void printPage(uint aPageIndex);

        
        /**
        * @brief Retrieves the size of the file
        */
        virtual size_t partSize() noexcept;
        virtual size_t partSizeInPages() noexcept;
        inline uint16_t getGrowthIndicator() const noexcept { return _growthIndicator; }
        inline uint16_t getGrowthIndicator() noexcept { return _growthIndicator; }
        inline std::string to_string() const noexcept { return PartitionBase::to_string() + std::string(", Growth : ") + std::to_string(getGrowthIndicator()); }
        inline std::string to_string() noexcept { return static_cast<const PartitionFile&>(*this).to_string(); }


    private:
        void create();
        void remove();

    private:
        /* An indicator how the partition will grow (indicator * block size) */
        uint16_t _growthIndicator;
};

std::ostream& operator<< (std::ostream& stream, const PartitionFile& aPartition);
