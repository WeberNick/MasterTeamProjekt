/*
 *  @file    types.hh
 *  @author  Nicolas Wipfler (nwipfler@mail.uni-mannheim.de)
 *           Nick Weber (nickwebe@pi3.informatik.uni-mannheim.de)
 *
 */
#pragma once

#include <cstddef>
#include <cstdint>
#include <limits>
#include <vector>
#include <string>
#include <cstring>
#include <mutex>
#include <shared_mutex>
#include <iostream>
#include <sstream>

using byte = std::byte;
using size_t = std::size_t;
using uint = unsigned int;
using char_vpt = std::vector<char*>;
using uint_vt = std::vector<uint>;
using byte_vpt = std::vector<byte *>;
using uint32_vt = std::vector<uint32_t>;
using sMtx = std::shared_mutex;
using mtx = std::mutex;

constexpr size_t INVALID = std::numeric_limits<size_t>::max();
constexpr uint32_t MAX32 =  std::numeric_limits<uint32_t>::max();

struct control_block_t
{
    const bool _install;
    const std::string _masterPartition;
    const std::string _tracePath;
    const size_t _pageSize;
    const size_t _noBufFrames;
    const bool _trace;

    bool install() const { return _install; }
    const std::string& mstrPart() const { return _masterPartition; }
    const std::string& tracePath() const { return _tracePath; }
    size_t pageSize() const { return _pageSize; }
    size_t frames() const { return _noBufFrames; }
    bool trace() const { return _trace; }
    inline friend std::ostream& operator<<(std::ostream& strm, const control_block_t& cb) {
        strm << "The following parameters are set:\n"
             << "Install: " << cb.install() << "\n"
             << "Master Partition Path: " << cb.mstrPart() << "\n"
             << "Path of Log File: " << cb.tracePath() << "\n"
             << "Page Size: " << cb.pageSize() << "\n"
             << "Buffer Frames: " << cb.frames() << "\n"
             << "Trace: " << cb.trace() << "\n";
        return strm << std::endl;
    }
};
using CB = control_block_t;

struct page_id_t
{
    uint8_t _fileID;
    uint32_t _pageNo; // correct? 

    uint8_t fileID() const { return _fileID; }
    uint32_t pageNo() const { return _pageNo; }

    bool operator==(const page_id_t& aOther) 
    {
        return (_fileID == aOther._fileID && _pageNo == aOther._pageNo);
    }
};
using PID = page_id_t;
using pid_vt = std::vector<PID>;

struct part_disk_t{ //partition disk type
    uint        _pID;
    uint        _pType;
    uint        _pGrowth;
    uint        _pName;
    uint        _pPath;
    uint        _size;
    char        _strings [1];

    std::string toString(){
        std::string rtn = "partID "+ std::to_string(_pID) + " Name " + _strings[_pName] +" Path "+ _strings[_pPath] +" type "+ std::to_string(_pType)+ " growth "+ std::to_string(_pGrowth);
        return rtn;
    }
    inline uint size() {return _size; }
};

struct part_mem_t
{
	uint        _pID;
	std::string _pName;
	std::string _pPath;
	uint         _pType;   // 1:= PartitionFile, 2:=partitionRaw
	uint        _pGrowth;
    inline friend std::ostream& operator<<(std::ostream& strm, const part_mem_t& part){ return strm; };
    inline friend std::string to_string(const part_mem_t& part){ std::ostringstream ss; ss << part; return ss.str(); } 
    std::string toString(){
        std::string rtn = "partID "+ std::to_string(_pID) + " Name "+ _pName +" Path "+ _pPath +" type "+ std::to_string(_pType)+ " growth "+ std::to_string(_pGrowth);
        return rtn;
    }
    uint diskSize() { 
        return sizeof(part_disk_t) + _pName.size() + _pPath.size() + 1;
    }


   void operator=(part_disk_t* aDisk){
        _pID = aDisk->_pID;
        _pName = & (aDisk->_strings[aDisk->_pName]);
        _pPath = &(aDisk->_strings[aDisk->_pPath]);
        _pType = aDisk->_pType;
        _pGrowth = aDisk->_pGrowth;
    }  
    //part_mem_t() : _pID(MAX32),_pName(NULL),_pPath(NULL),
    //    _pType(MAX32),_pGrowth(MAX32) { }

    part_disk_t* toDisk(){
        part_disk_t* t = (part_disk_t*) malloc( diskSize());
        t->_pID = _pID;
        t->_pGrowth = _pGrowth;
        t->_pName = 0;
        t->_pPath = _pName.size()+ 1;
        t->_pType = _pType;
        strcpy(& t->_strings[0],_pName.c_str());
        strcpy(& (t->_strings[t->_pPath]),_pPath.c_str());
        t->_size = diskSize();
        return t;
    }
};


struct seg_disk_t{ //Segment DiskType
  uint        _sPID;       // partition ID
  uint        _sID;        // segment ID
  uint         _sType;      // segment type; 1:= SegmentFSM, 2:=SegmentFSM_SP
  uint        _sFirstPage; // first segment index ( (C) Nico) page in order to load segment into memory
  uint          _size;
  char        _sName [1]; //segment name (unique) 
  inline friend std::ostream& operator<<(std::ostream& strm, const seg_disk_t& seg){ return strm; };
  inline friend std::string to_string(const seg_disk_t& seg){ std::ostringstream ss; ss << seg; return ss.str(); }
  std::string toString(){
        std::string rtn = "SegID "+ std::to_string(_sID) + " Name "+_sName+" Partition "+ std::to_string(_sPID) +" type "+ std::to_string(_sType) + " firstIndexPage "+std::to_string(_sFirstPage);
        return rtn;
    } 
    uint inline size() { return _size; }
};

struct seg_mem_t
{
  uint        _sPID;       // partition ID
  uint        _sID;        // segment ID
  std::string _sName; // segment name (unique)
  uint         _sType;      // segment type; 1:= SegmentFSM, 2:=SegmentFSM_SP
  uint        _sFirstPage; // first segment index ( (C) Nico) page in order to load segment into memory
  inline friend std::ostream& operator<<(std::ostream& strm, const seg_mem_t& seg){ return strm; };
  inline friend std::string to_string(const seg_mem_t& seg){ std::ostringstream ss; ss << seg; return ss.str(); }
  std::string toString(){
        std::string rtn = "SegID "+ std::to_string(_sID) + " Name "+_sName+" Partition "+ std::to_string(_sPID) +" type "+ std::to_string(_sType) + " firstIndexPage "+std::to_string(_sFirstPage);
        return rtn;
    } 
  uint diskSize() {
      return sizeof(seg_disk_t)+_sName.size();
    }

  seg_disk_t* toDisk(){ //convert to disk writable tuple
    seg_disk_t* lSegT = (seg_disk_t*) malloc(diskSize());
    lSegT->_sID = _sID;
    lSegT->_sPID = _sPID;
    lSegT->_sType= _sType;
    lSegT->_sFirstPage=_sFirstPage;
    strcpy(lSegT->_sName,_sName.c_str());
    lSegT->_size= diskSize();
    return lSegT;
  }
  void operator=(seg_disk_t* aDisk){
        _sPID = aDisk->_sPID;
        _sID=aDisk->_sID;
        _sName=aDisk->_sName;
        _sType=aDisk->_sType;
        _sFirstPage=aDisk->_sFirstPage;
    }
      
};

enum class PageStatus 
{
    kNoType = -1,
    kBUCKET0 = 0,
    kBUCKET1 = 1,
    kBUCKET2 = 2,
    kBUCKET3 = 3,
    kBUCKET4 = 4,
    kBUCKET5 = 5,
    kBUCKET6 = 6,
    kBUCKET7 = 7,
    kBUCKET8 = 8,
    kBUCKET9 = 9,
    kBUCKET10 = 10,
    kBUCKET11 = 11,
    kBUCKET12 = 12,
    kBUCKET13 = 13,
    kBUCKET14 = 14,
    kBUCKET15 = 15,
    kPageStatusSize = 16
};

enum LOCK_MODE
{
    kNoType = -1,
    kNOLOCK = 0,
    kSHARED = 1,
    kEXCLUSIVE = 2,
    kLockModeSize = 3
};
