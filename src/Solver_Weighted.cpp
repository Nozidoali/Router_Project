#include "Solver_Weighted.h"
#include "Logic_Astar.h"
#include "Logic_UnionFind.h"
#include <queue>
#include <iostream>
#include <map>
using namespace std;

typedef pair<int, Point> DIS_POINT;

/**
 * Link
 * 
 * return the shortest path between two points
 */
Segment * Link( RoutingInst * rst, Point a, Point b ) {

// let segment->p1 to be the start point and p2 to be the end point
    Astar_Man * pMan = new Astar_Man( rst, a, b, 1 );
    Segment * segment = new Segment( a, b );
 
    priority_queue<DIS_POINT> q;
    Point dir[4] = {Point(1,0),Point(-1,0),Point(0,1),Point(0,-1)};

    // set a to be the start point
    q.push( make_pair( distance( a,b ), a ) );
    pMan->visit( a );
    pMan->go( a, -1, distance( a,b ) );
    
    // loop until finding b
    while( !q.empty() ) {
        DIS_POINT target = q.top();
        pMan->visit( target.second );
        q.pop();
        // quit if reach the end point
        if (target.second == b) {
            pMan->retrace( rst, segment, b );
            delete pMan;
            return segment;
        }
        for(unsigned int i=0;i<4;i++) {
            Point point = target.second + dir[i];
            int index = rst->toIndex( point, target.second );
            // continue if point is out of range
            if ( !pMan->IsValid( point ) ) {
                continue;
            }
            // continue if point is visited
            if ( pMan->isVisited(point) ) {
                continue;
            }

// ==============================A* search=============================
            int cost = 0;
            // net cost of target
            cost += target.first - distance( target.second, b );
            // weight of the edge between target and point
            cost += Weight( rst->edgeCaps[index], rst->edgeUtils[index] );
            // weight of estimate distance
            cost += distance( point, b );

            if ( pMan->go( point, i, cost ) ) {
                q.push( make_pair( cost, point ) );
            }
        }
    }

    delete segment;
    delete pMan;
    return NULL;
}
struct Edge {
    Point a,b;
    int ia,ib;
    bool operator < (const Edge e) const { return distance(a,b) > distance(e.a, e.b); };
    Edge(Point _a, Point _b, int _ia, int _ib);
};

void SolveNet ( RoutingInst * rst, Net * net) {
    
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

    int total = net->numPins;
    int count = 0;

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
            net->nroute.AddSegment(Link( rst, a, b ));
// =================================End=================================
        }
    }
}

void Weighted_Solver :: Solve() {
    for(int i=0;i<rst->numNets;i++) {
        cerr << "Net: " << i+1 << " / " << rst->numNets << "\r";
        fflush(stderr);
        Net * net = rst->nets + i;
        SolveNet( rst, net );
    }
}