/**
 *  @file    segment_manager.cc
 *  @author  Nicolas Wipfler (nwipfler@mail.uni-mannheim.de)
 *  @brief   This class manages multiple pages
 *  @bugs    Currently no bugs known
 *  @todos   Implement:
 *              Segment::
 *  @section TBD
 */

#include "segment.hh"

Segment::Segment() :
    _segID(0),
    _pages()
{}

Segment::~Segment()
{}

const uint Segment::getNewPage()
{
    
}

const int Segment::loadPage(byte* aPageBuffer, const uint aPageNo)
{

}

const int Segment::storePage(const byte* aPageBuffer, const uint aPageNo)
{

}

const int Segment::storeSegment()
{

}

const int Segment::loadSegment()
{
    
}