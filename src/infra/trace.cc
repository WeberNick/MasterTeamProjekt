#include "trace.hh"

Trace::Trace() :
    _logPath(),
    _logStream(),
    _init(false)
{}

Trace::~Trace()
{
    _logStream.close();
}

void Trace::init(const CB& aCB)
{
    if(!_init)
    {
        _logPath = aCB.tracePath();
        _logStream.open(_logPath.c_str(), std::ofstream::out | std::ofstream::app);
    }
}

void Trace::log(const char* aFileName, const uint aLineNumber, const char* aFunctionName, const std::string& aMessage)
{
    std::time_t lCurrTime = std::time(nullptr);
    _logStream << std::asctime(std::localtime(&lCurrTime))
        << ": " << aFileName 
        << ", line " << aLineNumber
        << ", " << aFunctionName
        << ": '" << aMessage << "'"
        << std::endl;
}

