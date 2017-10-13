// #include "infra/bit_intrinsics.hh"
#include "infra/types.hh"
#include "partition/partition_file.hh"
#include "partition/partition_manager.hh"
#include "segment/segment.hh"
#include "segment/segment_manager.hh"


#include <iostream>

/* Pass path to partition as argument when executing!
   
   Print Overview:
   >      indicates print from main
   bold   indicates print from other methods
   normal indicates terminal output (e.g. output from creation of partition with linux command) */
int main(const int argc, const char *argv[]) {
	std::cout << "\n";
	
	PartitionManager lPartMngr;
    PartitionFile *lPartFile = lPartMngr.createPartitionFileInstance(argv[1], c_PartitionSizeInPages(), c_PageSize(), c_GrowthIndicator());
    SegmentManager lSegMngr(*lPartFile);

    std::cout << "> Creating partition on disk..." << std::endl;

    if (lPartFile->createPartition() != 0) // This could be commented out
        std::cout << "\033[0;31m> Something went wrong while creating the partition...\033[0m" << std::endl; // will be printed as long as reservePage in fsip is not implemented
    if (lPartMngr.getPartition(lPartFile->getID())->openPartition() != 0)
        std::cout << "\033[0;31m> Something went wrong while opening the partition...\033[0m" << std::endl;

    std::cout << "> Partition with path " << lPartMngr.getPartition(lPartFile->getID())->getPath() << " is open!" << std::endl;

    /* Add further tests...
    std::cout<<"highest bit set in all 1 "<<idx_highest_bit_set<uint64_t>(~0)<<std::endl;
    std::cout<<"highest bit set in 1"<<idx_highest_bit_set<uint64_t>(1)<<std::endl;
    std::cout<<"lowest bit set in all 1 "<<idx_lowest_bit_set<uint64_t>(~0)<<std::endl;
    */

    uint nxFreePage = lPartMngr.getPartition(lPartFile->getID())->allocPage();
    std::cout << "> Get free page results in page " << nxFreePage << std::endl;

    if (lPartMngr.getPartition(lPartFile->getID())->freePage(2) != 0)
        std::cout << "\033[0;31m> Something went wrong while freeing the page...\033[0m" << std::endl;

    Segment* lFirstSeg = lSegMngr.createNewSegment();
	Segment* lSecondSeg = lSegMngr.createNewSegment();
	Segment* lThirdSeg = lSegMngr.createNewSegment();
	if (lSegMngr.getNoSegments() != 3)
		std::cout << "\033[0;31m> SegmentManager handling of _segments incorrect...\033[0m" << std::endl;
	
	uint lSegID = lFirstSeg->getID();
	std::cout << "> Compare this address: " << (void*)lSegMngr.getSegment(lSegID) << std::endl;
	std::cout << "> with this:            " << (void*)lFirstSeg << std::endl;
	
	uint lSegPageIndex = lFirstSeg->getNewPage();
	std::cout << "> SegmentPageIndex: " << lSegPageIndex << std::endl;
	uint lSegNoPages = lFirstSeg->getNoPages();
	std::cout << "> SegmentNoPages:   " << lSegNoPages << std::endl;
	uint lSegIndexFirst = lFirstSeg->getIndex();
	uint lSegIndexThird = lThirdSeg->getIndex();
	std::cout << "> SegIndexFirst:    " << lSegIndexFirst << std::endl;
	std::cout << "> SegIndexThird:   " << lSegIndexThird << std::endl;

	std::cout << "> Compare this address: " << (void*)&lSecondSeg->getPartition() << std::endl;
	std::cout << "> with this:            " << (void*)lPartFile << std::endl;

	// lSegMngr.storeSegmentManager();
	// lSegMngr.loadSegmentManager();

	// if (lSegMngr.getNoSegments() != 3 or lSegMngr.getSegment(0) != lFirstSeg)
	if (lSegMngr.getNoSegments() != 3)
	    std::cout << "\033[0;31m> SegmentManager handling of _segments incorrect...\033[0m" << std::endl;
		// std::cout << "\033[0;31m> SegmentManager (de-)serialization went wrong...\033[0m" << std::endl;

	// this is printed because !_isOpen
    if (lPartMngr.getPartition(lPartFile->getID())->closePartition() != 0)
        std::cout << "\033[0;31m> Something went wrong closing the partition...\033[0m" << std::endl;

    std::cout << "> Partition is closed!" << std::endl;
    std::cout << "> Remove the partition from disk...\n" << std::endl;

    if (lPartFile->removePartition() != 0) // This could be commented out
        std::cout << "\033[0;31m> Something went wrong removing the partition...\033[0m\n" << std::endl;

    return 0;
}