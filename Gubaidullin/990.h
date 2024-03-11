#pragma once
#include <vector>
#include <string>

class UnionFind
{
    std::vector<int> arr_;
public:
    UnionFind(int count) : arr_(count)
    {
        for (int i = 0; i < count; ++i)
        {
            arr_[i] = i;
        }
    }
    int parent(int val)
    {
        return arr_[val] == val ? val : arr_[val] = parent(arr_[val]);
    }
    void unite(int first, int second)
    {
        arr_[parent(first)] = parent(second);
    }
    bool sameParent(int first, int second)
    {
        return parent(first) == parent(second);
    }
};

class Solution 
{
public:
    bool equationsPossible(std::vector<std::string>& equations) 
    {
        UnionFind unionFind(26);

        for (std::string& str : equations)
        {
            if (str[0] == str[3])
            {
                if (str[1] == '=')
                {
                    continue;
                }
                else
                {
                    return false;
                }
            }

            if (str[1] == '=')
            {
                unionFind.unite(str[0] - 'a', str[3] - 'a');
            }
        }

        for (std::string& str : equations)
        {
            if (str[1] == '!' && unionFind.sameParent(str[0] - 'a', str[3] - 'a'))
            {
                return false;
            }
        }

        return true;
    }
};