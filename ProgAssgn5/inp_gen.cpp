/*Author:Kushagra Indurkhya
Input generator*/
#include <fstream>
#include <iostream>
#include <stdlib.h>
using namespace std;
int main()
{
    ofstream out_file("input_params.txt");
    if (!out_file.is_open())
    {
        cout << "Unable to create file\n";
        exit(1);
    }
    int n,k;

    /**/
    n=400;/*Insert Number of vertices here*/
    k=5;/*Insert Number of threads here*/
    /**/
    out_file << k <<" " << n << endl;
    bool ** adj_mat=new bool*[n];
    for(int i=0;i<n;i++)
        adj_mat[i]=new bool[n];
    for(int i=1;i<=n;i++)
        out_file << i <<" ";
    out_file << endl;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            adj_mat[i][j] = false;

    for (int i = 1; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (rand() % 2 && i != j)
            {
                adj_mat[i][j] = true;
                adj_mat[j][i] = true;
            }
    for(int i=1;i<=n;i++)
    {
        out_file << i <<" ";
        for(int j=1;j<=n;j++)
            out_file << adj_mat[i-1][j-1] <<" ";
        out_file << endl;
    }
    return 0;
}