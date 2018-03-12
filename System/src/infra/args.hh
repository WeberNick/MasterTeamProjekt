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
 
#ifndef INFRA_ARGS_HH
#define INFRA_ARGS_HH

#include "argbase.hh"
#include "types.hh"
#include <cstdlib>
#include <string>


class Args {
  public:
    Args();

  public:
    inline const bool help() { return _help; }
    inline void help(const bool &x) { _help = x; }

    inline const bool nsm() { return _nsm; }
    inline void nsm(const bool &x) { _nsm = x; }

    inline const bool pax() { return _pax; }
    inline void pax(const bool &x) { _pax = x; }

    inline const uint core() { return _core; }
    inline void core(const uint &x) { _core = x; }

    inline const uint pageSize() { return _pageSize; }
    inline void pageSize(const uint &x) { _pageSize = x; }

    inline const uint alignment() { return _alignment; }
    inline void alignment(const uint &x) { _alignment = x; }

    inline const bool test() { return _test; }
    inline void test(const bool &x) { _test = x; }

    inline const bool install() { return _install; }
    inline void install(const bool &x) { _install = x; }

    inline const bool print() { return _print; }
    inline void print(const bool &x) { _print = x; }

    inline const bool trace() { return _trace; }
    inline void trace(const bool &x) { _trace = x; }

    inline const uint runs() { return _runs; }
    inline void runs(const uint &x) { _runs = x; }

    inline const std::string path() { return _path; }
    inline void path(const std::string &x) { _path = x; }

    inline const std::string masterPartition() { return _masterPartition; }
    inline void masterPartition(const std::string &x) { _masterPartition = x; }

    inline const uint segmentIndexPage() { return _segmentIndexPage; }
    inline void segmentIndexPage(const uint &x) { _segmentIndexPage = x; }

    inline const uint partitionSizeInPages() { return _partitionSize; }
    inline void partitionSizeInPages(const uint &x) { _partitionSize = x; }

    inline const uint growthIndicator() { return _growthIndicator; }
    inline void growthIndicator(const uint &x) { _growthIndicator = x; }

  private:
    bool _help;
    bool _nsm;
    bool _pax;
    bool _test;
	bool _install;
    bool _print;
    bool _trace;
    uint _core;
    uint _pageSize;
    uint _alignment;
    uint _runs;
    std::string _path;
	std::string _masterPartition;
    uint _segmentIndexPage;
    uint _partitionSize;
    uint _growthIndicator;
};

typedef std::vector<argdescbase_t<Args> *> argdesc_vt;

void construct_arg_desc(argdesc_vt &aArgDesc);

#endif
