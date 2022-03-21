#include "graph.h"

#include <algorithm>
#include <cmath>
#include <iostream>

inline double distanceCalc(const double & xa, const double & ya, const double & xb, const double & yb){
    return std::sqrt( std::pow((xb - xa), 2.0) + std::pow( (yb - ya), 2.0) ); 
}

/* GraphVertex methods */


/* GraphEdge methods */
GraphEdge::GraphEdge(const std::string & src,
                    const std::string & dest,
                    const double& weight,
                    const double& pheromone):
    m_srcNodeIdx(src),
    m_destNodeIdx(dest), 
    m_weight(weight),
    m_pheromone(pheromone),
    m_pheromoneToAdd(0.0)
{};

void GraphEdge::pheromoneSet(const double& v){

    if(v< 0.0)
        throw GraphExceptionPheromoneInvalidValue();
    m_pheromone = v;
}

void GraphEdge::pheromoneToAdd(const double& v){
    if(v< 0.0)
        throw GraphExceptionPheromoneInvalidValue();
    m_pheromoneToAdd += v;
}

void GraphEdge::pheromoneUpdate(const double & evapCoef){
    m_pheromone = m_pheromone * (1 - evapCoef);
    m_pheromone = m_pheromone + m_pheromoneToAdd;
    m_pheromoneToAdd = 0.0;
}

double operator+(GraphEdge lhs, const GraphEdge& rhs){
    lhs.m_weight += rhs.m_weight;
    return lhs.weight();
}

std::istream& operator>> (std::istream& is, GraphEdge& ge){
    std::string src, dst;
    is >> src >> dst;
    ge.m_srcNodeIdx = src;
    ge.m_destNodeIdx = dst;
    return is;
}

std::ostream& operator<< (std::ostream& os, GraphEdge& ge){
    os << " (" << ge.source() << "' " << ge.destination() << ") W = " << ge.weight();
    return os;
}


/* GraphNode methods */
void GraphNode::pushEdge(const std::string& destination, const uint & x, const uint & y){

    double weight = distanceCalc(this->x(), this->y(), x, y );
/*     
     std::sqrt( std::pow(static_cast<double>(this->x()) - static_cast<double>(x), 2.0) + 
                                std::pow(static_cast<double>(this->y()) - static_cast<double>(y), 2.0) );  */
    m_edges.push_back( GraphEdge(this->name(), destination,  weight) );
}

GraphEdge& GraphNode::operator[](std::string key){

    auto it = std::find_if( m_edges.begin(), m_edges.end(), [&key](const GraphEdge & edge)->bool { return ( edge.destination().compare(key) == 0); } );
    if(it == m_edges.end())
        throw GraphExceptionEdgeDoesNotExist();
    return *it;
}

std::istream& operator>> (std::istream& is, GraphNode& gn){
    std::string vertexName;
    double x,y;
    is >> vertexName >> x >> y;
    gn.m_name = vertexName;
    gn.m_x = x;
    gn.m_y = y;
    return is;
}

std::ostream& operator<< (std::ostream& os, GraphNode& gn){
    os << " \"" << gn.name() << "\" <" << gn.x() << ", " << gn.y() << "> \n";
    for(auto& e : gn.edges()){
        os << e << "\n";
    }
    return os;
}

/* Graph methods */
void Graph::pushNode(std::string name, uint x, uint y){
    m_nodes.emplace(std::piecewise_construct,
                    std::forward_as_tuple(name),
                    std::forward_as_tuple(name, x, y) );
}

void Graph::pushEdge(std::string source, std::string destination){
    m_nodes.at(source).pushEdge(destination, m_nodes[destination].x(), m_nodes[destination].y() );
    m_nodes.at(destination).pushEdge(source, m_nodes[source].x(), m_nodes[source].y() );
}

void Graph::makeComplete(){
    std::vector<std::string> verts;
    for(auto &n: m_nodes){
        verts.push_back(n.first);
    }
    for(int i = 0; i < verts.size(); i++){
        for(int j = i + 1; j < verts.size(); j++){
            pushEdge(verts[i], verts[j]);
        }
    }
}

void Graph::recalcWeights(){
    for(auto &n : m_nodes){
        for(auto &e : n.second.edges()){
            e.weight( distanceCalc(n.second.x(), n.second.y(), m_nodes[e.destination()].x(), m_nodes[e.destination()].y() ) );
        }
    }
}

GraphEdge& Graph::getEdge(std::string source, std::string destination){
    return (*this)[source][destination];
}

GraphNode& Graph::operator[](int n){
    auto iter = m_nodes.begin();
    for(int i = 0; i < n; i++){
        iter++;
        if(iter == m_nodes.end()){
            throw std::out_of_range("Graph::operator[](int n): n " + std::to_string(n) + " is out of range!");
        }
    }
    return iter->second;
}

std::ostream& operator<< (std::ostream& os, Graph& g){
    for(auto & v : g.nodes() ){
        os << v.second;
    }
    return os;
}