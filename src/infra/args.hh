/**
 *	@file 	args.hh
 *	@author	Nick Weber (nickwebe@pi3.informatik.uni-mannheim.de)
 *	@brief	Implementation of command line arguements
 *	@bugs 	Currently no bugs known
 *	@todos	Currently no todos
 *
 *	@section DESCRIPTION
 *	This class implements the command line arguements. A command line arguement has the form:
 * 	--[command] [optional parameter]
 * 	Where '--' indicates a command will follow,
 *	'command' is the command name (e.g., 'pagesize'),
 *	'optional parameter' is an optional parameter only needed for certain commands (e.g., for 'pagesize' an integer
 *is needed)
 */
#pragma once 

#include "argbase.hh"
#include "types.hh"
#include <cstdlib>
#include <string>


class Args {
  public:
    Args();

  public:
    inline bool help() { return _help; }
    inline void help(const bool &x) { _help = x; }
    
    inline bool test() { return _test; }
    inline void test(const bool &x) { _test = x; }

    inline bool install() { return _install; }
    inline void install(const bool &x) { _install = x; }

    inline bool trace() { return _trace; }
    inline void trace(const bool &x) { _trace = x; }

    inline uint segmentIndexPage() { return _segmentIndexPage; }
    inline void segmentIndexPage(const uint &x) { _segmentIndexPage = x; }

    inline uint bufferFrames() { return _noBufFrames; }
    inline void bufferFrames(const uint &x) { _noBufFrames = x; }

    inline const std::string masterPartition() { return _masterPartition; }
    inline void masterPartition(const std::string &x) { _masterPartition = x; }
    
    inline const std::string tracePath() { return _tracePath; }
    inline void tracePath(const std::string &x) { _tracePath = x; }


  private:
    bool _help;
    bool _test;
	bool _install;
    bool _trace;
    uint _segmentIndexPage;
    uint _noBufFrames;
	std::string _masterPartition;
    std::string _tracePath;
};

using argdesc_vt = std::vector<argdescbase_t<Args> *>;

void construct_arg_desc(argdesc_vt &aArgDesc);