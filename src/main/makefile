CXXFLAGS = -O3 -Wall -std=c++17
CC = g++

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
READFLAG = -lreadline
else
READFLAG = -ledit
endif

LINKER = \
	-lstdc++fs \
	-lpthread \
	\

INCL = -I../.. -I..

HDR = \
	
MAIN_HDR = \
	../infra/args.hh \
	../infra/types.hh \
	../infra/exception.hh \
	../infra/trace.hh \
	../infra/file_util.hh \
	../infra/employee_t.hh \
	../cli/parser.hh \
	db_instance_manager.hh \
	\

DBIM_HDR = \
	../infra/types.hh \
	../infra/exception.hh \
	../infra/trace.hh \
	../infra/partition_t.hh \
	../infra/segment_t.hh \
	../buffer/buf_mngr.hh \
	../partition/partition_manager.hh \
	../segment/segment_manager.hh \
	../segment/segment_fsm_sp.hh \
	../interpreter/interpreter_sp.hh \
	\

OBJ = \
	../infra/args.o \
	../infra/exception.o \
	../infra/trace.o \
	../infra/partition_t.o \
	../infra/segment_t.o \
	../infra/employee_t.o \
	\
	../cli/linereaderedit.o \
	../cli/datejd.o \
	../cli/stime.o \
	../cli/stimestamp.o \
	../cli/parser.o \
	\
	../partition/partition_manager.o \
	../partition/partition_base.o \
	../partition/partition_file.o \
	../partition/partition_raw.o \
	\
	../segment/segment_manager.o \
	../segment/segment_base.o \
    ../segment/segment_fsm.o \
	../segment/segment_fsm_sp.o \
	\
	../interpreter/interpreter_fsip.o \
	../interpreter/interpreter_fsm.o \
	../interpreter/interpreter_sp.o \
	\
	../buffer/buf_mngr.o \
	../buffer/buf_hash_table.o \
	../buffer/buf_cntrl_block.o \
	\
	db_instance_manager.o \
	\

ALL = main 

all : $(ALL)

main : db_instance_manager.o main.o
	$(CC) main.o $(OBJ) -o main $(LINKER) $(READFLAG)

main.o : main.cc $(HDR) $(MAIN_HDR)
	$(CC) -c $(CXXFLAGS) $(INCL) main.cc

db_instance_manager.o : db_instance_manager.cc db_instance_manager.hh $(HDR) $(DBIM_HDR)
	$(CC) -c $(CXXFLAGS) $(INCL) db_instance_manager.cc

clean :
	rm -f *.o a.out $(ALL)
