#include<vector>
#include<stdexcept>
#include<cmath>

#include "flow_graph.hpp"

using namespace std;


FlowEdge::FlowEdge(int v, int w, int cap)
{
    this->v = v;
    this->w = w;
    this->cap = cap;
    this->flo = 0;
}

int FlowEdge::from()
{
    return v;
}

int FlowEdge::to()
{
    return w;
}

int FlowEdge::capacity()
{
    return cap;
}

int FlowEdge::flow()
{
    return flo;
}

int FlowEdge::other(int vertex)
{
    if (vertex == v) { return w; }
    else if (vertex == w) { return v; }
    else { throw invalid_argument("Not a valid edge end-point."); }
}

int FlowEdge::residualCapacityTo(int vertex)
{
    if (vertex == v) { return flo; }
    else if (vertex == w) { return cap - flo; }
    else { throw invalid_argument("Not a valid edge end-point."); }
}

void FlowEdge::addResidualFlowTo(int vertex, int delta)
{
    if (vertex == v) { flo -= delta; }
    else if (vertex == w) { flo += delta; }
    else { throw invalid_argument("Not a valid edge end-point."); }
}


FlowGraph::FlowGraph(int V) : adjacent(V), indeg(V, 0), outdeg(V, 0)
{
    this->V = V;
}

int FlowGraph::noVertices()
{
    return V;
}

void FlowGraph::addEdge(int v, int w, int cap)
{
    FlowEdge* e = new FlowEdge(v, w, cap);
    outdeg[v]++;
    indeg[w]++;

    adjacent[v].push_back(e);
    adjacent[w].push_back(e);
}

vector<FlowEdge*>& FlowGraph::adj(int v)
{
    return adjacent[v];
}

int FlowGraph::source()
{
    for (int i = 0; i < V; i++)
    {
        if (!indeg[i]) { return i; }
    }
    throw invalid_argument("No source found");
}

int FlowGraph::sink()
{
    for (int i = 0; i < V; i++)
    {
        if (!outdeg[i]) { return i; }
    }
    throw invalid_argument("No sink found");
}
