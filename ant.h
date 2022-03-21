#ifndef ANT_H
#define ANT_H

#include "graph.h"
#include <vector>
#include <chrono>
#include <random>

class Ant
{
private:

    Graph & m_graph;
    std::vector<std::string> m_path;

    double m_pathWeight;
    double m_Q;
    double m_alfa;
    double m_beta;

    std::mt19937 m_rnd;


public:
    Ant(Graph & graph, double Q = 0.1, double a = 1.0, double B = 2.0):
        m_rnd((std::mt19937::result_type)
            std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count()
            ),
        m_graph(graph),
        m_Q(Q),
        m_alfa(a),
        m_beta(B)
    {}

    void turnback();
    void init();
    void init(std::string v);
    void move(std::string dest);
    double pathWeight();
    const std::vector<std::string>& path() { return m_path; }
};



#endif /* ANT_H */