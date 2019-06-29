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
};

#endif
