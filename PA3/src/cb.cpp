#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <list>
using namespace std;

typedef pair<int, int16_t> AdjPair;
#define WHITE 0
#define GRAY  1
#define BLACK 2

struct Edge{
  int u, v;
  int16_t w;
  bool operator<(const Edge &e){
    return (this->w < e.w);
  }
  bool operator==(const Edge &e){
    return (e.u == this->u) && (e.v == this->v) && (e.w == this->w);
  }
  Edge &operator=(const Edge &e){
    this->u = e.u;
    this->v = e.v;
    this->w = e.w;
    return *this;
  }
};

struct subset{
    int parent;
    int rank;
};

class DisjoinSet{
  public :   
    DisjoinSet(uint32_t &);
    ~DisjoinSet();
    int find(int &);
    void Union(int &, int &); 
    uint32_t nSubset = 0;
    subset *subs;
};

class Graph{
  public:
    Graph(uint32_t , uint32_t , bool);
    ~Graph();
    void addEdge(int &, int &, int16_t &);
    void KruskalMST(vector<Edge *> &);
    void ForDirect(vector<Edge *> &);
    bool DFS();
    bool DFS_Visit(int &, uint8_t *);
    uint32_t nVertices = 0;
    uint32_t nEdges = 0;
    bool directed = false;
    list<AdjPair> *adj;
    vector<Edge> edges;
};

class CycleBreaking{
  public:
    CycleBreaking() {};
    ~CycleBreaking() {};
    void processing();
    void readFile(char *);
    void writeFile(char *);
    bool Directed = false;
    int rmWeightSum = 0;
    Graph *graph;
    vector<Edge*> removedEdges;
};

void CycleBreaking::processing(){
    if(Directed)
        graph->ForDirect(removedEdges);
    else
        graph->KruskalMST(removedEdges);
    for (uint32_t i = 0; i < removedEdges.size(); ++i)
        rmWeightSum += removedEdges[i]->w;
}

void CycleBreaking::readFile(char *filename){
    fstream fin;
    fin.open(filename, ios::in);
    uint32_t nV = 0, nE = 0;
    char type;
    fin >> type;
    if (type == 'd')
        Directed = true;
   
    fin >> nV;
    fin >> nE;

    graph = new Graph(nV, nE, Directed); 

    for (uint32_t k = 0; k < nE; ++k){
        int i, j;
        int16_t w;
        fin >> i >> j >> w;
        graph->addEdge(i, j, w);
    }

    fin.close();
}

void CycleBreaking::writeFile(char *filename){
    fstream fout;
    fout.open(filename, ios::out);
    
    fout << rmWeightSum << endl;
    for (uint32_t i = 0; i < removedEdges.size(); ++i)
        fout << removedEdges[i]->u << " " << removedEdges[i]->v << " " << removedEdges[i]->w << "\n";

    fout.flush();
    fout.close();
    delete graph;
}


Graph::Graph(uint32_t nv, uint32_t ne, bool dir) : nVertices(nv), nEdges(ne), directed(dir){
    if (directed)
        adj = new list<AdjPair>[nv];
    edges.reserve(ne);
}

Graph::~Graph(){
    if (directed)
        delete[] adj;
}
void Graph::addEdge(int &u, int &v, int16_t &w){
    if (directed)
        adj[u].push_back(make_pair(v, w));
    edges.push_back(Edge{u, v, w});
}

void Graph::KruskalMST(vector<Edge *> & remove){
    DisjoinSet set(nVertices);
    sort(edges.begin(), edges.end());
    for (int32_t i = edges.size() - 1; i >= 0; --i){
        if (set.find(edges[i].u) != set.find(edges[i].v))
            set.Union(edges[i].u, edges[i].v);
        else
            remove.push_back(&edges[i]);
    }
}

void Graph::ForDirect(vector<Edge *> &remove){
    vector<Edge *> preremove;
    KruskalMST(preremove);
    for (uint32_t i = 0; i < preremove.size(); ++i){
        Edge e = *preremove[i];
        list<AdjPair>::iterator it = std::find(adj[e.u].begin(), adj[e.u].end(), make_pair(e.v, e.w));
        adj[e.u].erase(it);
    }
    for (uint32_t i = 0; i < preremove.size(); ++i){
        if ((*preremove[i]).w < 0) {
            remove.push_back(preremove[i]);
            continue;
        }
        adj[(*preremove[i]).u].push_back(make_pair((*preremove[i]).v, (*preremove[i]).w));
        if (DFS()){
            adj[(*preremove[i]).u].pop_back();
            remove.push_back(preremove[i]);
        }
    }
}

bool Graph::DFS(){
    uint8_t color[nVertices] = {WHITE};
    for (int v = 0; v < int(nVertices); ++v)
        if(color[v] == WHITE)
            if (DFS_Visit(v, color))
                return true;
    return false;
}

bool Graph::DFS_Visit(int &u, uint8_t *color){
    color[u] = GRAY;
    for (auto i = adj[u].begin(); i != adj[u].end(); ++i){
        int v = (*i).first;
        if ((color[v] == GRAY) || (color[v] == WHITE && DFS_Visit(v, color)))
            return true;
    }
    color[u] = BLACK;
    return false;
}
DisjoinSet::DisjoinSet(uint32_t &n) : nSubset(n){
    subs = new subset[n];
    for (uint32_t i = 0; i < nSubset; ++i){
        subs[i].parent = i;
        subs[i].rank = 0;
    }
}

DisjoinSet::~DisjoinSet(){
    delete[] subs;
}

int DisjoinSet::find(int &i){
    if (i != subs[i].parent)
        subs[i].parent = find(subs[i].parent);
    return subs[i].parent;
}

void DisjoinSet::Union(int &i, int &j){
    int root_i = find(i);
    int root_j = find(j);
    if (subs[root_i].rank > subs[root_j].rank)
        subs[root_j].parent = root_i;
    else{
        subs[root_i].parent = root_j;
        if (subs[root_i].parent == subs[root_j].parent)
            subs[root_j].rank++;
    }
}
int main(int argc, char* argv[]){
    CycleBreaking cb;
    cb.readFile(argv[1]);
    cb.processing();
    cb.writeFile(argv[2]);
}