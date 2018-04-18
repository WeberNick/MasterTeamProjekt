/**
 *  @file	interpreter_fsm.hh
 *  @author	Jonas Thietke (jthietke@mail.uni-mannheim.de),
            Aljoscha Narr (alnarr@mail.uni-mannheim.de)
 *  @brief	A class implementing a Free Space Management via an FSIP for segments for little Endian
 *  @bugs	might not work for big Endian
 *  @todos  change comments (+ insert description for aPageStatus)
 *  @section TBD
 */
#pragma once
#include "infra/types.hh"
#include "infra/exception.hh"
#include "infra/trace.hh"
#include "infra/bit_intrinsics.hh"
#include "infra/header_structs.hh"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <cmath>

class InterpreterFSM {

  public:
    /* standard constructor */
    explicit InterpreterFSM();
    /* If CC and AO are needed, implement them correctly */
    explicit InterpreterFSM(const InterpreterFSM&) = delete;
    explicit InterpreterFSM(InterpreterFSM&&) = delete;
    /* specifies the assign operator of a intermediate buffer with delete */
    InterpreterFSM& operator=(const InterpreterFSM&) = delete;
    InterpreterFSM& operator=(InterpreterFSM&&) = delete;

  public:
    /*	set the page pointer and header */
    inline void attach(byte *aPP);
    /*	unset the page pointer and header */
    void detach();

  public:
    /**
     *	@brief	initialize the FSM through setting all bits to 0 and the header
     *
     *	@param	aPP - Pointer to the start of the page
     *	@param	aLSN - Log Sequence Number
     *	@param	aPageIndex - Page index inside the partition
     *	@param	aPID - The ID of the partition this page is stored in
     *	@param	aNoPages - Number of stored Pages in FSMP
     */
    void initNewFSM(byte *aPP, const uint64_t aLSN, const uint32_t aPageIndex, const uint8_t aPID, const uint32_t aNoPages);

    /**
     *	@brief	looks for the next free block in the FSIP and reserves the page
     *
     *	@param	aPageStatus - 
     *
     * 	@return either an offset to the free block or -1 if no free block was found
     */
    int getFreePage(const PageStatus aPageStatus);

    /**
     *	@brief	change the status of page at the given logical position within the segment
     *
     *	@param	aPageNo - Page index inside the partition
     *	@param	aPageStatus - 
     */
    void changePageStatus(const uint aPageNo, const PageStatus aStatus);
    
    /**
     *  @brief
     *
     *  @param aSizeWithoutOverhead -
     *  @param aNoOfBytes -
     *
     *  @return 
     */
    PageStatus calcPageStatus(const uint aSizeWithoutOverhead, const uint aNoBytes);
    PageStatus getPageStatus(const uint aPageNo);

  public:
    /* Getter */
    inline byte *pagePtr() { return _pp; }
    inline uint32_t getNextFSMPage() { return _header->_nextFSM; } // 0 if not existing, a physical index otherwise
    inline fsm_header_t *getHeaderPtr() { return (fsm_header_t *)(_pp + _pageSize - sizeof(fsm_header_t)); }

    private:
        friend class SegmentFSM;
        /*	size of the page */
        static size_t _pageSize;
        /*	Set page size */
        static void setPageSize(const size_t aPageSize);


  private:
    /*	pointer to the beginning of the page */
    byte *_pp;
    /*	FSIP Header */
    fsm_header_t *_header;
};

void InterpreterFSM::attach(byte *aPP) {
    _pp = aPP;
    _header = getHeaderPtr();
}