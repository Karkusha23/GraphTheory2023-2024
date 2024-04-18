#pragma once
#include <vector>
#include <algorithm>

class Solution 
{
public:
    long long maximumImportance(int n, std::vector<std::vector<int>>& roads) 
    {
        std::vector<int> deg(n, 0);

        for (std::vector<int>& road : roads)
        {
            ++deg[road[0]];
            ++deg[road[1]];
        }

        std::vector<int> vertex(n);

        for (int i = 0; i < n; ++i)
        {
            vertex[i] = i;
        }

        std::sort(vertex.begin(), vertex.end(), [&deg](int first, int second) { return deg[first] < deg[second]; });

        std::vector<int> numbers(n);

        for (int i = 0; i < n; ++i)
        {
            numbers[vertex[i]] = i + 1;
        }

        long long result = 0;

        for (int i = 0; i < n; ++i)
        {
            result += (long long)numbers[i] * (long long)deg[i];
        }

        return result;
    }
};