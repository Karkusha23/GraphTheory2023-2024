#pragma once
#include <vector>
#include <stack>

int sumDist(const std::vector<std::vector<int>>& list, int node)
{
    int n = list.size();

    std::stack<int> stack;
    stack.push(node);

    std::vector<int> dist(n, 0);

    while (!stack.empty())
    {
        int vertex = stack.top();
        stack.pop();

        for (const int& neighbor : list[vertex])
        {
            if (!dist[neighbor] && neighbor != node)
            {
                dist[neighbor] = dist[vertex] + 1;
                stack.push(neighbor);
            }
        }
    }

    int sum = 0;

    for (const int& val : dist)
    {
        sum += val;
    }

    return sum;
}

int getCount(const std::vector<std::vector<int>>& list, std::vector<int>& counts, int node, int parent)
{
    for (const int& neighbor : list[node])
    {
        if (neighbor != parent)
        {
            counts[node] += getCount(list, counts, neighbor, node);
        }
    }
    return ++counts[node];
}


class Solution 
{
public:
    std::vector<int> sumOfDistancesInTree(int n, std::vector<std::vector<int>>& edges) 
    {
        // Adjacency list
        std::vector<std::vector<int>> list(n);

        for (std::vector<int>& edge : edges)
        {
            list[edge[0]].push_back(edge[1]);
            list[edge[1]].push_back(edge[0]);
        }

        std::vector<int> counts(n);
        getCount(list, counts, 0, -1);

        std::vector<int> sums(n, -1);
        sums[0] = sumDist(list, 0);

        std::stack<int> stack;
        stack.push(0);

        while (!stack.empty())
        {
            int vertex = stack.top();
            stack.pop();

            for (const int& neighbor : list[vertex])
            {
                if (sums[neighbor] < 0)
                {
                    sums[neighbor] = sums[vertex] + n - 2 * counts[neighbor];
                    stack.push(neighbor);
                }
            }
        }

        return sums;
    }
};