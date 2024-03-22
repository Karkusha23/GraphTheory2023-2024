#pragma once
#include <vector>
#include <stack>
#include <algorithm>

struct SolutionNode
{
    int vertex;
    int parent;
    bool canBeHalved;
};

class Solution 
{
public:
    int minimumTotalPrice(int n, std::vector<std::vector<int>>& edges, std::vector<int>& price, std::vector<std::vector<int>>& trips) 
    {
        // Building adjacency list

        std::vector<std::vector<int>> list(n);

        for (std::vector<int>& edge : edges)
        {
            list[edge[0]].push_back(edge[1]);
            list[edge[1]].push_back(edge[0]);
        }

        // Count how many times each vertex was visited

        std::vector<int> freq(n, 0);

        for (std::vector<int>& trip : trips)
        {
            int start = trip[0];
            int end = trip[1];

            if (start == end)
            {
                ++freq[start];
                continue;
            }

            std::vector<int> parents(n, -1);
            parents[start] = start;

            std::stack<int> stack;
            stack.push(start);

            while (true)
            {
                int vertex = stack.top();
                stack.pop();

                bool indicator = false;

                for (int neighbor : list[vertex])
                {
                    if (parents[neighbor] < 0)
                    {
                        parents[neighbor] = vertex;

                        if (neighbor == end)
                        {
                            indicator = true;
                            break;
                        }

                        stack.push(neighbor);
                    }
                }

                if (indicator)
                {
                    break;
                }
            }

            int now = end;

            while (parents[now] != now)
            {
                ++freq[now];
                now = parents[now];
            }

            ++freq[now];
        }

        for (int i = 0; i < n; ++i)
        {
            freq[i] *= price[i];
        }

        // Dynamic programming
        
        std::vector<std::vector<int>> solutions(2, std::vector<int>(n, -1));

        std::stack<SolutionNode> stackSol;
        stackSol.push({ 0, -1, true });

        while (!stackSol.empty())
        {
            SolutionNode node = stackSol.top();

            int subsumNotHalved = freq[node.vertex];
            int subsumHalved = freq[node.vertex] / 2;

            bool indicator = true;

            for (int neighbor : list[node.vertex])
            {
                if (neighbor != node.parent)
                {
                    if (solutions[0][neighbor] < 0 || solutions[1][neighbor] < 0)
                    {
                        indicator = false;
                        if (solutions[0][neighbor] < 0)
                        {
                            stackSol.push({ neighbor, node.vertex, false });
                        }
                        if (solutions[1][neighbor] < 0)
                        {
                            stackSol.push({ neighbor, node.vertex, true });
                        }
                    }
                    else
                    {
                        subsumNotHalved += solutions[1][neighbor];
                        subsumHalved += solutions[0][neighbor];
                    }
                }
            }

            if (indicator)
            {
                stackSol.pop();

                if (node.canBeHalved)
                {
                    solutions[1][node.vertex] = std::min(subsumHalved, subsumNotHalved);
                }
                else
                {
                    solutions[0][node.vertex] = subsumNotHalved;
                }
            }
        }

        return solutions[1][0];
    }
};