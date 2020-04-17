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


const int MAX_INT = numeric_limits<int>::max();


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
    fill(parentEdge.begin(), parentEdge.end(), nullptr);
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

    queue<int> q;
    q.push(s);
    visited[s] = true;
    parentEdge[s] = nullptr;
    while (!q.empty())
    {
        int v = q.front();
        q.pop();

        if (v == t) { break; }

        for (FlowEdge* e : G->adj(v))
        {
            int w = e->other(v);
            if (e->residualCapacityTo(w) >= delta && !visited[w])
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
    int bottleneck = MAX_INT;
    for (int v = t; v != s; v = parentEdge[v]->other(v))
    {
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
    cerr<<"\ttask - maxflow or bipartite_matching"<<endl;
    cerr<<"\tgraphFile - file containing information about graph"<<endl;
    cerr<<"\tresultsFile - optionally write a result to a file"<<endl;
    exit(EXIT_FAILURE);
}


inline void writeResults(char* fileName, char* graphName, int V, int E, FordFulkerson* ff, double processingTime)
{
    ofstream resultsFile(fileName, ios_base::app);

    if(!resultsFile.is_open()) { displayError("Cannot open results file."); }
    resultsFile<<graphName<<" ";
    resultsFile<<V<<" ";
    resultsFile<<E<<" ";
    resultsFile<<ff->flow()<<" ";
    resultsFile<<FIXED_FLOAT(processingTime, 6)<<endl;
    resultsFile.close();
}


void task1(int argc, char** argv)
{

    ifstream graphFile(argv[2]);
    if (!graphFile.is_open()) { displayError("Cannot open graph file."); }

    string line;
    getline(graphFile, line);
    stringstream ss(line);

    int V, E;
    ss>>V>>E;

    FlowGraph graph(V);
    while (getline(graphFile, line))
    {
        int x, y, cap;
        stringstream ss(line);

        ss>>x>>y>>cap;
        graph.addEdge(x, y, cap);
    }
    graphFile.close();

    int s = graph.source();
    int t = graph.sink();

    clock_t start = clock();
    FordFulkerson ff(&graph, s, t);
    clock_t stop = clock();
    double processingTime = (double)(stop - start)/CLOCKS_PER_SEC;

    vector<int> cut = ff.getCut();

    cout<<"Graph Info:"<<endl;
    cout<<"\tVertices - "<<V<<", Edges - "<<E<<endl<<endl;
    cout<<"MaxFlow-MinCut Solution :"<<endl;
    cout<<"\tSource - "<<s<<", Target - "<<t<<endl;
    cout<<"\tMax Flow Value - "<<ff.flow()<<endl<<endl;
    cout<<"\tNo. of vertices in min cut - "<<cut.size()<<endl;
    cout<<"\tVertices in min cut - "<<endl<<"\t\t";
    for (int v : cut)
    {
        cout<<v<<" ";
    }
    cout<<endl<<endl;
	cout<<"\tTime Taken - "<<FIXED_FLOAT(processingTime, 6)<<" seconds"<<endl;

    if(argc == 4) { writeResults(argv[3], argv[2], V, E, &ff,  processingTime); }
}


void task3(int argc, char** argv)
{
    ifstream graphFile(argv[2]);
    if (!graphFile.is_open()) { displayError("Cannot open graph file."); }

    string line;
    getline(graphFile, line);
    stringstream ss(line);

    int E, n1, n2;
    ss>>n1>>n2>>E;

    int n = n1 + n2, V = n + 2, s = n, t = n + 1;

    FlowGraph graph(V);
    while (getline(graphFile, line))
    {
        int x, y;
        stringstream ss(line);

        ss>>x>>y;
        x = x - 1;
        y = y - 1 + n1;
        graph.addEdge(x, y, 1);
    }
    graphFile.close();

    for (int v = 0; v < n1; v++)
    {
        graph.addEdge(s, v, 1);
    }

    for (int v = n1; v < n; v++)
    {
        graph.addEdge(v, t, 1);
    }

    clock_t start = clock();
    FordFulkerson ff(&graph, s, t);
    clock_t stop = clock();
    double processingTime = (double)(stop - start)/CLOCKS_PER_SEC;

    cout<<"Graph Info:"<<endl;
    cout<<"\tVertices - "<<n<<", Edges - "<<E<<endl<<endl;
    cout<<"Bipartite Matching Solution - "<<endl;
    cout<<"\tMaximum Matching Size - "<<ff.flow()<<endl;
    cout<<"\tEdges in the matching - "<<endl;
    for (FlowEdge* e : graph.adj(s))
    {
        if (!e->flow()) { ; }

        int v = e->to();
        for (FlowEdge* ev : graph.adj(v))
        {
            int w = ev->other(v);
            if (w == s || !ev->flow()) { continue; }
            cout<<"\t\t("<<v + 1<<", "<<w + 1 - n1<<")"<<endl;
            break;
        }
    }
    cout<<endl;
	cout<<"\tTime Taken - "<<FIXED_FLOAT(processingTime, 6)<<" seconds"<<endl;

    if(argc == 4) { writeResults(argv[3], argv[2], n, E, &ff,  processingTime); }
}

int main(int argc, char** argv)
{
    if (argc < 3 || argc > 4) { displayError("No. of command-line arguments do not match."); }

    if (!strcmp("max_flow", argv[1]))
    {
        task1(argc, argv);
        return 0;
    }

    if (!strcmp("bipartite_matching", argv[1]))
    {
        task3(argc, argv);
        return 0;
    }

    displayError("Incorrect command line argument");

    return 0;
}
