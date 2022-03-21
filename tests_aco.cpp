#include <gtest/gtest.h>
#include <cmath>

#include "graph.h"
#include "ant.h"


class GraphTest : public ::testing::Test {

    protected:
        void SetUp() override {
            graph.pushNode("A", 0, 0);
            graph.pushNode("B", 5, 5);
            graph.pushNode("C", 6, 8);
            graph.pushNode("D", 7, 9);
            graph.pushEdge("A","B");
            graph.pushEdge("A","C");
            graph.pushEdge("A","D");
            graph.pushEdge("B","C");

            abEdgeWeight = std::sqrt(std::pow(graph["B"].x() - graph["A"].x(), 2.0) + std::pow(graph["B"].y() - graph["A"].y(), 2.0));
            bcEdgeWeight = std::sqrt(std::pow(graph["C"].x() - graph["B"].x(), 2.0) + std::pow(graph["C"].y() - graph["B"].y(), 2.0));
            pathWeight = abEdgeWeight + bcEdgeWeight;
        }

        // void TearDown() override {}

        Graph graph;

        double abEdgeWeight;
        double bcEdgeWeight;
        double pathWeight;
};


TEST_F(GraphTest, GraphEdgeWeightIsCalculatedAsDistanceBetweenNodesInKarthesianCoordSys){
    EXPECT_DOUBLE_EQ(graph.getEdge("A", "B").weight(), abEdgeWeight);
}

TEST_F(GraphTest, GraphEdegeIsAddedToBothNodesAsUndriectedEdge){
    ASSERT_NO_THROW(graph.getEdge("A", "B"));
    ASSERT_NO_THROW(graph.getEdge("B", "A"));
    EXPECT_EQ(graph.getEdge("A","B").source(), graph.getEdge("B","A").destination() );
    EXPECT_EQ(graph.getEdge("A","B").destination(), graph.getEdge("B","A").source() );
}

TEST_F(GraphTest, GraphEdgesCanBeAccessedThroughCollection){
    int count = 0;
    ASSERT_NO_THROW({
        for(auto& n: graph["A"].edges()){
            count++;
        }
    });
    EXPECT_EQ(count, 3);
}

TEST_F(GraphTest, GraphNodesCanBeAccessedThroughCollection){
    int count = 0;
    ASSERT_NO_THROW({
        for(auto& n: graph.nodes()){
            count++;
        }
    });
    EXPECT_EQ(count, 4);
}

TEST_F(GraphTest, GraphEdgeWeightIsTheSameInBothDirections){
    EXPECT_EQ(graph.getEdge("A","B").weight(), graph.getEdge("B","A").weight());
}

TEST_F(GraphTest, GraphPathIsCalculatedAsSumOfEdgesWeights ){
    EXPECT_DOUBLE_EQ( (graph.getEdge("A", "B") + graph.getEdge("B", "C")) , pathWeight);
}

TEST_F(GraphTest, GraphPheriomoneSetChangesValueOfPheromoneToGivenValue ){
    graph["A"]["B"].pheromoneSet(10.0);
    EXPECT_DOUBLE_EQ( graph["A"]["B"].pheromone(), 10.0);
}

TEST_F(GraphTest, GraphPheriomoneUpdateEveporatesThePheromoneWithGivenCoeficient){
    graph["A"]["B"].pheromoneSet(10.0);
    graph["A"]["B"].pheromoneUpdate(40.0);
    
    EXPECT_DOUBLE_EQ( graph["A"]["B"].pheromone(), 6.0);
}

TEST_F(GraphTest, GraphPheriomoneUpdateAddsPheromoneToAddValueAfterEvaporation){
    graph["A"]["B"].pheromoneSet(10.0);
    graph["A"]["B"].pheromoneToAdd(80.0);
    graph["A"]["B"].pheromoneUpdate(40.0);
    
    EXPECT_DOUBLE_EQ( graph["A"]["B"].pheromone(), 86.0);
}

TEST_F(GraphTest, GraphAfterPheriomoneUpdateValueToAddIsCleared){
    graph["A"]["B"].pheromoneSet(10.0);
    graph["A"]["B"].pheromoneToAdd(80.0);
    graph["A"]["B"].pheromoneUpdate(40.0);
    graph["A"]["B"].pheromoneUpdate(0.0);
    
    EXPECT_DOUBLE_EQ( graph["A"]["B"].pheromone(), 86.0);
}