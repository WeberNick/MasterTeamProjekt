/**
 *  @file 	buf_mngr.hh
 *  @author	Nick Weber (nickwebe@pi3.informatik.uni-mannheim.de)
 *  @brief  Class implementing a access control block for the buffer franes	
 *  @bugs	Currently no bugs known
 *  @todos	TBD
 *  @section TBD
 */

#pragma once

#include "../infra/types.hh"
#include "../infra/exception.hh"
#include "../infra/trace.hh"

#include <mutex>
#include <shared_mutex>

class BufferControlBlock;
using BCB = BufferControlBlock;

class BufferControlBlock final
{
    private:
        friend class BufferManager;
        friend class BufferHashTable;
        explicit BufferControlBlock();
        explicit BufferControlBlock(const BufferControlBlock&) = delete;
        explicit BufferControlBlock(BufferControlBlock&&) = delete;
        BufferControlBlock& operator=(const BufferControlBlock&) = delete;
        BufferControlBlock& operator=(BufferControlBlock&&) = delete;
        ~BufferControlBlock() = default;


    public:
        //getter
        inline PID&     getPID() noexcept { return _pageID; }
        inline size_t   getFrameIndex() noexcept { return _frameIndex; }
        inline sMtx&    getMtx() noexcept { return _pageMtx; }
        inline LOCK_MODE getLockMode() noexcept { return _mode; }
        inline bool     getModified() noexcept { return _modified; }
        inline size_t   getFixCount() noexcept { return _fixCount; }
        inline size_t   incrFixCount() noexcept { return ++_fixCount; }
        inline size_t   decrFixCount() noexcept { return --_fixCount; }
        inline BCB*     getNextInChain() noexcept { return _nextInChain; }
        //setter
        inline void     setPID(const PID aPID) noexcept { _pageID = aPID; }
        inline void     setFrameIndex(const size_t aFrameIndex) noexcept { _frameIndex = aFrameIndex; }
        inline void     setLockMode(LOCK_MODE aMode) noexcept { _mode = aMode; }
        inline void     setModified(const bool aModified) noexcept { _modified = aModified; }
        inline void     setFixCount(const int aFixCount) noexcept { _fixCount = aFixCount; }
        inline void     setNextInChain(BCB* aBCB) noexcept { _nextInChain = aBCB; }

    public:
        void upgradeLock(LOCK_MODE aMode) noexcept;

    private:
        static const CB*  _cb;
        static void setCB(const CB* aControlBlock) noexcept;

    private:
        PID         _pageID; 
        size_t      _frameIndex;    //buffer pool index where page now stored
        sMtx        _pageMtx;       //semaphore to protect page
        LOCK_MODE   _mode;
        bool        _modified;
        size_t      _fixCount;
        //LSN infos
        BCB*        _nextInChain;   //hash overflow chain forward pointer
};
