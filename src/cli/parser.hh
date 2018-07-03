/**
 * @file   parser.hh
 * @author Nicolas Wipfler (nwipfler@mail.uni-mannheim.de)
 * @brief  Class implementing the parser for the command line interface
 * @bugs   TBD
 * @todos  TBD
 */
#pragma once

#include "linereaderedit.hh"
#include "../infra/types.hh"
#include "../infra/partition_t.hh"
#include "../infra/segment_t.hh"
#include "../infra/employee_t.hh"
#include "../infra/exception.hh"

#include "../main/db_instance_manager.hh"
#include "../partition/partition_manager.hh"
#include "../segment/segment_manager.hh"
#include "../threading/pool.hh"

#include <array>

class CommandParser;
using CP = CommandParser;

class CommandParser {
  private:
    class Command {
      public:
        explicit Command(const CP& aCP,
                         const char* aName,
                         const bool aHasParams,
                         const size_t aCommandLength,
                         const size_t aNumParams,
                         int (CP::*aFunc)(const char_vpt*) const,
                         const char* aMsg,
                         const char* aUsageInfo);
        Command& operator=(const Command& aCommand);

      public:                                    //          Example
        const CP& _cp;                           //                            - Reference to the Command Parser instance
        const char* _name;                       // "CREATE SEGMENT"           - The name of the command
        bool _hasParams;                         // true                       - Whether the command has parameters
        size_t _comLength;                       // 2                          - The number of words the command consists of (CREATE and SEGMENT)
        size_t _numParams;                       // 2                          - The number of parameters
        int (CP::*_func)(const char_vpt*) const; // com_create_s               - A function pointer implementing the logic of this command
        const char* _helpMsg;                    // "Create .."                - A help message
        const char* _usageInfo;                  // "Usage - str: partname .." - A detailed message on how to use the command
    };

    enum CommandStatus {
        EXIT  = -1, // regular exit
        OK    = 0,  // ok, continue with next command
        WRONG = 1,  // wrong type of an argument
        ERROR = 2   // error, recover and continue
    };

  public:
    explicit CommandParser();
    explicit CommandParser(const CommandParser&) = delete;
    explicit CommandParser(CommandParser&&) = delete;
    CommandParser& operator=(const CommandParser&) = delete;
    CommandParser& operator=(CommandParser&&) = delete;
    ~CommandParser() = default;

  private:
    void runcli();
    const Command* findCommand(const char_vpt* splits);
    std::string findCommand(const std::string& arg) const;

    void printw() const;
    void printh() const;
    void printe() const;
    void pprints(const std::string& caption, const string_vt& list) const;
    /**
     * @brief Print one line of + of size length+2
     * 
     * @param length the length which determines the printed line
     */
    void printp(uint8_t length) const;

  private:
    int com_help(const char_vpt* args) const;
    int com_create_p(const char_vpt* args) const;
    int com_drop_p(const char_vpt* args) const;
    int com_create_s(const char_vpt* args) const;
    int com_drop_s(const char_vpt* args) const;
    int com_insert_tuple(const char_vpt* args) const;
    int com_show_part(const char_vpt* args) const;
    int com_show_parts(const char_vpt* args) const;
    int com_show_seg(const char_vpt* args) const;
    int com_show_segs(const char_vpt* args) const;
    int com_exit(const char_vpt* args) const;

  public:
    static CommandParser& getInstance() {
        static CommandParser lComPars;
        return lComPars;
    }

    void init(const CB& aControlBlock, const char* aPrompt = "mdb > ", const char aCommentChar = '#');
    void multiexec(const string_vt& commands);

  private:
    static const char* _HELP_FLAG;
    const std::array<const Command, 11> _commands;
    size_t _maxCommandLength;

    LineReaderEdit _reader;
    const CB* _cb;
};
