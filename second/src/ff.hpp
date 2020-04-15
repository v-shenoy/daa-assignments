#ifndef ff_hpp
#define ff_hpp

#include<vector>
#include<string>
#include "flow_graph.hpp"

/** \mainpage
* The aim of the assignment was to implement FordFulerson algorithm the maximum flow
* in a network, and also find the associated minimum cut between the source and the
* target vertex.
*
* We were also supposed to use the FordFulkerson algorithm to solve the
* Bipartite Matching problem.
*
*/

/**
 * Class template for the DiGraph abstract data-type.
 *
 * The class is separate and decouples the algorithm from the data-type
 * representing the flow network.
 */
class FordFulkerson
{
    private:
        /**
         * Number of vertices in the digraph.
         */
        int V;

        /**
         * The value of the flow in the network at a given state.
         */
        int flo;

        /**
         * scaling parameter to speed up the algorithm.
         */
        int delta;

        /**
         * Boolean vector to mark the vertices visited while finding
         * the augmenting path.
         */
        std::vector<bool> visited;

        /**
         * Vector containing pointers to the parent edges to reconstruct
         * the augmenting path.
         */
        std::vector<FlowEdge*> parentEdge;

        /**
         * \brief Used to initialize delta to the largest power of 2
         * less than the maximum capacity of an edge in the network.
         *
         * @param G - pointer to the flow network
         */
        void initDelta(FlowGraph* G);

        /**
         * \brief Runs a modified version of BFS to find the augmenting path
         * and returns whether it has found one.
         *
         * @param G - pointer to the flow network
         * @param s - source vertex
         * @param t - sink/target vertex
         */
        bool hasAugmentingPath(FlowGraph* G, int s, int t);

        /**
         * \brief Augments the path found bythe hasAugmentingPath function.
         *
         * @param s - source vertex
         * @param t - sink/target vertex
         */
        void augment(int s, int t);

    public:
        /**
         * \brief Constructor used for FordFulkerson object.
         *
         * The constructor also runs the FordFulkerson algorithm and finds
         * the maximum flow, and the minimum cut solutions.
         *
         * @param G - pointer to the flow network
         * @param s - source vertex
         * @param t - sink/target vertex
         */
        FordFulkerson(FlowGraph* G, int s, int t);

        /**
         * \brief Returns the max-flow value calculated by the algorithm.
         */
        int flow();

        /**
         * \brief Returns whether the vertex passed is in the given ST Cut
         * (in the source set)
         *
         * @param v - vertex to be checked
         */
        bool inCut(int v);

        /**
         * \brief Returns a vector of vertices present in the source set of
         * the min cut.
         */
        std::vector<int> getCut();
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
 * \brief Function for running Task 1 (Finding MaxFlow-MinCut).
 *
 * @param argv - command line args passed while executing
 */
void task1(char** argv);

/**
 * \brief Runs a dfs algorithm partitioning the vertices
 * into two sets of the bipartite graph.
 *
 * @param v - vertex from which dfs begins executing
 * @param visited - vector of bools keeping track of visited vertices
 * @param component - vector keeping track of bipartite set
 * @param G - the graph on which we are running thte dfs
 */
void dfsMarkBipartiteSet(int v, std::vector<bool>& visited, std::vector<bool>& component, FlowGraph* G);

/**
 * \brief Function for running Task 3 (Finding Maximum Bipartite Matching).
 *
 * @param argv - command line args passed while executing
 */
void task3(char** argv);

#endif
