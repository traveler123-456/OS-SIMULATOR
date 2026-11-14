#include "CommandParser.h"

#include "File.h"
#include "MemoryManagement.h"
#include "ProcessManagement.h"
void
AvilableCommands ()
{
  cout << "Available Commands:\n";

  cout << "\nFile system:\n";
  cout << "  mkdir <name> ...         create folder(s)\n";
  cout << "  cd <name> | cd ..        change directory\n";
  cout << "  ls                       list current directory\n";
  cout << "  pwd                      print current path\n";
  cout << "  touch <name> ...         create empty file(s)\n";
  cout << "  write <file> <text>      write text to a file\n";
  cout << "  cat <file> ...           print file content\n";
  cout << "  rm <file> ...            delete file(s)\n";
  cout << "  rmdir <name> ...         delete empty folder(s)\n";

  cout << "\nProcesses:\n";
  cout << "  newproc <name> <burst> <memory> [arrival]\n";
  cout << "  list | ps                show all processes\n";
  cout << "  info <pid>               show one process\n";
  cout << "  state <pid> <newstate>   new, ready, running, terminated\n";
  cout << "  kill <pid>               remove a process\n";

  cout << "\nScheduling:\n";
  cout << "  schedule rr <quantum>    run Round Robin\n";

  cout << "\nMemory:\n";
  cout << "  mem                      show memory map\n";
  cout << "  alloc <pid>              allocate memory (first fit)\n";
  cout << "  free <pid>               release memory\n";

  cout << "\nOther:\n";
  cout << "  help                     show this list\n";
  cout << "  exit                     quit the simulator\n";
}

typedef enum { PROCESS_CMD_OK, PROCESS_CMD_ERR } IS_PROCESS_CMD;

IS_PROCESS_CMD isprocess(string cmd) {
    if (cmd == "newproc" || cmd == "list" || cmd == "ps" || cmd == "kill" || cmd == "info" ||
        cmd == "state" || cmd == "schedule") {
        return PROCESS_CMD_OK;
    }

    return PROCESS_CMD_ERR;
}

bool CommandParser::execute(string command) {
    stringstream ss(command);
    string cmd;
    ss >> cmd;

    if (cmd.empty()) return true;

    vector<string> argument;
    string arg;
    while (ss >> arg) { argument.push_back(arg); }
    if (cmd == "help") {
        AvilableCommands();
        return true;
    } else if (cmd == "mkdir") {
        if (argument.size() == 0) {
            cout << "mkdir: missing operand\n";
            return true;
        }
        for (string& x : argument) {
            if (fs->mkdir(x)) {
                continue;
            } else {
                cout << "ERROR : DIRECTORY ALREADY EXISTS\n";
                return true;
            }
        }
        return true;
    } else if (cmd == "ls") {
        if (argument.size() != 0) {
            for (int i = 0; i < argument.size(); i++) {
                cout << "ls: cannot access '";
                cout << argument[i];
                cout << "': No such file or directory\n";
            }
            return true;
        } else {
            fs->ls();
        }
        return true;
    } else if (cmd == "pwd") {
        fs->pwd();
        return true;
    } else if (cmd == "cd") {
        if (argument.size() > 1) {
            cout << "bash: cd: too many arguments\n";
            return true;
        } else if (argument.size() == 0) {
            fs->current = fs->root;
        } else {
            fs->cd(argument[0]);
        }
        return true;
    } else if (cmd == "touch" || cmd == "write" || cmd == "cat" || cmd == "rm" || cmd == "rmdir") {
        FileCommands(cmd, argument, fs->current);
        return true;
    } else if (cmd == "mem" || cmd == "alloc" || cmd == "free") {
        MemoryManagement(cmd, argument);
        return true;
    } else if (!isprocess(cmd)) {
        ProcessManagement(cmd, argument);
        return true;
    } else if (cmd == "exit") {
        return false;
    } else {
        cout << "bash: " << cmd << ": command not found\n";
        return true;
    }
}
