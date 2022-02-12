/*
Author:Kushagra Indurkhya
*/
#include <random>
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <vector>
#include <unistd.h>
#include <semaphore.h>
#include "logger.cpp"

using namespace std;
int n,x;
double lambda_param,r,gamma_param;

//Semaphores
sem_t mutex,block;

//Struct passed to thread func
typedef struct thread_arg{
  int local_id;
  double eat_time;
  vector<logger> buffer; //local buffer
} t_arg;


int eating=0,waiting=0;
bool must_wait = false;

void korean(t_arg* data)
{
  // Request access log
  logger* r_log=new logger(data->local_id,"requested access ");
  data->buffer.push_back(*r_log);

  sem_wait(&mutex); //acquire mutex
  if(must_wait || eating+1>x) //group is formed
  {
    waiting++;
    must_wait = true;
    sem_post(&mutex);
    sem_wait(&block);
  }
  else //group is not formed
  {
    eating++;
    must_wait = (waiting>0 && eating == x);
    sem_post(&mutex);
  }
  logger* eat_log=new logger(data->local_id,"started eating ");
  data->buffer.push_back(*eat_log);
  //Simulate Eating
  this_thread::sleep_for(chrono::duration<double,milli>(data->eat_time));

  sem_wait(&mutex);
  eating--;
  
  //The group has left
  if(eating == 0)
  {
    int k= min(x,waiting);
    waiting -= k;
    eating += k;
    must_wait = (waiting>0 && eating == x);
    //Signalling block k times
    for(int i=0;i<k;i++)
      sem_post(&block);
  }
  logger* exit_log=new logger(data->local_id,"exited ");
  data->buffer.push_back(*exit_log);
  sem_post(&mutex);

}


int main ()
{ 

  //Opening input file
  ifstream infile("input.txt");
  if (!infile.is_open())
  {
      cout << "Unable to open input file\n";
      exit(1);
  }

  //Taking input for n,k,lambda1,lambda2
  infile >>n>>x>>lambda_param>>r>>gamma_param;

  default_random_engine generator;
  //Uniform distribution for number of customers
  uniform_int_distribution<int> customer_distribution(1,r*x);
  //Exponential distribution for delay in customer entry
  exponential_distribution<double> delay_expo(1/lambda_param);
  //Exponential distribution for eating time
  exponential_distribution<double> eat_time_expo(1/gamma_param);


  //Initializing semaphores
  sem_init(&mutex, 0, 1);
  sem_init(&block, 0, 0);


  vector<thread> threads;
  t_arg a[n];
  for(int i=0;i<n;)
  {
      int s=customer_distribution(generator);
      double delay = delay_expo(generator);
      // cout<<s<<" customers will enter after " << delay <<" seconds\n";
      this_thread::sleep_for(chrono::duration<double,milli>(delay));
      for (int j=0; j < s; j++)
      {
          
          a[i].local_id = i;
          a[i].eat_time = eat_time_expo(generator);
          threads.push_back(thread(korean,&a[i]));
          i++;
          if(!(i<n)) break; //if n threads have been creaated break 
      }
  }
  for (auto& th : threads) th.join(); //join all the threads


  vector<logger> complete_log;
  long long  waiting = 0;
  for (int i = 0; i < n; i++)
  {
    //dumping thread buffer log to complete log
    vector<logger> buff=a[i].buffer;
    for (logger w:buff) complete_log.push_back(w);
    long long curr_waiting_time =buff[1].get_epoch() - buff[0].get_epoch();
    // cout<<i<<"  "<<curr_waiting_time<<endl;
    waiting += curr_waiting_time;
  }

  
  // opening output file
  ofstream log_file("log.txt");
  if (!log_file.is_open())
  {
      cout << "Unable to generate output file\n";
      exit(1);
  }
  //sorting complete log and giving output to log file
  sort(complete_log.begin(),complete_log.end(),compare_log_obj);
  for (logger w:complete_log) log_file<<w.fetch_log();

  cout << waiting/n <<endl;
  
  return 0;
}