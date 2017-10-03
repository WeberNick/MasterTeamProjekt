#include "file_partition.hh"

FilePartition::FilePartition(const char* aPath, const uint64_t aFilePartitionSize, const uint aPageSize, const uint aGrowthIndicator, const uint aFilePartitionID) :
	_partitionPath(aPath),
	_partitionSize(aFilePartitionSize),
	_pageSize(aPageSize),
	_growthIndicator(aGrowthIndicator),
	_partitionID(aFilePartitionID),
	_segmentIndexPage(0),
	_isCreated(false),
	_isOpen(false)
{}

FilePartition::~FilePartition()
{
	if(_isCreated)
	{
		removePartition();
	}
}

const int FilePartition::openPartition(const std::string aMode)
{
	if(aMode != "r" && aMode != "w" && aMode != "rw")
	{
		std::cerr << "Unknown mode: '" << aMode << "'" << std::endl;
		return -1;
	}
	int lMode;
	if(aMode == "r") 		lMode = O_RDONLY;
	else if(aMode == "w") 	lMode = O_WRONLY;
	else if(aMode == "rw") 	lMode = O_RDWR;
	int lFileDescriptor = open(_partitionPath, lMode);
	if(lFileDescriptor == -1)
	{
		std::cerr << "Error opening the file: " << std::strerror(errno) << std::endl;
		return -1;
	}
	return lFileDescriptor;
}

const int FilePartition::closePartition(const int aFileDescriptor)
{
	if(close(aFileDescriptor) != 0)
	{
		std::cerr << "Error closing the file: " << std::strerror(errno) << std::endl;
		return -1;
	}
	return 0;
}

const int FilePartition::createPartition(mode_t aAccessRights)
{
	if(_isCreated)
	{
		return -1;
	}
	std::string lCommand = "dd if=/dev/zero of=" + std::string(_partitionPath) + " bs=" + std::to_string(_partitionSize) + " count=1";
	std::cout << "The following command will be executed: '" << lCommand << "'" << std::endl;
	system(lCommand.c_str());
	std::cout << "A partition with " << _partitionSize << " Bytes was successfully created!" << std::endl;
	_isCreated = true;
	if(init() != 0 )
	{
		std::cerr << "The partition could not be initialized and will be removed!" << std::endl;
		removePartition();
		return -1;
	}
	//reserve page for segment index page at partition index 1
	if(reserve(1) != 0)
	{
		return -1;
	}
	return 0;
}

const int FilePartition::removePartition()
{
	if(!_isCreated)
	{
		return -1;
	}
	std::string lCommand = "rm " + std::string(_partitionPath);
	std::cout << "The following command will be executed: '" << lCommand << "'" << std::endl;
	system(lCommand.c_str());
	std::cout << "FilePartition was successfully removed!" << std::endl;
	_isCreated = false;
	return 0;
}

const int FilePartition::allocPage()
{
	byte* lPagePointer = new byte[_pageSize];
	const uint lNumberOfTotalPages = totalPages();
	FSIPInterpreter fsip;
	fsip.attach(lPagePointer);
	uint lIndexOfFSIP = 0;
	int lAllocatedPageIndex;
	int lFileDescriptor = openPartition("read");
	do
	{
		readPage(lFileDescriptor, lPagePointer, lIndexOfFSIP, _pageSize);	//Read FSIP into buffer
		lAllocatedPageIndex = fsip.getNewPage(lPagePointer, LSN, _partitionID);	//Request free block from FSIP
		lIndexOfFSIP += (1 + fsip.noManagedPages());							//Prepare next offset to FSIP
		if(lIndexOfFSIP >= lNumberOfTotalPages)								//Next offset is bigger than the partition
		{
			std::cerr << "No free block available in the partition!" << std::endl;
			return -1;
		}
	}
	while(lAllocatedPageIndex == -1);	//if 'lAllocatedPageIndex != -1' a free block was found
	delete[] lPagePointer;
	closePartition(lFileDescriptor);
	return lAllocatedPageIndex;	//return offset to free block
}

const int FilePartition::freePage(const uint aPageNo)
{
	byte* lPagePointer = new byte[_pageSize];
	int lFileDescriptor = openPartition("read");
	if(readPage(lFileDescriptor, lPagePointer, aPageNo, _pageSize) == -1)
	{
		std::cerr << "An error occured while trying to free the block: " << aPageNo << std::endl;
		return -1;
	}
	FSIPInterpreter fsip;
	fsip.attach(lPagePointer);
	fsip.freePage(aPageNo);
	fsip.detach();
	delete[] lPagePointer;
	closePartition(lFileDescriptor);
	return 0;
}

const int FilePartition::readPage(const int aFileDescriptor, byte* aBuffer, const uint aPageNo, const uint aBufferSize)
{
	if(pread(aFileDescriptor, aBuffer, aBufferSize, (aPageNo * _pageSize)) == -1)
	{
		std::cerr << "Error reading the file: " << std::strerror(errno) << std::endl;
		return -1;
	}
	return 0;
}

const int FilePartition::writePage(const int aFileDescriptor, const byte* aBuffer, const uint aPageNo, const uint aBufferSize)
{
	if(pwrite(aFileDescriptor, aBuffer, aBufferSize, (aPageNo * _pageSize)) == -1)
	{
		std::cerr << "Error writing the file: " << std::strerror(errno) << std::endl;
		return -1;
	}
	return 0;
}

const uint FilePartition::totalPages()
{
	return std::ceil(_partitionSize / (double) _pageSize);
}

const int FilePartition::init()
{
	byte* lPagePointer = new byte[_pageSize];	//buffer to write the initial FSIP into
	uint lPagesPerFSIP = 32576;					//todo: auslagern, berechnung in fsip?
	uint lCurrentPageNo = 0;
	FSIPInterpreter fsip;
	uint remainingPages = totalPages();
	int lFileDescriptor = openPartition("write");
	if(lFileDescriptor == -1)
	{
		return -1;
	}
	while(remainingPages > 1)
	{
		--remainingPages;
		fsip.initNewFSIP(lPagePointer, LSN, lCurrentPageNo, _partitionID, remainingPages);
		if(writePage(lFileDescriptor, lPagePointer, lCurrentPageNo, _pageSize) == -1)
		{
			return -1;
		}
		lCurrentPageNo += (lPagesPerFSIP + 1);
		remainingPages -= (remainingPages > lPagesPerFSIP) ? lPagesPerFSIP : remainingPages;
	}
	delete[] lPagePointer;
	if(closePartition(lFileDescriptor) == -1)
	{
		return -1;
	}
	return 0;
}


const int FilePartition::reserve(const uint aIndex)
{
	_segmentIndexPage = aIndex;

	//todo

	return -1;
}


