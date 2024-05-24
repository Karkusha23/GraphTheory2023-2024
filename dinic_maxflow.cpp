#include "Graph.h"
#include <list>
#include <climits>
#include <utility>
#include <iostream>
#include <stack> 
#include <algorithm> 

using namespace std;

bool graph::BFS(size_t s, size_t t, size_t V)
{
    for (size_t i = 0 ; i < V ; i++) {
        level[i] = -1;
    }
 
    level[s] = 0;
    list< size_t > q;
    q.push_back(s);
 
    typename vector<Edge>::iterator i;
    while (!q.empty()) {
        size_t u = q.front();
        q.pop_front();
        for (i = adj[u].begin(); i != adj[u].end(); i++) {
            Edge &e = *i;
            if (level[e.v] < 0  && e.flow < e.C) {
                level[e.v] = level[u] + 1;
                q.push_back(e.v);
            }
        }
    }
 
    return level[t] < 0 ? false : true ;
}

double graph::sendFlow(size_t init_u, double init_flow, size_t t, vector<size_t>& start, vector<pair<bool,double>>& SnapShots)
{

    double retVal = 0;

    stack<size_t> SnapShotStack;

    SnapShots[init_u].first = false;
    SnapShots[init_u].second = init_flow;

    SnapShotStack.push(init_u);
    while (!SnapShotStack.empty()) {
        size_t u = SnapShotStack.top();

        if (u == t) {
            retVal = SnapShots[u].second;
            SnapShotStack.pop();
            continue;
        }
        Edge& e = adj[u][start[u]];
        double flow = SnapShots[u].second;

        if (SnapShots[u].first) {
            if (retVal > 0) {
                e.flow += retVal;
                adj[e.v][e.rev].flow -= retVal;
            }

            if (retVal <= 0 && (start[u] + 1) < adj[u].size()) {
                start[u]++;
                Edge& new_e = adj[u][start[u]];
                if (level[new_e.v] == level[u] + 1 && new_e.flow < new_e.C) {
                    double curr_flow = min(flow, new_e.C - new_e.flow);
                    SnapShots[new_e.v].first = false;
                    SnapShots[new_e.v].second = curr_flow;
                    SnapShotStack.push(new_e.v);
                }
            }
            else {
                SnapShotStack.pop();
            }
        }
        else {
            SnapShots[u].first = true;
            if (level[e.v] == level[u]+1 && e.flow < e.C) {
                double curr_flow = min(flow, e.C - e.flow);
                SnapShots[e.v].first = false;
                SnapShots[e.v].second = curr_flow;
                SnapShotStack.push(e.v);
            }
        }
    }

    return retVal;
}

void graph::find_cut(size_t u_init, vector<bool>& mincut, size_t& length)
{
    stack <size_t> stk;
    stk.push(u_init);
    while (!stk.empty()) {
        size_t u = stk.top();
        stk.pop();
        if (mincut[u] == true) {
            continue;
        }
        mincut[u] = true;
        length ++;
        for (int i = 0 ; i < adj[u].size(); i ++) {
            int k = adj[u].size() - 1 - i;
            Edge &e = adj[u][k];
            if (e.flow < e.C && mincut[e.v] == false) {
                stk.push(e.v);
            }
        }
    }
}

pair<double,size_t> graph::DinicMaxflow(size_t s, size_t t, size_t V, vector<bool>& mincut)
{
    if (s == t)
        return make_pair(-1,0);
 
    double total = 0;

    vector<size_t> start(V+1);
    vector<pair<bool,double>> SnapShots(V+1);

    while (BFS(s, t, V) == true){
        fill(start.begin(),start.end(),0);
        double flow = sendFlow(s, numeric_limits<double>::max(), t, start, SnapShots);
        while (flow > 0) {
            total += flow;
            flow = sendFlow(s, numeric_limits<double>::max(), t, start, SnapShots);
        }
    }
    size_t length = 0;
    fill(mincut.begin(),mincut.end(),false);
    find_cut(s,mincut,length);
    return make_pair(total,length);
}

pair<double, size_t> graph::Maxflow(size_t s, size_t t, vector<bool>& mincut) {
    make_adj_list();
    return DinicMaxflow(s, t, n, mincut);
}