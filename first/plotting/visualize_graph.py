import argparse

import matplotlib.pyplot as plt
import matplotlib.colors as colors
import networkx as nx


def generate_colors():

    golden_ratio_conjugate = 0.618033988749895
    hue = 0

    while True:
        hue += golden_ratio_conjugate
        hue %= 1

        r, g, b = colors.hsv_to_rgb((hue, 1, 0.7))
        hex_val = colors.to_hex((r, g, b))

        yield hex_val


if __name__ == "__main__":

    parser = argparse.ArgumentParser(
        prog="visualize_graph",
        description="Display graph and its strongly-connected components")
    parser.add_argument("graph_name", type=str, help="name of the graph")
    args = parser.parse_args()

    graph = nx.DiGraph()
    edges = set()

    no_of_vertices, no_of_edges = 0, 0
    with open(args.graph_name, "r") as graph_file:
        graph_info = graph_file.readlines()

        no_of_vertices, no_of_edges = map(int, graph_info[0].split())
        for line in graph_info[1:]:
            if len(line) == 0:
                continue

            x, y = map(int, line.split())
            graph.add_edge(x, y)
            edges.add((x, y))

    pos = nx.spring_layout(graph, k=0.5, iterations=20)

    scc_sets = nx.strongly_connected_components(graph)
    color_list = generate_colors()
    no_of_scc = 0

    for index, component_vertices in enumerate(scc_sets):

        no_of_scc += 1
        component = graph.subgraph(component_vertices)
        component_color = next(color_list)

        nx.draw_networkx_nodes(
            graph, pos, with_labels=True, nodelist=component_vertices,
            node_color=component_color, alpha=0.7, cmap=plt.cm.jet)

        labels = {vertex: vertex for vertex in component_vertices}
        nx.draw_networkx_labels(graph, pos, labels, font_size=12)

        nx.draw_networkx_edges(
            graph, pos,
            edgelist=component.edges, edge_color=component_color,
            alpha=0.6, width=2.5, cmap=plt.cm.jet)
        edges = edges.difference(component.edges)

    nx.draw_networkx_edges(
            graph, pos,
            edgelist=edges, alpha=0.6)

    plt.title(f"Vertices - {no_of_vertices}, Edges - {no_of_edges}, Components - {no_of_scc}", y=-0.1)
    plt.show()
    plt.clf()
