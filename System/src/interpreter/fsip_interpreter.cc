#include "fsip_interpreter.hh"

FSIPInterpreter::FSIPInterpreter() : _pp(NULL), _header(NULL), _pageSize(0)
{}

FSIPInterpreter::~FSIPInterpreter(){}

void FSIPInterpreter::detach()
{
	_pp = NULL;
	_header = NULL;
	_pageSize = 0;
}

void FSIPInterpreter::initNewFSIP(byte* aPP, const uint64_t aLSN, const uint32_t aPageIndex, const uint8_t aPID, const uint32_t aNoBlocks)
{
	attach(aPP);
	uint32_t max = aNoBlocks / 32; //wie weit ist Seite frei?
	uint32_t i = 0;
	while (i < max){
		*((uint32_t*) (aPP + i))=0; //setze 0
		++i;
	}
	//nur noch die ersten bits 0, den Rest auf 1
	uint32_t lMask = 0;
	lMask = ~lMask;
	lMask = lMask >> 32 - (aNoBlocks % 32);
	*((uint32_t*) (aPP + i))=lMask;
	max = _pageSize - sizeof(fsip_header_t); //neues Limit
	lMask = 0;
	lMask = ~lMask; //lMask nur noch 1er
	while(i < max){
		*((uint32_t*) (aPP + i)) = lMask;
		++i; //mal hinzugefuegt da dauerschleife.
	}
	//header setzten
	basic_header_t lBTemp = {aLSN,aPageIndex,aPID,1,0,0};
	fsip_header_t temp = {lBTemp,aNoBlocks,0,aNoBlocks};
	*(fsip_header_t*)(aPP + _pageSize - sizeof(fsip_header_t)) = temp;
}

int FSIPInterpreter::getNewPage(byte* aPP, const uint64_t aLSN, const uint8_t aPID) //added LSN and PID to param list, pls update header for allocated block
{
	if(_header->_freeBlocksCount == 0){
		return -1;
	}
	attach(aPP);
	uint32_t lPosFreeBlock = _header->_nextFreeBlock;
		byte* lPP = aPP;
		lPP += lPosFreeBlock/8; // set pointer lPosfreeBlocks/8 bytes forward
		uint8_t lMask = 1;
		uint8_t lPartBits = *(uint8_t*) lPP; //get 8 bit Int representation of the lPP byte pointer 
		lPartBits |= (lMask << lPosFreeBlock % 8); //set complement bit at lPosFreeBlock in lPartBits

	size_t lCondition = ((_pageSize - sizeof(fsip_header_t))/8) - 1;
	for(uint32_t j = lPosFreeBlock/64; j <= lCondition; ++j){ //looping through FSIP with step 8 
		uint64_t* lPP = ((uint64_t*) aPP) + j;
		uint64_t lPartBytes = *lPP; //cast to 8 Byte Int Pointer, add the next j 8Byte block and dereference
		if((~lPartBytes) != 0){
			uint32_t lCalcFreePos = idx_highest_bit_set<uint64_t>(~lPartBytes); //find the first leftmost zero
			//idx_complement_bit<uint64_t>(lPP,lCalcFreePos); //set the bit to 1
			_header->_nextFreeBlock = (j*64) + lCalcFreePos;
			//change LSN
			break; 
		}
	}
	--(_header->_freeBlocksCount);
	return lPosFreeBlock + _header->_basicHeader._pageIndex;
}

int FSIPInterpreter::reservePage(const uint aPageIndex)
{
	//pls implement this
	return -1;
}

void FSIPInterpreter::freePage(const uint aPageIndex)
{
	uint lPageIndex = aPageIndex;
	lPageIndex -= _header->_basicHeader._pageIndex;

	if(_header->_nextFreeBlock > lPageIndex){
		_header->_nextFreeBlock = lPageIndex;
	}
	byte* lPP = _pp;
	lPP += (lPageIndex / 8);
	uint8_t lCurrByte = (uint8_t) *lPP;
	uint8_t lBitindex = 7 - (lPageIndex % 8);
	uint8_t lMask = 1;
	lMask << lBitindex;
	lCurrByte &= lMask;
	++(_header->_freeBlocksCount);
	//ändert das tatsächlich den Wert, oder ändert das nur was aufm Stack?
	//Antwort: nur auf dem stack
}
