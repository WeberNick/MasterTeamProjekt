/*
 *  @file    types.hh
 *  @author  Nicolas Wipfler (nwipfler@mail.uni-mannheim.de)
 *           Nick Weber (nickwebe@pi3.informatik.uni-mannheim.de)
 *
 */

#ifndef INFRA_TYPES_HH
#define INFRA_TYPES_HH

#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>

typedef std::byte byte;
typedef std::vector<byte *> byte_vpt;
typedef unsigned int uint;
typedef std::vector<uint> uint_vt;
typedef std::vector<uint32_t> uint32_vt;

enum class PageStatus {
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
    kEndType = 16
};

struct part_t
{
	uint _pID;
	std::string _pName;
	std::string _pPath;
	int _pType;
	bool _pGrowth;
};
typedef std::vector<part_t> part_vt;

struct seg_t
{
	uint _sPID;
	uint _sID;
	std::string _sName;
	int _sType;
	uint _sFirstPage;
};
typedef std::vector<seg_t> seg_vt;


#endif
