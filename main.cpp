#include <iostream>
#include <fstream>
#include <queue>
#include <algorithm>
#include <map>

using namespace std;
ofstream outputFile("output.txt");
map<string,int> comingOrder; // process name and order

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

    bool operator <(const process & rhs) const{
        if(this->priority == rhs.priority){
            return comingOrder[this->name] > comingOrder[rhs.name];
        }
        return this->priority > rhs.priority;
    }
};
struct program{
    int index;
    int numberOfInstructions;
    vector<int> instructionExecutionTimes;
};
void printQueue(priority_queue<process> queue){
    string statusOfReadyQueue;
    statusOfReadyQueue.append("HEAD-");

    if(queue.empty()){
        statusOfReadyQueue = "HEAD--TAIL";
        outputFile << statusOfReadyQueue << endl;
        return;
    }
    while(!queue.empty()){
        process topProcess = queue.top();
        statusOfReadyQueue.append(topProcess.name + "[" + to_string(topProcess.wereLeftAt) + "]-");
        queue.pop();
    }

    statusOfReadyQueue.append("TAIL");
    outputFile << statusOfReadyQueue << endl;
}


int main() {

    int currentTime = 0; // current time
    vector<int> arrivalTimes; // arrival times of process just like in timeLine, for simplicity
    priority_queue<process> readyQueue; // ready queue of the os
    vector<process> timeLine; // processes as they are coming first as a timeline
    vector<program> programs; // programs => code files
    map<string,int> turnaroundTimes; // process name and turnaround time
    map<string,int> waitingTimes; // process name and waiting time

    // DEFINITION FILE PARSING -------------------------------------------
    ifstream def;
    def.open("definition.txt");
    if (def.is_open()){
        while(!def.eof()){ // while end of file is not reached
            process newProcess;

            def >> newProcess.name;
            def >> newProcess.priority;
            def >> newProcess.codeFile;
            def >> newProcess.arrivalTime;

            timeLine.push_back(newProcess);
        }
    }else{
        cout << "Unable to open definition.txt file." << endl;
    }
    // -------------------------------------------
    // CODE FILES PARSING -------------------------------------------
    for(int i=1;i<5;i++) { // code 1 to code 4
        ifstream codeFile;
        program newProgram = {};
        codeFile.open("code" + to_string(i) + ".txt");
        if (codeFile.is_open()) {
            int lineCounter = 0; // Used to count number of lines in the file including exit
            while(!codeFile.eof()){ // while end of file is not reached
                string instructionName;
                int executionTime; // Time needed to execute next instruction
                codeFile >> instructionName; // this is not important for our calculations won't be used
                codeFile >> executionTime;
                lineCounter++;
                newProgram.instructionExecutionTimes.push_back(executionTime);
            }
            newProgram.numberOfInstructions = lineCounter;
            newProgram.index = i;
            programs.push_back(newProgram);
        }
        else{
            cout << "Unable to open code" + to_string(i) + ".txt file." << endl;
        }
    }
    // -------------------------------------------
    // SCHEDULING -------------------------------------------
    /* In below statement sort timeline according to arrival time because we don't know
    * definition file is sorted according to arrival times of processes.*/
    sort(timeLine.begin(),timeLine.end(),[](const process & a, const process & b){
        return a.arrivalTime > b.arrivalTime;
    });

    int counter = 1; // order number
    // populate comingOrder vector to use it for FIFO of same priority processes
    for(auto it = timeLine.end()-1;it != timeLine.begin()-1;it--){
        comingOrder[it->name] = counter;
        counter++;
    }

    // for convinience seperate vector just including arrival times of processes
    // these are the critical times that scheduler get a interrupt.
    for(auto & process : timeLine){
        arrivalTimes.push_back(process.arrivalTime);
    }


    // terminating condition: when ready queue is empty and every process has arrived.
    int lastComingProcessArrival = timeLine.front().arrivalTime;

    while(currentTime < lastComingProcessArrival || !readyQueue.empty()){
        if(readyQueue.empty()){ // no process has come yet
            outputFile << to_string(currentTime) + ":";
            printQueue(readyQueue);
            int nextTime = timeLine.back().arrivalTime;
            while(timeLine.back().arrivalTime == nextTime){ // after reaching a first arriving process,
                // remove every process arrived at that time. Add them to ready queue.
                process nextProcess = timeLine.back();
                timeLine.pop_back();
                arrivalTimes.pop_back();
                readyQueue.push(nextProcess);
            }
            currentTime = nextTime; // advance time until first arriving process
            outputFile << to_string(currentTime) + ":";
            printQueue(readyQueue);
        }else{
            bool thereIsAProcessLeaving = false; // will be true if a process leaves ready queue

            // Execute instructions until the process finishes or a new process comes in
            while(!thereIsAProcessLeaving){
                process enteringCPU = readyQueue.top();
                readyQueue.pop();
                string s(1, enteringCPU.codeFile.at(enteringCPU.codeFile.length()-1)); // getting last char converting to string
                int indexOfCode = stoi(s);
                int numOfInstructions = programs.at((unsigned long)indexOfCode-1).numberOfInstructions;
                vector<int> executionTimes = programs.at((unsigned long)indexOfCode-1).instructionExecutionTimes;

                int executionTime = executionTimes.at((unsigned long)enteringCPU.wereLeftAt-1);
                // execute this statement
                currentTime += executionTime;
                enteringCPU.wereLeftAt++;
                readyQueue.push(enteringCPU);

                //waiting time update
                auto copyQueue(readyQueue);
                while(!copyQueue.empty()){
                    auto next = copyQueue.top();
                    if(next.name != enteringCPU.name) waitingTimes[next.name] += executionTime;
                    copyQueue.pop();
                }

                // turnAround time calculation if the process finishes
                if(enteringCPU.wereLeftAt == 1 + numOfInstructions){
                    thereIsAProcessLeaving = true;
                    turnaroundTimes[enteringCPU.name] = currentTime - enteringCPU.arrivalTime;  // calculate turnaround time
                    readyQueue.pop();
                    outputFile << to_string(currentTime) + ":";
                    printQueue(readyQueue); // when a process terminates print the ready queue
                }
                // If a new process comes in, add it to ready queue. And stop execution of current one.
                if(!arrivalTimes.empty() && currentTime >= arrivalTimes.back()){

                    int nextTime = timeLine.back().arrivalTime;

                    while(timeLine.back().arrivalTime == nextTime){ // after reaching a first arriving process,
                        // remove every process arrived at that time. Add them to ready queue.
                        process nextProcess = timeLine.back();
                        timeLine.pop_back();
                        arrivalTimes.pop_back();
                        waitingTimes[nextProcess.name] += currentTime - arrivalTimes.back(); // If a process has come and waited an instruction to finish
                        readyQueue.push(nextProcess);
                    }

                    outputFile << to_string(currentTime) + ":";
                    printQueue(readyQueue);
                    break;
                }
            }
        }
    }
    // -------------------------------------------
    // Turnaround and Waiting Time Outputting -------------------------------------------
    outputFile << endl;
    string last = (--turnaroundTimes.end())->first; // last element in the map should cause not to print endl
    for(auto &x : turnaroundTimes){
        outputFile << "Turnaround time for " + x.first + " = " + to_string(x.second) + " ms" << endl;
        if(x.first != last){
            outputFile << "Waiting time for " + x.first + " = " + to_string(waitingTimes[x.first]) << endl;
        }else{
            outputFile << "Waiting time for " + x.first + " = " + to_string(waitingTimes[x.first]);
        }
    }
    outputFile.close();
    // -------------------------------------------

    return 0; // end of the program

}