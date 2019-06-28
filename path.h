#ifndef PATH_H
#define PATH_H

#include <string>

using namespace std;

enum CPUStates{ //enum for CPU related functions in processParser.h
	S_USER = 1,
	S_NICE,
	S_SYSTEM,
	S_IDLE,
	S_IOWAIT,
	S_IRQ,
	S_SOFTIRQ,
	S_STEAL,
	S_GUEST,
	S_GUEST_NICE
};

//Class for providing various paths
class Path {
    public: 
        static string basePath() {
            return "/proc/";
        }

        static string cmdPath() {
            return "/cmdline";
        }

        static string statusPath() {
            return "/status";
        }

        static string statPath() {
            return "stat";
        }

        static string upTimePath() {
            return "uptime";
        }

        static string memInfoPath() {
            return "meminfo";
        }

        static string versionPath() {
            return "version";
        }
};

#endif

// Testing
// #include <iostream>
// using namespace std;
// int main() {   
//     cout << "listing: " + Path::basePath() << endl;
//     string command = "ls " + Path::basePath();
//     int n = command.length();  
//     char char_command[n+1];  
//     strcpy(char_command, command.c_str());  
//     system(char_command);
// }
