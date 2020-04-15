#include<vector>
#include<queue>
#include<limits>
#include<cmath>
#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
#include<unordered_map>
#include<iomanip>
#include <string.h>
#include<stdexcept>

#include "flow_graph.hpp"
#include "ff.hpp"


using namespace std;

/**
 * Macro short-handfor printing a decimal x with a precision of y.
 */
#define FIXED_FLOAT(x, y) fixed<<setprecision(y)<<(x)


FordFulkerson::FordFulkerson(FlowGraph* G, int s, int t)
{
    V = G->noVertices();
    visited.resize(V);
    parentEdge.resize(V);

    initDelta(G);
    flo = 0;
    while (delta >= 1)
    {
        while (hasAugmentingPath(G, s, t))
        {
            augment(s, t);
        }
        delta /= 2;
    }
}

void FordFulkerson::initDelta(FlowGraph* G)
{
    delta = 1;

    int upperLimit = 0;
    for (int i = 0; i < V; i++)
    {
        for (FlowEdge* e : G->adj(i))
        {
            upperLimit = max(upperLimit, e->capacity());
        }
    }

    while (delta <= upperLimit)
    {
        delta = delta << 1;
    }
    delta = delta >> 1;
}

bool FordFulkerson::hasAugmentingPath(FlowGraph* G, int s, int t)
{
    fill(visited.begin(), visited.end(), false);
    fill(parentEdge.begin(), parentEdge.end(), nullptr);

    queue<int> q;
    q.push(s);
    visited[s] = true;
    while (!q.empty())
    {
        int v = q.front();
        q.pop();

        if (v == t) { break; }

        for (FlowEdge* e : G->adj(v))
        {
            int w = e->other(v);
            int cap = e->residualCapacityTo(w);
            if (cap >= delta && cap > 0 && !visited[w])
            {
                parentEdge[w] = e;
                visited[w] = true;
                q.push(w);
            }
        }
    }
    return visited[t];
}

void FordFulkerson::augment(int s, int t)
{
    int bottleneck = numeric_limits<int>::max();
    for (int v = t; v != s; v = parentEdge[v]->other(v))
    {
        FlowEdge* e = parentEdge[v];
        bottleneck = min(bottleneck, parentEdge[v]->residualCapacityTo(v));
    }

    for (int v = t; v != s; v = parentEdge[v]->other(v))
    {
        parentEdge[v]->addResidualFlowTo(v, bottleneck);
    }

    flo += bottleneck;
}

int FordFulkerson::flow()
{
    return flo;
}

bool FordFulkerson::inCut(int v)
{
    return visited[v];
}

vector<int> FordFulkerson::getCut()
{
    vector<int> cut;
    for (int i = 0; i < V; i++)
    {
        if (inCut(i)) { cut.push_back(i); }
    }

    return cut;
}


inline void displayError(string errorMsg)
{
    cerr<<"Error - "<<errorMsg<<endl;
    cerr<<"Usage -\n\t./ff task graphFile [resultsFile]"<<endl<<endl;
    cerr<<"\task - maxflow or bipartite_matching"<<endl;
    cerr<<"\tgraphFile - file containing information about graph"<<endl;
    cerr<<"\tresultsFile - optionally write a result to a file"<<endl;
    exit(EXIT_FAILURE);
}


void task1(char** argv)
{

    ifstream graphFile(argv[2]);
    if (!graphFile.is_open())
    {
        displayError("Cannot open graph file.");
    }

    string line;
    getline(graphFile, line);
    stringstream ss(line);

    int V, E, s, t, sofar = 0;
    ss>>V>>E>>s>>t;

    unordered_map<int, int> vertexMap, inverseVertexMap;

    FlowGraph graph(V);
    while (getline(graphFile, line))
    {
        int x, y, cap;
        stringstream ss(line);

        ss>>x>>y>>cap;
        if (!vertexMap[x])
        {
            sofar++;
            vertexMap[x] = sofar;
            inverseVertexMap[sofar] = x;
        }
        if (!vertexMap[y])
        {
            sofar++;
            vertexMap[y] = sofar;
            inverseVertexMap[sofar] = y;
        }
        graph.addEdge(vertexMap[x] - 1, vertexMap[y] - 1, cap);
    }
    graphFile.close();

    s = vertexMap[s] - 1;
    t = vertexMap[t] - 1;

    clock_t start = clock();
    FordFulkerson ff(&graph, s, t);
    clock_t stop = clock();
    double processingTime = (double)(stop - start)/CLOCKS_PER_SEC;

    vector<int> cut = ff.getCut();

    cout<<"Graph Info:"<<endl;
    cout<<"\tVertices - "<<V<<", Edges - "<<E<<endl<<endl;
    cout<<"MaxFlow-MinCut Solution :"<<endl;
    cout<<"\tSource - "<<inverseVertexMap[s + 1]<<", Target - "<<inverseVertexMap[t + 1]<<endl;
    cout<<"\tMax Flow Value - "<<ff.flow()<<endl<<endl;
    cout<<"\tNo. of vertices in min cut - "<<cut.size()<<endl;
    cout<<"\tVertices in min cut - "<<endl<<"\t\t";
    for (int v : cut)
    {
        cout<<inverseVertexMap[v + 1]<<" ";
    }
    cout<<endl<<endl;
	cout<<"\tTime Taken - "<<FIXED_FLOAT(processingTime, 6)<<" seconds"<<endl;
}


void dfsMarkBipartiteSet(int v, vector<bool>& visited, vector<bool>& component, FlowGraph* G)
{
    visited[v] = true;

    for (FlowEdge* e : G->adj(v))
    {
        int w = e->other(v);
        if (!visited[w])
        {
            visited[w] = true;
            component[w] = !component[v];
            dfsMarkBipartiteSet(w, visited, component, G);
        }
        else if (component[w] == component[v])
        {
            throw invalid_argument("Not a bipartite graph.");
        }
    }
}


void task3(char** argv)
{
    ifstream graphFile(argv[2]);
    if (!graphFile.is_open())
    {
        displayError("Cannot open graph file.");
    }

    string line;
    getline(graphFile, line);
    stringstream ss(line);

    int n, E, sofar = 0;
    ss>>n>>E;

    int V = n + 2, s = n, t = n + 1;

    unordered_map<int, int> vertexMap, inverseVertexMap;

    FlowGraph graph(V);
    while (getline(graphFile, line))
    {
        int x, y;
        stringstream ss(line);

        ss>>x>>y;
        if (!vertexMap[x])
        {
            sofar++;
            vertexMap[x] = sofar;
            inverseVertexMap[sofar] = x;
        }
        if (!vertexMap[y])
        {
            sofar++;
            vertexMap[y] = sofar;
            inverseVertexMap[sofar] = y;
        }
        graph.addEdge(vertexMap[x] - 1, vertexMap[y] - 1, 1);
    }
    graphFile.close();

    vector<bool> visited(n, false), component(n, false);
    for (int i = 0; i < n; i++)
    {
        if (!visited[i])
        {
            component[i] = true;
            dfsMarkBipartiteSet(i, visited, component, &graph);
        }
    }


    for (int v = 0; v < n; v++)
    {
        if (component[v]) { graph.addEdge(s, v, 1); }
        else { graph.addEdge(v, t, 1); }
    }

    clock_t start = clock();
    FordFulkerson ff(&graph, s, t);
    clock_t stop = clock();
    double processingTime = (double)(stop - start)/CLOCKS_PER_SEC;

    cout<<"Graph Info:"<<endl;
    cout<<"\tVertices - "<<n<<", Edges - "<<E<<endl<<endl;
    cout<<"Bipartite Matching Solution - "<<ff.flow()<<endl;
    cout<<"\tMaximum Matching Size - "<<ff.flow()<<endl;
    cout<<"\tEdges in the matching - "<<endl<<endl;
    for (FlowEdge* e : graph.adj(s))
    {
        if (e->flow() == 0) { continue; }

        int v = e->to();
        for (FlowEdge* ev : graph.adj(v))
        {
            int w = ev->other(v);
            if (w == s || ev->flow() == 0) { continue; }
            cout<<"\t\t("<<inverseVertexMap[v + 1]  <<", "<<inverseVertexMap[w + 1]<<")"<<endl;
            break;
        }
    }
    cout<<endl;
	cout<<"\tTime Taken - "<<FIXED_FLOAT(processingTime, 6)<<" seconds"<<endl;

}

int main(int argc, char** argv)
{
    if (argc < 3 || argc > 4)
    {
        displayError("No. of command-line arguments do not match.");
    }

    if (!strcmp("maxflow", argv[1]))
    {
        task1(argv);
        return 0;
    }

    if (!strcmp("bipartite_matching", argv[1]))
    {
        task3(argv);
        return 0;
    }

    displayError("Incorrect command line argument");

    return 0;
}
