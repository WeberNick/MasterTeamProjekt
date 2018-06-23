/**
 * @file    segment_fsm_sp.hh
 * @author  Nick Weber (nickwebe@pi3.informatik.uni-mannheim.de)
 * @brief   Class implementing a Segment with Free Space Management for N-ary Storage Model (SP)
 * @bugs    Currently no bugs known.
 * @todos   -are all changes correct?
 */

#pragma once

#include "../infra/types.hh"
#include "../infra/tuples.hh"
#include "../infra/exception.hh"
#include "../infra/trace.hh"
#include "../infra/header_structs.hh"
#include "../interpreter/interpreter_sp.hh"
#include "../buffer/buf_cntrl_block.hh"
#include "../buffer/buf_mngr.hh"
#include "segment_base.hh"
#include "segment_fsm.hh"

#include <vector>
#include <algorithm>
#include <cstring>

class SegmentFSM_SP : public SegmentFSM
{
  private:
    friend class SegmentManager;
    explicit SegmentFSM_SP() = delete;
    explicit SegmentFSM_SP(const uint16_t aSegID, PartitionBase& aPartition, const CB& aControlBlock);
    explicit SegmentFSM_SP(PartitionBase& aPartition, const CB& aControlBlock);
    explicit SegmentFSM_SP(const SegmentFSM_SP&) = delete;
    explicit SegmentFSM_SP(SegmentFSM_SP&&) = delete;
    SegmentFSM_SP& operator=(const SegmentFSM_SP&) = delete;
    SegmentFSM_SP& operator=(SegmentFSM_SP&&) = delete;
    ~SegmentFSM_SP() = default;

  public:
    template<typename Tuple_T>
    TID insertTuple(const Tuple_T& aTuple);
    TID insertTuple(byte* aTuple, const uint aTupleSize);
    tid_vt insertTuples(const byte_vpt& aTuples, const uint aTupleSize);
    template<typename Tuple_T>
    Tuple_T getTuple(const TID& aTID);
    int getMaxFreeBytes() noexcept { return getPageSize() - sizeof(segment_fsm_sp_header_t) -sizeof(sp_header_t);}
    void loadSegmentUnbuffered(const uint32_t aPageIndex) ;
    void readPageUnbuffered(uint aPageNo, byte* aPageBuffer, uint aBufferSize);   

protected:
	void erase();
};

template<typename Tuple_T>
TID SegmentFSM_SP::insertTuple(const Tuple_T& aTuple)
{
    TRACE("Trying to insert tuple : " + aTuple.to_string());
	// get page with enough space for the tuple and load it into memory
	bool emptyfix = false;
	PID lPID = getFreePage(aTuple.size(), emptyfix);
	BCB* lBCB;
	if(emptyfix){//the page is new, use different command on buffer
		lBCB = _bufMan.emptyfix(lPID);
	}
	else{
		lBCB = _bufMan.fix(lPID, kEXCLUSIVE); 
	}
	byte* lBufferPage = _bufMan.getFramePtr(lBCB);

	InterpreterSP lInterpreter;
	//if the page is new, it has to be initialised first.
	if(emptyfix){
		lInterpreter.initNewPage(lBufferPage);
	}

	// attach page to sp interpreter
	lInterpreter.attach(lBufferPage);
	// if enough space is free on nsm page, the pointer will point to location on page where to insert tuple
	auto [tplPtr, tplNo] = lInterpreter.addNewRecord(aTuple.size()); //C++17 Syntax. Return is a pair, assign pair.first to tplPtr and pair.second to tplNo
    const TID resultTID = {lPID.pageNo(), tplNo};
	
	if(!tplPtr) // If true, not enough free space on nsm page => getFreePage buggy
	{
		const std::string lErrMsg("Not enough free space on nsm page.");
        TRACE(lErrMsg);
        throw NSMException(FLF, lErrMsg);
	}
    aTuple.toDisk(tplPtr);
	lInterpreter.detach();
	lBCB->setModified(true);
	_bufMan.unfix(lBCB);
    TRACE("Inserted tuple successfully.");
    return resultTID;
}

template<typename Tuple_T>
Tuple_T SegmentFSM_SP::getTuple(const TID& aTID)
{
    Tuple_T result;
    auto it = std::find_if(_pages.begin(), _pages.end(), [aTID] (const std::pair<PID, BCB*>& elem) { return elem.first.pageNo() == aTID.pageNo(); }); //get iterator to PID in page vector
    if(it == _pages.cend()){ return result; }
    size_t index = it - _pages.cbegin();
    byte* lPagePtr = getPage(index, kNOLOCK);
    InterpreterSP lInterpreter;
    lInterpreter.attach(lPagePtr);
    byte* lTuplePtr = lInterpreter.getRecord(aTID.tupleNo());
    if(lTuplePtr){ result.toMemory(lTuplePtr); }
    return result;
}
