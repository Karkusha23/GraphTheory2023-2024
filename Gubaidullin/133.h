#pragma once
#include <vector>
#include <algorithm>
#include <stack>

class Node 
{
public:
    int val;
    std::vector<Node*> neighbors;
    Node() 
    {
        val = 0;
        neighbors = std::vector<Node*>();
    }
    Node(int _val)
    {
        val = _val;
        neighbors = std::vector<Node*>();
    }
    Node(int _val, std::vector<Node*> _neighbors) 
    {
        val = _val;
        neighbors = _neighbors;
    }
};

struct NodePair
{
    Node* oldNode;
    Node* newNode;
};

class Solution 
{
public:

    Node* cloneGraph(Node* node) 
    {
        if (!node)
        {
            return nullptr;
        }

        std::vector<Node*> vertices(100);
        std::generate(vertices.begin(), vertices.end(), [](){ return nullptr; });

        std::stack<NodePair> stack;

        Node* to_return = new Node(node->val);
        vertices[node->val - 1] = to_return;

        stack.push({ node, to_return });

        while (!stack.empty())
        {
            NodePair pair = stack.top();
            stack.pop();

            for (Node* neighbor : pair.oldNode->neighbors)
            {
                if (vertices[neighbor->val - 1])
                {
                    pair.newNode->neighbors.push_back(vertices[neighbor->val - 1]);
                }
                else
                {
                    Node* newestNode = new Node(neighbor->val);
                    pair.newNode->neighbors.push_back(newestNode);
                    vertices[neighbor->val - 1] = newestNode;
                    stack.push({ neighbor, newestNode });
                }
            }
        }

        return to_return;
    }
};