/**
 *  @file    basic_interpreter.hh
 *  @author  Nick Weber (nickwebe@pi3.informatik.uni-mannheim.de), Jonas Thietke (jthietke@mail.uni-mannheim.de)
 *  @brief   A class implementing a basic, content independent, block interpreter
 *  @bugs    Currently no bugs known, but there are supposed to be a lot
 *  @todos   probably a lot
 *  @section TBD
 */
#ifndef BASIC_INTERPRETER_HH
#define BASIC_INTERPRETER_HH

#include "infra/types.hh"

class BasicInterpreter
{
	public:
		struct header_Block {
			uint64_t _LSN; 
			uint32_t _offset; //offset from the start of the partition
			uint8_t _PID; //partition ID
			uint8_t _padding1;
			uint8_t _padding2;
			uint8_t _padding3;
	};
		BasicInterpreter();
		~BasicInterpreter();

	public:
		void init(byte* aPP, uint64_t aLSN, uint32_t aOffset, uint8_t aPID);
		
		inline uint16_t getBlockSize{return (_blocksize)};
		inline uint16_t getHeaderSize{return (_headersize)};
		inline header_Block* getHeader{return (_header)};
		inline uint_32_t getPartitionOffset{ return (_header._offset)}

	private:
		byte* _pp;
		header_Block* _header;
		static uint16_t _blocksize //4096 bytes
		static uint16_t _headersize //16 bytes

};



#endif