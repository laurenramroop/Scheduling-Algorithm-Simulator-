/**
 * PROJECT 1: Scheduling Algorithms 
 * @author Lauren Ramroop
 *
 * Generates 25 different jobs for a total of 5 scheduling algorithms
 * Calculates completion time, turnaround time, and average turnaround time for each schedule 
 * Prints job name, CPU Burst, Arrival time, priority, completion time, and turnaround time for each job
 * NOTE this code takes a long time to compile. Try commenting out each scheduling alg
 * function call in main. 
 */

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <cmath>
#include <vector> 
#include <iomanip>
#include <climits>
#include <unordered_map> 
using namespace std;

/**
 * Job class
 * Stores cpu burst, arrival time, priority, remaining time, completion time, and 
   turnaround time
 * Comes with matching getters and setters 
 */
class Job{
private:
  string jobName;
  int cpuBurst = rand() % 15 + 1;
  int arrivalTime = rand() % 250 + 1;
  int priority = rand() % 5 + 1;
  int remainingTime = cpuBurst;
  int completionTime;
  int turnAroundTime = 0;

public:
  //getters and setters
  string getJobName() {return jobName;}
  int getCPUBurst() {return cpuBurst;}
  int getArrivalTime(){return arrivalTime;}
  int getRemainingTime(){return remainingTime;}
  int getPriority(){return priority;}
  int getCompletionTime(){return completionTime;}
  int getTurnAroundTime(){return turnAroundTime;}

  void setJobName(string name){jobName = name;}
  void setCPUBurst(int cpuBurst){this->cpuBurst = cpuBurst;}
  void setArrivalTime(int arrivalTime){this->arrivalTime = arrivalTime;}
  void setRemainingTime(int remainingTime){this->remainingTime = remainingTime;}
  void setPriority(int priority){this->priority = priority;}
  void setCompletionTime(int completionTime){this->completionTime = completionTime;}
  void setTurnAroundTime(int turnAroundTime){this->turnAroundTime = turnAroundTime;}
};

//calling function headers
void printJobs(Job jobList[]);
void printSchedule(vector<Job> jobSchedule);

/**
 * Schedules an array of 25 jobs using First In First Out principles, sorted by smallest 
   arrival time
 * Uses helper function to print schedule
 * Runtime: O(n^2) due to sorting jobs in a nested for loop 
 *
 * @param jobList - array of 25 jobs
 * @param totalTurnAroundTimes - ref array of total turnaround time for all jobs
 * @param totalThroughputTimes - ref array of total throughput time for all jobs 
 */
void scheduleFIFO(Job jobList[], int(&totalTurnAroundTimes)[5], double(&totalThroughPutTimes)[5]){
  vector<Job> schedule; //schedule of all jobs running 
  int totalTurnAroundTime = 0;
  int jobCount = 0; //number of jobs scheduled, used for throughput
  int totalTime = 0; // total time, used for throughput
  int currentTime = 0;
  
  
  //sort by smallest arrival time 
  for (int i = 0; i < 25 - 1; i++) {
      for (int j = 0; j < 25 - i - 1; j++) {
          if (jobList[j].getArrivalTime() > jobList[j + 1].getArrivalTime()) {
              // Swap the jobs
              Job temp = jobList[j];
              jobList[j] = jobList[j + 1];
              jobList[j + 1] = temp;
          }
      }
  }
  
  // Calculate current time, throughput time, and completion time
  //first job
  // Calculate completion time (as current time plus CPU burst)
  int completionTime = currentTime + jobList[0].getCPUBurst();
  jobList[0].setCompletionTime(completionTime);;

  // Calculate turnaround time (as completion time minus arrival time)
  int turnAroundTime = jobList[0].getCompletionTime() - jobList[0].getArrivalTime();
  jobList[0].setTurnAroundTime(turnAroundTime);
  
  for (int i = 1; i < 25; i ++){
    //if job has arrived, put arrival time as current time 
    if (jobList[i].getArrivalTime() > currentTime){
      currentTime = jobList[i].getArrivalTime();
  
    }

    // Calculate completion time (as current time plus CPU burst)
    completionTime = currentTime + jobList[i].getCPUBurst();
    jobList[i].setCompletionTime(completionTime);;
    
    // Calculate turnaround time (as completion time minus arrival time)
    turnAroundTime = jobList[i].getCompletionTime() - jobList[i].getArrivalTime();
    jobList[i].setTurnAroundTime(turnAroundTime);

    //sum up all turnaround times 
    totalTurnAroundTime += jobList[i].getTurnAroundTime();
    
    //add job to schedule
    schedule.push_back(jobList[i]);
    jobCount++; //increment every time job is on schedule 
  }
  
  printSchedule(schedule);

  //calculate TAT, add it to main TAT array, and print
  double avgTAT = totalTurnAroundTime /double(25);
  totalTurnAroundTimes[0] = avgTAT;
  cout<<"Average Turnaround time for FIFO: " + to_string(avgTAT)<<endl;

  //calculate throughput time, add it to main throughput array, and print
  double avgThroughPutTime = jobCount / (double) currentTime;
  totalThroughPutTimes[0] = avgThroughPutTime;
  cout<<"Throughput time for FIFO for " + to_string(jobCount) + " scheduled jobs is: " +  to_string(avgThroughPutTime)<<endl<<endl;
  
  return; 
}

/**
 * Schedules an array of 25 jobs using Shortest Job First principles, sorted by smallest 
   CPU time
 * Uses helper function to print schedule
 * Runtime: O(n^2) due to sorting jobs in a nested for loop 
 *
 * @param jobList - array of 25 jobs
 */
void scheduleSJF(Job jobList[], int(&totalTurnAroundTimes)[5], double(&totalThroughPutTimes)[5]){
  vector<Job> schedule; //schedule of all jobs running 
  int totalTurnAroundTime = 0;//total TAT for all jobs
  int currentTime = 0;
  int jobCount = 0; //number of jobs scheduled, used for throughput
  int totalTime = 0; // total time, used for throughput
  
  //sort by smallest cpu time and smallest arrival time
  for (int i = 0; i < 25 - 1; i++) {
      for (int j = 0; j < 25 - i - 1; j++) {
          //if job has a smaller cpu burst and has arrived, swap
          //or if jobs have the same cpu burst and have the same arrival time, swap
          if (jobList[j].getCPUBurst() > jobList[j + 1].getCPUBurst() && jobList[j].getArrivalTime() > jobList[j + 1].getArrivalTime()) {
              // Swap the jobs
              Job temp = jobList[j];
              jobList[j] = jobList[j + 1];
              jobList[j + 1] = temp;
          }

      }
  }

  //first job
  // Calculate completion time (as current time plus CPU burst)
  int completionTime = currentTime + jobList[0].getCPUBurst();
  jobList[0].setCompletionTime(completionTime);;

  //if job has arrived, set its arrival time as current time 
  for (int i = 1; i < 25; i ++){
    if (jobList[i].getArrivalTime() > currentTime){
      currentTime = jobList[i].getArrivalTime();
    }

    //calculate completion time
    int completionTime = currentTime + jobList[i].getCPUBurst();
    jobList[i].setCompletionTime(completionTime);
    currentTime = completionTime;

    //calculate turnaround time
    int turnAroundTime = jobList[i].getCompletionTime() - jobList[i].getArrivalTime();
    jobList[i].setTurnAroundTime(turnAroundTime);

    //add all turnaround times up 
    turnAroundTime += jobList[i].getTurnAroundTime();
    
    //add job to schedule + increment jobCount
    schedule.push_back(jobList[i]);
    jobCount++;
  }

  printSchedule(schedule);
  
  //calculate TAT, add it to main TAT array, and print
  double avgTAT = totalTurnAroundTime / double(25);
  totalTurnAroundTimes[1] = avgTAT;
  cout<<"Average Turnaround time for SJF: " + to_string(avgTAT)<<endl;

  //calculate throughput time, add it to main throughput array, and print
  double throughPutTime = jobCount / (double) currentTime;
  totalThroughPutTimes[1] = throughPutTime;
  cout<<"Throughput time for SJF for " + to_string(jobCount) + " scheduled jobs is: " +  to_string(throughPutTime)<<endl<<endl;

}

/**
 * Schedules an array of 25 jobs using Shortest Remaining Time principles, preempting by 
   smallest remaining CPU time
 * Uses helper function to print schedule
 * Runtime: O(n^2) due to sorting jobs in a nested for loop 
 *
 * @param jobList - array of 25 jobs
 */
void scheduleSRT(Job jobList[], int(&totalTurnAroundTimes)[5], double(&totalThroughPutTimes)[5]) {
  vector<Job> schedule; // Schedule of all jobs running
  int totalTurnAroundTime = 0;
  int currentTime = 0;
  int jobCount = 0; 

  int remainingJobs = 25; // Number of jobs that have not completed yet
  
  while (remainingJobs > 0) {
    int shortestTime = INT_MAX;
    int shortestJobIndex = -1;
    
    // Find the job with the shortest remaining time among arrived jobs
    for (int i = 0; i < 25; i++) {
      if (jobList[i].getArrivalTime() <= currentTime && jobList[i].getRemainingTime() > 0) {
        // Calculate remaining time as the difference between the CPU burst and how long it has run
        int remainingTime = jobList[i].getCPUBurst() - (currentTime - jobList[i].getArrivalTime());
        if (remainingTime < shortestTime) {
          shortestTime = remainingTime;
          shortestJobIndex = i;
        }
      }
    }

    if (shortestJobIndex == -1) {
      // No job can be scheduled at this time, so increment the current time
      currentTime++;
    } else {
      // Schedule the shortest job, preempt
      Job& shortestJob = jobList[shortestJobIndex];
      shortestJob.setRemainingTime(shortestJob.getRemainingTime() - 1);
      currentTime++;

      // Check if the job has completed
      if (shortestJob.getRemainingTime() == 0) {
        remainingJobs--;

        
        // Calculate completion time, turnaround time, and update total turnaround time
        shortestJob.setCompletionTime(currentTime);
        int turnAroundTime = shortestJob.getCompletionTime() - shortestJob.getArrivalTime();
        shortestJob.setTurnAroundTime(turnAroundTime);
        totalTurnAroundTime += turnAroundTime;

        // Add the job to the schedule + increment jobCount
        schedule.push_back(shortestJob);
        jobCount++; 
      }
    }
  }
  
  printSchedule(schedule);
  
  //calculate TAT, add it to main TAT array, and print
  double avgTAT = totalTurnAroundTime / double(25);
  totalTurnAroundTimes[2] = avgTAT;
  cout<<"Average Turnaround time for SRT: " + to_string(avgTAT)<<endl;

  //calculate throughput time, add it to main throughput array, and print
  double throughPutTime = jobCount / (double) currentTime;
  totalThroughPutTimes[2] = throughPutTime;
  cout<<"Throughput time for SRT for " + to_string(jobCount) + " scheduled jobs is: " +  to_string(throughPutTime)<<endl<<endl;
}

/**
 * Schedules an array of 25 jobs using priority principles, sorted by smallest priority
 * Uses helper function to print schedule
 * Runtime: O(n^2) due to sorting jobs in a nested for loop 
 *
 * @param jobList - array of 25 jobs
 */
void schedulePriority(Job jobList[], int(&totalTurnAroundTimes)[5], double(&totalThroughPutTimes)[5]){
  vector<Job> schedule; //schedule of all jobs running 
  int totalTurnAroundTime = 0;
  int currentTime = 0;
  int jobCount = 0; 

  //sort by smallest cpu time and smallest arrival time
  for (int i = 0; i < 25 - 1; i++) {
      for (int j = 0; j < 25 - i - 1; j++) {
          //if job has a larger priority and has arrived, or if they have
          //same priority but different arrival times, swap
          if (jobList[j].getPriority() < jobList[j + 1].getPriority() || 
            (jobList[j].getPriority() == jobList[j + 1].getPriority() && jobList[j].getArrivalTime() > jobList[j + 1].getArrivalTime())) {
              // Swap the jobs
              Job temp = jobList[j];
              jobList[j] = jobList[j + 1];
              jobList[j + 1] = temp;
          }

      }
  }

  //if job has arrived, set its arrival time as current time 
  for (int i = 1; i < 25; i ++){
    if (jobList[i].getArrivalTime() > currentTime){
      currentTime = jobList[i].getArrivalTime();
    }

    //calculate completion time
    int completionTime = currentTime + jobList[i].getCPUBurst();
    jobList[i].setCompletionTime(completionTime);
    currentTime = completionTime;

    //calculate turnaround time
    int turnAroundTime = jobList[i].getCompletionTime() - jobList[i].getArrivalTime();
    jobList[i].setTurnAroundTime(turnAroundTime);

    //sum up all turnaround times 
    totalTurnAroundTime += jobList[i].getTurnAroundTime();

    //add job to schedule + increment jobCount
    schedule.push_back(jobList[i]);
    jobCount++;
  }

  printSchedule(schedule);
  
  //calculate TAT, add it to main TAT array, and print
  double avgTAT = totalTurnAroundTime / double(25);
  totalTurnAroundTimes[3] = avgTAT;
  cout<<"Average Turnaround time for Priority: " + to_string(avgTAT)<<endl;

  //calculate throughput time, add it to main throughput array, and print
  double throughPutTime = jobCount / (double) currentTime;
  totalThroughPutTimes[3] = throughPutTime;
  cout<<"Throughput time for priority for " + to_string(jobCount) + " scheduled jobs is: " +  to_string(throughPutTime)<<endl<<endl;
}

/**
 * Schedules an array of 25 jobs using Round Robin principles, each job running for quant
   at each period, followed by a context switch
 * Uses helper function to print schedule
 * Runtime: O(n^2) due to sorting jobs in a nested for loop 
 *
 * @param jobList - array of 25 jobs
 */
void scheduleRR(Job jobList[], int(&totalTurnAroundTimes)[5], double(&totalThroughPutTimes)[5]) {
  vector<Job> schedule; // Schedule of all jobs running
  int totalTurnAroundTime = 0;
  int currentTime = 0;
  int quant = rand() % 10 + 5; // Random time slice b/w 5 and 10
  int maxContextSwitch = static_cast<int>(ceil(0.3 * quant)); // 30% of the quant, rounded up
  int contextSwitch = rand() % (maxContextSwitch - 1) + 1; // Random context switch b/w 1 and maxContextSwitch
  int remainingJobs = 25; // Number of jobs that have not completed yet
  int currentIndex = 0; // Index of the current job to be executed
  int jobCount = 0;

  // Algorithm will keep running until all jobs finish
  while (remainingJobs > 0) {
    currentIndex = currentIndex % 25; // Wrap around the job list

    // If job has arrived and if it still needs to run
    if (jobList[currentIndex].getArrivalTime() <= currentTime && jobList[currentIndex].getRemainingTime() > 0){
      // Schedule job to run, calculate execution time based on quant
      Job& currentJob = jobList[currentIndex];
      int timeToExecute = min(quant, currentJob.getRemainingTime());

      // Update remaining time
      currentJob.setRemainingTime(currentJob.getRemainingTime() - timeToExecute);
      currentTime += timeToExecute;

      // Check if the job has completed
      if (currentJob.getRemainingTime() == 0) {
        remainingJobs--;

        // Calculate completion time
        currentJob.setCompletionTime(currentTime);

        // Calculate turnaround time as the difference between completion time and arrival time
        int turnAroundTime = currentJob.getCompletionTime() - currentJob.getArrivalTime();
        currentJob.setTurnAroundTime(turnAroundTime);

        // Print remaining time for the job
        cout << "Job " << currentIndex + 1 << " Remaining Time: " << currentJob.getRemainingTime() << endl;
      }
      
        // Print remaining time for the job
        cout << "Job " << currentIndex + 1 << " Remaining Time: " << currentJob.getRemainingTime() << endl;
      
      // Add the job to the schedule + increment jobCount
      schedule.push_back(currentJob);
      jobCount++;

      // Move to the next job
      currentIndex++;
      } else {
        // Job hasn't arrived yet, move to the next job
        currentIndex++;
      }

    // Apply the context switch
    currentTime += contextSwitch;
  }

    printSchedule(schedule);
    cout << "Quant: " << to_string(quant) << endl;
    cout << "Context Switch: " << to_string(contextSwitch) << endl;

    // Calculate TAT, add it to the main TAT array, and print
    double avgTAT = totalTurnAroundTime / double(25);
    totalTurnAroundTimes[4] = avgTAT;
    cout << "Average Turnaround time for RR: " + to_string(avgTAT) << endl;

    // Calculate throughput time, add it to the main throughput array, and print
    double throughPutTime = jobCount / (double)currentTime;
    totalThroughPutTimes[4] = throughPutTime;
    cout << "Throughput time for RR for " + to_string(jobCount) + " scheduled jobs is: " + to_string(throughPutTime) << endl << endl;
}
  
/**
 * Calculates smallest turnaround time
 * Runtime: O(n) due to sorting turnaround times in a for loop 
 *
 * @param &totalTurnAroundTimes - array of turnaround times for each algorithm
 */
double smallestTAT(int(&totalTurnAroundTimes)[5]){
  double min = totalTurnAroundTimes[0];
  
  //find smallest element in array 
  for(int i = 0; i < 5; i++){
      if(totalTurnAroundTimes[i] < min){
          min = totalTurnAroundTimes[0];
      }
  }

  //output smallest element and respective alg
  if (min == totalTurnAroundTimes[0]) cout<<"FIFO.";
  else if (min == totalTurnAroundTimes[1]) cout<<"SJF.";
  else if (min == totalTurnAroundTimes[2]) cout<<"SRT.";
  else if (min == totalTurnAroundTimes[3]) cout<<"Priority.";
  else cout<< "RR.";

  return min;
}

/**
 * Calculates largest throughput time
 * Runtime: O(n) due to sorting throughput times in a for loop 
 *
 * @param &totalThroughPutTimes - array of through put times for each algorithm
 */
double largestThroughPutTime(double(&totalThroughPutTimes)[5]){
  double max = totalThroughPutTimes[0];
  //find largest element in array 
  for(int i = 0; i < 5; i++){
      if(totalThroughPutTimes[i] > max){
          max = totalThroughPutTimes[0];
      }
  }

  //output largest element and respective alg
  if (max == totalThroughPutTimes[0]) cout<<"FIFO.";
  else if (max == totalThroughPutTimes[1]) cout<<"SJF.";
  else if (max == totalThroughPutTimes[2]) cout<<"SRT.";
  else if (max == totalThroughPutTimes[3]) cout<<"Priority.";
  else cout<< "RR.";
  
  return max;
}

/**
 * Prints an array of 25 jobs including their name, CPU burst, Arrival Time, and Priority
 * Runtime: O(n) due to using a for loop
 * 
 * @param jobList - array of 25 jobs
 */
void printJobs(Job jobList[]){
  cout<<"Jobs to be run:"<<endl;
  cout<<"NAME:"<<"\t"<<"CPU BURST:"<<"\t"<<"ARRIVAL TIME:"<<"\t"<<"PRIORITY:"<<"\t"<<endl;
  for (int i = 0; i < 25; i++){
    cout<<jobList[i].getJobName()<<"\t"<<jobList[i].getCPUBurst()<<"\t"<<jobList[i].getArrivalTime()<<"\t"<<jobList[i].getPriority()<<"\t"<<"\t"<<endl<<endl;
  }
  return;
}

/**
 * Prints a table and Gantt Diagram of jobs running on schedule, including its Name, CPU Burst, Arrival Time, Priority,
 * Completion Time, and Turnaround Time
 * Runtime: O(n) due to using a for loop
 * 
 * @param jobSchedule - vector of jobs to simulate Gantt Chart
 */
void printSchedule(vector<Job> jobSchedule){
  cout<<"JOB CHART: "<<endl;
  cout<<"NAME:"<<"\t"<<"CPU BURST:"<<"\t"<<"ARRIVAL TIME:"<<"\t"<<"PRIORITY:"<<"\t"<<"COMPLETION TIME:"<<"\t"<<"TURN AROUND TIME:"<<"\t"<<endl;
  for(int i = 0; i < jobSchedule.size(); i++){
    cout<<jobSchedule[i].getJobName()<<"\t"<<jobSchedule[i].getCPUBurst()<<"\t"<<jobSchedule[i].getArrivalTime()<<"\t"<<jobSchedule[i].getPriority()<<"\t"<<jobSchedule[i].getCompletionTime()<<"\t"<<jobSchedule[i].getTurnAroundTime()<<"\t"<<endl;
  }
  cout<<endl;
  cout<<"Gantt Diagram: "<<endl;
  for (int i = 0; i <jobSchedule.size(); i++){
    cout<<"|" + jobSchedule[i].getJobName();  
  }
  cout<<endl<<endl;
  return; 
}

int main() {
  srand(time(NULL)); //truly random
  
  Job jobList[25]; //array used because only works with 25 jobs and for performance
  int allTAT[5] = {0}; //array of all turnaround times for all schedules 
  int timeInterval = 100; //time interval to calculate throughput, # of jobs completed within interval 
  int totalTurnAroundTimes[5] = {}; //initializing empty array
  double totalThroughPutTimes[5] = {}; 
  
  //manually setting first job to arrive at t = 0
  Job job;
  job.setArrivalTime(0);
  job.setJobName("p" + to_string(1));
  jobList[0] = job;

  //randomly generating rest of jobs 
  for (int i = 1; i < 25; i++){
    Job job; //create instance of job on stack
    job.setJobName("p" + to_string(i+1)); //generate + set name
    jobList[i] = job;
  }

  //print list of jobs
  printJobs(jobList);
  
  //sorting algorithms
  cout<<"-------------------------------------------------------------"<<endl;
  cout<<"FIFO SCHEDULING: "<<endl<<endl;
  scheduleFIFO(jobList, totalTurnAroundTimes, totalThroughPutTimes);
  cout<<"-------------------------------------------------------------"<<endl;
  cout<<"SHORTEST JOB FIRST SCHEDULING: "<<endl<<endl;
  scheduleSJF(jobList, totalTurnAroundTimes, totalThroughPutTimes);
  cout<<"-------------------------------------------------------------"<<endl;
  cout<<"SHORTEST REMAINING TIME SCHEDULING: "<<endl<<endl;
  scheduleSRT(jobList, totalTurnAroundTimes, totalThroughPutTimes);
  cout<<"-------------------------------------------------------------"<<endl;
  cout<<"PRIORITY SCHEDULING: "<<endl<<endl;
  schedulePriority(jobList, totalTurnAroundTimes, totalThroughPutTimes);
  cout<<"-------------------------------------------------------------"<<endl;
  cout<<"ROUND ROBIN SCHEDULING: "<<endl<<endl;
  scheduleRR(jobList, totalTurnAroundTimes, totalThroughPutTimes);

  //TODO
  cout<<"SUMMARY"<<endl;
  cout<<"Algorithm with smallest TAT: "<<endl;
  largestThroughPutTime(totalThroughPutTimes);
  cout<<endl;
  cout<<"Algorithm with largest throughput time: "<<endl;
  smallestTAT(totalTurnAroundTimes);
  
  return 0;
}