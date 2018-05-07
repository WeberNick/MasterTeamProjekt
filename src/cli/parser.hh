/**
 * @file   parser.hh
 * @author Nicolas Wipfler (nwipfler@mail.uni-mannheim.de)
 * @brief  Class implementing the parser for the command line interface
 * @bugs   TBD
 * @todos  TBD
 */
#pragma once

#include "../infra/linereaderedit.hh"
#include "../infra/types.hh"
//#include "../main/db_instance_manager.hh"
//#include "../partition/partition_manager.hh"
//#include "../segment/segment_manager.hh"

class CommandParser;
using CP = CommandParser;

class CommandParser {
  private:
    class Command {
        public:
            explicit Command(
                const CP& aCP, 
                const char* aName, 
                const bool aHasParams, 
                const size_t aCommandLength, 
                const size_t aNumParams, 
                int (CP::*aFunc)(const char_vpt&) const,
                const char* aMsg);
            Command& operator=(const Command& aCmd);
        public:
            const CP& _cp;
            const char* _name;
            bool _hasParams;
            size_t _comLength;
            size_t _numParams;
            int (CP::*_func)(const char_vpt&) const;
            const char* _helpMsg;
    };

    using CMD = Command;

  public:
    explicit CommandParser();
    explicit CommandParser(const CommandParser&) = delete;
    explicit CommandParser(CommandParser&&) = delete;
    CommandParser& operator=(const CommandParser&) = delete;
    CommandParser& operator=(CommandParser&&) = delete;
    ~CommandParser();

  private:
    void runcli();
    CMD* findCommand(const std::vector<char*>& splits);
    void printw() const;
    void printh() const;
    void printe() const;
    void close();

  private:
     int com_help(const char_vpt& args) const;
     int com_exit(const char_vpt& args) const;
     int com_create_p(const char_vpt& args) const;
     int com_drop_p(const char_vpt& args) const;
     int com_create_s(const char_vpt& args) const;
     int com_drop_s(const char_vpt& args) const;
     int com_show_p(const char_vpt& args) const;
     int com_show_s(const char_vpt& args) const;

  public:
    static CommandParser& getInstance() {
        static CommandParser lComPars;
        return lComPars;
    }

    void init(const CB& aControlBlock, const char* aPrompt = "mdb > ", const char aCommentChar = '#');

  private:
    std::vector<Command> _commands;
    LineReaderEdit _reader;

    size_t _maxCommandLength;
    bool _exit;
    bool _init;
    const CB* _cb;
};

