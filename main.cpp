#include <iostream>
#include <vector>
#include <limits>

#include "graph.h"
#include "ant.h"
#include "argprs.h"


int main(int argc, char ** argv){

    /* Config - parsing args */
    ArgPrs argConf(argc, argv);

    try{
        argConf.parse();
    }
    catch(const std::exception & x){
        std::cerr << "Failure: " << x.what() << "\n";
        std::cerr << argConf.UsageInfo();
        return -1;
    }

    /* Graph  */
    Graph graph;
     /* User input */
    std::string name;
    std::string target;
    unsigned int x,y;
    std::cout << "Insert graph nodes in format: name x y\n Insert ; wehn all nodes are added.\nNodes:\n";
    while(std::cin >> name){
        if(name == ";"){
            break;
        }
        if (std::cin  >> x >> y){
            graph.pushNode(name, x, y);
        }
        else{
            std::cerr << "Invalid input\n";
            return -1;
        }
    }

    std::cout << "Insert start/stop node:\n";
    std::cin >> target;
    try{
        std::string test = graph[target].name();
    }
    catch (const std::exception & e){
        std::cerr << "You have to specify existing start/stop node\n";
    }   
    graph.makeComplete();

    /* Display graph data */
    std::cout << "================= GRAPH =================\n";
    std::cout << graph << "\n";
    std::cout << "=========================================\n";

    /* Initialise graph with pheromone */
    for(auto &node : graph.nodes()){
        for(auto &edge: node.second.edges()){
            edge.pheromoneSet( argConf.Q());
        }
    }
    
    /* Initialise ant colony */
    std::vector<Ant> antColony;
    for(int i = 0; i < argConf.AntNo(); i++){
        antColony.push_back(Ant(graph, argConf.Q() , argConf.Alfa(), argConf.Beta() ));
        antColony[i].init();
    }

    /* start calculations */
    int iterNo = 0;
    int noChangeCount = 0;
    int degeneratedPathes = 0;
    int vert = graph.nodes().size();
    double bestPathWeight = std::numeric_limits<double>::max();
    std::vector<std::string> bestPath;


    while(true){
        iterNo++;

        /* Do an iteration */
        for(int i = 0; i < vert; i++){
            /* make every ant move */
            for (auto &ant : antColony){
                ant.move(target);
            }
            /* peheromone evaporation and update */
            for(auto &node : graph.nodes()){
                for(auto &edge: node.second.edges()){
                    edge.pheromoneUpdate(argConf.p());
                }
            }
        }

        /* Save the best existing path */
        noChangeCount++;
        for(int i = 0; i < argConf.AntNo(); i++){
            if(antColony[i].pathWeight() < bestPathWeight){
                if(( antColony[i].path().size() >  vert) &&
                    (antColony[i].path().front().compare(target) == 0 ) &&
                    (antColony[i].path().back().compare(target) == 0 ))
                {
                    bestPathWeight = antColony[i].pathWeight();
                    bestPath = antColony[i].path();
                    noChangeCount = 0;

                    std::cout << "[" << iterNo <<"] Best path {";
                    for(auto & V: bestPath){
                        std::cout << " " << V;
                    }
                    std::cout << " } W = " << bestPathWeight << "\n";
                }
                else
                {
                    if( antColony[i].path().size() <=  vert)
                        degeneratedPathes++;
                }
            }
        }

        /* Reinitialize */
        if(noChangeCount < argConf.SpNoChangeIterations())
            for (auto &ant : antColony) ant.init("G");
        else
            for (auto &ant : antColony) ant.init();

        /* Stop condition */
        if(noChangeCount > argConf.MaxNoChangeIterations())
            break;
    }

    std::cout << "[" << iterNo <<"] Best path did not change for " << argConf.MaxNoChangeIterations() << " iterations -> STOP\n";
    std::cout << "Best path {";
    for(auto & V: bestPath){
        std::cout << " " << V;
    }
    std::cout << " } W = " << bestPathWeight << "\n";
    std::cout << "Degenerated Pathes " << degeneratedPathes << "\n";
}