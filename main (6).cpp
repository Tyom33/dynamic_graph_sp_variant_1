#include <iostream>
#include <map>
#include <set>
#include <utility> 
#include <vector>
#include <limits>
#include <algorithm>

class FloydWarshall{
public:
  void floydWarshall(std::map<std::pair<int, int>, int>& shortestpaths, 
  std::map<int, std::set<std::pair<int, int>>>& adjList, int a, int b) {
    auto it = shortestpaths.find({a, b});
    if (it != shortestpaths.end()) {
      std::cout << it->second << std::endl;
    }
    else{
      int n = adjList.rbegin()->first + 1;
      std::vector<std::vector<long int>> dist(n, std::vector<long int>(n, 
      std::numeric_limits<int>::max()));
      for (const auto& pair : adjList) {
        int u = pair.first;
        for (const auto& edge : pair.second) {          
          int v = edge.first;
          int weight = edge.second;
          dist[u][v] = weight;
          shortestpaths[{u, v}] = weight;
        }
      }

      for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
          for (int j = 0; j < n; j++) {
            if (dist[i][k] != std::numeric_limits<long int>::max() &&
              dist[k][j] != std::numeric_limits<long int>::max() &&
              dist[i][k] + dist[k][j] < dist[i][j]) {
              dist[i][j] = dist[i][k] + dist[k][j];
            }
          }
        }
      }
      std::cout << dist[a][b] << std::endl;
    }
  }
};
class Djikstra{
public:
  std::pair<long int, std::vector<int>> djikstra(std::map<std::pair<int, int>, int>& 
  shortestpaths, std::map<int, std::set<std::pair<int, int>>>& adjList, int start, int end){
    int n = adjList.rbegin()->first + 1;
    std::vector<long int> dist(n, std::numeric_limits<long int>::max());
    dist[start] = 0;
    std::vector<bool> visited(n, false);
    std::vector<std::vector<int>> vertexQueue(n);
    for (int i = 0; i < n; i++) {
      int nearest = -1;
      for (int v = 0; v < n; v++) {
        if (!visited[v] && (nearest == -1 || dist[nearest] > dist[v])) {
          nearest = v;
        }
      }
      if (dist[nearest] == std::numeric_limits<long int>::max()) {
        break; 
      }

      visited[nearest] = true;
      for (auto& elem : adjList[nearest]) {
        if (dist[elem.first] > dist[nearest] + elem.second) {
          dist[elem.first] = dist[nearest] + elem.second;
          vertexQueue[elem.first] = vertexQueue[nearest];
          vertexQueue[elem.first].push_back(elem.first);
        }
      }
    }
    return { dist[end], vertexQueue[end] };
  }
};

class Graph{
public:
  std::map<int, std::set<std::pair<int, int>>> adjList;
  bool constructionTime = false;
  std::map<std::pair<int, int>, int> shortestpaths;
};

class delegating{
public:
  Graph adjList;
  Djikstra djikstra;
  FloydWarshall floydWarshall;
  void sp_length(int a, int b){
    if (adjList.constructionTime){
      auto result = djikstra.djikstra(adjList.shortestpaths, adjList.adjList, a, b);
      std::cout << "Shortest path length with Djikstra: " << result.first;
    }
    else{
      std::cout << "Shortest path length with Floyd Warshall: ";
      floydWarshall.floydWarshall(adjList.shortestpaths, adjList.adjList, a, b);
    }
    std::cout << std::endl;
  }
  void sp_route(int a, int b){
    auto result = djikstra.djikstra(adjList.shortestpaths, adjList.adjList, a, b);
    std::cout << "Shortest path route: ";
    for (int vertex : result.second) {
      std::cout << vertex << " ";
    }
    std::cout << std::endl;
  }
  void construction_started(){
    adjList.constructionTime = true;
  }
  void construction_finished(){
    adjList.constructionTime = false;
  }
  void street_added(int u, int v, int weight){    
    if (adjList.constructionTime) {
      int i = 0;
      while (i < adjList.adjList.size()){
        auto verticesToRemove = adjList.shortestpaths.find({i, u});
        if (verticesToRemove != adjList.shortestpaths.end()) {
          adjList.shortestpaths.erase(verticesToRemove);
          ++i;
        } 
        else{
          ++i;
        }
      }
      adjList.adjList[u].insert(std::make_pair(v, weight));
      adjList.adjList[v].insert(std::make_pair(u, weight));
    }
    else {
      std::cout << "It's not construction Time";
    }
  }

  void crossroad_added(int u){
    if (adjList.constructionTime){
      int i = 0;
      while (i < adjList.adjList.size()){
        auto verticesToRemove = adjList.shortestpaths.find({i, u});
        if (verticesToRemove != adjList.shortestpaths.end()){
          adjList.shortestpaths.erase(verticesToRemove); 
          ++i;
        } 
        else{
          ++i;
        }
      }
      adjList.adjList[u];
    }
    else{
      std::cout << "It's not construction Time";
    }
  }
  void street_removed(int u, int v, int weight){
    if (adjList.constructionTime){
      int i = 0;
      while (i < adjList.adjList.size()){
        auto verticesToRemove = adjList.shortestpaths.find({i, u});
        if (verticesToRemove != adjList.shortestpaths.end()){
          adjList.shortestpaths.erase(verticesToRemove);
          ++i;
        }   
        else {
          ++i;
        }
      }
      auto it_u = adjList.adjList.find(u);
      auto it_v = adjList.adjList.find(v);
      if (it_u != adjList.adjList.end() && it_v != adjList.adjList.end()){
        it_u->second.erase(std::make_pair(v, weight));
        it_v->second.erase(std::make_pair(u, weight));
      }
      else{
        std::cout << "One or both of the vertices are not in the graph." << std::endl;
      }
    }
    else {
      std::cout << "It's not construction time" << std::endl;
    }
  }
  void crossroad_removed(int u){
    if (adjList.constructionTime){
      int i = 0;
      while (i < adjList.adjList.size()){
        auto verticesToRemove = adjList.shortestpaths.find({i, u});
        if (verticesToRemove != adjList.shortestpaths.end()){
          adjList.shortestpaths.erase(verticesToRemove); 
          ++i;
        } 
        else{
          ++i;
        }
      }
      // Удаляем все вхождения вершины в списке смежности
      adjList.adjList.erase(u);
      // Удаляем вершину из списков смежности других вершин
      for (auto& pair : adjList.adjList){
        auto& adjacencyList = pair.second;
        for (auto it = adjacencyList.begin(); it != adjacencyList.end();){
          if (it->first == u){
            it = adjacencyList.erase(it);
          } 
          else{
            ++it;
          }
        }
      }
    }
    else{
      std::cout << "It's not construction time" << std::endl;
    }
  }
  void printGraph() {
    std::cout << "Graph Size is :" << adjList.adjList.size() << "\n";
    for (const auto& pair : adjList.adjList) {        
      std::cout << pair.first << " -> ";
      for (const auto& edge : pair.second) {
        std::cout << "(" << edge.first << ", " << edge.second << ") ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }
};

int main() {
  delegating run;
  run.construction_started();
  run.crossroad_added(0);
  run.crossroad_added(1);
  run.crossroad_added(2);
  run.crossroad_added(3);
  run.crossroad_added(4);
  run.street_added(0, 1, 7);
  run.street_added(0, 3, 2);
  run.street_added(1, 4, 4);
  run.street_added(1, 2, 5);
  run.street_added(3, 4, 5);
  run.street_added(4, 2, 1);
  run.crossroad_added(5);
  run.crossroad_added(6);
  run.street_added(5, 6, 29);
  run.crossroad_removed(2);
  run.printGraph();
  run.sp_length(0, 4);
  run.sp_route(0, 4);
  run.construction_finished();
  run.sp_length(0, 4);

  run.construction_started();
  run.crossroad_removed(3);
  run.printGraph();
  run.sp_length(0, 4);
  run.sp_route(0, 4);
  run.construction_finished();
  run.sp_length(0, 4);

  run.construction_started();
  run.crossroad_added(7);
  run.street_added(0, 7, 1);
  run.street_added(4, 7, 1);
  run.crossroad_removed(4);
  run.printGraph();
  run.sp_length(0, 1);
  run.sp_route(0, 1);
  run.construction_finished();
  run.sp_length(0, 1);
  
  run.construction_started();
  run.crossroad_added(8);
  run.crossroad_added(9);
  run.crossroad_removed(7);
  run.street_added(1, 8, 11);
  run.street_added(5, 8, 73);
  run.printGraph();
  run.sp_length(1, 8);
  run.sp_route(1, 8);
  run.construction_finished();
  run.sp_length(1, 8);
  
  run.construction_started();
  run.crossroad_added(10);
  run.street_added(9, 10, 101);
  run.printGraph();
  run.sp_length(9, 10);
  run.sp_route(9, 10);
  run.construction_finished();
  run.sp_length(9, 10);
  
  run.construction_started();
  run.crossroad_added(11);
  run.crossroad_added(12);
  run.crossroad_added(13);
  run.crossroad_added(14);
  run.street_added(11, 12, 1);
  run.street_added(12, 14, 3);
  run.street_added(11, 13, 5);
  run.street_added(13, 14, 7);
  run.printGraph();
  run.sp_length(11, 14);
  run.sp_route(11, 14);
  run.construction_finished();
  run.sp_length(11, 14);
return 0;
}
