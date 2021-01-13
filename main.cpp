#include "Graph.h"
#include <iostream>
#include "HashTable.h"
/*You can add additional libraries here to read and process the dataset*/
#include <fstream> // file read/write

using namespace std;

// tester
int main() {

    // read input file
    ifstream inputF;

    // Metal trade Graph
    Graph graph;

    fstream file("world_trade.txt");

    string vertices;
    getline(file, vertices, '\t'); // skip header
    getline(file, vertices, '\n'); // number of vertices

    int numvertices = atoi( vertices.c_str() );
    HashTable<int, Node> nodeTable;

    // Build the graph from the txt input file
    for (int i = 0; i < numvertices; i++) {

        string vid, country, continent, gdp;
        getline(file, vid, '\t');
        getline(file, country, '\t');
        getline(file, continent, '\t');
        getline(file, gdp, '\n');

        Node node(atoi(vid.c_str()), country, continent, atol(gdp.c_str()));
        nodeTable.Insert(node.getVid(), node);

        graph.addNode(node);
    }

    getline(file, vertices, '\n'); // skip header

    // create connections
    string line;
    while (getline(file, line)) {
        int pos = line.find("\t");
        int vid1 = atoi(line.substr(0, pos).c_str());

        line = line.substr(pos+1);
        pos = line.find("\t");
        int vid2 = atoi(line.substr(0, pos).c_str());

        line = line.substr(pos+1);
        long import = atol(line.c_str());

        graph.addConnection(nodeTable.Get(vid1), nodeTable.Get(vid2), import);
    }
    file.close();


	// YOU CAN WRITE YOUR TEST CASES BELOW
    Node node1(1,"Honduras","",1), node2(1,"Guatemala","",1);
    list<string> llist = graph.findLeastCostPath(node1,node2);
    for(list<string>::iterator it=llist.begin() ; it!=llist.end() ; ++it){
        cout << *it << " ";
    }


/*
    HashTable<int,int> h;
    h.Insert(1,1);
    h.Insert(12,12);
    h.Insert(9,9);
    h.Insert(8,8);
    h.Insert(3,3);
    h.Insert(4,4);
    h.Insert(13,13);
    h.Insert(52,52);
    h.Insert(20,20);
    h.Insert(2,2);
    h.Insert(15,15);
    h.Insert(22,22);
    h.Insert(36,36);
    h.Insert(37,37);
    h.Insert(27,27);
    h.Insert(101,101);
    h.Insert(112,112);
    h.Insert(120,120);
    h.Insert(108,108);
    h.Insert(130,130);
    h.Insert(104,104);
    h.Insert(117,117);
    h.Insert(133,133);
    h.Insert(143,143);
    h.Insert(192,192);
    h.Insert(154,154);
    h.Insert(200,200);
    h.Insert(204,204);
    h.Insert(217,217);
    h.Insert(233,233);
    h.Insert(243,243);
    h.Insert(292,292);
    h.Insert(254,254);
    h.Insert(330,330);
    h.Insert(304,304);
    h.Insert(317,317);
    h.Insert(333,333);
    h.Insert(343,343);
    h.Insert(392,392);
    h.Insert(354,354);
    h.Insert(430,430);
    h.Insert(404,404);
    h.Insert(417,417);
    h.Insert(433,433);
    h.Insert(443,443);
    h.Insert(492,492);
    h.Insert(454,454);
    h.Insert(530,530);
    h.Insert(504,504);
    h.Insert(517,517);
    h.Insert(533,533);
    h.Insert(543,543);
    h.Insert(592,592);
    h.Insert(554,554);
    int* keys = new int[54];
    h.getKeys(keys);
    for(int i=0 ; i<54 ; ++i){
        cout << keys[i] << "," << h.Get(keys[i]) << endl;
    }*/
    return 0;
}
