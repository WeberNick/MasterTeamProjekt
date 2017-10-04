/**
 *  @file    segment_manager.hh
 *  @author  Nick Weber (nickwebe@pi3.informatik.uni-mannheim.de), Nicolas Wipfler (nwipfler@mail.uni-mannheim.de)
 *  @brief   This class manages multiple segments
 *  @bugs    Currently no bugs known
 *  @todos   - Pass Header information to getNewSegment for creation of Segment()
 *           - Implement:
 *               SegmentManager::storeSegmentManager()
 *               SegmentManager::loadSegmentManager()
 *               SegmentManager::storeSegments()
 *               SegmentManager::loadSegments()
 *  @section TBD
 */

#ifndef SEGMENT_MANAGER_HH
#define SEGMENT_MANAGER_HH

#include "infra/types.hh"
#include "infra/header_structs.hh"
#include "partition/partition_file.hh"
#include "segment.hh"

#include <vector>

class SegmentManager
{
	public:
		explicit SegmentManager(PartitionFile& aPartition);
		SegmentManager(const SegmentManager& aSegmentManager) = delete;
		SegmentManager& operator=(const SegmentManager& aSegmentManager) = delete;
		~SegmentManager();	                    // delete all segments

	public:
		Segment* getNewSegment();				// create and add new segment (persistent), return it
		const int storeSegmentManager();		// serialization
		const int loadSegmentManager();			// deserialization

	public:
		inline const uint getNoSegments() { return _segments.size(); }				
		Segment* getSegment(const uint aSegmentNo);

	private:
		const int storeSegments();				// serialize segments? called by storeSegmentManager
		const int loadSegments();				// deserialize segments? called by storeSegmentManager

	private:
		/* ID Counter for Segments */
		uint _counterSegmentID;
		/* Stores all managed Segments */
		std::vector<Segment*> _segments;
		/* Indices of Pages in the Partition where the SegmentManager itself is spread; Default is Page 1 */
		std::vector<uint32_t> _ownPages;		
		/* Number of Pages that can be managed on one SegmentManager Page */
		uint32_t _maxSegmentsPerPage;
		/* Partition the SegmentManager belongs to */
		PartitionFile& _partition;
};

#endif