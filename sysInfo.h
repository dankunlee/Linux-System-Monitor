#ifndef SYSINFO_H
#define SYSINFO_H

#include <string>
#include <vector>
#include "processParser.h"

using namespace std;

class SysInfo {
    private:
        vector<string> lastCPU_stats;       //currentCPU_stats values will be stored here whenever it gets updated  
        vector<string> currentCPU_stats;    //vector of the cpu info for a particular cpu
        vector<string> coresStats;          //vector of a core's usages (in percentage)
        vector<vector<string>> lastCPU_coresStats;      //currentCPU_coresStats values will be stored here whenever it gets updated  
        vector<vector<string>> currentCPU_coresStats;   //2-D vector of every core with its usage in percentage

        string cpuPercent;
        float memPercent;
        string osName;
        string kernelVer;
        long upTime;
        int totalProc;
        int runningProc;
        int threads;

    public:
        SysInfo() {
            this->setLastCPU_measures();    //needs to be called before setAttributes() to initializes lastCPU_stats
            this->setOtherCores(ProcessParser::getNumbCores());     //needs to be called before setAttributes() to initializes lastCPU_coresStats
            this->setAttributes();

            this-> osName = ProcessParser::getOS_name();
            this-> kernelVer = ProcessParser::getSysKernelVersion();
        }

        //SETTERS
        void setAttributes();           //initializes or updates basic attributes
        void setCPU_coresStats();       //updates and creates new datasets for CPU calculation
        void setLastCPU_measures();     //initializes or updates current cpu info into last cpu info
        void setOtherCores(int size);   //initializes or updates other attributes (vector objects) with given size (number of cores)
                                        //and sets previous data for a specific core

        //GETTERS
        string getMemPercent() const;
        long getUpTime() const;
        string getThreads() const;
        string getTotalProc() const;
        string getRunningProc() const;
        string getKernelVersion() const;
        string getOS_name() const;
        string getCPU_percent() const;

        vector<string> getCoresStats() const;   //creates a string that represents a progress bar showing current CPU utilization
};

//SETTERS for the constructor
void SysInfo::setAttributes() {
    this->memPercent = ProcessParser::getSysRAM_percent();
    this->upTime = ProcessParser::getSysUpTime();
    this->totalProc = ProcessParser::getTotalProcesses();
    this->runningProc = ProcessParser::getTotalRunningProcesses();
    this->threads = ProcessParser::getTotalThreads();
    this->currentCPU_stats = ProcessParser::getSysCPU_percent();
    this->cpuPercent = ProcessParser::printCPU_stats(this->lastCPU_stats, this->currentCPU_stats);
    this->lastCPU_stats = this->currentCPU_stats;
    this->setCPU_coresStats();
}

void SysInfo::setCPU_coresStats() { //every core is updated and previous data becomes the current data of calculated measures
    for(int i=0; i < this->currentCPU_coresStats.size(); i++){
        this->currentCPU_coresStats[i] = ProcessParser::getSysCPU_percent(to_string(i)); //gets the data from files
    }

    for(int i=0; i < this->currentCPU_coresStats.size();i++){
        this->coresStats[i] = ProcessParser::printCPU_stats(this->lastCPU_coresStats[i],this->currentCPU_coresStats[i]);    //calculates every core percentage of usage
    }

    this->lastCPU_coresStats = this->currentCPU_coresStats;
}

void SysInfo::setLastCPU_measures() {
    this->lastCPU_stats = ProcessParser::getSysCPU_percent();
}

void SysInfo::setOtherCores(int size) {
    this->coresStats = vector<string>();
    this->coresStats.resize(size);
    this->lastCPU_coresStats = vector<vector<string>>();
    this->lastCPU_coresStats.resize(size);
    this->currentCPU_coresStats = vector<vector<string>>();
    this->currentCPU_coresStats.resize(size);
    
    for (int i = 0; i < size; i++) {
        this->lastCPU_coresStats[i] = ProcessParser::getSysCPU_percent(to_string(i));
    }
}

//GETTERS - return values defined from setAttributes function
string SysInfo::getMemPercent() const {
    return to_string(this->memPercent);
}

long SysInfo::getUpTime() const {
    return this->upTime;
}

string SysInfo::getTotalProc() const {
    return to_string(this->totalProc);
}

string SysInfo::getRunningProc() const {
    return to_string(this->runningProc);
}

string SysInfo::getThreads() const {
    return to_string(this->threads);
}

string SysInfo::getCPU_percent() const {
    return this->cpuPercent;
}

//GETTERS - return values defined from the constructor (apart from setAttributes function)
string SysInfo::getKernelVersion() const {
    return this->kernelVer;
}

string SysInfo::getOS_name() const {
    return this->osName;
}

vector<string> SysInfo::getCoresStats() const {
    vector<string> progressBar;
    for (int i = 0; i < this->coresStats.size(); i++) {
        string status = "CPU" + to_string(i) + ": ";
        float check = stof(this->coresStats[i]);

        if (!check || this->coresStats[i] == "nan") {
            return vector<string>();
        }

        status += Utility::getProgressBar(this->coresStats[i]);
        progressBar.push_back(status);
    }

    return progressBar;
}

#endif
