/*Author:Kushagra Indurkhya
Sequential graph coloring*/
#include <iostream>
#include <fstream>
#include <iterator>
#include <set>
#include <vector>
#include <chrono>
#include <sys/time.h>
using namespace std;
class graph
{
    public:
        int vertices;
        bool** adj_matrix;
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

void check(int* n,graph inp)
{
    bool flag=true;
    for (int i = 0; i < inp.vertices; i++)
        for(auto j:inp.adj_list[i])
                if(n[j] == n[i])
                    flag=false;
    if(flag) cout << "CORRECT\n";
    else cout << "INCORRECT\n";
}



int main()
{
    //Opening input file
    ifstream infile("input_params.txt");
    if (!infile.is_open())
    {
        cout << "Unable to open input file\n";
        exit(1);
    }
    int n,v;
    infile >> n;
    infile >> v;
    graph inp_graph = *(new graph(v));
    int temp;
    char temp_space;
    for(int i=-1;i<v;i++)
        for (int j = -1; j <v; j++)
        {
            if(i==-1 && j==-1)
                continue;
            
            infile >> temp;
            
            if(i>=0 && j>=0 && temp)    
                inp_graph.add_edge(i,j);
        }
    infile.close();
    
    int color[v];
    bool available_color[v];
    for (int i = 0; i < v; i++)
        color[i]=-1;


    auto start = std::chrono::high_resolution_clock::now();
    for (int i=0;i<v;i++)
    {
        for(int k=0;k<v;k++)
            available_color[k]=true;
        for(auto j:inp_graph.adj_list[i])
                if(color[j]!=-1) available_color[color[j]]=false;
        
        for(int k=0;k<v;k++)
            if(available_color[k])
            {
                color[i]=k;
                break;
            }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

    set <int> colors_used;
    for(int a:color)
    {
        colors_used.insert(a);
    }

    //*********************************************
    cout <<"Time: "<< duration.count()<<endl <<"Colors: "<< colors_used.size()<< endl;
    check(&color[0],inp_graph);
    //*********************************************
    
    
}