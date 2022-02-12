/*
File:cas.cpp
Implementing compare and swap
Author:Kushagra Indurkhya (CS19B1017)

*/
#include <atomic>
#include <thread>
#include <vector>
#include <chrono>
#include <random>
#include"logger.cpp"
using namespace std;

//Global variables
atomic<bool> lock;
int n,k;
double l1,l2;

//Seed for random value generator
long SEED;

void test_CAS(t_arg* data)
{

    // thread::id t_id = this_thread::get_id();

    //Random distribution generator
    default_random_engine generator(SEED);
    exponential_distribution<double> critical_section_time(1/l1);
    exponential_distribution<double> remaining_section_time(1/l2);


    int local_id = data->local_id;
    for(int i=0;i<k;i++)
    {
       /*entry section starts*/
       //logging request time
        logger* r_log=new logger(i,local_id,"requested ");
        data->buffer.push_back(*r_log);

        bool expected=false;
        while (!lock.compare_exchange_strong(expected,true))
        {
            expected = false;
        }
        /*entry section ends*/
        
         /*Critical section starts*/
         //Logging entry time
        logger* entry_log=new logger(i,local_id,"entered   ");
        data->buffer.push_back(*entry_log);

        //Updating waiting time and worst waiting time
        long curr_wait=entry_log->epoch-r_log->epoch;
        data->waiting_time.push_back(curr_wait);
        if(curr_wait > data->worst_waiting_time) 
          data->worst_waiting_time=curr_wait;

        //Simulating critical section
        this_thread::sleep_for(chrono::duration<double>(critical_section_time(generator))/1000);
         /*Critical section ends*/


        /*exit section starts*/
          //Logging exit time
          logger* exit_log=new logger(i,local_id,"exited    ");
          data->buffer.push_back(*exit_log);
        
          lock = false;
        /*exit section ends*/

        //Remainder Section
        this_thread::sleep_for(chrono::duration<double>(remaining_section_time(generator))/1000);
    }
}

int main ()
{
  //Giving epoch to random generator seed for uniqueness
  SEED = std::chrono::system_clock::now().time_since_epoch().count();
  
  vector<thread> threads;

  //Opening input file
  ifstream infile("inp-params.txt");
  if (!infile.is_open())
  {
      cout << "Unable to open input file\n";
      exit(1);
  }

  //Taking input for n,k,lambda1,lambda2
  infile >>n>>k>>l1>>l2;

  //Array of structs to be passed as argument to thread func
  t_arg a[n];


  //Creating n threads
  for (int i=0; i<n ;i++)
  {
    a[i].local_id=i;
    a[i].worst_waiting_time=-1;
    threads.push_back(thread(test_CAS,&a[i]));

  }

  //Joining n threads
  for (auto& th : threads) th.join();


  double total=0;
  int max_wait=-1;


  vector<logger> complete_log;

  for (size_t i = 0; i < n; i++)
  {
    //dumping thread buffer log to complete log
    for (logger w:a[i].buffer) complete_log.push_back(w);
    //clearing buffer vector
    a[i].buffer.clear();
    //summing waiting time
    for (int j:a[i].waiting_time) total+=j;
    //Cecking for max waiting time
    if(a[i].worst_waiting_time > max_wait) max_wait=a[i].worst_waiting_time;
  }

  
  // opening output file
  ofstream log_file("CAS-log.txt");
  if (!log_file.is_open())
  {
      cout << "Unable to generate output file\n";
      exit(1);
  }
  //sorting complete log and giving output to log file
  sort(complete_log.begin(),complete_log.end(),compare_log_obj);
  for (logger w:complete_log) log_file<<w.fetch_log();
  cout<<"Worst  Waiting time "<<max_wait<<endl;
  cout<<"Avg    Waiting time "<<total/(n*k)<<endl;


  
  return 0;
}