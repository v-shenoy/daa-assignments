#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include<sstream>
#include<algorithm>
#include<ctime>
#include<unordered_map>
#include<vector>
#include<stack>
#include<iomanip>
#include<cstdlib>

#include "scc.hpp"

using namespace std;


/**
 * Macro short-handfor printing a decimal x with a precision of y.
 */
#define FIXED_FLOAT(x, y) fixed<<setprecision(y)<<(x)


DiGraph::DiGraph(int noVertices) : adj(noVertices)
{
    this->noVertices = noVertices;
}

void DiGraph::addEdge(int x, int y)
{
    adj[x].push_back(y);
}

vector<vector<int> > DiGraph::getTranspose()
{
    vector<vector<int> > transposeAdj(noVertices);

    for(int x=0;x<noVertices;x++)
    {
        for(auto y : adj[x])
        {
            transposeAdj[y].push_back(x);
        }
    }

    return transposeAdj;
}

void DiGraph::fillStack(int curr, vector<bool>& visited, stack<int>& visitOrder)
{
    if(visited[curr])
    {
        return;
    }

    visited[curr] = true;
    for(auto neighbour : adj[curr])
    {
        fillStack(neighbour, visited, visitOrder);
    }
    visitOrder.push(curr);
}

vector<vector<int> > DiGraph::dfs()
{
    vector<vector<int> > components;
    vector<bool> visited(noVertices);

    stack<int> visitOrder, dfsHelper;
    for(int i=0;i<noVertices;i++)
    {
        fillStack(i, visited, visitOrder);
    }

    fill(visited.begin(), visited.end(), false);
    while(!visitOrder.empty())
    {
        int curr = visitOrder.top();
        visitOrder.pop();

        if(visited[curr])
        {
            continue;
        }

        vector<int> component;

        dfsHelper.push(curr);
        visited[curr] = true;
        while(!dfsHelper.empty())
        {
            curr = dfsHelper.top();
            dfsHelper.pop();

            component.push_back(curr);

            for(auto neighbour : transposeAdj[curr])
            {
                if(!visited[neighbour])
                {
                    visited[neighbour] = true;
                    dfsHelper.push(neighbour);
                }
            }
        }
        components.push_back(component);
    }

    return components;
}

void DiGraph::dcscVisitor(int curr, vector<bool>& visited, vector<bool>& done, vector<vector<int> >& adjSet)
{
    stack<int> dfsHelper;

    dfsHelper.push(curr);
    visited[curr] = true;
    while(!dfsHelper.empty())
    {
        curr = dfsHelper.top();
        dfsHelper.pop();

        for(auto neighbour : adjSet[curr])
        {
            if(!visited[neighbour] && !done[neighbour])
            {
                visited[neighbour] = true;
                dfsHelper.push(neighbour);
            }
        }
    }
}

void DiGraph::dcsc(vector<int>& vertexSet, vector<bool>& done, vector<vector<int> >& components)
{
    if(vertexSet.empty())
    {
        return;
    }

    int curr = vertexSet[0];

    vector<bool> pred(noVertices), desc(noVertices);
    dcscVisitor(curr, pred, done, transposeAdj);
    dcscVisitor(curr, desc, done, adj);

    vector<int> component;
    for(auto vertex: vertexSet)
    {
        if(pred[vertex] && desc[vertex])
        {
            component.push_back(vertex);
            done[vertex] = true;
        }
    }
    components.push_back(component);

    vector<int> newVertexSet;
    for(auto vertex: vertexSet)
    {
        if(pred[vertex] && !done[vertex])
        {
            newVertexSet.push_back(vertex);
        }
    }
    dcsc(newVertexSet, done, components);

    newVertexSet.clear();
    for(auto vertex: vertexSet)
    {
        if(desc[vertex] && !done[vertex])
        {
            newVertexSet.push_back(vertex);
        }
    }
    dcsc(newVertexSet, done, components);

    newVertexSet.clear();
    for(auto vertex: vertexSet)
    {
        if(!pred[vertex] && !desc[vertex] && !done[vertex])
        {
            newVertexSet.push_back(vertex);
        }
    }
    dcsc(newVertexSet, done, components);
}

vector<vector<int> > DiGraph::getStronglyConnectedComponents(ALG_TYPE alg)
{
    transposeAdj = getTranspose();

    if(alg == ALG_DFS)
    {
        return dfs();
    }

    vector<int> vertexSet;
    for(int i=0;i<noVertices;i++)
    {
        vertexSet.push_back(i);
    }
    vector<bool> done(noVertices);

    vector<vector<int> > components;
    dcsc(vertexSet, done, components);

    return components;
}

inline void displayError(string errorMsg)
{
    cerr<<"Error - "<<errorMsg<<endl;
    cerr<<"Usage -\n\t./getScc graphFile alg [resultsFile]"<<endl<<endl;
    cerr<<"\tgraphFile - file containing information about graph"<<endl;
    cerr<<"\talg - algorithm to use for getting components (dfs or dcsc)"<<endl;
    cerr<<"\tresultsFile - optional file for writing back results"<<endl;
    exit(EXIT_FAILURE);
}


inline void writeResults(char* fileName, char* graphName, int noVertices, int noEdges, int noComponents, int maxComponentSize, double processingTime)
{
    ofstream resultsFile(fileName, ios_base::app);

    if(!resultsFile.is_open())
    {
        displayError("Cannot open results file.");
    }
    resultsFile<<graphName<<" ";
    resultsFile<<noVertices<<" ";
    resultsFile<<noEdges<<" ";
    resultsFile<<noComponents<<" ";
    resultsFile<<maxComponentSize<<" ";
    resultsFile<<FIXED_FLOAT(processingTime, 6)<<endl;
    resultsFile.close();
}

int main(int argc, char** argv)
{
    if(argc < 3 || argc > 4)
    {
        displayError("No. of command-line arguments do not match.");
    }

    ifstream graphFile(argv[1]);
    if(!graphFile.is_open())
    {
        displayError("Cannot open graph file.");
    }

    ALG_TYPE alg;
    if(!strcmp(argv[2], "dfs"))
    {
        alg = ALG_DFS;
    }
    else if(!strcmp(argv[2], "dcsc"))
    {
        alg = ALG_DCSC;
    }
    else
    {
        displayError("Unknown algorithm for getting components.");
    }

    string line;
    getline(graphFile, line);
    stringstream ss(line);

    int noVertices, noEdges, sofar = 0;
    ss>>noVertices>>noEdges;

    unordered_map<int, int> vertexMap, inverseVertexMap;

    DiGraph graph(noVertices);
    while(getline(graphFile, line))
    {
        int x, y;
        stringstream ss(line);

        ss>>x>>y;
        if(!vertexMap[x])
        {
            sofar++;
            vertexMap[x] = sofar;
            inverseVertexMap[sofar] = x;
        }
        if(!vertexMap[y])
        {
            sofar++;
            vertexMap[y] = sofar;
            inverseVertexMap[sofar] = y;
        }
        graph.addEdge(vertexMap[x]-1, vertexMap[y]-1);
    }
    graphFile.close();

    clock_t start = clock();
    vector<vector<int> > components = graph.getStronglyConnectedComponents(alg);
    clock_t stop = clock();
    double processingTime = (double)(stop - start)/CLOCKS_PER_SEC;

    cout<<"Graph Info:"<<endl;
    cout<<"\tVertices - "<<noVertices<<", Edges - "<<noEdges<<endl;
    cout<<"\tNo. of strongly connected components - "<<components.size()<<endl;

    sort(
        components.begin(),
        components.end(),
        [](const vector<int> & a, const vector<int> & b)
            {
                return a.size() > b.size();
            }
    );
    cout<<"\tNo. of vertices in largest strongly connected component - "<<components[0].size()<<endl;
	cout<<"\tProcessing time - "<<FIXED_FLOAT(processingTime, 6)<<" seconds"<<endl<<endl;

    if(argc == 4)
    {
        writeResults(argv[3], argv[1], noVertices, noEdges, components.size(), components[0].size(), processingTime);
    }

    for(int compNo=0;compNo<components.size();compNo++)
    {
        cout<<"Component No. - "<<compNo+1<<endl;
        cout<<"Component Size - "<<components[compNo].size()<<endl<<"\t";

        for(auto vertex : components[compNo])
        {
            cout<<inverseVertexMap[vertex+1]<<" ";
        }
        cout<<endl;
    }

    return 0;
}