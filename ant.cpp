#include "ant.h"

#include <algorithm>
#include <cmath>
#include <iostream>


void Ant::turnback(){
    m_pathWeight = 0.0;
    if(m_path.size() <= 0){
        throw std::logic_error("Ant::reinit(): Ant object was never used - cannot turnback");
    }
    std::string v = m_path.back();
    m_path.clear();
    m_path.push_back(v);
}

void Ant::init(){
    int n = std::uniform_int_distribution<>(0, (m_graph.nodes().size() - 1))(m_rnd);
    std::string v = m_graph[n].name();
    m_path.clear();
    m_path.push_back(v);
    m_pathWeight = 0.0;
}

void Ant::init(std::string v){
    m_path.clear();
    m_path.push_back(v);
    m_pathWeight = 0.0;
}


void Ant::move(std::string dest){
    double Pz = 0.0;
    double Pa = 0.0;
    std::list<GraphEdge *> m_edges;

    double rNum = std::uniform_real_distribution<>(0.0, 1.0)(m_rnd);

    /* get the vertice ant is in */
    std::string v = m_path[ m_path.size() - 1 ];

    /* find nodes that can be visited */
    if( m_path.size() == ( m_graph.nodes().size())){

        auto it = std::find_if( m_graph[v].edges().begin(), m_graph[v].edges().end(), [&dest](const GraphEdge & e)->bool { return ( e.destination().compare(dest) == 0 ); } );
        if(it != m_graph[v].edges().end()){
            m_edges.push_back(&(*it));
        }
    }
    else{
        for( auto & e : m_graph[v].edges() ){
            auto it = std::find_if( m_path.begin(), m_path.end(), [&e](const std::string & v)->bool { return ( v.compare(e.destination()) == 0 ); } );
            if(it == m_path.end()){
                m_edges.push_back(&e);
            }
        }
    }

    /* Calculate the probabilities for each path */
    for( auto & e : m_edges){
        Pz += std::pow(e->pheromone(), m_alfa) * std::pow(( 1.0 / e->weight() ), m_beta);
    }

    bool added = false;
    /* calculate the probabilities and choose the path */
    for(auto & e : m_edges){
        double Pk = ( std::pow(e->pheromone(), m_alfa) * std::pow(( 1.0 / e->weight() ), m_beta) ) / Pz;
        if ( rNum >= Pa && rNum < (Pa + Pk)){
            m_path.push_back(e->destination());
            m_pathWeight += e->weight();
            e->pheromoneToAdd( (m_Q / e->weight()) );
            added = true;
            break;
        }
        Pa += Pk;
    }
    if(added == false){
       // std::cout << "Nie dodano nic do ścieżki rand = " << rNum << " Pa = " << Pa << " Pz = " << Pz <<  "\n";
    }
}


double Ant::pathWeight(){
/*     double weight = 0.0;
    for(int v = 0; v < m_path.size() - 1; v++){
        weight += m_graph[ m_path[v] ][ m_path[v+1] ].weight();
    }
    return weight; */
    return m_pathWeight;
}