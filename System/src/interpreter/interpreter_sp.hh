/**
 *  @file   interpreter_sp.hh
 *  @author Nick Weber (nickwebe@pi3.informatik.uni-mannheim.de)
 *  @brief  A page interpreter class to operate on slotted pages (NSM)
 *  @bugs   Currently no bugs known
 *  @todos  Currently no todos
 *
 *  @section DESCRIPTION
 *  TODO
 */
#ifndef INTERPRETER_SP_HH
#define INTERPRETER_SP_HH

#include "infra/types.hh"
#include <iostream>

class SP_Interpreter 
{
	public:
		struct header_t 
		{
			uint16_t _noRecords;      // number of records stored on this page
			uint16_t _freeSpace;      // total number of free bytes
			uint16_t _nextFreeRecord; // 8 byte aligned offset to next free record
			uint16_t _placeholder;
		};

		struct slot_t 
		{
			uint16_t _offset; // offset to record
		};

	public:
		SP_Interpreter();
		static void setPageSize(const size_t aPageSize);

	public:
		inline void  attach(byte* aPP);
		void  detach();

	public:
		void  initNewPage(byte* aPP); // combines initialization of fresh page with attach
		byte* addNewRecord(const uint aRecordSize); // returns 0 if page is full

	public:
		inline byte*     pagePtr(){ return _pp; }
		inline header_t* header(){ return _header; }
		inline uint 	 freeSpace(){ return header()->_freeSpace; }
		inline uint 	 noRecords(){ return header()->_noRecords; }

		inline byte*   	 getRecord(const uint aRecordNo);
		inline slot_t& 	 slot(const uint i){ return _slots[- (int) i]; }
		inline size_t  	 getPageSize(){ return _pageSize; }

	private:
		inline header_t* get_hdr_ptr() { return ((header_t*) (_pp + _pageSize - sizeof(header_t))); }
		inline slot_t*   get_slot_base_ptr() { return ((slot_t*) (_pp + _pageSize - sizeof(header_t) - sizeof(slot_t))); }

	private:
		byte*     _pp;
		header_t* _header;
		slot_t*   _slots;  // new
		static bool _pageSizeSet;
		static size_t _pageSize;
};

void SP_Interpreter::attach(byte* aPP)
{
	_pp = aPP;
	_header = get_hdr_ptr();
	_slots  = get_slot_base_ptr();
}

byte* SP_Interpreter::getRecord(const uint aRecordNo) 
{ 
	if(aRecordNo >= noRecords()) { 
		return 0; 
	}
	return (_pp + slot(aRecordNo)._offset); 
}

#endif
