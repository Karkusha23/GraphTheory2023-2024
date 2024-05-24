#include <iostream>
#include <iomanip>
#include <chrono>

#include "Graph.h"
#include "readData.h"



int main(int argc, char* argv[]) {

    if (argc < 3) {
        std::cout << "Not enough arguments" << std::endl;
        return 1;
    }

    if (argc > 5) {
        std::cout << "Too many arguments" << std::endl;
        return 1;
    }

    graph gr;
    if (argv[2][0] == '-' && argv[2][1] == 'w') {
        gr = graph(argv[3], 0, true);
    }
    else {
        gr = graph(argv[2], 0, false);
    }

    vector<bool> mincut(gr.get_vertex_number(), false);


    if (argv[1][0] == '-' && argv[1][1] == 'f') {
        auto time_start = std::chrono::high_resolution_clock::now();
        auto ret = gr.EdmondsKarpMaxFlow(0, gr.get_vertex_number() - 1, mincut);
        auto time_end = std::chrono::high_resolution_clock::now();
        std::cout << ret.first << std::endl << std::setprecision(6) << std::chrono::duration<double, std::milli>(time_end - time_start).count() << std::endl;

        mincut.assign(gr.get_vertex_number(), false);

        time_start = std::chrono::high_resolution_clock::now();
        ret = gr.Maxflow(0, gr.get_vertex_number() - 1, mincut);
        time_end = std::chrono::high_resolution_clock::now();
        std::cout << ret.first << std::endl << std::setprecision(6) << std::chrono::duration<double, std::milli>(time_end - time_start).count() << std::endl;

        return 0;
    }
    else if (argv[1][0] == '-' && argv[1][1] == 'c') {
        size_t* R, nR;
        read_seed(argv[argc - 1], nR, R);
        Stats before, after;
        size_t* ret = new size_t[nR];
        size_t nRet = gr.MQI(nR, R, ret, before, after);
        std::cout << nRet << std::endl;
        for (size_t i = 0; i < nRet; ++i) {
            std::cout << ret[i] << " ";
        }
        std::cout << std::endl;
        std::cout << (float)before.cutsize / (float)before.vol << " " << before.modularity << std::endl;
        std::cout << (float)after.cutsize / (float)after.vol << " " << after.modularity << std::endl;
    }

    return 0;
}