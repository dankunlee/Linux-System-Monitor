#ifndef PROCESSPARSER_H
#define PROCESSPARSER_H

#include <string>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <dirent.h>
#include <cstring>
#include <algorithm>
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
        static vector<string> getSysCPU_percent(string coreNumb = "");   //gets the cpu info for a particular cpu
        static float getSysActiveCPU_time(vector<string> values);   //gets the active time for a particular cpu
        static float getSysIdleCPU_time(vector<string> values);     //gets the idle time for a particular cpu
        static string printCPU_stats(vector<string> values1, vector<string> values2);  //gets the overall cpu usage
        static float getSysRAM_percent();           //gets the total usage of RAM
        static string getSysKernelVersion();        //gets the version of the kernel
        static string getOS_name();                 //gets the name of the OS
        static int getTotalThreads();               //gets the total number of threads of every processes
        static int getTotalProcesses();             //gets the total number of processes
        static int getTotalRunningProcesses();      //gets the total number of currently running processes
};

string ProcessParser::getVMsize(string pid) {
    string line;
    string name = "VmData";

    ifstream lineStream = Utility::getFileStream(Path::basePath() + pid + Path::statusPath()); //gets file stream for /proc/<pid>/status
    while (getline(lineStream, line)) { //reads line by line
        if (line.compare(0, name.size(), name) == 0) { //searches for the line with "VmData"
            // istringstream stringStream(line);   //the line will have a format like: "VmData:        188 kB"
            // istream_iterator<string> iterator(stringStream), end; //slices the string line into words and converts to a vector
            // vector<string> values(iterator, end);
            vector<string> values = Utility::getLineVector(line);

            return to_string(stof(values[1]) / float(1024*1024)); //converts kB to GB and to string
        }
    }

    return "-1"; //in case of error
}

string ProcessParser::getProcUpTime(string pid) {
    string line;
    ifstream lineStream = Utility::getFileStream(Path::basePath() + pid + "/" + Path::statPath());
    getline(lineStream, line);
    vector<string> values = Utility::getLineVector(line);
    return to_string(float(stof(values[13])/sysconf(_SC_CLK_TCK)));
}

long int ProcessParser::getSysUpTime() {
    string line;
    ifstream lineStream = Utility::getFileStream(Path::basePath() + Path::upTimePath());
    getline(lineStream, line);
    vector<string> values = Utility::getLineVector(line);
    return stoi(values[0]);
}

string ProcessParser::getCPU_Percent(string pid) {
    string line;

    ifstream lineStream = Utility::getFileStream(Path::basePath() + pid + "/" + Path::statPath()); //gets file stream for /proc/<pid>/stat
    getline(lineStream, line);  //the line will have a format like: "1 (sh) S 0 1 1 0 -1 4194560 752 0 15 0 3 0 0 0 20 0 1 0 2140 4612096 199 18446744073709551615 ... "
    // istringstream stringStream(line);   
    // istream_iterator<string> iterator(stringStream), end; //slices the string line into words and converts to a vector
    // vector<string> values(iterator, end);
    vector<string> values = Utility::getLineVector(line);

    float uTime = stof(ProcessParser::getProcUpTime(pid));  //time a process has been scheduled in user mode
    float sTime = stof(values[14]);     //time a process has been scheduled in kernel mode
    float cuTime = stof(values[15]);    //time a process's waited-for children have been scheduled in user mode
    float csTime = stof(values[6]);     //time a process's waited-for children have been scheduled in kernel mode
    float startTime = stof(values[21]); //time a process started after system boot
    float upTime = ProcessParser::getSysUpTime(); //time since the system was started
    float freq = sysconf(_SC_CLK_TCK); //number of clock ticks per second

    float totalTime = uTime + sTime + cuTime + csTime;
    float seconds = upTime - startTime/freq;
    float cpuPercent = 100*(totalTime/freq)/seconds;

    return to_string(cpuPercent);
}

string ProcessParser::getProcUser(string pid) {
    string line;
    string name = "Uid";
    string userID;
    string userName;

    ifstream lineStream = Utility::getFileStream(Path::basePath() + pid + Path::statusPath()); //gets file stream for /proc/<pid>/status
    while (getline(lineStream, line)) { //reads line by line
        if (line.compare(0, name.size(), name) == 0) { //searches for the line with "Uid"
            vector<string> values = Utility::getLineVector(line);   //the user id line will have a format like "Uid:      1000         1000         1000         1000"
            userID = values[1];
            break;
        }
    }

    lineStream = Utility::getFileStream("/etc/passwd"); //user name can be looked up using the user id retrieved from above
    name = "x:" + userID;
    while (getline(lineStream, line)) { //reads line by line
        if (line.find(name) != string::npos) { //if the line contains "x: <userID>" <-- the line will have a format like "chronic:x:1000:1000::/home...."
            userName = line.substr(0, line.find(":"));  //user name is the letters up to the first appearance of a colon
            return userName;
        }
    }
    return "";  //in case there is no user name found 
}

vector<string> ProcessParser::getPID_List() {
    vector<string> pidList;
    DIR *dir;
    if (!(dir = opendir("/proc"))) {    //in case opening "/proc" fails
        throw runtime_error(strerror(errno));
    }

    while(dirent *dirP = readdir(dir)) {
        if (dirP->d_type == DT_DIR) {   //check if each item in "/proc" is a directoy
            if (all_of(dirP->d_name, dirP->d_name + strlen(dirP->d_name), [](char c) {return isdigit(c);})) {   //if the direcotry name in "/proc" is composed of digits  
                pidList.push_back(dirP->d_name);                                                                //adds to the vector
            }
        }
    }

    if (closedir(dir)) {    //throw error if closing the directory fails
        throw runtime_error(strerror(errno));
    }

    return pidList;
}

string ProcessParser::getCMD(string pid) {
    string line;
    ifstream lineStream = Utility::getFileStream(Path::basePath() + pid + Path::cmdPath());
    getline(lineStream, line);
    return line;
}

int ProcessParser::getNumbCores() {
    string line;
    string name = "cpu cores";
    string cpuNumb;

    ifstream lineStream = Utility::getFileStream(Path::basePath() + "cpuinfo"); //gets file stream for /proc/cpuinfo
    while (getline(lineStream, line)) { //reads line by line
        if (line.compare(0, name.size(), name) == 0) { //searches for the line with "cpu cores"
            vector<string> values = Utility::getLineVector(line);   //the line will have a format like "cpu cores : 1"
            cpuNumb = values[3];
            break;
        }
    }

    return stoi(cpuNumb);
}

vector<string> ProcessParser::getSysCPU_percent(string coreNumb) {
    string line;
    string name = "cpu" + coreNumb;
    
    ifstream lineStream = Utility::getFileStream(Path::basePath() + Path::statPath());
    while (getline(lineStream, line)) { //reads line by line
        if (line.compare(0, name.size(), name) == 0) { //searches for the line with "cpu" + coreNumb
            vector<string> values = Utility::getLineVector(line);   //the line will have a format like "cpu cores : 1"
            return values;
        }
    }

    return vector<string> ();
}

float ProcessParser::getSysActiveCPU_time(vector<string> values) {
    return (stof(values[S_USER]) +
            stof(values[S_NICE]) +
            stof(values[S_SYSTEM]) +
            stof(values[S_IRQ]) +
            stof(values[S_SOFTIRQ]) +
            stof(values[S_STEAL]) +
            stof(values[S_GUEST]) +
            stof(values[S_GUEST_NICE]));
}

float ProcessParser::getSysIdleCPU_time(vector<string> values){
    return (stof(values[S_IDLE]) + stof(values[S_IOWAIT]));
}

string ProcessParser::printCPU_stats(vector<string> values1, vector<string> values2) {
    float activeTime = getSysActiveCPU_time(values2) - getSysActiveCPU_time(values1);
    float idleTIme = getSysIdleCPU_time(values2) - getSysIdleCPU_time(values1);
    float totalTime = activeTime + idleTIme;
    float cpuUsage = (activeTime / totalTime) * 100.0;
    return to_string(cpuUsage);
}

float ProcessParser::getSysRAM_percent() {
    string line;
    string nameMemFree = "MemFree";
    string nameMemAvailalbe = "MemAvailable";
    string nameBuffers = "Buffers";
    float memFree, memAvailable, buffers;
    vector<string> values;
    int count = 0;

    ifstream lineStream = Utility::getFileStream(Path::basePath() + Path::memInfoPath());
    while(getline(lineStream, line)) {
        if (count == 3) break;

        if (line.compare(0, nameMemFree.size(), nameMemFree) == 0) {
            values = Utility::getLineVector(line); 
            memFree = stof(values[1]);
            count += 1;
        }

        else if (line.compare(0, nameMemAvailalbe.size(), nameMemAvailalbe) == 0) {
            values = Utility::getLineVector(line); 
            memAvailable = stof(values[1]);
            count += 1;
        }

        else if (line.compare(0, nameBuffers.size(), nameBuffers) == 0) {
            values = Utility::getLineVector(line); 
            buffers = stof(values[1]);
            count += 1;
        }
    }

    return float(100.0 * (1 - (memFree / (memAvailable - buffers))));
}

string ProcessParser::getSysKernelVersion() {
    string line;
    string name = "Linux version";
    string version = "";

    ifstream lineStream = Utility::getFileStream(Path::basePath() + Path::versionPath());
    while (getline(lineStream, line)) {
        if (line.compare(0, name.size(), name) == 0) {
            vector<string> values = Utility::getLineVector(line);
            version = values[2];
            break;
        }
    }

    return version;
}

string ProcessParser::getOS_name() {
    string line;
    string name = "PRETTY_NAME";
    string osName = "";

    ifstream lineStream = Utility::getFileStream("/etc/os-release");
    while (getline(lineStream, line)) {
        if (line.compare(0, name.size(), name) == 0) {
            osName = line.substr(name.size() + 2,line.size() - name.size() -3);
            break;
        }
    }

    return osName;
}

int ProcessParser::getTotalThreads() {
    vector<string> pidList = ProcessParser::getPID_List();
    string name = "Threads";
    ifstream lineStream;
    string line;
    vector<string> value;
    int thread;
    int totalThreads = 0;

    for (string pid:pidList) {  //for every pid in the vector list, find thread values and add them up
        lineStream = Utility::getFileStream(Path::basePath() + pid + Path::statusPath());
        while (getline(lineStream, line)) {
            if (line.compare(0, name.size(), name) == 0) {
                value = Utility::getLineVector(line);
                thread = stoi(value[1]);
                totalThreads += thread;
                break;
            }
        }
    }

    return totalThreads;
}

int ProcessParser::getTotalProcesses() {
    string line;
    string name = "processes";

    ifstream lineStream = Utility::getFileStream(Path::basePath() + Path::statPath());
    while (getline(lineStream, line)) {
        if (line.compare(0, name.size(), name) == 0) {
            vector<string> values = Utility::getLineVector(line);
            return stoi(values[1]);
        }
    }

    return 0;
}

int ProcessParser::getTotalRunningProcesses() {
    string line;
    string name = "procs_running";

    ifstream lineStream = Utility::getFileStream(Path::basePath() + Path::statPath());
    while (getline(lineStream, line)) {
        if (line.compare(0, name.size(), name) == 0) {
            vector<string> values = Utility::getLineVector(line);
            return stoi(values[1]);
        }
    }

    return 0;
}

#endif
