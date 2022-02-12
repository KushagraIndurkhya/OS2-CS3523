/*Author:Kushagra Indurkhya
Fine-grained graph coloring*/
#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <thread>
#include <mutex>
#include <chrono>
#include <sys/time.h>

using namespace std;

/*Class: Graph
    Members:
            int vertices : no of vertices
            vector<int>* adj_list : array of vectors to store adjacency list
    Methods:
            graph(int n):constructor
            add_edge(add an edge from i to j)*/
class graph
{
    public:
        int vertices;
        vector<int>* adj_list;
        graph(int n)
        {
            vertices = n;
            adj_list=new vector<int>[n];
        }
        void add_edge(int i,int j)
        {
            adj_list[i].push_back(j);
        }
};
/*Global Variables*/
graph* inp_graph;
map <int,vector<int>> partitions;
map <int,int> indexing;
int *color;
int n;
mutex *mtx;

/*Print correct if */
void check()
{
    bool flag=true;
    for (int i = 0; i < n; i++)
        for(auto j:inp_graph->adj_list[i])
                if(color[j] == color[i])
                    flag=false;
    if(flag) cout << "CORRECT\n";
    else cout << "INCORRECT\n";
}
//Color vertex with vertex id:i
void color_vertex(int i)
{
    bool* available_color=new bool[n];
    //initialize available_color to true
    for(int i=0;i<n;i++)
        available_color[i]=true;
    //mark colors of all adjacent vertices
    for(auto j:inp_graph->adj_list[i])
            if(color[j]!=-1) available_color[color[j]]=false;
    //Mark vertex i the lowest unmarked color in available_color array
    for(int k=0;k<n;k++)
        if(available_color[k])
        {
            color[i]=k;
            break;
        } 
}
void color_partition(int part)
{
    vector<int> external_vertices;
    //Identifying external vertices
    for(int v:partitions[part])
    {
        bool flag=true;
        for(auto j:inp_graph->adj_list[v])
        {
            if(indexing[j]!=part)
            {
                external_vertices.push_back(v);
                flag = false;
                break;
            }       
        }
        if(flag)
        {
            color_vertex(v); //Coloring external vertices
        }
    }

    //Coloring external vertices       
    for(int i:external_vertices)
    {
        bool inserted_flag=false;//is mtx[i] locked
        for (auto k:inp_graph->adj_list[i])
        {
            if(k>i && !inserted_flag)
            {
                mtx[i].lock();//lock mtx[i]
                inserted_flag=true;
            }
            mtx[k].lock();
        }
        //mtx[i] is not locked ...ie i is greater than the last element of its adjacency list
        if(!inserted_flag) mtx[i].lock() ;

        color_vertex(i);//color vertex i

        //Unlock all acquired locks
        mtx[i].unlock();
        for(auto k:inp_graph->adj_list[i])
            mtx[k].unlock();
    }
}


int main()
{
    /*Input*/
    ifstream infile("input_params.txt");
    if (!infile.is_open())
    {
        cout << "Unable to open input file\n";
        exit(1);
    }
    int k;
    infile >> k;
    infile >> n;
    inp_graph = new graph(n);
    int temp;
    char temp_space;
    for(int i=-1;i<n;i++)
    {
        for (int j = -1; j <n; j++)
        {
            if(i==-1 && j==-1)//Skipping row nos and col nos from the input
                continue;
            infile >> temp;
            if(i>=0 && j>=0 && temp)    
                inp_graph->add_edge(i,j);//Adding edge between j in adjacency list of i
        }
    }
    infile.close();
    /*............................................................*/
    /*Partitioning*/
    vector<int> vert;
    for (int i=0;i<n;i++)
        vert.push_back(i);
    random_shuffle(vert.begin(),vert.end());//Randomly shuffling the vertices
    for (int i=0;i<n;)
    {
        for (int j=0;j<k;j++)
        {
            if(i<n)
            {
            //assigning partition j vertex at vert[i]
            partitions[j].push_back(vert[i]);
            indexing[vert[i]]=j;
            i++;
            }
            //all vertices have been partitioned
            else break;
        }
    }

    /*Initializing color array to -1*/
    color=new int[n];
    for (int i = 0; i < n; i++)
        color[i]=-1;

    vector<thread> threads;
    mtx = new mutex[n]; //array of mutex
    auto start = std::chrono::high_resolution_clock::now(); //Clock Starts
    for(auto w:partitions)
            threads.push_back(thread (color_partition,w.first));
    for (auto &th:threads)
        th.join();
    auto stop = std::chrono::high_resolution_clock::now();//Clock Stops
    auto duration =  std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

    /*............................................................*/
    /*OUTPUT*/
    ofstream out_file("output-f.txt");
    if (!out_file.is_open())
    {
         cout << "Unable to open output file\n";
        exit(1);
    }
    out_file << "Fine-grained Lock\n";
    set <int> colors_used;//For counting unique colors
    out_file <<"Colors:" << endl;
    for(int i=0;i<n;i++)
    {
       colors_used.insert(color[i]);
       out_file << "v"<<1+i<<"-"<<color[i]<<", ";
    }
    out_file << endl;
    out_file << "Time taken by the algorithm using: "<<duration.count() <<" Millisecond\n";
    out_file <<"No. of colours used: "<< colors_used.size()<< endl;

    /*............................................................*/
   
    //*********************************************
    cout <<"Time: "<< duration.count()<<endl << "Colors: "<<colors_used.size()<< endl;
    check();//check the coloring
    //*********************************************

    return 0;
}