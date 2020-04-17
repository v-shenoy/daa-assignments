import argparse

import matplotlib.pyplot as plt


if __name__ == "__main__":

    parser = argparse.ArgumentParser(
        prog="visualize_graph",
        description="Display graph and its strongly-connected components")
    parser.add_argument("result_file_name", type=str, help="name of the results file")
    args = parser.parse_args()

    results = []
    with open(args.result_file_name, "r") as result_file:
        lines = result_file.readlines()

        for line in lines:
            line = line.split()
            graph = line[0].split("/")[-1]
            edges = float(line[2])
            flow = float(line[3])
            processing_time = float(line[-1])

            results.append([graph, edges * flow, processing_time])

        results = sorted(results, key=lambda x: x[1])

    graphs, graph_size, processing_time = [], [], []
    for result in results:
        print(result)
        graphs.append(result[0])
        graph_size.append(result[1])
        processing_time.append(result[2])

    plt.plot(graph_size, processing_time)
    plt.scatter(graph_size, processing_time, color="red")

    for idx, graph in enumerate(graphs):
        x = graph_size[idx]
        y = processing_time[idx]

    plt.title(f"Time v/s Graph ( E * f )", y=-0.15)
    plt.show()
    plt.clf()
