#pragma once
#include <vector>
#include <algorithm>

class Solution
{
public:
    std::vector<int> findSmallestSetOfVertices(int n, std::vector<std::vector<int>>& edges) 
    {
        std::vector<int> in_degrees(n);

        for (std::vector<int>& edge : edges)
        {
            ++in_degrees[edge[1]];
        }

        std::vector<int> result;

        for (int i = 0; i < n; ++i)
        {
            if (!in_degrees[i])
            {
                result.push_back(i);
            }
        }

        return result;
    }
};