#include "buf_cntrl_block.hh"

/**
 * @brief Construct a new BufferControlBlock::BufferControlBlock object
 * 
 */
BufferControlBlock::BufferControlBlock() :
    _pageID(),
    _frameIndex(-1),
    _pageMtx(),
    _mode(LOCK_MODE::kNoType),
    _modified(false),
    _fixCount(0),
    _nextInChain(nullptr)
{}

void BufferControlBlock::lock(LOCK_MODE aMode) noexcept
{
    switch(aMode)
    {
        case LOCK_MODE::kNOLOCK:
            break;
        case LOCK_MODE::kSHARED:
            lock_shared();
            break;
        case LOCK_MODE::kEXCLUSIVE:
            lock();
            break;
        default:
            TRACE("Lock type not supported");
            ASSERT_MSG("Invalid default-case of switch statement reached");
            break;
    }
}

void BufferControlBlock::unlock() noexcept
{
    TRACE("Unlock BCB with '" + lockModeToString(getLockMode()) + "' lock and with PID : " + _pageID.to_string());
    switch(getLockMode())
    {
        case LOCK_MODE::kNOLOCK:
            break;
        case LOCK_MODE::kSHARED:
            decrFixCount();
            if(getFixCount() == 0) setLockMode(LOCK_MODE::kNOLOCK);
            _pageMtx.unlock_shared();
            break;
        case LOCK_MODE::kEXCLUSIVE:
            decrFixCount();
            setLockMode(LOCK_MODE::kNOLOCK);
            _pageMtx.unlock();
            break;
        default:
            TRACE("Lock type not supported");
            ASSERT_MSG("Invalid default-case of switch statement reached");
            break;
    }
    TRACE("BCB with lock '" + lockModeToString(getLockMode()) + "' and with PID : " + _pageID.to_string() + " unlocked");
}

void BufferControlBlock::upgradeLock(LOCK_MODE aMode) noexcept
{
    TRACE("Trying to upgrade lock from '" + lockModeToString(getLockMode()) + "' to '" + lockModeToString(aMode) + "' for BCB with PID : " + _pageID.to_string());
    if(toType(getLockMode()) < toType(aMode)) // is upgrade needed?
    {
        switch(aMode)
        {
            case LOCK_MODE::kNOLOCK:
                setLockMode(aMode);
                break;
            case LOCK_MODE::kSHARED:
                _pageMtx.lock_shared();
                setLockMode(aMode);  
                incrFixCount();
                break;
            case LOCK_MODE::kEXCLUSIVE:
                if(toType(getLockMode()) == toType(LOCK_MODE::kSHARED))
                {
                    _pageMtx.unlock_shared();
                }
                _pageMtx.lock();
                setLockMode(aMode);
                setFixCount(1);
                break;
            default:
                const std::string lErrMsg("Lock type not supported");
                TRACE(lErrMsg);
                ASSERT_MSG("Invalid default-case of switch statement reached");
                break;
        }
    }
    TRACE("Successfully upgraded lock for BCB with PID : " + _pageID.to_string());
}

std::ostream& operator<< (std::ostream& stream, BCB* aBCB)
{
    stream << aBCB->to_string();
    return stream;
}
