#include "../infra/args.hh"
#include "../infra/types.hh"
#include "../infra/exception.hh"
#include "../infra/trace.hh"
#include "../infra/file_util.hh"
#include "../infra/tuples.hh"
#include "../cli/parser.hh"
#include "db_instance_manager.hh"

#include <iostream>
#include <cstdlib>

void testJonas1() {
    const control_block_t lCB = {
            true,
            std::string(std::getenv("HOME")) + std::string("/MasterTeamProjekt/MasterPartition"),
            std::string(std::getenv("HOME")) + std::string("/MasterTeamProjekt/"),
            4096,
            100000,
            true
        };
        std::cout << lCB;
    
    Trace::getInstance().init(lCB);
    PartitionManager::getInstance().init(lCB);
    SegmentManager::getInstance().init(lCB);
    BufferManager::getInstance().init(lCB);
    DatabaseInstanceManager::getInstance().init(lCB); // installs or boots the DBS

    TRACE("Trace works");
    std::string lHome(std::getenv("HOME"));
    std::string lPath = lHome + std::string("/MasterTeamProjekt/Partition");
    std::cout << "Path: " << lPath << std::endl;
    //PartitionFile* lFile = PartitionManager::getInstance().createPartitionFileInstance(lPath, "MyPartition", 100); 
  //  if(lFile==nullptr) std::cout<<"fail"<<std::endl;
  //  SegmentManager::getInstance().createNewSegmentFSM(*lFile,"blub");
    
    //a test scenario

    //install    
   
    TRACE("create a new Partition");
    PartitionFile*  lPart = PartitionManager::getInstance().createPartitionFileInstance(lPath,"blub",100);
    TRACE("create a Segment");
    SegmentManager::getInstance().createNewSegmentFSM_SP(*lPart,"bla");
    char lTuple[] = "ThisDoesNotWork";
    TRACE("get segment and insert tuple");
    SegmentFSM_SP* lSeg = (SegmentFSM_SP*) SegmentManager::getInstance().getSegment("bla");
    //insert a tuple
     TRACE("INSERT STUFF");
    for(size_t i =0; i<100000;++i){
        Employee_T emp ("zwei",i, 2);
        lSeg->insertTuple(emp);
    }
        //((PartitionFile*)lPart)->printPage(0);

   // lSeg->insertTuple((byte*) &lTuple, sizeof(lTuple));
   // char lTuple2[] = "SomeMoreRandomChars";
   // lSeg->insertTuple((byte*) &lTuple2,20);
//TRACE("getMasterSegment");
  /*  lSeg = (SegmentFSM_SP*) SegmentManager::getInstance().getSegment(0);
    lSeg->getPage(0,kSHARED);
    lSeg->printPageToFile(0,false);*/

   // TRACE(" FLUSH PAGE TO DISK");
    //BufferManager::getInstance().flushAll();
  //  TRACE(" TRY TO LOAD TUPLE AGAIN");
  //  lSeg->getPage(0,kSHARED);
  //  lSeg->printPageToFile(0,true);
    DatabaseInstanceManager::getInstance().shutdown();
    TRACE("SHUTDOWN COMPLETED");
}
void testJonas2(){
    const control_block_t lCB = {
            true,
            std::string(std::getenv("HOME")) + std::string("/MasterTeamProjekt/MasterPartition"),
            std::string(std::getenv("HOME")) + std::string("/MasterTeamProjekt/"),
            4096,
            100000,
            true
        };
        std::cout << lCB;
    
    Trace::getInstance().init(lCB);
    PartitionManager::getInstance().init(lCB);
    SegmentManager::getInstance().init(lCB);
    BufferManager::getInstance().init(lCB);
    DatabaseInstanceManager::getInstance().init(lCB); // installs or boots the DBS

    TRACE("Trace works");
    std::string lHome(std::getenv("HOME"));
    std::string lPath = lHome + std::string("/MasterTeamProjekt/Partition");
    std::cout << "Path: " << lPath << std::endl;
    TRACE("create a new Partition");
    PartitionFile*  lPart = PartitionManager::getInstance().createPartitionFileInstance(lPath,"blub",100);
    lPart->open();
    for (size_t i = 0;i<5000;++i){
        uint page = lPart->allocPage();
        std::cout<<page<<std::endl;
        if(page!=i+1){
            break;
        }
    }
    ((PartitionFile*)lPart)->printPage(0);
    lPart->close();
}
void testJonas3(){
    const control_block_t lCB = {
            false,
            std::string(std::getenv("HOME")) + std::string("/MasterTeamProjekt/MasterPartition"),
            std::string(std::getenv("HOME")) + std::string("/MasterTeamProjekt/"),
            4096,
            100000,
            true
        };
        std::cout << lCB;
    
    Trace::getInstance().init(lCB);
    PartitionManager::getInstance().init(lCB);
    SegmentManager::getInstance().init(lCB);
    BufferManager::getInstance().init(lCB);
    DatabaseInstanceManager::getInstance().init(lCB); // installs or boots the DBS

    /*TRACE("GET SEGMENT");
    SegmentFSM_SP* lSeg = (SegmentFSM_SP*) SegmentManager::getInstance().getSegment(1);
    TRACE("GET PAGE");
    lSeg->getPage(0,kSHARED);
    lSeg->printPageToFile(0,false);
    TRACE("DELETE PARTITION");*/
   // PartitionManager::getInstance().deletePartition("blub");
  //  PartitionFile*  lPart = PartitionManager::getInstance().createPartitionFileInstance(std::getenv("HOME") + std::string("/MasterTeamProjekt/Partition"),"whatsoever",100);
 //   TRACE("create a Segment");
  //  SegmentFSM_SP* lSeg = (SegmentFSM_SP*) SegmentManager::getInstance().createNewSegmentFSM_SP(*lPart,"bliblablub");
    SegmentFSM_SP* lSeg = (SegmentFSM_SP*) SegmentManager::getInstance().getSegment("bla");
    ((PartitionFile*) PartitionManager::getInstance().getPartition(2))->printPage(2);
    TRACE("INSERT STUFF");
    for(size_t i =0; i<20;++i){
        Employee_T emp ("zwei",i, 1);
        lSeg->insertTuple(emp);
    }
      
    DatabaseInstanceManager::getInstance().shutdown();
    TRACE("SHUTDOWN COMPLETED");
}

void testTupleToDIsk()
{

    Partition_T lPart(64, "My Partition", "/path/to/partition/name", 1, 100);
    const size_t size = lPart.size();
    byte* buf = new byte[4096];
    lPart.toDisk(buf);
    char* t = (char*)buf;
    for(size_t i = 0; i < size; ++i)
    {
        std::cout << "ToChar: " << *t << std::endl;
        ++t;
    }
    Partition_T lPart2;
    lPart2.toMemory(buf);
    std::cout << lPart << std::endl;
    std::cout << lPart2 << std::endl;
}

void testNick()
{
    std::cout << "######  Nick's Test Method is Executed ##########" << std::endl;
    // ASSIGN APPROPRIATE TESTING PARAS
    const bool          C_INSTALL                   = true;
    const std::string   C_MASTER_PARTITION_PATH     = std::string(std::getenv("HOME")) + std::string("/Desktop/Partitions/MasterPartition");
    const std::string   C_TRACE_DIR_PATH            = std::string(std::getenv("HOME")) + std::string("/Desktop/");
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
    std::cout << lCB;

    Trace::getInstance().init(lCB);
    PartitionManager::getInstance().init(lCB);
    SegmentManager::getInstance().init(lCB);
    BufferManager::getInstance().init(lCB);
    DatabaseInstanceManager::getInstance().init(lCB); // installs or boots the DBS

    PartitionManager& pm = PartitionManager::getInstance();
    SegmentManager& sm = SegmentManager::getInstance();
    BufferManager& bm = BufferManager::getInstance();
    DatabaseInstanceManager& dbim = DatabaseInstanceManager::getInstance();

    const std::string lPathToPartitions = std::string(std::getenv("HOME") + std::string("/Desktop/Partitions/"));

    if(C_INSTALL)
    {
        TRACE("### TEST FROM INSTALL ###");
        TRACE("## TEST : Create Partition");
        std::cout << "## TEST : Create Partition" << std::endl;
        PartitionFile* myPart = pm.createPartitionFileInstance(lPathToPartitions + "MyPartition", "MyPartition", 1000);
        std::cout << *myPart << std::endl;
        TRACE("## TEST : Create Segment");
        std::cout << "## TEST : Create Segment" << std::endl;
        SegmentFSM_SP* mySeg = sm.createNewSegmentFSM_SP(*myPart, "MySegment");
        std::cout << *mySeg << std::endl;
        TRACE("## TEST : Create Tuple");
        std::cout << "## TEST : Create Tuple" << std::endl;
        Employee_T empTmp("Nick Weber", 2395, 24);
        std::cout << "## TEST : Tuple Created : " << empTmp << std::endl;
        TRACE("## TEST : Insert Tuple");
        std::cout << "## TEST : Insert Tuple" << std::endl;
        TID myTID = mySeg->insertTuple<Employee_T>(empTmp);
        std::cout << "TID : " << myTID.to_string() << std::endl;
        TRACE("## TEST : Get Tuple");
        std::cout << "## TEST : Get Tuple" << std::endl;
        //Does not work
        Employee_T emp = mySeg->getTuple<Employee_T>(myTID);
        std::cout << emp.to_string() << std::endl;


        TRACE("## TEST : Bulk Insert");
        std::cout << "## TEST : Bulk Insert" << std::endl;
        #include <array>
        #include <vector>
        #include <unordered_map>
        const std::array<std::string, 4>    forenames   = {"Nick", "Nicolas", "Jonas", "Aljoscha"};
        const std::array<std::string, 4>    lastnames   = {"Weber", "Wipfler", "Thietke", "Narr"};
        const std::array<uint8_t, 4>        ages        = {24, 22, 24, 22};
        const std::array<double, 4>       salaries    = {999.99, 2499.32, 4715.12, 2394.56};

        constexpr size_t noPartitions = 1;
        constexpr size_t noSegementsPerPartition = 100;
        constexpr size_t noTuplesPerSegment = 5000;

        std::array<PartitionFile*, noPartitions> partitions;
        std::array<std::array<SegmentFSM_SP*, noSegementsPerPartition>, noPartitions> segments;
        std::array<std::array<std::array<TID, noTuplesPerSegment>, noSegementsPerPartition>, noPartitions> tids;

        for(size_t i = 0; i < noPartitions; ++i)
        {
            const std::string partName = std::string("Partition") + std::to_string(i);
            TRACE("## TEST : Create " + partName);
            std::cout << "## TEST : Create " << partName << std::endl;
            partitions[i] = pm.createPartitionFileInstance(lPathToPartitions + partName, partName, 100);
            std::cout << *partitions.at(i) << std::endl;
            for(size_t j = 0; j < noSegementsPerPartition; ++j)
            {
                const std::string segName = std::string("Segment") + std::to_string(j) + std::string("_") + partName;
                //TRACE("## TEST : Create " + segName);
                //std::cout << "## TEST : Create " << segName << std::endl;
                segments[i][j] = sm.createNewSegmentFSM_SP(*partitions.at(i), segName);
                std::cout << *segments[i][j] << std::endl;

                for(size_t k = 0; k < noTuplesPerSegment; ++k)
                {
                    const size_t r1 = rand() % (3 - 0 + 1) + 0;
                    const size_t r2 = rand() % (3 - 0 + 1) + 0;
                    const size_t r3 = rand() % (3 - 0 + 1) + 0;
                    const size_t r4 = rand() % (3 - 0 + 1) + 0;

                    const std::string name = forenames.at(r1) + " " + lastnames.at(r2);
                    const uint8_t age = ages.at(r3);
                    const double salary = salaries.at(r4);
                    Employee_T tmp(name, salary, age);

                    tids[i][j][k] = segments[i][j]->insertTuple<Employee_T>(tmp);
                }
            }
        }

        for(size_t i = 0; i < noPartitions; ++i)
        {
            for(size_t j = 0; j < noSegementsPerPartition; ++j)
            {
                for(size_t k = 0; k < noTuplesPerSegment; ++k)
                {
                    Employee_T emp = segments[i][j]->getTuple<Employee_T>(tids[i][j][k]);
                    std::cout << emp.to_string() << std::endl;
                }
            }
        }

        TRACE("## TEST : Shutdown");
        dbim.shutdown();
    }
    else
    {
        TRACE("### TEST FROM BOOT ###");
        TRACE("## TEST : Boot and get loaded Partition");
        std::cout << "## TEST : Boot and get loaded Partition" << std::endl;
        PartitionFile* myPart = static_cast<PartitionFile*>(pm.getPartition("MyPartition"));
        std::cout << *myPart << std::endl;

        TRACE("## TEST : Boot and get loaded segment");
        std::cout << "## TEST : Boot and get loaded segment" << std::endl;
        SegmentFSM_SP* mySeg = static_cast<SegmentFSM_SP*>(sm.getSegment("MySegment"));
        std::cout << *mySeg << std::endl;

        TRACE("## TEST : Get Tuple from loaded segment");
        std::cout << "## TEST : Get Tuple from loaded segment" << std::endl;
        const TID myTID = {3,0};
        Employee_T emp = mySeg->getTuple<Employee_T>(myTID);
        std::cout << emp.to_string() << std::endl;


        dbim.shutdown();
    }
}


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

    if (!parse_args<Args>(1, argc, argv, lArgDesc, lArgs)) {
        std::cerr << "Error while parsing arguments." << std::endl;
        return -1;
    } 

    if (lArgs.help() || argc == 0) {
       print_usage(std::cout, argv[0], lArgDesc);
       return 0;
    }
/*    if(lArgs.install() && !FileUtil::hasValidDir(lArgs.masterPartition())) //for install
    {
        std::cerr << "Cannot create master partition at invalid path!" << std::endl;
        //return -1; //wait until boot and so on works and uncomment this
    }
    if(!lArgs.install() && !(FileUtil::exists(lArgs.masterPartition()))) //for boot
    {
        std::cerr << "Given path to the master partition is invalid." << std::endl;
        //return -1; //wait until boot and so on works and uncomment this
    }*/
    if(lArgs.trace() && !FileUtil::hasValidDir(lArgs.tracePath()))
    {
        std::cerr << "The path where to store the trace file is invalid." << std::endl;
        return -1;
    }


    // DONT CHANGE THESE
    //const bool          C_INSTALL                   = lArgs.install();
    //const std::string   C_MASTER_PARTITION_PATH     = lArgs.masterPartition();
    //const std::string   C_TRACE_DIR_PATH            = lArgs.tracePath();
    //const size_t        C_PAGE_SIZE                 = 4096;
    //const size_t        C_BUFFER_POOL_SIZE          = lArgs.bufferFrames();
    //const bool          C_TRACE_ACTIVATED           = lArgs.trace();

    // Actual programm starts here.     
    try
    {
        // ASSIGN APPROPRIATE TESTING PARAS
        const bool          C_INSTALL                   = true;
        const std::string   C_MASTER_PARTITION_PATH     = std::string(std::getenv("HOME")) + std::string("/MasterTeamProjekt/MasterPartition");
        const std::string   C_TRACE_DIR_PATH            = std::string(std::getenv("HOME")) + std::string("/MasterTeamProjekt/");
        const size_t        C_PAGE_SIZE                 = 4096;
        const size_t        C_BUFFER_POOL_SIZE          = lArgs.bufferFrames();
        const bool          C_TRACE_ACTIVATED           = true;

        


       // testTupleToDIsk();

      //  CommandParser::getInstance().init(lCB);
        // init all global singletons
        //Trace::getInstance().init(lCB);
        //PartitionManager::getInstance().init(lCB);
        //SegmentManager::getInstance().init(lCB);
        //BufferManager::getInstance().init(lCB);
        //DatabaseInstanceManager::getInstance().init(lCB); // installs or boots the DBS

        
        testNick();
       // testJonas1();
      //testJonas3();
    //  testStartUp(lCB2);
        // testStartUp(lCB2);
	    // Test call in test.hh
     /*   if (lArgs.test()) {
            // start thread for cli
            std::cout << "test." << std::endl;
            test(lCB);
            return EXIT_SUCCESS;
        }*/

       // CommandParser::getInstance().init(lCB, "mdb > ", '#');
    } catch(const ReturnException& ex) {
         // Any exceptions from which there is no recover possible, are catched here 
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
