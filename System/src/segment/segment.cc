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

Segment::Segment(const uint16_t aSegID, PartitionBase& aPartition) :
    SegmentBase(aSegID, aPartition),
    _maxSize(0)
{
	if(_partition.open() == -1){/*error handling*/}
	_maxSize = (_partition.getPageSize() - sizeof(segment_page_header_t)) / sizeof(uint32_t);
	if(_partition.close() == -1){/*error handling*/}
}

Segment::Segment(PartitionBase& aPartition) :
    SegmentBase(aPartition),
    _maxSize(0){}

Segment::~Segment(){}

int Segment::getNewPage()
{
	if (_pages.size() < _maxSize) {
		if(_partition.open() == -1){return -1;}
		int lPageIndex = _partition.allocPage();
		if(_partition.close() == -1){return -1;}
		//todo init page, e.g. NSM/PAX..
		if (lPageIndex != -1) {
			_pages.push_back((uint32_t)lPageIndex);
			return _pages.size() - 1;
		}
	}
	return -1;
}

int Segment::loadPage(byte* aPageBuffer, const uint aPageNo)
{
	if(_partition.open() == -1){return -1;}
	if(_partition.readPage(aPageBuffer, _pages[aPageNo], _partition.getPageSize()) == -1){return -1;}
	if(_partition.close() == -1){return -1;}
	return 0;
}

int Segment::storePage(const byte* aPageBuffer, const uint aPageNo)
{
	if(_partition.open() == -1){return -1;}
	if(_partition.writePage(aPageBuffer, _pages[aPageNo], _partition.getPageSize()) == -1){return -1;}
	if(_partition.close() == -1){return -1;}
	return 0;
}

int Segment::storeSegment()
{
    std::cout<<"storing segment "<<_segID<<" on page "<<_header._basicHeader._pageIndex<<std::endl;
    /* assuming header stores up to date information
       segment 1 on position 1
       header last
       partition needs to be opened beforehand */
    byte *lPageBuffer = new byte[_partition.getPageSize()];
    size_t lPageSize = _partition.getPageSize();
    for (uint i = 0; i < _pages.size(); ++i) {
        std::cout<<_pages.at(i)<<std::endl;
        *(((uint32_t *)lPageBuffer) + i) = _pages.at(i);
    }
    uint8_t lVersion = 1;
    // basic_header: LSN, PageIndex, PartitionId, Version, unused
    basic_header_t lBasicHeader = {0, _index, _partition.getID(), lVersion, 0};
    // segment_page_header_t: maxSize, currSize, segID, version, unused, basicHeader
    segment_page_header_t lSegmentHeader = {_maxSize, _pages.size(), _segID, lVersion, 0, lBasicHeader}; //is this correct, Jonas?
    *(segment_page_header_t *)(lPageBuffer + lPageSize - sizeof(segment_page_header_t)) = lSegmentHeader; //is this correct, Jonas?
    _partition.writePage(lPageBuffer, _index, _partition.getPageSize());
    delete[] lPageBuffer;
    return 0;
}

int Segment::loadSegment(const uint32_t aPageIndex)
{
    std::cout<<"loading segment from page "<<aPageIndex<<" isOpen "<<_partition.isOpen()<< std::endl;
    
    // to be set beforehand: partition, and it has to be opened
    byte *lPageBuffer = new byte[_partition.getPageSize()];
    size_t lPageSize = _partition.getPageSize();
    _partition.readPage(lPageBuffer, aPageIndex, _partition.getPageSize());
    segment_page_header_t lHeader = *(segment_page_header_t *)(lPageBuffer + lPageSize - sizeof(segment_page_header_t)); //is this correct, Jonas?
    for (uint i = 0; i < lHeader._currSize; ++i) {
        _pages.push_back(*(((uint32_t *)lPageBuffer) + i));
    }
    // some more variables to be set
    _segID = lHeader._segID;
    _index = aPageIndex;
    _maxSize = lHeader._maxSize;
    return 0;
}