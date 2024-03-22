#pragma once
#include <vector>
#include <stack>

typedef long long ll;

ll distanceSqr(ll x1, ll y1, ll x2, ll y2)
{
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

class Solution 
{
public:
    int maximumDetonation(std::vector<std::vector<int>>& bombs) 
    {
        int n = bombs.size();

        // Adjacenty list
        std::vector<std::vector<int>> list(n);

        for (int i = 0; i < n - 1; ++i)
        {
            for (int j = i + 1; j < n; ++j)
            {
                ll dist = distanceSqr(bombs[i][0], bombs[i][1], bombs[j][0], bombs[j][1]);

                if ((ll)bombs[i][2] * (ll)bombs[i][2] >= dist)
                {
                    list[i].push_back(j);
                }
                if ((ll)bombs[j][2] * (ll)bombs[j][2] >= dist)
                {
                    list[j].push_back(i);
                }
            }
        }

        int maxCount = 0;

        for (int i = 0; i < n; ++i)
        {
            std::vector<bool> visited(n, false);
            visited[i] = true;

            int count = 1;

            std::stack<int> stack;
            stack.push(i);

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

            if (count > maxCount)
            {
                maxCount = count;
            }
        }

        return maxCount;
    }
};