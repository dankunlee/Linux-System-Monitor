#ifndef PROCESSCONTAINER_H
#define PROCESSCONTAINER_H

#include <vector>
#include <string>
#include "process.h"
#include "processParser.h"

using namespace std;

class ProcessContainer{
    private:
        vector<Process> list;  
        
    public:
        ProcessContainer() {
            this->updateList();
        }
        void updateList();      //updates the list - vector of Process classes 
        string printList();     //returns all processes' info into a single combined string value
        vector<string> getList();   //returns last 10 processes' info (about pid, user, mem, cpu, uptime, cmd)
};

void ProcessContainer::updateList() {
    vector<string> pidList = ProcessParser::getPID_List();
    this->list.clear();
    for (int i = 0; i < pidList.size(); i++) {
        Process process(pidList[i]);
        this->list.push_back(process); 
    }
}

string ProcessContainer::printList() {
    string connectedList = "";

    for (int i = 0; i < this->list.size(); i++) {
        connectedList += list[i].getProcess();
    }

    return connectedList;
}

vector<string> ProcessContainer::getList() {
    vector<string> processList;
    string process;

    for (int i = this->list.size() - 10; i < this->list.size(); i++) { //iterates only last 10 processes
        process = this->list[i].getProcess();
        processList.push_back(process);
    }

    return processList;
}

#endif