#include "buf_cntrl_block.hh"

BufferControlBlock::BufferControlBlock() :
    _pageID(),
    _frameIndex(-1),
    _pageMutex(),
    _modified(false),
    _fixCount(0),
    _nextInChain(nullptr)
{}

BufferControlBlock::~BufferControlBlock()
{

}


