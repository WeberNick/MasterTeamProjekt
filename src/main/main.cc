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

void testRawPartitions()
{
    
    bool              C_INSTALL                   = true;
    std::string       C_MASTER_PARTITION_PATH     = "/home/nickwebe/Desktop/MasterPartition";
    const std::string C_TRACE_DIR_PATH            = "/home/nickwebe/Desktop/";
    const size_t      C_PAGE_SIZE                 = 4096;
    const size_t      C_BUFFER_POOL_SIZE          = 10000;
    const bool        C_TRACE_ACTIVATED           = true;
    
    control_block_t lCB = {	
        C_INSTALL,	
        C_MASTER_PARTITION_PATH,	
        C_TRACE_DIR_PATH,	
        C_PAGE_SIZE,	
        C_BUFFER_POOL_SIZE,	
        C_TRACE_ACTIVATED	
    };

    try
    {
        // init all global singletons
        Trace::getInstance().init(lCB);
        PartitionManager::getInstance().init(lCB);
        SegmentManager::getInstance().init(lCB);
        BufferManager::getInstance().init(lCB);
        DatabaseInstanceManager::getInstance().init(lCB);
    }
    catch(const ReturnException& ex)
    {
        // Any exceptions from which there is no recovery possible, are catched here 
        std::cerr << ex.what() << std::endl;
    }

    PartitionManager& lPartMngr = PartitionManager::getInstance();
    SegmentManager& lSegMngr = SegmentManager::getInstance();

    auto [lPart, created] = lPartMngr.createPartitionRawInstance("/mnt/hdd/", "MyPartition");



}


/***********************************************************************
*  todo: test install, boot, (shutdown), buf manager, and everything  *
***********************************************************************/

/* Pass path to partition as argument when executing!
   
   Print Overview:
   >      indicates print from main
   bold   indicates print from other methods
   normal indicates terminal output (e.g. output from creation of partition with linux command) */
int main(const int argc, const char* argv[])
{
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

    bool              C_INSTALL                   = false; // will be set in parser, depending on user input
    std::string       C_MASTER_PARTITION_PATH     = lArgs.masterPartition();
    const std::string C_TRACE_DIR_PATH            = lArgs.tracePath();
    const size_t      C_PAGE_SIZE                 = 4096;
    const size_t      C_BUFFER_POOL_SIZE          = lArgs.bufferFrames();
    const bool        C_TRACE_ACTIVATED           = lArgs.trace();
    
    control_block_t lCB = {	
        C_INSTALL,	
        C_MASTER_PARTITION_PATH,	
        C_TRACE_DIR_PATH,	
        C_PAGE_SIZE,	
        C_BUFFER_POOL_SIZE,	
        C_TRACE_ACTIVATED	
    };

    // Actual programm starts here.     
    try
    {
        // init all global singletons
        Trace::getInstance().init(lCB);
        PartitionManager::getInstance().init(lCB);
        SegmentManager::getInstance().init(lCB);
        BufferManager::getInstance().init(lCB);
        CommandParser::getInstance().init(lCB, "mdb > ", '#');
    }
    catch(const ReturnException& ex)
    {
        // Any exceptions from which there is no recovery possible, are catched here 
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
