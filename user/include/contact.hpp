#pragma once

#include <map>
#include <cstring>
#include <vector>
#include <sstream>
#include <iostream>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/map.hpp>

// #include <boost/graph/adjacency_list.hpp>
// #include <boost/graph/topological_sort.hpp>
// #include <boost/graph/graphviz.hpp>

// typedef boost::adjacency_list<boost::listS, boost::vecS, boost::directedS> digraph;
const static int SIZE = 100;

struct contact_data{
    std::map<int, std::string> uv;
    std::map<std::string, int> vu;
    std::vector<std::vector<int>> graph;
	std::string uid;

    template<class Archive>
    void serialize(Archive & archive){
        archive(uv, vu, graph, uid);
    }
};
typedef struct contact_data contact_data;
class Contact
{
private:
    int n = 0; // Current node
    
    std::map<int, std::string> uid_from_vertex;
    std::map<std::string, int> vertex_from_uid;
    std::vector<std::vector<int>> graph;
    void addToMap(int n, std::string uid);
    void addGraph(contact_data d, int v);
    void addEdge(int u, int v);
    void DFSUtil(int u, std::vector<bool> &visited, contact_data g);
    bool find(std::string uid);
    
public:
	std::string uid;
    Contact(std::string uid_hash);
    void AddContact(std::string uid, contact_data data);
    void PrintGraph();
	void PrintAllUIDS();
	void DFS(contact_data data);
    std::string Serialize();
    contact_data Deserialize(std::string cdata);
};

// Set up the user's contact list
Contact::Contact(std::string uid_hash){
	/*for(int i = 0; i < SIZE; i++){
		graph[i].resize(SIZE);
	}*/

    /*for(int i = 0; i < graph->size(); i++){
        for(int j = 0; j < graph[i].size(); j++){
            graph[i][j] = -1;
        }
    }*/
	graph.resize(SIZE);
    addToMap(0, uid_hash);
	this->uid = uid_hash;
}

void Contact::PrintGraph(){
    for (int v = 0; v < graph.size(); v++){
		std::cout << v << "";
        for (auto x : graph[v])
           std::cout << "-> " << x;
        printf("\n");
    }
	//std::cout << "TEST: " << graph[1][0] << std::endl;
}

void Contact::PrintAllUIDS(){
	for(int i = 0 ; i < this->graph.size(); i++){
		if(this->graph[i].size() > 0){
			std::cout << uid_from_vertex[i] << std::endl;
		}
	}
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
    this->n = 1;
    for(int i = 0; i < this->graph.size(); i++){
		if(this->graph[i].size() > 0){
			std::cout << this->graph[i].size() << std::endl;
			this->n = i + 1;
		}	
	}
	
	std::cout << "N IS: " << this->n << std::endl;
	this->DFS(d);
}

std::string Contact::Serialize(){
	contact_data d;
   	d.uv = uid_from_vertex;
	d.vu = vertex_from_uid;
	d.graph = this->graph;
	d.uid = this->uid;
	//mcpy(d.graph, this->graph, sizeof(this->graph));
	/*for(int i = 0; i < this->graph->size(); i++){
		for(int j = 0; i < this->graph[i].size(); j++){
			d.graph[i].push_back(this->graph[i][j]);
		}
	}*/
	std::stringstream ss;
    {
        cereal::PortableBinaryOutputArchive oarchive(ss);
	   	// Create an output archive
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
void Contact::DFSUtil(int u, std::vector<bool> &visited, contact_data g){
    visited[u] = true; 	
    // std::cout << u << " "; 
    for (int i=0; i< g.graph[u].size(); i++){ 
        if (visited[g.graph[u][i]] == false){
			std::cout << "N = " << this->n << std::endl;
			std::cout << "ADDING " << (u + this->n) << "," << (g.graph[u][i] + this->n) << std::endl;
            std::cout << g.uv[u] << std::endl;	
		    addEdge(u + this->n, g.graph[u][i]  + this->n);
			addToMap(u + this->n, g.uv[u]);
		   // n++;	
            DFSUtil(g.graph[u][i], visited, g); 
		}
	}
} 
  
// This function does DFSUtil() for all  
// unvisited vertices. 
void Contact::DFS(contact_data data){ 
    std::vector<bool> visited(SIZE, false); 
    for (int u = 0; u < SIZE; u++) 
        if (visited[u] == false) 
            DFSUtil(u, visited, data);
	//if(this->n == 1){
	addEdge(this->n,0);
	std::cout << "ADDING " << data.uid << std::endl;
	addToMap(this->n, data.uid);
	//}      
} 
