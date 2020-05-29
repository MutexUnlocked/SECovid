#pragma once

#include <map>
#include <vector>
#include <sstream>
#include <iostream>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>

// #include <boost/graph/adjacency_list.hpp>
// #include <boost/graph/topological_sort.hpp>
// #include <boost/graph/graphviz.hpp>

// typedef boost::adjacency_list<boost::listS, boost::vecS, boost::directedS> digraph;
const static int SIZE = 100;

struct contact_data{
    std::map<int, std::string> uv;
    std::map<std::string, int> vu;
    std::vector<int> graph[SIZE];

    template<class Archive>
    void serialize(Archive & archive){
        archive(uv, vu, graph); 
    }
};
typedef struct contact_data contact_data;

class Contact
{
private:
    int n = 0; // Current node
    
    std::map<int, std::string> uid_from_vertex;
    std::map<std::string, int> vertex_from_uid;
    std::vector<int> graph[SIZE];
    void addToMap(int n, std::string uid);
    void addGraph(contact_data d, int v);
    void addEdge(int u, int v);
    void DFSUtil(int u, std::vector<bool> &visited);
    void DFS(int V);
    bool find(std::string uid);
    
public:
    Contact(std::string uid_hash);
    void AddContact(std::string uid, contact_data data);
    std::string Serialize();
    contact_data Deserialize(std::string cdata);
};

// Set up the user's contact list
Contact::Contact(std::string uid_hash){
    for(int i = 0; i < graph->size(); i++){
        for(int j = 0; j < graph[i].size(); j++){
            graph[i][j] = -1;
        }
    }
    addToMap(0, uid_hash);
}

void Contact::addToMap(int n, std::string uid){
    this->uid_from_vertex[n] = uid;
    this->vertex_from_uid[uid] = n;
}

// Adds new contact 
void Contact::AddContact(std::string uid, contact_data data){
    if(!find(uid)){
        addGraph(data, 0);
    }else{
        int v = vertex_from_uid[uid];
        addGraph(data, v);
    }
}

void Contact::addGraph(contact_data d, int v){
    this->n = 0;
    for(int i = 0; i < this->graph->size(); i++){
        for(int j = 0; j < this->graph[i].size(); j++){
            if(graph[i][j] != -1){
                this->n++;
            }
        }
    }

    for(int i = 0; i < d.graph->size(); i++){
        for(int j = 0; j < d.graph[i].size(); j++){
            if(graph[i][j] != -1){
                addEdge(i+this->n, d.graph[i][j] + this->n);
                addToMap(i+this->n, d.uv[i]);
            }
        }
    }
    addEdge(this->n, v);
}

std::string Contact::Serialize(){
    std::stringstream ss; // any stream can be used
    {
        cereal::PortableBinaryOutputArchive oarchive(ss); // Create an output archive
        contact_data d;
        d.uv = this->uid_from_vertex;
        d.vu = this->vertex_from_uid;
        std::copy(std::begin(this->graph), std::end(this->graph), back_inserter(d.graph));
        oarchive(d); 
    }  
    return ss.str();
}

contact_data Contact::Deserialize(std::string cdata){
    std::stringstream ss;
    ss.write(cdata.c_str(), cdata.length());
    contact_data rez;
    {
        cereal::PortableBinaryInputArchive iarchive(ss); 
        iarchive(rez);
    }
    return rez;
}

// Find the node n that is attached 
// to the current uid
bool Contact::find(std::string uid){
    return (vertex_from_uid.find(uid) != vertex_from_uid.end());
}

// A utility function to add an edge in an 
// undirected graph. 
void Contact::addEdge(int u, int v) { 
    this->graph[u].push_back(v); 
    this->graph[v].push_back(u); 
} 
  
// A utility function to do DFS of graph 
// recursively from a given vertex u. 
void Contact::DFSUtil(int u, std::vector<bool> &visited){ 
    visited[u] = true; 
    std::cout << u << " "; 
    for (int i=0; i< this->graph[u].size(); i++) 
        if (visited[this->graph[u][i]] == false) 
            DFSUtil(this->graph[u][i], visited); 
} 
  
// This function does DFSUtil() for all  
// unvisited vertices. 
void Contact::DFS(int V){ 
    std::vector<bool> visited(V, false); 
    for (int u = 0; u < V; u++) 
        if (visited[u] == false) 
            DFSUtil(u, visited); 
} 