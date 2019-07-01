#ifndef PROCESSPARSER_H
#define PROCESSPARSER_H

#include <string>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <dirent.h>
#include "path.h"
#include "utility.h"

using namespace std;

class ProcessParser {
    public:
        static string getVMsize(string pid);        //gets the size of Virtual Memory (RAM usage) of a process in GB
        static string getProcUpTime(string pid);    //gets the time a process has been scheduled in user mode
        static long int getSysUpTime();             //gets the time since the system was started
        static string getCPU_Percent(string pid);   //gets the percentage of usage of CPU of a process
        static string getProcUser(string pid);      //gets the username of a process
        static vector<string> getPID_List();        //gets the list of PIDs in a vector
        static string getCMD(string pid);           //gets the command that executed the process
        static int getNumbCores();                  //gets the number of CPU cores
        static vector<string> getSysCPU_percent(string coreNumb = "");   //gets the cpu info for particular cpu
        float getSysActiveCPU_time(vector<string> values);          //gets the active time for a particular cpu
        float getSysIdleCPU_time(vector<string> values);            //gets the idle time for a particular cpu
        string printCPU_stats(vector<string> values1, vector<string> values2);  //gets the overall cpu usage
};

#endif
