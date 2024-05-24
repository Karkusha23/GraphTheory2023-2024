#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <unordered_map>
#include <utility>
#include <stdint.h>
#include <typeinfo>

#include "Graph.h"

using namespace std;

void graph::build_map(unordered_map<size_t, size_t>& R_map,
	unordered_map<size_t, size_t>& degree_map, size_t* R, size_t nR) {
	size_t deg;
	for (size_t i = 0; i < nR; i++) {
		R_map[R[i]] = i;
	}
	for (auto iter = R_map.begin(); iter != R_map.end(); ++iter) {
		size_t u = iter->first;
		deg = get_degree_unweighted(u);
		for (size_t j = ai[u]; j < ai[u + 1]; j++) {
			size_t v = aj[j];
			if (R_map.count(v) > 0) {
				deg--;
			}
		}
		degree_map[u] = deg;
	}
}

void graph::build_list(unordered_map<size_t, size_t>& R_map, unordered_map<size_t, size_t>& degree_map,
	size_t src, size_t dest, size_t A, size_t C) {
	for (auto R_iter = R_map.begin(); R_iter != R_map.end(); ++R_iter) {
		size_t u = R_iter->first;
		size_t u1 = R_iter->second;
		for (size_t j = ai[u]; j < ai[u + 1]; j++) {
			size_t v = aj[j];
			auto got = R_map.find(v);
			if (R_map.count(v) > 0) {
				size_t v1 = got->second;
				double w = A;
				if (v1 > u1) {
					addEdge(u1, v1, w);
				}
			}
		}
	}
	for (auto R_iter = R_map.begin(); R_iter != R_map.end(); ++R_iter) {
		size_t u1 = src;
		size_t v = R_iter->first;
		size_t v1 = R_iter->second;
		auto got = degree_map.find(v);
		double w = A * got->second;
		addEdge(u1, v1, w);
		u1 = v1;
		v1 = dest;
		size_t u = v;
		w = C * get_degree_unweighted(u);
		addEdge(u1, v1, w);
	}
}

size_t graph::MQI(size_t nR, size_t* R, size_t* ret_set, Stats& beforeStats, Stats& afterStats){
	size_t total_iter = 0;
	unordered_map<size_t, size_t> R_map;
	unordered_map<size_t, size_t> old_R_map;
	unordered_map<size_t, size_t> degree_map;
	build_map(R_map, degree_map, R, nR);
	size_t nedges = 0;
	double condOld = 1;
	double condNew;
	size_t total_degree = ai[n];
	Stats set_stats = get_stats(R_map, nR, true);
	beforeStats = set_stats;
	size_t curvol = set_stats.vol;
	size_t curcutsize = set_stats.cutsize;
	nedges = curvol - curcutsize + 2 * nR;
	if (curvol == 0 || curvol == total_degree) {
		size_t j = 0;
		for (auto R_iter = R_map.begin(); R_iter != R_map.end(); ++R_iter) {
			ret_set[j] = R_iter->first;
			j++;
		}
		return nR;
	}
	condNew = (double)curcutsize / (double)min(total_degree - curvol, curvol);
	total_iter++;
	size_t nverts = nR + 2;
	for (int i = 0; i < nverts; i++) {
		adj[i].clear();
	}
	vector<bool> mincut(nverts);
	build_list(R_map, degree_map, nR, nR + 1, curvol, curcutsize);
	pair<double, size_t> retData = DinicMaxflow(nR, nR + 1, nverts, mincut);
	size_t nRold = nR;
	size_t nRnew = 0;
	while (condNew < condOld) {
		nRnew = nRold - retData.second + 1;
		size_t* Rnew = (size_t*)malloc(sizeof(size_t) * nRnew);
		size_t iter = 0;
		for (auto R_iter = R_map.begin(); R_iter != R_map.end(); ++R_iter) {
			size_t u = R_iter->first;
			size_t u1 = R_iter->second;
			if (!mincut[u1]) {
				Rnew[iter] = u;
				iter++;
			}
		}
		condOld = condNew;
		old_R_map = R_map;
		R_map.clear();
		degree_map.clear();
		build_map(R_map, degree_map, Rnew, nRnew);
		set_stats = get_stats(R_map, nRnew);
		curvol = set_stats.vol;
		curcutsize = set_stats.cutsize;
		nedges = curvol - curcutsize + 2 * nRnew;
		if (nRnew > 0) {
			condNew = (double)curcutsize / (double)min(total_degree - curvol, curvol);
			//cout << "curvol: " << curvol << " condNew: " << condNew << endl;
			nverts = nRnew + 2;
			for (int i = 0; i < nverts; i++) {
				adj[i].clear();
			}
			build_list(R_map, degree_map, nRnew, nRnew + 1, curvol, curcutsize);
			retData = DinicMaxflow(nRnew, nRnew + 1, nverts, mincut);
			//cout << "here " << nedges << " " << curvol << " " << curcutsize << endl;
		}
		else {
			size_t j = 0;
			for (auto R_iter = old_R_map.begin(); R_iter != old_R_map.end(); ++R_iter) {
				ret_set[j] = R_iter->first;
				j++;
			}
			afterStats = get_stats(old_R_map, nRold, true);
			return old_R_map.size();
		}
		free(Rnew);
		nRold = nRnew;
		total_iter++;
	}

	size_t j = 0;
	for (auto R_iter = old_R_map.begin(); R_iter != old_R_map.end(); ++R_iter) {
		ret_set[j] = R_iter->first;
		j++;
	}
	afterStats = get_stats(old_R_map, nRold, true);
	return old_R_map.size();
}