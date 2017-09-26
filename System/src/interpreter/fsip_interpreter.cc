#include "fsip_interpreter.hh"

FSIPInterpreter::FSIPInterpreter() : _pp(NULL), _header(NULL), _blockSize(0)
{}

FSIPInterpreter::~FSIPInterpreter(){}

void FSIPInterpreter::detach()
{
	_pp = NULL;
	_header = NULL;
	_blockSize = 0;
}

void FSIPInterpreter::initNewFSIP(byte* aPP, const uint64_t aLSN, const uint32_t aOffset, const uint8_t aPID, const uint32_t aNoBlocks)
{
	attach(aPP);
	//todo
}

const int FSIPInterpreter::getNewBlock(byte* aPP, const uint64_t aLSN, const uint8_t aPID) //added LSN and PID to param list, pls update header for allocated block
{
	attach(aPP);
	BasicInterpreter lInterp;
	uint32_t lResult = -1;
	uint32_t lReturnValue = _header->_nextFreeBlock;
	size_t lCondition = (lInterp.getBlockSize() - (sizeof(FSIP_header_t) + lInterp.getHeaderSize())) - 1;
	for(uint32_t j = lReturnValue; j <= lCondition; ++j){ //hier wären kommentare schön
		uint8_t lPartBits = *(uint8_t*)(aPP + j); //sieht für mich fehleranfällig aus
		if((~lPartBits) != 0){
			for(uint8_t i = 7; i<= 0; --i){
				uint8_t	lTemp= lPartBits;
				lTemp >> i;
				if(lTemp % 2 == 0){
					lResult = 7 - i;
					lResult += lInterp.getPartitionOffset();
					uint8_t lMask = 1;
					lPartBits |= (lMask << lResult);
					_header->_nextFreeBlock = i;
					// increase LSN ... Nick?
					break;
				}
			}
			break; 
		}
	}
	return lResult;
}

void FSIPInterpreter::freeBlock(uint aOffset)
{
	BasicInterpreter lInterp;
	aOffset -= lInterp.getPartitionOffset();

	if(_header->_nextFreeBlock > aOffset){
		_header->_nextFreeBlock = aOffset;
	}
	byte* lPP = _pp;
	lPP += (aOffset / 8);
	uint8_t lCurrByte = (uint8_t) *lPP;
	uint8_t lBitindex = 7 - (aOffset % 8);
	uint8_t lMask = 1;
	lMask << lBitindex;
	lCurrByte &= lMask;
	//ändert das tatsächlich den Wert, oder ändert das nur was aufm Stack?
	//Antwort: nur auf dem stack
}
