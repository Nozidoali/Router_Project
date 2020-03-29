#include "Task_Net.h"

#include "Logic_Astar.h"
#include <queue>
#include <iostream>
#include <map>
#include <algorithm>
#include <thread>


#include "Logic_UnionFind.h"
struct Edge {
    Point a,b;
    int ia,ib;
    bool operator < (const Edge e) const { return distance(a,b) > distance(e.a, e.b); };
    Edge(Point _a, Point _b, int _ia, int _ib);
};
NetTask :: NetTask( RoutingInst * _rst, Net * _net ) {
    rst = _rst;
    net = _net;
    tasks.reserve(net->numPins-1);

    // Declare a priority queue to maintain the MST
    priority_queue<Edge> queue;

    // Add all the possible edges to the queue
    for(int i=0;i<net->numPins;i++) {
        for(int j=i+1;j<net->numPins;j++) {
            queue.push(Edge(net->pins[i], net->pins[j], i, j));
        }
    }

    // init Union Find Solver
    UnionFind uf(net->numPins);
    while(!queue.empty()) {
        Edge e = queue.top();
        queue.pop();
        if (uf.isMerged(e.ia,e.ib)) {
            // continue if we have already connected them
            continue;
        }
        else {

            // merge to indicate that we have already connected a and b
            uf.Merge(e.ia,e.ib);

            Point a = e.a;
            Point b = e.b;

// =================================Start=================================
            tasks.push_back( new PinsTask( rst, net, a, b ));
// =================================End=================================
        }
    }
}

NetTask :: ~NetTask() {
    tasks.clear();
}

static void SolveLocal( vector<PinsTask *> tasks, int start, int end ) {
    for(int i=start;i<end;i++) {
        tasks[i]->Solve();
    }
} 

void NetTask :: Solve() {

#ifdef MULTI_THREAD
    int size = net->numPins - 1;

    int numThread = 8;
    int steps = size / numThread;
    thread * workers[ numThread ];

    // assign the working region of workers
    for(int i=0;i<numThread;i++) {
        int start = i*steps;
        int end = i==numThread-1 ? size : (i+1)*steps; 
        workers[i] = new thread ( SolveLocal, tasks, start, end );
    }
    
    // join all the threads together
    for(int i=0;i<numThread;i++) {
        workers[i]->join();
    }
#endif

    for(int i=0;i<net->numPins-1;i++) {
        tasks[i]->Solve();
    }
    
}


void NetTask :: Remove() {
    for(int i=0;i<net->numPins-1;i++) {
        tasks[i]->Remove();
    }
}

void NetTask :: Save() {
    for(int i=0;i<net->numPins-1;i++) {
        tasks[i]->Save();
    }
}