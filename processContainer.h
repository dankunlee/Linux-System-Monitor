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
        vector<vector<string>> getList();   //returns last 10 processes' info (about pid, user, mem, cpu, uptime, cmd)
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

vector<vector<string>> ProcessContainer::getList() {
    vector<vector<string>> values;
    vector<string> processList;

    for(int i=0; i < this->list.size(); i++){
        processList.push_back(this->list[i].getProcess());
    }

    int lastIndex = 0;
    for (int i=0; i < processList.size(); i++){
        if(i %10 == 0 && i > 0){
          vector<string>  temp(&processList[i-10], &processList[i]);
          values.push_back(temp);
          lastIndex = i;
        }

        if(i == (this->list.size() - 1) && (i-lastIndex)<10){
            vector<string> sub(&processList[lastIndex],&processList[i+1]);
            values.push_back(sub);
        }
   }
    return values;
}

#endif
