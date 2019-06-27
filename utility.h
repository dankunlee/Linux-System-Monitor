#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

//Class for providing various helper functions
class Utility {
    public: 
        static string convertTime(long int input); //takes an input in seconds and converts to hh:mm:ss format
        static string getProgressBar(string percent); //converts a string to a user friendly bar shape
        static ifstream getFileStream(string path); //file reading function: returns a file stream
        static vector<string> getLineVector(string line);   //converts a string line to a string vector
};

string Utility::convertTime(long int input) {
    long hour = input / 3600;
    long minute = (input - (hour*3600)) / 60;
    long second = input%60;
    return to_string(hour) + ":" + to_string(minute) + ":" + to_string(second);
}

// 50 bars makes 0 to 100  --> every 2% is one bar(|)
string Utility::getProgressBar(string percent) {
    size_t found = percent.find("\n");
    if (found != string::npos) percent = percent.substr(0, found);  //remove a new line character if there is one attached

    string bar = "0% ";
    int totalBarSize = 50;
    int currentBarSize = stoi(percent) / 2;
    
    for (int i = 0; i < totalBarSize; i++) {
        if (i <= currentBarSize) bar += "|";    //create bars every 2%
        else bar += " ";    //fill rest of the space with spaces for formatting purpose
    }

    return bar += " " + percent + "/100%";
    // return bar +=" " + percent.substr(0,5) + "/100%";
}

ifstream Utility::getFileStream(string path) {
    ifstream file(path);
    if  (!file) {
        throw runtime_error("Non Existing PID");
    }
    return file;
}

ifstream Utility::getFileStream(string path) {
    ifstream file(path);
    if  (!file) {
        throw runtime_error("Non Existing PID");
    }
    return file;
}

vector<string> Utility::getLineVector(string line) {
    istringstream stringStream(line);   
    istream_iterator<string> iterator(stringStream), end; //slices the string line into words and converts to a vector
    vector<string> values(iterator, end);

    return values;
}

#endif
