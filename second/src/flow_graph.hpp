#ifndef flow_graph_hpp
#define flow_graph_hpp

#include <vector>

/**
 * Class template for an edge in a flow network.
 *
 */
class FlowEdge
{
    private:
        /**
         * Source vertex of the edge.
         *
         */
        int v;
        
        /**
         * Destination vertex of the edge.
         *
         */
        int w;

        /**
         *  Capacity of the edge.
         *
         */
        int cap;

        /**
         *  Current flow through the edge.
         *
         */
        int flo;

    public:
        /**
         * \brief Constructor used for FordFulkerson object.
         *
         *
         * @param v - source vertex of the edge.
         * @param w - destination vertex of the edge.
         * @param cap - capacity of the edge.
         */
        FlowEdge(int v, int w, int cap);

        /**
         * \brief Returns the source vertex of the edge
         */
        int from();

        /**
         * \brief Returns the destination vertex of the edge
         */
        int to();

        /**
         * \brief Returns the capacity of the edge
         */
        int capacity();

         /**
         * \brief Returns the current flow through the edge
         */
        int flow();

        /**
         * \brief Returns the other end point of the edge.
         *
         * @param vertex - one end point of the edge
         */
        int other(int vertex);

        /**
         * \brief Returns the residual capacity.
         *
         * @param vertex - vertex used for distinguishing if edge is backward or forward.
         */
        int residualCapacityTo(int vertex);

        /**
         * \brief Changes the flow of the edge by using delta.
         *
         * @param vertex - vertex used for distinguishing if edge is backward or forward.
         * @param delta - amount of change in the flow
         */
        void addResidualFlowTo(int vertex, int delta);
};

/**
 * Class template for the flow network.
 *
 */
class FlowGraph
{
    private:
        /**
         * Number of vertices in the digraph.
         */
        int V;

        /** 
         * Adjacency list for storing the graph's edges.
         *
         */
        std::vector<std::vector<FlowEdge*> > adjacent;

    public:
        /**
         * \brief Creates the FlowGraph object.
         *
         * @param V - number of vertices in the flow graph
         */
        FlowGraph(int V);

        /**
         * \brief Returns the umber of vertices in the digraph.
         */
        int noVertices();

        /**
         * \brief Adds a flow edge between vertices with given capacity.
         *
         * It adds two directed edges (forward & backward).
         *
         * @param v - one end point of the edge
         * @param w - other end point of the edge
         * @param cap - capacity of the edge
         */
        void addEdge(int v, int w, int cap);

        /**
         * \brief Returns the adjacencvy list of a vertex
         *
         * @param v - vertex for which we want the adjacency list
         */
        std::vector<FlowEdge*>& adj(int v);
};

#endif
