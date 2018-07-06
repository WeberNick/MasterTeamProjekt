#include "../infra/args.hh"
#include "../infra/types.hh"
#include "../infra/exception.hh"
#include "../infra/trace.hh"
#include "../infra/file_util.hh"
#include "../infra/employee_t.hh"
#include "../cli/parser.hh"
#include "db_instance_manager.hh"

#include <iostream>
#include <cstdlib>


/***********************************************************************
*  todo: test install, boot, (shutdown), buf manager, and everything  *
***********************************************************************/

/* Pass path to partition as argument when executing!

   Print Overview:
   >      indicates print from main
   bold   indicates print from other methods
   normal indicates terminal output (e.g. output from creation of partition with linux command) */
int main(const int argc, const char* argv[]) {
    /* Parse Command Line Arguments */
    Args lArgs;
    argdesc_vt lArgDesc;
    construct_arg_desc(lArgDesc);

    if (!parse_args<Args>(1, argc, argv, lArgDesc, lArgs))
    {
        std::cerr << "Error while parsing arguments." << std::endl;
        return -1;
    }
    if (lArgs.help() || argc == 0)
    {
       print_usage(std::cout, argv[0], lArgDesc);
       return 0;
    }
    if(lArgs.trace() && !FileUtil::hasValidDir(lArgs.tracePath()))
    {
        std::cerr << "The path where to store the trace file is invalid." << std::endl;
        return -1;
    }

    const bool          C_INSTALL                   = false;
    const std::string   C_MASTER_PARTITION_PATH     = std::string(std::getenv("HOME")) + "/Desktop/" + std::string("/MasterPartition");
    const std::string   C_PARTITION_PATH            = std::string(std::getenv("HOME")) + "/Desktop/" + std::string("/Partition");
    const std::string   C_TRACE_DIR_PATH            = std::string(std::getenv("HOME")) + "/Desktop/" + std::string("/");
    const size_t        C_PAGE_SIZE                 = 4096;
    const size_t        C_BUFFER_POOL_SIZE          = 100000;
    const bool          C_TRACE_ACTIVATED           = true;

    const control_block_t lCB = {
        C_INSTALL,
        C_MASTER_PARTITION_PATH,
        C_TRACE_DIR_PATH,
        C_PAGE_SIZE,
        C_BUFFER_POOL_SIZE,
        C_TRACE_ACTIVATED
    };

    
    Trace::getInstance().init(lCB);
    PartitionManager::getInstance().init(lCB);
    SegmentManager::getInstance().init(lCB);
    BufferManager::getInstance().init(lCB);
    DatabaseInstanceManager::getInstance().init(lCB); // installs or boots the DBS
    CommandParser::getInstance().init(lCB, "mdb > ", '#');
    return EXIT_SUCCESS;
}
