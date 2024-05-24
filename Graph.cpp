#include "Graph.h"

graph::graph() : n(0), m(0), ai(nullptr), aj(nullptr), a(nullptr),
degrees(nullptr), volume(0), level(nullptr) {}

graph::graph(size_t _m, size_t _n, size_t* _ai, size_t* _aj, double* _a,
    double* _degrees) {
    m = _m;
    n = _n;
    ai = _ai;
    aj = _aj;
    a = _a;
    degrees = _degrees;
    volume = 0;
    if (ai != NULL) {
        volume = (double)ai[n];
    }
    adj.resize(n);
    for (int i = 0; i < n; i++) {
        adj[i].clear();
    }
    level = NULL;

}

graph::graph(const char* file_path, int indOffset, bool needWeights) : n(0), m(0), ai(nullptr), aj(nullptr), a(nullptr),
degrees(nullptr), volume(0), level(nullptr) {
    read_and_convert(file_path, indOffset, n, m, ai, aj, a, needWeights);
    if (ai != nullptr) {
        volume = (double)ai[n];
    }
    level = new int[n];
    adj.resize(n + 2);
}

graph::~graph() {
    delete[] ai;
    ai = nullptr;
    delete[] aj;
    aj = nullptr;
    delete[] a;
    a = nullptr;
    delete[] degrees;
    degrees = nullptr;
    delete[] level;
    level = nullptr;
}

graph& graph::operator=(graph&& X) {
    if (this == &X)
        return *this;

    m = X.m;
    n = X.n;
    volume = X.volume;

    delete[] ai;
    ai = X.ai;
    X.ai = nullptr;

    delete[] aj;
    aj = X.aj;
    X.aj = nullptr;

    delete[] a;
    a = X.a;
    X.a = nullptr;

    delete[] degrees;
    degrees = X.degrees;
    X.degrees = nullptr;

    delete[] level;
    level = X.level;
    X.level = nullptr;

    adj = X.adj;

    return *this;
}

void graph::addEdge(size_t u, size_t v, double C, bool isDirected) {
    Edge p{ v, 0, C, (size_t)adj[v].size() };

    Edge q{ u, 0, isDirected ? 0 : C, (size_t)adj[u].size() };

    adj[u].push_back(p);
    adj[v].push_back(q);
}

size_t graph::get_vertex_number() const {
    return n;
}

size_t graph::get_edges_number() const {
    return m;
}

size_t graph::get_degree_unweighted(size_t id)
{
    return ai[id + 1] - ai[id];
}

double graph::get_modularity(const unordered_map<size_t, size_t>& R_map)
{
    bool* in_cluster = new bool[n];

    for (size_t i = 0; i < n; ++i)
    {
        in_cluster[i] = false;
    }

    for (auto it = R_map.begin(); it != R_map.end(); ++it)
    {
        in_cluster[it->first] = true;
    }

    size_t Lc = 0;

    size_t k_in = 0;
    size_t k_out = 0;

    for (size_t i = 0; i < n; ++i)
    {
        for (size_t j = ai[i]; j < ai[i + 1]; ++j)
        {
            if (in_cluster[i])
            {
                ++k_out;

                if (in_cluster[aj[j]])
                {
                    ++Lc;
                }
            }

            if (in_cluster[aj[j]])
            {
                ++k_in;
            }
        }
    }

    delete[] in_cluster;

    double tmp = double(k_in * k_out) / double(2 * m);

    return (double(Lc) / double(m)) - tmp * tmp;
}

Stats graph::get_stats(unordered_map<size_t, size_t>& R_map, size_t nR, bool need_mod)
{
    size_t curvol = 0;
    size_t curcutsize = 0;
    for (auto R_iter = R_map.begin(); R_iter != R_map.end(); ++R_iter) {
        size_t v = R_iter->first;
        size_t deg = get_degree_unweighted(v);
        curvol += deg;
        for (size_t j = ai[v]; j < ai[v + 1]; j++) {
            if (R_map.count(aj[j]) == 0) {
                curcutsize++;
            }
        }
    }

    if (need_mod) {
        double modularity = get_modularity(R_map);
        return Stats(curvol, curcutsize, modularity);
    }
    return Stats(curvol, curcutsize, 0.0);
}

void graph::make_adj_list()
{
    for (int i = 0; i < n + 2; i++)
    {
        adj[i].clear();
    }
    for (int row = 0; row < n; ++row) 
    {
        int len = ai[row + 1] - ai[row];
        for (int i = 0; i < len; ++i) 
        {
            addEdge(row, aj[ai[row] + i], a[ai[row] + i], true);
        }
    }
}
