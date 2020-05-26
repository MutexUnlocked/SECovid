#pragma once

#include<bits/stdc++.h> 

class Contact
{
private:
    std::vector<std::vector<int>> cts; 
    void DFSUtil(int u, std::vector<std::vector<int>>, 
				std::vector<bool> &visited);
public:
    void AddContact(int u, int v);
    void DFS(int V);
};


// A utility function to add an edge in an 
// undirected graph. 
void Contact::AddContact(int u, int v) 
{ 
	this->cts[u].push_back(v); 
	this->cts[v].push_back(u); 
} 

// A utility function to do DFS of graph 
// recursively from a given vertex u. 
void Contact::DFSUtil(int u, std::vector<std::vector<int>>, 
					std::vector<bool> &visited) 
{ 
	visited[u] = true; 
	std::cout << u << " "; 
	for (int i = 0; i < this->cts[u].size(); i++) 
		if (visited[this->cts[u][i]] == false) 
			DFSUtil(this->cts[u][i], this->cts, visited); 
} 

// This function does DFSUtil() for all 
// unvisited vertices. 
void Contact::DFS(int V) 
{ 
	std::vector<bool> visited(V, false); 
	for (int u = 0; u < V; u++) 
		if (visited[u] == false) 
			DFSUtil(u, this->cts, visited); 
} 


