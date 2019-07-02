#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "processParser.h"

using namespace std;

class Process {
    private:
        string pid;
        string user;
        string cmd;
        string cpu;
        string mem;
        string upTime;

    public:
        Process(string pid) {
             this->pid = pid;
             this->user = ProcessParser::getProcUser(pid);
             this->cmd = ProcessParser::getCMD(pid);
             this->cpu = ProcessParser::getCPU_Percent(pid);
             this->mem = ProcessParser::getVMsize(pid);
             this->upTime = ProcessParser::getProcUpTime(pid);
        }

        void setPID(string pid) {this->pid = pid;}

        string getPID() const {return this->pid;}
        string getUser() const {return this->user;}
        string getCMD() const {return this->cmd;}
        string getCPU() const {return this->cpu;}
        string getMeM() const {return this->mem;}
        string getUpTime() const {return this->upTime;}

        string getProcess();    //nicely formats pid, user, memory, cpu, cmd and up time into a single string value
};

string Process::getProcess() {
    if(!ProcessParser::isPID_existing(this->pid))
        return "";
    
    this->cpu = ProcessParser::getCPU_Percent(pid);
    this->mem = ProcessParser::getVMsize(pid);
    this->upTime = ProcessParser::getProcUpTime(pid);

    return (this->pid + "   "
                    + this->user
                    + "   "
                    + this->mem.substr(0,5)
                    + "     "
                    + this->cpu.substr(0,5)
                    + "     "
                    + this->upTime.substr(0,5)
                    + "    "
                    + this->cmd.substr(0,30)
                    + "...");
}

#endif
