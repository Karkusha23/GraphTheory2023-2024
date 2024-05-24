#include "readData.h"

bool myobject(edge i, edge j) {
    if (i.u < j.u) {
        return true;
    }
    else if (i.u == j.u) {
        return (i.v < j.v);
    }
    else {
        return false;
    }
}

void list_to_CSR(size_t m, size_t n, int offset, size_t* ei, size_t* ej, double* w, size_t* ai, size_t* aj, double* a) {
    fill(ai, ai + m + 1, 0);
    edge* edge_list = new edge[n];
    for (size_t i = 0; i < n; i++) {
        edge_list[i].u = ei[i] - offset;
        edge_list[i].v = ej[i] - offset;
        edge_list[i].w = w[i];
    }
    sort(edge_list, edge_list + n, myobject);
    for (size_t i = 0; i < n; i++) {
        ai[edge_list[i].u + 1]++;
        aj[i] = edge_list[i].v;
        a[i] = edge_list[i].w;
    }
    for (size_t i = 1; i <= m; i++) {
        ai[i] += ai[i - 1];
    }
    delete[] edge_list;
}

void readList(const char* filename, size_t& m, size_t& n, size_t*& ei, size_t*& ej, double*& w, bool needWeights) {

    std::ifstream inFile(filename);

    if (!inFile)
    {
        throw std::exception("Can not open file to read");
    }

    inFile >> m >> n;

    ei = new size_t[n];
    ej = new size_t[n];
    w = new double[n];

    for (size_t i = 0; i < n; ++i)
    {
        inFile >> (ei)[i];
        inFile >> (ej)[i];
        if (needWeights)
            inFile >> (w)[i];
        else
            (w)[i] = 1.0;
    }

    inFile.close();
}

void read_and_convert(const char* filename, int offset, size_t& nverts, size_t& nedges, size_t*& ret_ai, size_t*& ret_aj, double*& ret_a, bool needWeights) {
    size_t* ei = nullptr;
    size_t* ej = nullptr;
    double* w = nullptr;
    size_t m = 0;
    size_t n = 0;
    readList(filename, m, n, ei, ej, w, needWeights);
    size_t* ai = new size_t[m + 1];
    size_t* aj = new size_t[n];
    double* a = new double[n];
    list_to_CSR(m, n, offset, ei, ej, w, ai, aj, a);
    delete[] ei;
    delete[] ej;
    delete[] w;

    nverts = m;
    nedges = n;
    ret_ai = ai;
    ret_aj = aj;
    ret_a = a;
}

#include <sstream>

void read_seed(const char* filename, size_t& n, size_t*& ids) {
    std::ifstream inFile(filename);

    if (!inFile) {
        throw std::exception("Can not open file to read");
    }
    inFile >> n;
    size_t* seedids = (size_t*)malloc(sizeof(size_t) * n);
    for (size_t i = 0; i < n; i++) {
        inFile >> seedids[i];
    }
    ids = seedids;
}

