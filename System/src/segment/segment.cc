/**
 *  @file    segment.cc
 *  @author  Nick Weber (nickwebe@pi3.informatik.uni-mannheim.de), 
 			 Nicolas Wipfler (nwipfler@mail.uni-mannheim.de)
 *  @brief   This class manages multiple pages
 *  @bugs    Currently no bugs known
 *  @todos   Implement storeSegment and loadSegment
 *  @section TBD
 */

#include "segment.hh"

Segment::Segment(const uint aSegID, PartitionFile& aPartition) :
    _segID(aSegID),
    _pages(),
    _partition(aPartition),
    _maxSize(0),
    _index(0),
    _header()
{
	_partition.openPartition();
	_maxSize = (_partition.getPageSize() - sizeof(segment_page_header_t)) / sizeof(uint32_t);
	int lSegmentIndex = _partition.allocPage();
	_index = (lSegmentIndex > 0) ? (uint32_t)lSegmentIndex : 0;
	uint64_t lLSN = 0;		//todo
	uint8_t lVersion = 0;	//todo
	uint8_t lUnused = 0;
	basic_header_t lBasicHeader = {lLSN, _index, _partition.getID(), lVersion, lUnused, lUnused};
	_header = {lBasicHeader, _maxSize, 0, lVersion, lUnused, lUnused, lUnused};
	_partition.closePartition();
}

Segment::~Segment(){}

int Segment::getNewPage()
{
	if (_pages.size() < _maxSize) {
		int lPageIndex = _partition.allocPage();
		if (lPageIndex != -1) {
			_pages.push_back((uint32_t)lPageIndex);
			return _pages.size() - 1;
		}
	}
	return -1;
}

int Segment::loadPage(byte* aPageBuffer, const uint aPageNo)
{
	if(_partition.openPartition() == -1)
	{
		return -1;
	}
	if(_partition.readPage(aPageBuffer, _pages[aPageNo], _partition.getPageSize()) == -1)
	{
		return -1;
	}
	return 0;
}

int Segment::storePage(const byte* aPageBuffer, const uint aPageNo)
{
	if(_partition.openPartition() == -1)
	{
		return -1;
	}
	if(_partition.writePage(aPageBuffer, _pages[aPageNo], _partition.getPageSize()) == -1)
	{
		return -1;
	}
	return 0;
}

int Segment::storeSegment()
{
	 //assuming header stores up to date information
	 //segment 1 on position 1
	 //header last

    byte* lPageBuffer = new byte[_partition.getPageSize()];
     size_t lPageSize = _partition.getPageSize();
     for (uint i=0;i<_pages.size();++i){
         *((uint32_t*) (lPageBuffer + i))=_pages.at(i);
     }
     *(segment_page_header_t*) (lPageBuffer + lPageSize - sizeof(segment_page_header_t) )=_header;
    return 0;
}

int Segment::loadSegment(const uint32_t aPageIndex)
{
	 //to be set beforehand: partition, and it has to be opened
	 
     byte* lPageBuffer = new byte[_partition.getPageSize()];
     size_t lPageSize = _partition.getPageSize();
     _partition.readPage(lPageBuffer,aPageIndex,_partition.getPageSize());
     _header = *(segment_page_header_t*) (lPageBuffer + lPageSize - sizeof(segment_page_header_t));
     for (uint i=0;i<_header._currSize;++i){
         _pages.push_back( *((uint32_t*) (lPageBuffer + i)) );
     }
// //some more variables to be set
    return 0;
}