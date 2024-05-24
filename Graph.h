#pragma once

#include <unordered_map>
#include <vector>
#include <tuple>
#include <iostream>

#include "readData.h"

using namespace std;

struct Edge
{
    size_t v ; 
    double flow ; 
    double C; 
    size_t rev; 
};

struct Stats {
    size_t vol;
    size_t cutsize;
    double modularity;
    Stats(size_t _vol, size_t _cutsize, double _modularity) : vol(_vol), cutsize(_cutsize), modularity(_modularity) {}
    Stats() : vol(0), cutsize(0), modularity(0.0) {}
};

class graph{
    size_t m;
    size_t n;
    size_t* ai;
    size_t* aj;
    double* a;
    double* degrees;
    double volume;
public:
    int* level;
    vector<vector<Edge>> adj;

    graph();
    graph(size_t,size_t,size_t*,size_t*,double*,double*);
    graph(const char *file_path, int indOffset, bool needWeights = true);
    ~graph();

    graph& operator=(graph&& X);
    
    //common functions
    size_t get_vertex_number() const;
    size_t get_edges_number() const;
    size_t get_degree_unweighted(size_t id);
    void addEdge(size_t u, size_t v, double C, bool isDirected = false);
    Stats get_stats(unordered_map<size_t, size_t>& R_map, size_t nR, bool need_mod = false);
    void make_adj_list();

    bool BFS(size_t s, size_t t, size_t V);
    double sendFlow(size_t u, double flow, size_t t, vector<size_t>& start, vector<pair<bool, double>>& SnapShots);
    pair<double, size_t> Maxflow(size_t s, size_t t, vector<bool>& mincut);
    pair<double,size_t> DinicMaxflow(size_t s, size_t t, size_t V, vector<bool>& mincut);
    void find_cut(size_t u, vector<bool>& mincut, size_t& length);

    //functions in MQI.cpp
    size_t MQI(size_t nR, size_t* R, size_t* ret_set, Stats& beforeStats, Stats& afterStats);
    void build_map(unordered_map<size_t, size_t>& R_map,unordered_map<size_t, size_t>& degree_map,
                   size_t* R, size_t nR);
    void build_list(unordered_map<size_t, size_t>& R_map, unordered_map<size_t, size_t>& degree_map, size_t src, size_t dest, size_t a, size_t c);

    double get_modularity(const unordered_map<size_t, size_t>& R_map);

    bool findShortestPath(size_t s, size_t t, vector<Edge*>& parents, double& flow);
    pair<double, size_t> EdmondsKarpMaxFlow(size_t s, size_t t, vector<bool>& mincut);

    void check() {
        for (int i = 0; i < n + 1; ++i) {
            cout << ai[i] << " ";
        }
        cout << endl;
        for (int i = 0; i < m; ++i) {
            cout << aj[i] << " ";
        }
        cout << endl;
    }
};