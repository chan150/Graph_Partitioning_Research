//
//  RandomAssignment.cpp
//  Boost
//
//  Created by Steve DengZishi on 7/13/17.
//  Copyright © 2017 Steve DengZishi. All rights reserved.
//

#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <cmath> // To use sqrt()
#include <list> // To use STL linked-list in dfs topological sort
#include <stack> // To use STL stack in topological sort
#include <queue> // To use STL queue in bfs, dijkstra's, maxflow
#include <cstring> // To use memset()
#include <cstdlib> // To use atoi(), abs(), malloc()
#include <algorithm> // To use sort(), next_permutation(), min(), max() etc.
#include <unordered_map> // To allow O(1) mapping access using key->value
#include <set> // To sort and remove duplicate when inserted
#include <unordered_set> //To remove duplicates and count size
#include <functional> //To use std::hash
#include <fstream> // To use c++ input and output to files

using namespace std;

//global variables here
vector<int>* shard;
int* prevShard;
fstream inFile;
string fileName;
int partitions;
int nodes;
int edges;

//functions here
unsigned int int_hash(unsigned int x) {
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

void randomShard(){
    //random sharding according using a integer hash then mod 8 to distribute to shards
    for(int i=0;i<nodes;i++){
        //        cout<<"i is: "<<i<<endl;
        unsigned hash_val = int_hash(i);
        //        cout<<"hash is: "<<hash_val<<endl;
        int shardID = hash_val % partitions;
        //        cout<<"shard ID is: "<<shardID<<endl;
        shard[shardID].push_back(i);
        //mark prevShard for the 1st random Sharding
        prevShard[i]=shardID;
    }
}

void printShard(){
    for(int i=0;i<partitions;i++){
        printf("Shard %d:\n",i);
        for(int j=0;j<shard[i].size();j++){
            cout<<shard[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}

//start of main program
int main(){
    
    //get stdin from shell script
    cin>>fileName;
    cin>>partitions;
    
    inFile.open(fileName,ios::in);
    
    if(!inFile){
        cerr<<"Error occurs while opening the file"<<endl;
        exit(1);
    }
    
    //read number of nodes and edges
    inFile>>nodes>>edges;
    
    //create data structures with variable size on the heap
    shard=new vector<int>[partitions];
    prevShard=new int[nodes];
    
    //random sharding
    randomShard();
//    printShard();
    //output
    FILE* outFile = fopen("sharding_result.bin", "wb");
    
    //write block of data to stream
//    fwrite(prevShard, sizeof(int), nodes, outFile);
//    fwrite(shard, sizeof(vector<int>), partitions, outFile);
    for(int i=0;i<partitions;i++){
        fprintf(outFile,"%d\n",int(shard[i].size()));
        for(int j=0;j<shard[i].size();j++){
            fprintf(outFile,"%d ", shard[i][j]);
        }
    }
    
    for(int i=0;i<nodes;i++){
        fprintf(outFile,"%d ", prevShard[i]);
    }
    
    //delete dynamic allocation
    delete [] shard;
    delete [] prevShard;
    
    fclose(outFile);
    
    return 0;
}
