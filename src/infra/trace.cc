#include "trace.hh"

Trace::Trace() :
    _logPath(),
    _logStream(),
    _cb(nullptr),
    _init(false)
{}

Trace::~Trace()
{
    if(_cb->trace())
    {
        TRACE("Close log file");
        _logStream.close();
    }
}

void Trace::init(const CB& aCB)
{
    if(!_init)
    {
        _cb = &aCB;
        _logPath = _cb->tracePath();
        _logPath.append("logs/");
        fs::create_directory(_logPath);
        std::time_t lCurrTime = std::time(nullptr);
        std::string lTime = std::string(std::ctime(&lCurrTime));
        _logPath.append(lTime.substr(0, lTime.size() - 1).append(".txt"));
        if(_cb->trace())
        {
            _logStream.open(_logPath.c_str(), std::ofstream::out | std::ofstream::app);
            TRACE("Log file created and open");
        }
        _init = true;
    }
}

void Trace::log(const char* aFileName, const uint aLineNumber, const char* aFunctionName, const std::string& aMessage)
{
    if(_cb->trace())
    {
        std::time_t lCurrTime = std::time(nullptr);
        std::string lTime = std::ctime(&lCurrTime);
        lTime = lTime.substr(0, lTime.size() - 1);
        _logStream << lTime 
            << ": " << aFileName 
            << ", line " << aLineNumber
            << ", " << aFunctionName
            << ": '" << aMessage << "'"
            << std::endl;
    }
}

