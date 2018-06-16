#include "buf_hash_table.hh"

BufferHashTable::BufferHashTable(const size_t aHashTableSize) :
	_size(aHashTableSize),
	_hashTable(nullptr)
{ _hashTable = new HashBucket[_size]; }

BufferHashTable::~BufferHashTable()
{
//	BCB* lBCB = nullptr;
// 	for(size_t i = 0; i < _size; ++i)
// 	{
// 		lBCB = _hashTable[i].getBCB();
// 		while(_hashTable[i].getBCB())
// 		{
// 		    lBCB = _hashTable[i].getBCB();
// 			_hashTable[i].setBCB(lBCB->_nextInChain);
// 			delete lBCB;
// 		}
// 	}
	delete[] _hashTable;
}


size_t BufferHashTable::hash(const PID& aPageID)
{
    std::hash<uint> lHash;
    return (lHash(aPageID.fileID()) + lHash(aPageID.pageNo())) % _size;
}
sMtx&    BufferHashTable::getBucketMtx(const size_t aHash)
{ 
	TRACE("got mutex of bucket "+std::to_string(aHash));
	return _hashTable[aHash].getMtx(); }


