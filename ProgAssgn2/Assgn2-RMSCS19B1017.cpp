/*
RMS-Scheduling
Author: Kushagra Indurkhya
*/
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>

using namespace std;

//defining context swtching time = 10 microseconds
//***************************
// #define CONTEXT_SWITCH 0.010
//***************************

struct process
{
    public:
        //meta -provided in input
        int process_id; //id of the process
        int p_time;//cpu execution 
        int period;//period of the process
        int repetition;//number of time the process repeats
        

        double priority;

        //helper
        int iteration_left; //no of iterations left
        double curr_start;     //start time of current process
        double curr_deadline;  //current deadline of current process
        int cpu_execution;  //cpu_execution left in current process

        //stats
        int missed_deadline;    //no of deadlines missed
        int completed_deadline; //no of deadlines completed
        double waiting_time;       //time waited

        //Constructor
        process(int p_id,int t,int p,int k)
        {
            this->process_id=p_id;
            this->p_time=t;
            this->period=p;
            this->repetition = k;
            

            this->priority = 1/(double)p;

            curr_deadline = 0+p;
            iteration_left=k;
            curr_start = 0;
            cpu_execution = p_time;

            missed_deadline = 0;
            completed_deadline=0;
        }

        /*
        Complete_iteration : Update the member variables when a process finishes executing
        -Updates deadline and start of the process
        -increments completed deadline
        -resets cpu_execution
        -decrements number of processes left
        */
        void complete_iteration()
        {
            this->curr_deadline += this->period;
            this->curr_start += this->period;

            this->completed_deadline++;

            this->cpu_execution = p_time;

            this->iteration_left--;
        }
        /*
        Complete_iteration : Update the member variables when a process finishes executing
        -Updates deadline and start of the process
        -increments missed deadline deadline
        -adds period to waiting time
        -resets cpu_execution
        -decrements number of processes left
        */
        void miss(double t)
        {
            this->missed_deadline ++;
            
            //Subtracting the waiting time in the current process
            this->waiting_time -= t-curr_start-(p_time-cpu_execution);
            //adding period to waiting time
            this->waiting_time += period;

            this->curr_deadline += this->period;
            this->curr_start += this->period;
            this->cpu_execution = p_time;

            this->iteration_left--;
        }
};

/*
Compare(a,b)- helper function for Sorting the vector of processes

takes two processes and returns true if a has a higher priority than b (comparing by priority ie 1/period)
if both a and b have equal priority then returns true if a has a smaller process id
*/
bool compare(process a, process b)
{
    if(a.priority != b.priority)
        return a.priority > b.priority;
    else
        return a.process_id < b.process_id;
}

/*
get_min_process_start_time(vector <process> p,int t)

takes a vector p of processes and time t
returns the idx of the process having the most priority and 
can be executed (ie start_time <= t and has processes left)

returns -1 if no process can be executed
*/
int get_min_process_start_time(vector <process> p,int t)
{
    for(int i=0;i<p.size();i++)
            if(p[i].curr_start <=t && p[i].iteration_left > 0 )
                return i;
    return -1;
}


int main()
{
    int n,p_id,ti,p,k;

    //opening input and output files
    ifstream infile("inp-params.txt");
    ofstream log_file("RMS-Log.txt"),stat_file("RMS-Stats.txt");

    if (!infile.is_open())
    {
        cout << "Input file not found\n";
        exit(1);
    }
    if (!log_file.is_open() || !stat_file.is_open())
    {
        cout << "Unable to generate output file\n";
        exit(1);
    }
    // Taking input from file
    infile >> n;

    vector <process> processes;

    //constructing n process structs and pushing them in a vector
    for (int i = 0; i < n; i++)
    {
        infile >> p_id >> ti >> p >> k ;
        processes.push_back(*(new process(p_id,ti,p,k))) ;
    }
    
    //Outputing process info to log file
    for (int i = 0; i < n; i++)
    {
        infile >> p_id >> ti >> p >> k ;
        log_file << "Process P" << processes[i].process_id 
                <<": processing time=" << processes[i].p_time 
                << "; deadline:"<< processes[i].curr_deadline
                <<"; period:"<< processes[i].period
                <<" joined the system at time "<<processes[i].curr_start<<"\n";
    }
    
   //.....................................Scheduling.........................................................
    //sorting processes vector
    sort(processes.begin(),processes.end(),compare);
    int last_idx= -1,p_idx,
        k_flag,
        was_running=1;
    
    //infinite  for loop


    // variables for timing the selection  process
    //***************************
    // clock_t start_time,end_time;
    //****************************


    for(double t=0;true;t++)
    {
        //starting the clock
        //***************************
            // start_time=clock();
        //****************************


        //retrieving the id of process with highest priority
        p_idx = get_min_process_start_time(processes,t);

        if(p_idx != -1)
        {
            if(!was_running)
                log_file << "CPU Idle till :"<<t<<"\n";

            if(last_idx == -1 || !was_running) //new process is starting from idle state
                log_file <<"Process P"<< processes[p_idx].process_id <<" starts execution at time " <<t<< "\n";

            else
            {

                if(last_idx != p_idx)
                {

                    // last process has finished and new process is starting
                    if(processes[last_idx].cpu_execution == processes[last_idx].p_time )
                    {
                        //adding context switch time to t
                        //***************************
                            // t+= CONTEXT_SWITCH;
                        //****************************
                        if(processes[p_idx].cpu_execution == processes[p_idx].p_time)
                            log_file <<"Process P"<< processes[p_idx].process_id <<" starts execution at time " <<t<< "\n";
                        else
                            log_file <<"Process P"<< processes[p_idx].process_id <<" resumes execution at time " <<t<< "\n";
                    }
                    else // this process is preempting the last process
                    { 
                         
                        
                        log_file <<"Process P"<< processes[last_idx].process_id 
                        <<" preempted by Process P" <<processes[p_idx].process_id 
                        <<" at time " <<t
                        << ". Remainig processing time:"<<processes[last_idx].cpu_execution<<"\n";

                        //adding context switch time to t
                        //***************************
                            // t+= CONTEXT_SWITCH;
                        //****************************

                        log_file <<"Process P"<< processes[p_idx].process_id <<" starts execution at time " <<t<< "\n";
                    }
                }
            }
            
            //ending the clock and adding the time taken in selection to t
            //***************************
            // end_time = clock();
            // t+= (end_time-start_time)*.001;
            //***************************

            processes[p_idx].cpu_execution--; //Executing the process
            if(processes[p_idx].cpu_execution==0) //process finished
            {   
                processes[p_idx].complete_iteration();
                log_file <<"Process P"<< processes[p_idx].process_id  <<" finishes execution at time " <<t+1 << "\n";
            }
            last_idx = p_idx; //updating last idx
            was_running=1;
        }
        else //no runnable process was found idle state
        {
            was_running=0;
            continue;
        }

        k_flag=0;
        
        for(int i=0;i<n;i++)
        {
            //process will miss the deadline
            if(processes[i].iteration_left > 0 && t+processes[i].cpu_execution >= processes[i].curr_deadline)
            {
                processes[i].miss(t);
                log_file <<"Process P"<< processes[i].process_id  <<" missed deadline at time " <<t << "\n";
            }
            if(processes[i].iteration_left>0)
                k_flag=1;
        }

        //if all the process have all the iterations completed
        if(!k_flag)
            break; 
        //updating the waiting time
        for(int i=0;i<n;i++)
        {
            if
            (
                i!=p_idx 
                && processes[i].curr_start < t+1
                && processes[i].iteration_left >0
            )
            processes[i].waiting_time ++;
        }
           
    }

    //outputing to stats file
    double avg = 0.0;
    int completed=0,missed=0,total=0;
    for(int i=0;i<n;i++)
    {
        avg += processes[i].waiting_time/(double)processes[i].repetition;
        total += processes[i].repetition;
        completed += processes[i].completed_deadline;
        missed += processes[i].missed_deadline;
    }

    stat_file   << "Number of processes that came in the system: " << total <<"\n"
                << "Number of processes that successfully completed: " << completed <<"\n"
                << "Number of processes that missed their deadlines: " << missed << "\n"
                << "Average waiting time for each process: " <<avg/n <<" Milliseconds \n" ;

    return 0;
}