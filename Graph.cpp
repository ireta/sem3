#include "Graph.h"

/*Do not add new libraries or files*/

struct Order{
    bool operator()(const Graph::Weights& lhs, const Graph::Weights& rhs){return lhs.weight<rhs.weight;}
};

Graph::Graph() {

}

Graph::Graph(const Graph& rhs) {
    adjList = rhs.adjList;
}

Graph& Graph::operator=(const Graph& rhs) {
    adjList = rhs.adjList;
    return *this;
}

Graph::~Graph() {

}


void Graph::addNode(const Node &node) {
    list<Edge> add;
    adjList.Insert(node.getCountry(),add);
}

void Graph::addConnection(const Node& headNode, const Node& tailNode, int import) {
    Edge add(tailNode,import);
    adjList.Get(headNode.getCountry()).push_back(add);
}

list<Node> Graph::getAdjacentNodes(const Node& node) {
    list<Node> result;
    list<Edge> edges = adjList.Get(node.getCountry());
    for(list<Edge>::iterator it=edges.begin() ; it!=edges.end() ; ++it){
        result.push_back(it->getTailNode());
    }
    return result;
}

long Graph::getTotalImports(const Node& node) {
    long result = 0;
    list<Edge> edges = adjList.Get(node.getCountry());
    for(list<Edge>::iterator it=edges.begin() ; it!=edges.end() ; ++it){
        result += it->getImport();
    }
    return result;
}

void Graph::deleteNode(const Node& node) {
    string* keys = new string[adjList.Size()];
    list<Edge>* edges;
    adjList.getKeys(keys);
    for(int i=0 ; i<adjList.Size() ; ++i){
        edges = &adjList.Get(keys[i]);
        for(list<Edge>::iterator it=edges->begin() ; it!=edges->end() ; ++it){
            if(it->getTailNode().getCountry() == node.getCountry()){
                it = edges->erase(it);
                --it;
            }
        }
    }
    delete[] keys;
    adjList.Delete(node.getCountry());
}

list<string> Graph::findLeastCostPath(const Node& srcNode, const Node& destNode) {
    list<string> result;
    vector<Weights> init;
    string* keys = new string[adjList.Size()];
    adjList.getKeys(keys);
    Weights w,v;
    for(int i=0 ; i<adjList.Size() ; ++i){
        w.country = keys[i];
        w.before = "";
        if(keys[i] == srcNode.getCountry()){
            w.weight = 0;
        }else{
            w.weight = numeric_limits<long>::max();
        }
        init.push_back(w);
    }
    delete[] keys;
    priority_queue<Weights,vector<Weights>,Order> pq;
    list<Edge> edges;
    w.before = "";
    w.country = srcNode.getCountry();
    w.weight = 0;
    pq.push(w);
    unsigned int i=0;
    while(!pq.empty()){
        w = pq.top();
        pq.pop();
        edges = adjList.Get(w.country);
        for(list<Edge>::iterator it=edges.begin() ; it!=edges.end() ; ++it){
            for(i=0 ; i<init.size() ; ++i){
                if(init[i].country == it->getTailNode().getCountry()){
                    break;
                }
            }
            if(init[i].weight > w.weight + it->getImport()){
                init[i].weight = w.weight + it->getImport();
                init[i].before = w.country;
                v.country = init[i].country;
                v.weight = init[i].weight;
                pq.push(v);
            }
        }
    }
    for(i=0 ; i<init.size() ; ++i){
        if(init[i].country == destNode.getCountry()){
            break;
        }
    }
    w = init[i];
    while(w.before != ""){
        result.push_front(w.country);
        for(i=0 ; i<init.size() ; ++i){
            if(init[i].country == w.before){
                break;
            }
        }
        w = init[i];
    }
    result.push_front(srcNode.getCountry());
    return result;
}


bool Graph::isCyclic() {
    string* keys = new string[adjList.Size()];
    adjList.getKeys(keys);
    Node tmp;
    list<string> result;
    for(int i=0 ; i<adjList.Size() ; ++i){
        tmp.setCountry(keys[i]);
        result = getBFSPath(tmp,tmp);
        if(result.back() == result.front() && result.size() > 2){
            delete[] keys;
            return true;
        }
    }
    delete[] keys;
    return false;
}


list<string> Graph::getBFSPath(const Node& srcNode, const Node& destNode) {
    int siz = adjList.Size();
    pair<string,bool>* visited = new pair<string,bool>[siz];
    string* keys = new string[siz];
    adjList.getKeys(keys);
    for(int i=0 ; i<siz ; ++i){
        visited[i].first = keys[i];
        visited[i].second = false;
        if(keys[i] == srcNode.getCountry()){
            visited[i].second = true;
        }
    }
    list<string> Queue;
    list<string> result;
    result.push_back(srcNode.getCountry());
    Queue.push_back(srcNode.getCountry());
    string last = Queue.front();
    int i=0;
    while(!Queue.empty()){
        last = Queue.front();
        Queue.pop_front();
        for(list<Edge>::iterator it=adjList.Get(last).begin() ; it!=adjList.Get(last).end() ; ++it){
            if(it->getTailNode().getCountry() == destNode.getCountry()){
                result.push_back(destNode.getCountry());
                delete[] keys;
                delete[] visited;
                return result;
            }
            for(i=0 ; i<siz ; ++i){
                if(keys[i] == it->getTailNode().getCountry()){
                    break;
                }
            }
            if(!visited[i].second){
                visited[i].second = true;
                Queue.push_back(it->getTailNode().getCountry());
                result.push_back(it->getTailNode().getCountry());
            }
        }
    }
    delete[] keys;
    delete[] visited;
    return result;
}
