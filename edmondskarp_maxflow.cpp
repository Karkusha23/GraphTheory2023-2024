#include "Graph.h"
#include <queue>

bool graph::findShortestPath(size_t s, size_t t, vector<Edge*>& parents, double& flow)
{
	parents.assign(n, nullptr);

	vector<int> visited(n, false);

	vector<size_t> dist(n, -1);
	dist[s] = 0;

	vector<double> minC(n, numeric_limits<double>::max());

	auto cmp = [&dist](size_t left, size_t right) { return dist[left] < dist[right]; };
	priority_queue<size_t, vector<size_t>, decltype(cmp)> queue(cmp);
	queue.push(s);

	while (!queue.empty())
	{
		size_t now = queue.top();
		queue.pop();

		if (visited[now])
		{
			continue;
		}

		if (now == t)
		{
			visited[now] = true;
			break;
		}

		for (Edge& edge : adj[now])
		{
			if (visited[edge.v] || edge.flow == edge.C)
			{
				continue;
			}

			if (dist[now] + 1 < dist[edge.v])
			{
				dist[edge.v] = dist[now] + 1;
				minC[edge.v] = min(minC[now], edge.C - edge.flow);
				parents[edge.v] = &edge;
			}

			queue.push(edge.v);
		}

		visited[now] = true;
	}

	flow = minC[t];

	return visited[t];
}

pair<double, size_t> graph::EdmondsKarpMaxFlow(size_t s, size_t t, vector<bool>& mincut)
{
	make_adj_list();

	vector<Edge*> parents;
	double flow;

	double resultFlow = 0;

	while (findShortestPath(s, t, parents, flow))
	{
		Edge* now = parents[t];

		while (now != nullptr)
		{
			now->flow += flow;

			Edge* rev = &adj[now->v][now->rev];

			if (rev->C == 0)
			{
				rev->C = rev->flow = now->C;
			}

			rev->flow -= flow;

			now = parents[rev->v];
		}

		resultFlow += flow;
	}

	size_t length;

	find_cut(s, mincut, length);

	return make_pair(resultFlow, length);
}