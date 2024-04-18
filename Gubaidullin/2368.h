#pragma once
#include <vector>
#include <stack>

class Solution 
{
public:
    int reachableNodes(int n, std::vector<std::vector<int>>& edges, std::vector<int>& restricted) 
    {
        std::vector<bool> isRestricted(n, false);

        for (int vertex : restricted)
        {
            isRestricted[vertex] = true;
        }

        // Adjacency list
        std::vector<std::vector<int>> list(n);

        for (std::vector<int>& edge : edges)
        {
            if (!isRestricted[edge[0]] && !isRestricted[edge[1]])
            {
                list[edge[0]].push_back(edge[1]);
                list[edge[1]].push_back(edge[0]);
            }
        }

        std::vector<bool> visited(n, false);
        visited[0] = true;

        int count = 1;

        std::stack<int> stack;
        stack.push(0);

        while (!stack.empty())
        {
            int vertex = stack.top();
            stack.pop();

            for (int neighbor : list[vertex])
            {
                if (!visited[neighbor])
                {
                    visited[neighbor] = true;
                    ++count;
                    stack.push(neighbor);
                }
            }
        }

        return count;
    }
};