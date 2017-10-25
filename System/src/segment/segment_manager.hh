/**
 *  @file    segment_manager.hh
 *  @author  Nick Weber (nickwebe@pi3.informatik.uni-mannheim.de), Nicolas Wipfler (nwipfler@mail.uni-mannheim.de)
 *  @brief   This class manages multiple segments
 *  @bugs    Currently no bugs known
 *  @todos   -
 *  @section TBD
 */

#ifndef SEGMENT_MANAGER_HH
#define SEGMENT_MANAGER_HH

#include "infra/types.hh"
#include "infra/header_structs.hh"
#include "partition/partition_base.hh"
#include "segment_base.hh"
#include "segment.hh"

#include <map>

class SegmentManager
{
	private:
		explicit SegmentManager();
		SegmentManager(const SegmentManager& aSegmentManager) = delete;
		SegmentManager& operator=(const SegmentManager& aSegmentManager) = delete;
		~SegmentManager();	         // delete all segments

	public:
        /**
         *  @brief  This function is the only way to get access to the SegmentManager instance
         *
         *  @return reference to the only SegmentManager instance
         */
        static SegmentManager& getInstance()
        {
            static SegmentManager lSegmentManagerInstance;
            return lSegmentManagerInstance;
        }

	public:
		Segment* createNewSegment(PartitionBase& aPartition); // create and add new segment (persistent), return it
		// for further segment types... SegmentA* createNewSegmentA();			
		int storeSegmentManager(PartitionBase& aPartition);	 // serialization
		int loadSegmentManager(PartitionBase& aPartition);    // deserialization

	public:
		inline const uint getNoSegments() { return _segments.size(); }				
		SegmentBase* getSegment(const uint16_t aSegmentID);

	private:
		void storeSegments();
		void loadSegments(uint32_vt& aSegmentPages, PartitionBase& aPartition);

	private:
		/* ID Counter for Segments */
		uint16_t _counterSegmentID;
		/* Stores all managed Segments */
		std::map<uint16_t, SegmentBase*> _segments;
		/* Indices of Pages in the Partition where the SegmentManager itself is spread; Default is Page 1 */
		uint32_vt _indexPages;		
		/* Number of Pages that can be managed on one SegmentManager Page */
		uint32_t _maxSegmentsPerPage;
};

#endif