/*
 * WeightedGraph.h
 *
 *  Created on: 29 Apr 2023
 *      Author: 2002345
 */
#ifndef H_weightedGraph
#define H_weightedGraph

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cfloat>
#include <queue>

using namespace std;

class WeightedGraphType
{
protected:
    int gSize;        // number of vertices
    list<int> *graph; // Store adjacency list
    double **weights; // Store weights of edges
public:

    double* smallestWeight; // FOR TASK 6
    WeightedGraphType(int size = 0);
    ~WeightedGraphType();

    list<int> getAdjacencyList(int index)
    {
        return graph[index];
    }

    double getWeight(int i, int j)
    {
        return weights[i][j];
    }

    void printAdjacencyList();
    void printAdjacencyMatrix();
    list<int> breadthFirstTraversal();
    list<int> linearOrderTraversal();

    void shortestPath(int vertex);
};

void WeightedGraphType::shortestPath(int vertex)
{
    // Initialize the smallestWeight array with the weights of the edges from the given vertex
    for (int j = 0; j < gSize; j++)
        smallestWeight[j] = weights[vertex][j];

    // Create a boolean array to keep track of which vertices' weights have been found
    bool *weightFound;
    weightFound = new bool[gSize];

    // Initialize weightFound array to false for all vertices
    for (int j = 0; j < gSize; j++)
        weightFound[j] = false;

    // Mark the given vertex as found and set its weight to 0
    weightFound[vertex] = true;
    smallestWeight[vertex] = 0;

    // Find the shortest path for each vertex
    for (int i = 0; i < gSize - 1; i++)
    {
        double minWeight = DBL_MAX; // Initialize the minimum weight to the maximum possible value
        int v; // Variable to store the vertex with the minimum weight

        // Find the vertex with the minimum weight among the vertices that are not yet found
        for (int j = 0; j < gSize; j++)
        {
            if (!weightFound[j])
            {
                if (smallestWeight[j] < minWeight)
                {
                    v = j;
                    minWeight = smallestWeight[v];
                }
            }
        }

        weightFound[v] = true; // Mark the vertex with the minimum weight as found

        // Update the smallestWeight array for the neighboring vertices of the current vertex
        for (int j = 0; j < gSize; j++)
        {
            if (!weightFound[j])
            {
                if (minWeight + weights[v][j] < smallestWeight[j])
                    smallestWeight[j] = minWeight + weights[v][j];
            }
        }
    } //end for
} //end shortestPath


WeightedGraphType::WeightedGraphType(int size)
{
    gSize = 0;
    ifstream infile;
    char fileName[50] = "Weights.txt";

    //	cout << "Enter graph adjacency matrix file name: ";
    //	cin >> fileName;
    //	cout << endl;

    infile.open(fileName);

    if (!infile)
    {
        cout << "Cannot open input file." << endl;
        return;
    }

    gSize = size;

    graph = new list<int>[gSize];

    weights = new double *[gSize];

    //FOR TASK 6
    smallestWeight = new double[gSize];

    for (int i = 0; i < gSize; i++)
        weights[i] = new double[gSize];

    for (int i = 0; i < gSize; i++)
    {
        for (int j = 0; j < gSize; j++)
        {
            double value;
            infile >> value;
            if (value == 0)
                weights[i][j] = DBL_MAX; // system constant - maximum value of double
            else
            {
                weights[i][j] = value;
                graph[i].push_back(j);
            }
        }
    }
    infile.close();
}

WeightedGraphType::~WeightedGraphType()
{
    for (int i = 0; i < gSize; i++)
        delete[] weights[i];

    delete[] weights;

    for (int index = 0; index < gSize; index++)
        graph[index].clear();

    delete[] graph;
}

void WeightedGraphType::printAdjacencyMatrix()
{ // print adjacency matrix for debug purpose
    cout << "\nAdjacency Matrix" << endl;
    for (int i = 0; i < gSize; i++)
    {
        for (int j = 0; j < gSize; j++)
        {
            cout << setw(8) << (weights[i][j] == DBL_MAX ? 0.0 : weights[i][j]); // as adjacency value, zero means no direct connection
        }
        cout << endl;
    }
}

void WeightedGraphType::printAdjacencyList()
{ // print adjacency list for debug purpose
    cout << "\nAdjacency List" << endl;
    for (int index = 0; index < gSize; index++)
    {
        cout << index << ": ";
        for (int e : graph[index])
            cout << e << " ";
        cout << endl;
    }

    cout << endl;
}

list<int> WeightedGraphType::breadthFirstTraversal()
{
    queue<int> queue;
    list<int> nodeOrder;

    bool *visited;
    visited = new bool[gSize];

    for (int ind = 0; ind < gSize; ind++)
        visited[ind] = false;

    list<int>::iterator graphIt;

    for (int index = 0; index < gSize; index++)
        if (!visited[index])
        {
            queue.push(index);
            visited[index] = true;
            nodeOrder.push_back(index);

            while (!queue.empty())
            {
                int u = queue.front();
                queue.pop();

                for (graphIt = graph[u].begin(); graphIt != graph[u].end(); ++graphIt)
                {
                    int w = *graphIt;
                    if (!visited[w])
                    {
                        queue.push(w);
                        visited[w] = true;
                        nodeOrder.push_back(w);
                    }
                }
            }
        }

    delete[] visited;
    return nodeOrder;
} // end breadthFirstTraversal

list<int> WeightedGraphType::linearOrderTraversal()
{
    list<int> nodeOrder;

    for (int index = 0; index < gSize; index++)
    {
        nodeOrder.push_back(index);
    }

    return nodeOrder;
}

#endif