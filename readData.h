#pragma once
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <algorithm>

using namespace std;

struct edge {
    size_t u;
    size_t v;
    double w;
};

bool myobject(edge i, edge j);

void list_to_CSR(size_t m, size_t n, int offset, size_t* ei, size_t* ej, double* w, size_t* ai, size_t* aj, double* a);

void readList(const char* filename, size_t& m, size_t& n, size_t*& ei, size_t*& ej, double*& w, bool needWeights);

void read_and_convert(const char* filename, int offset, size_t& nverts, size_t& nedges, size_t*& ret_ai, size_t*& ret_aj, double*& ret_a, bool needWeights = true);

void read_seed(const char* filename, size_t& n, size_t*& ids);