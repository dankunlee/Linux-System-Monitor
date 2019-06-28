#ifndef PATH_H
#define PATH_H

#include <string>

using namespace std;

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
