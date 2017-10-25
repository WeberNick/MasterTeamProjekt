/**
 *  @file	segment.hh
 *  @author	Nick Weber (nickwebe@pi3.informatik.uni-mannheim.de),
            Nicolas Wipfler (nwipfler@mail.uni-mannheim.de)
 *  @brief	This class manages multiple pages
 *  @bugs	Currently no bugs known
 *  @todos  in getNewPage: implement init page, e.g. NSM/PAX..
 *  @section TBD
 */

#ifndef SEGMENT_HH
#define SEGMENT_HH

#include "infra/header_structs.hh"
#include "infra/types.hh"
#include "partition/partition_file.hh"
#include "segment_base.hh"
#include <vector>

class Segment : public SegmentBase {
  private:
    friend class SegmentManager;
    explicit Segment(const uint16_t aSegID, PartitionBase &aPartition);
    explicit Segment(PartitionBase &aPartition);
    Segment(const Segment &aSegment) = delete;
    Segment &operator=(const Segment &aSegment) = delete;
    ~Segment();

  public:
    int getNewPage(); // alloc free page, add it to managing vector and return its index in the partition

  public:
    int storeSegment();                         // serialization
    int loadSegment(const uint32_t aPageIndex); // deserialization

  private:
    /* The maximum number of pages a segment can manage */
    uint16_t _maxSize;
};

#endif