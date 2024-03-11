#pragma once
#include <vector>
#include <stack>

class Solution 
{
public:
    std::vector<int> findOrder(int numCourses, std::vector<std::vector<int>>& prerequisites) 
    {
        std::vector<std::vector<int>> list(numCourses);

        for (std::vector<int>& edge : prerequisites)
        {
            list[edge[0]].push_back(edge[1]);
        }

        std::vector<unsigned char> colors(numCourses);
        std::stack<int> stack;

        std::vector<int> result;
        result.reserve(numCourses);

        int i = 0;

        while (true)
        {
            int vertex;

            if (stack.empty())
            {
                while (i < numCourses && colors[i] == 2)
                {
                    ++i;
                }
                if (i == numCourses)
                {
                    break;
                }
                vertex = i;
                stack.push(vertex);
            }
            else
            {
                vertex = stack.top();
            }

            if (colors[vertex] == 2)
            {
                stack.pop();
                continue;
            }

            colors[vertex] = 1;

            bool indicator = true;

            for (int next_vertex : list[vertex])
            {
                switch (colors[next_vertex])
                {
                case 0:
                    stack.push(next_vertex);
                    indicator = false;
                    break;
                case 1:
                    return std::vector<int>();
                default:
                    break;
                }
            }

            if (indicator)
            {
                colors[vertex] = 2;
                result.push_back(vertex);
                stack.pop();
            }
        }
        
        return result;
    }
};