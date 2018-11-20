#include <iostream>
#include <fstream>
#include <queue>
#include <algorithm>
#include <map>

using namespace std;

struct process{
    int priority;
    string name;
    string codeFile;
    int arrivalTime;
    int wereLeftAt;

    process(){
        name = "";
        codeFile = "";
        priority = 0;
        arrivalTime = 0;
        wereLeftAt = 1;
    }

    string toString(){
        return "Process: " + name + " file: " + codeFile
               + " priority:" + to_string(priority) + " arrival: " + to_string(this->arrivalTime);
    }
    bool operator<(const process & rhs) const{
        return this->priority < rhs.priority;
    }
};
struct program{
    int index; // code(index).txt
    int numberOfInstructions;
    vector<int> instructionExecutionTimes;
};



int main() {

    int currentTime = 0;

    vector<int> arrivalTimes;
    priority_queue<process> readyQueue;
    vector<process> timeLine;
    vector<program> programs;
    map<string,int> turnaroundTimes;
    map<string,int> waitingTimes;


    // DEFINITION FILE PARSING ----------
    ifstream def;
    def.open("./Provided Files/definition.txt");
    if (def.is_open()){
        while(!def.eof()){ // while end of file is not reached
            process newProcess;

            def >> newProcess.name;
            def >> newProcess.priority;
            def >> newProcess.codeFile;
            def >> newProcess.arrivalTime;

            //cout << newProcess.toString() << endl;
            timeLine.push_back(newProcess);
        }
    }else{
        cout << "Unable to open definition.txt file." << endl;
    }
    // CODE FILES PARSING ----------
    for(int i=1;i<5;i++) { // code 1 to code 4
        ifstream codeFile;
        program newProgram = {};
        codeFile.open("./Provided Files/code" + to_string(i) + ".txt");
        if (codeFile.is_open()) {
            int lineCounter = 0; // Used to count number of lines in the file except exit
            while(!codeFile.eof()){ // while end of file is not reached
                string instructionName;
                int executionTime; // Time needed to execute next instruction
                codeFile >> instructionName; // this is not important for our calculations won't be used
                codeFile >> executionTime;
                lineCounter++;
                newProgram.instructionExecutionTimes.push_back(executionTime);
            }
            newProgram.numberOfInstructions = lineCounter - 1;
            newProgram.index = i;
            programs.push_back(newProgram);
        }
        else{
            cout << "Unable to open code" + to_string(i) + ".txt file." << endl;
        }
    }
    // SCHEDULING
    sort(timeLine.begin(),timeLine.end(),[](const process & a, const process & b){
        return a.arrivalTime > b.arrivalTime;
    });
    /* In above statement sort timeline according to arrival time because we don't know
     * definition file is sorted according to arrival times of processes.*/

    int lastComingProcessArrival = timeLine.back().arrivalTime;

    // terminating condition: when ready queue is empty and every process has arrived.

    // for convinience seperate vector just including arrival times of processes
    // these are the critical times that scheduler get a interrupt.
    for(auto & process : timeLine){
        arrivalTimes.push_back(process.arrivalTime);
    }


    while(currentTime < lastComingProcessArrival || !readyQueue.empty()){
        if(readyQueue.empty()){
            int nextTime = timeLine.back().arrivalTime;
            while(timeLine.back().arrivalTime == nextTime){ // after reaching a first arriving process or processes'
                // arrival times remove every process arrived at that time. Add them to ready queue.
                process nextProcess = timeLine.back();
                timeLine.pop_back();
                readyQueue.push(nextProcess);
            }
            currentTime = nextTime; // move that time immediately as this is the first arriving process
            arrivalTimes.pop_back(); // remove first critical time that we need to be careful
        }else{
            process enteringCPU = readyQueue.top();
            int indexOfCode = stoi(""+enteringCPU.codeFile.back());
            int wereLeftAt = enteringCPU.wereLeftAt;
            int numOfInstructions = programs.at(indexOfCode).numberOfInstructions;
            vector<int> executionTimes = programs.at(indexOfCode).instructionExecutionTimes;

            bool thereIsAProcessLeaving = false; // will be true if a process leaves ready queue

            // Execute instructions until a process leaves ready queue
            while(!thereIsAProcessLeaving){
                int executionTime = executionTimes.at(wereLeftAt-1);
                // execute this statement
                currentTime += executionTime;
                wereLeftAt++;

            }


        }

    }

    for(auto & process : timeLine){
        cout << process.toString() << endl;
    }

    return 0;

}
