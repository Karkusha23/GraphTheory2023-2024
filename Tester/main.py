import networkx as nx
import subprocess
from pathlib import Path
from random import randrange
import matplotlib.pyplot as plt
import numpy as np

TESTS_FOLDER = 'C:/Users/minri/source/repos/GraphProject/Tests/'
#TESTS_FOLDER = './TestData/'
TEST_FILENAME = 'test{}.txt'
TEST_FILE_PATH = TESTS_FOLDER + TEST_FILENAME
EXE_PATH = '../x64/Debug/GraphProject.exe'

PLOTDATA_FOLDER = '../PlotData/'

MQITEST_FILENAME = 'C:/Users/minri/source/repos/GraphProject/MQITests/{}_R{}.txt'
MQITEST_EDGES_FILENAME = 'C:/Users/minri/source/repos/GraphProject/MQITests/{}_edgelist.txt'


def generate_graph(n: int, u: int, filename: str, p=0.0, m=0):
    graph = nx.gnp_random_graph(n, p, directed=True) if m == 0 else nx.gnm_random_graph(n, m, directed=True)
    nx.set_edge_attributes(graph, {e: {'weight': randrange(1, u)} for e in graph.edges})
    with Path(filename).open("wb") as file:
        file.write((str(graph.number_of_nodes())+' ').encode())
        file.write((str(graph.number_of_edges())+'\n').encode())
        nx.write_weighted_edgelist(graph, file)

    # out = open(filename, 'wt')
    # out.write(str(n) + ' ' + str(m) + '\n')
    # for edge in graph.edges:
    #     out.write(str(edge[0] + 1) + ' ' + str(edge[1] + 1) + ' ' + str(randrange(1, u + 1)) + '\n')
    # out.close()


def measure_time(n: int, u: int, count: int, p=0.0, m=0):
    edmonds_carp_time = 0.0
    dinic_time = 0.0

    for i in range(count):
        generate_graph(n=n, p=p, m=m, u=u, filename=TEST_FILE_PATH.format(i))
        proc = subprocess.Popen([EXE_PATH, '-f', '-w', TEST_FILE_PATH.format(i)], stdout=subprocess.PIPE)
        proc.wait()
        (proc.stdout.readline())
        edmonds_carp_time += float(proc.stdout.readline())
        (proc.stdout.readline())
        dinic_time += float(proc.stdout.readline())

    edmonds_carp_time /= count
    dinic_time /= count

    return edmonds_carp_time, dinic_time


def writefile_edge_difference(n: int):
    ps = [i / 20 for i in range(1, 21)]
    outfile = open(PLOTDATA_FOLDER + f'plot{n}.txt', 'wt')
    outfile.write(str(len(ps)) + ' 2\n')
    for p in ps:
        print(n, p)
        edmonds, dinic = measure_time(n=n, p=p, u=10000, count=50)
        outfile.write(str(p) + ' ' + str(edmonds) + ' ' + str(dinic) + '\n')
    outfile.close()


def writefile_vertex_difference(nmin: int, nmax: int, m: int):
    ns = [int(nmin + (nmax - nmin) * (i / 20)) for i in range(21)]
    outfile = open(PLOTDATA_FOLDER + f'plot_vertex{m}.txt', 'wt')
    outfile.write(str(len(ns)) + ' 2\n')
    for n in ns:
        print(n, m)
        edmonds, dinic = measure_time(n=n, m=m, u=10000, count=50)
        outfile.write(str(n) + ' ' + str(edmonds) + ' ' + str(dinic) + '\n')
    outfile.close()


def get_plotdata(filename: str):
    infile = open(PLOTDATA_FOLDER + filename, 'rt')
    data = infile.read().replace('\n', ' ').split(' ')
    infile.close()

    count = int(data[0])
    plot_count = int(data[1])

    x = [float(data[i * (plot_count + 1) + 2]) for i in range(count)]
    ys = [[float(data[i * (plot_count + 1) + 3 + j]) for i in range(count)] for j in range(plot_count)]

    return x, ys


def get_approx_plot(x, y, lin=False):
    if lin:
        return [x[0], x[-1]], [y[0], y[-1]]
    new_x = [x[0] + (x[-1] - x[0]) * (i / 50) for i in range(0, 51)]
    return new_x, [(val - x[0])**2 * ((y[-1] - y[0]) / (x[-1] - x[0])**2) + y[0] for val in new_x]



def draw_algorithm_diff(mn: int, edges=True, edmonds=True, dinic=True, approx=False):
    if edges:
        x, ys = get_plotdata(f'plot{mn}.txt')
        plt.title(f'Max flow test n={mn}')
        plt.xlabel('Edge probability')
        plt.ylabel('Avg execution time (ms)')
    else:
        x, ys = get_plotdata(f'plot_vertex{mn}.txt')
        plt.title(f'Max flow test m={mn}')
        plt.xlabel('Vertex count')
        plt.ylabel('Avg execution time (ms)')
    if edmonds:
        plt.plot(x, ys[0], label='Edmonds-Karp algorithm', color='green')
        if approx:
            if edges:
                x_apo, y_apo = get_approx_plot(x, ys[0])
                plt.plot(x_apo, y_apo, label='Quadratic function')
            else:
                x_apo, y_apo = get_approx_plot(x, ys[0], True)
                plt.plot(x_apo, y_apo, label='Linear function')
    if dinic:
        plt.plot(x, ys[1], label='Dinic Algorithm', color='blue')
        if approx:
            if edges:
                x_apo, y_apo = get_approx_plot(x, ys[1], True)
                plt.plot(x_apo, y_apo, label='Linear function')
            else:
                x_apo, y_apo = get_approx_plot(x, ys[1])
                plt.plot(x_apo, y_apo, label='Quadratic function')

    plt.legend()
    plt.show()


def draw_algorithm_multi(mn, edges=True, edmonds=True, approx=True):
    plt.title('Max flow test: ' + ('Edmonds-Karp' if edmonds else 'Dinic') + ' algorithm')
    if edges:
        plots = [get_plotdata(f'plot{i}.txt') for i in mn]
        plt.xlabel('Edge probability')
        plt.ylabel('Avg execution time (ms)')
    else:
        plots = [get_plotdata(f'plot_vertex{i}.txt') for i in mn]
        plt.xlabel('Vertex count')
        plt.ylabel('Avg execution time (ms)')
    label = ('n=' if edges else 'm=')

    index = 0 if edmonds else 1
    for i in range(len(plots)):
        plt.plot(plots[i][0], plots[i][1][index], label=label+str(mn[i]))
        if approx:
            x_apo, y_apo = get_approx_plot(plots[i][0], plots[i][1][index], edges != edmonds)
            plt.plot(x_apo, y_apo, label=('Linear function' if edges != edmonds else 'Quadratic function'))
    plt.legend()
    plt.show()


def test_MQI(setName: str, index: int):
    proc = subprocess.Popen([EXE_PATH, '-c', MQITEST_EDGES_FILENAME.format(setName), MQITEST_FILENAME.format(setName, index)], stdout=subprocess.PIPE)
    proc.wait()
    nR = int(proc.stdout.readline())
    R = [int(i) for i in proc.stdout.readline().split()]
    R.sort()
    s = proc.stdout.readline().split()
    bCond = float(s[0])
    bMod = float(s[1])
    s = proc.stdout.readline().split()
    aCond = float(s[0])
    aMod = float(s[1])
    return (nR, R, bCond, bMod, aCond, aMod)





if __name__ == '__main__':
    Path(TESTS_FOLDER).mkdir(parents=True, exist_ok=True)

    #draw_algorithm_diff(10000, edges=False, dinic=False)
    #draw_algorithm_multi([150, 175, 200], edges=True, edmonds=True, approx=True)

    writefile_vertex_difference(300, 400, 10000)

    #for i in range(1, 4):
    #    print(test_MQI('karate', i))