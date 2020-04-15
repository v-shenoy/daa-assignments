#ifndef scc_hpp
#define scc_hpp

#include<vector>
#include<stack>
#include<unordered_map>
#include<string>

/** \mainpage
* The aim of the assignment was to implement two algorithms for finding strongly-connected components in
* a digraph, and compare them. 
*   
*   Algorithm 1 - Kosaraju et al.'s DFS based algorithm <br>
*   Algorithm 2 - Coppersmith et al.'s Divide and Conquer based algorithm
*/

/**
 * \brief Enum representing the algorithm to be used for calculating the strongly 
 * connected components.
 * 
 *      ALG_DFS - Kosaraju et al.'s DFS based algorithm
 *      ALG_DCSC - Coppersmith et al.'s Divide and Conquer based algorithm
 *
 */
enum ALG_TYPE
{
    ALG_DFS,
    ALG_DCSC
};

/**
 * Class template for the DiGraph abstract data-type.
 */
class DiGraph
{
    private:    
        /**
         * Number of vertices in the digraph.
         */
        int noVertices;

        /**
         * Adjacency list used to store the digraph's edges.
         */
        std::vector<std::vector<int> > adj;

        /**
         * Adjacency list used to store the edges in the transpose digraph.
         */
        std::vector<std::vector<int> > transposeAdj;

        /**
         * Compute and return the adjacency list for the transpose digraph.
         */
        std::vector<std::vector<int> > getTranspose();

        /**
         * \brief Determine the order of DFS calls on vertices in the transpose graph
         * for the second run of Kosaraju's algorithm.
         * 
         * @param curr - vertex on which DFS call is performed
         * @param visited - list of booleans to keep track of vertices to make sure they are processed only once
         * @param visitOrder - stack of vertices representing the visitation order in the second run
         */
        void fillStack(int curr, std::vector<bool>& visited, std::stack<int>& visitOrder);

        /**
         * \brief Private method to run Kosaraju et al.'s DFS algorithm, and return
         * strongly-connected components.
         */
        std::vector<std::vector<int> > dfs();

        /**
         * \brief Performs a DFS call on the current vertex, and marks its neighbours in a boolean array.
         * Used to calculate predecessor and descendent sets in the DCSC algorithm.
         *
         * @param curr - vertex on which to perform the DFS call
         * @param visited - list of booleans in which the visited vertices are marked as true
         * @param done - list of booleans representing vertices whose strong components have already been found
         * @param adjSet - the adjacency list to be used for edges (transposeAdj for predecessors, adj for descendents)
         */
        void dcscVisitor(int curr, std::vector<bool>& visited, std::vector<bool>& done, std::vector<std::vector<int> >& adjSet);

        /**
         * \brief Private method to run the Divide and Conquer algorithm and return 
         * strongly connected components.
         * 
         * @param vertexSet - list of vertices in the sub-graph we are finding the component for
         * @param done - list of booleans representing vertices whose strong components have already been found
         * @param adjSet - the adjacency list to be used for edges (transposeAdj for predecessors, adj for descendents)
         */
        void dcsc(std::vector<int>& vertexSet, std::vector<bool>& done, std::vector<std::vector<int> >& adjSet);
        
    public:
        /**
         * \brief Constructor used for creating the digraph object.
         *
         * @param noVertices - number of vertices in the digraph
         */
        DiGraph(int noVertices);

        /**
         * \brief Adds an edge between two vertices, x -> y
         *
         * @param x - source of the edge
         * @param y - destination of the edge
         */
        inline void addEdge(int x, int y);

        /**
         * Public interface exposed to the caller, for finding strongly-connected
         * components.
         *
         * @param ALG_TYPE - algorithm to be used for finding the components
         */
        std::vector<std::vector<int> > getStronglyConnectedComponents(ALG_TYPE);
};

/**
 * \brief Function for displaying any errors caused during execution.
 *
 * The function prints the associated error message, and the correct usage
 * for the program.
 *
 * @param errorMsg - cause of the error
 */
inline void displayError(std::string errorMsg);

/**
 * \brief Function for writing the results of one run in a file. The result
 * is appended to the end of the file.
 *
 * @param fileName - file name in which the results are appened
 * @param graphName - name of the digraph which was used
 * @param noVertices - number of vertices in the digraph
 * @param noEdges - number of edges in the digraph
 * @param noComponents - number of strong components in the digraph
 * @param maxComponentSize - number of vertices in the largest strong component
 * @param processingTime - time required to find strong components
 */
inline void writeResults(char* fileName, char* graphName, int noVertices, int noEdges, int noComponents, int maxComponentSize, double processingTime);

#endif
