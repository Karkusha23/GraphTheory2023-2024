#pragma once
#include <vector>

class Solution 
{
    std::vector<std::vector<int>> list;
    std::vector<int> solutions;
    int n;
    int k;

    int minCourses(int bitMap)
    {
        if (bitMap == (1 << n) - 1)
        {
            return 0;
        }

        if (solutions[bitMap] >= 0)
        {
            return solutions[bitMap];
        }

        int zeroDegrees = 0;

        for (int i = 0; i < n; ++i)
        {
            if (bitMap & (1 << i))
            {
                continue;
            }

            bool indicator = true;

            for (const int& prev : list[i])
            {
                if (!(bitMap & (1 << prev)))
                {
                    indicator = false;
                    break;
                }
            }

            if (indicator)
            {
                zeroDegrees |= 1 << i;
            }
        }

        if (__builtin_popcount(zeroDegrees) <= k)
        {
            return solutions[bitMap] = minCourses(bitMap | zeroDegrees) + 1;
        }

        int min = n + 1;

        for (int newMap = (zeroDegrees - 1) & zeroDegrees; newMap; newMap = (newMap - 1) & zeroDegrees)
        {
            if (__builtin_popcount(newMap) != k)
            {
                continue;
            }

            min = std::min(min, minCourses(bitMap | newMap) + 1);
        }

        return solutions[bitMap] = min;
    }

public:

    int minNumberOfSemesters(int n, std::vector<std::vector<int>>& relations, int k)
    {
        this->n = n;
        this->k = k;

        // Adjacency list
        list.resize(n);

        for (const std::vector<int>& edge : relations)
        {
            list[edge[1] - 1].push_back(edge[0] - 1);
        }

        solutions.assign(1 << n, -1);

        return minCourses(0);
    }
};