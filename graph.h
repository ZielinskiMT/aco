#ifndef GRPAH_H
#define GRPAH_H

#include <unordered_map>
#include <list>
#include <iterator>
#include <string>
#include <iostream>

struct GraphExceptionEdgeDoesNotExist : public std::exception {
   const char * what () const throw () {
      return "GraphEdge[]: The requested graph edge does not exist";
   }
};

struct GraphExceptionPheromoneInvalidValue : public std::exception {
   const char * what () const throw () {
       return "GraphEdge[]: Pheromone valuebelow zero";
   }
};

struct GraphExceptionWeightInvalidValue : public std::exception {
   const char * what () const throw () {
      return "GraphEdge[]: Weight value below zero";
   }
};


class GraphVertex{
    protected:
        uint m_x;
        uint m_y;
        std::string m_name;

    public:
        GraphVertex(std::string name, uint x, uint y):
        m_x(x),
        m_y(y),
        m_name(name)
        {}

        const uint& x() const { return m_x; }
        const uint& y() const { return m_y; }
        const std::string& name() const { return m_name; }
};


class GraphEdge{
    private:
        std::string  m_srcNodeIdx;
        std::string   m_destNodeIdx;

        double m_weight;
        double m_pheromone;
        double m_pheromoneToAdd;

    public:
        GraphEdge(const std::string & src,
                    const std::string & dest,
                    const double& weight,
                    const double& pheromone = 0.0);

        const std::string & source() const { return m_srcNodeIdx; }
        const std::string & destination() const { return m_destNodeIdx; }
        const double& weight() const { return m_weight; }
        void weight(const double & val){ m_weight = val;}
        const double& pheromone() const { return m_pheromone; }
        void pheromoneSet(const double& v);
        void pheromoneToAdd(const double& v);
        void pheromoneUpdate(const double & evapCoef);

        friend double operator+(GraphEdge lhs, const GraphEdge& rhs);

        friend std::istream& operator>> (std::istream& is, GraphEdge& g);
        friend std::ostream& operator<< (std::ostream& os, GraphEdge& g);
};


class GraphNode: public GraphVertex{
    private:
        std::list<GraphEdge> m_edges;

    public:
        GraphNode(std::string name = "", uint x = 0, uint y = 0): GraphVertex(name, x, y) {};

        std::list<GraphEdge> & edges()  { return m_edges; }
        void pushEdge(const std::string& destination, const uint& x, const uint& y);

        GraphEdge& operator[](std::string key);

        friend std::istream& operator>> (std::istream& is, GraphNode& g);
        friend std::ostream& operator<< (std::ostream& os, GraphNode& g);
};


class Graph{
    private:
        std::unordered_map<std::string, GraphNode> m_nodes;

    public:
        Graph(){};

        std::unordered_map<std::string, GraphNode> & nodes() { return m_nodes; }
        void pushNode(std::string name, uint x, uint y);
        void pushEdge(std::string source, std::string destination);
        void makeComplete();
        void recalcWeights();
        GraphEdge& getEdge(std::string source, std::string destination);


        GraphNode& operator[](std::string key){
            return m_nodes[key];
        }
        GraphNode& operator[](int n);

        friend std::istream& operator>> (std::istream& is, Graph& g);
        friend std::ostream& operator<< (std::ostream& os, Graph& g);
};

#endif /* GRPAH_H */