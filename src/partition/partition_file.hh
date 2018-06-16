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
#include <fstream>      // std::ofstream
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;


class PartitionFile : public PartitionBase
{
    private:
        friend class PartitionManager;
        explicit PartitionFile(const std::string& aPath, const std::string& aName, const uint16_t aGrowthIndicator, const uint8_t aPartitionID, const CB& aControlBlock);
        explicit PartitionFile(const Partition_T& aTuple, const CB& aControlBlock);
        PartitionFile(const PartitionFile&) = delete;
        PartitionFile &operator=(const PartitionFile&) = delete;
    public:
        ~PartitionFile() = default;

    public:
        /**
         *  @brief  Wrapper for call to allocPage in PartitonBase (this handles specific behaviour)
         *  @return an index to the allocated page
         *  @see    partition_base.hh
         */
        virtual uint32_t allocPage();
        
        /**
        * @brief Retrieves the size of the file
        */
        virtual size_t partSize();
        virtual size_t partSizeInPages();
        inline uint16_t getGrowthIndicator() const noexcept { return _growthIndicator; }
        inline uint16_t getGrowthIndicator() noexcept { return _growthIndicator; }


    private:
        void create();
        void extend();
        void remove();
        void printPage(uint aPageIndex);

    private:
        /* An indicator how the partition will grow (indicator * block size) */
        uint16_t _growthIndicator;
};

