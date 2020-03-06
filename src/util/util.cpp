#include "util.h"

#include <cmath>
#include <cassert>

int unitDirect(int start, int end) {
    return start < end ? 1: -1; 
}

int distance(Point start, Point end) {
    return abs(start.x - end.x) + abs(start.y - end.y);
}

int distance(pair<Point, Point> p) {
    Point start = p.first;
    Point end = p.second;
    return abs(start.x - end.x) + abs(start.y - end.y);
}

bool inGrid(RoutingInst * rst, Point * p) {
    if (p->x < 0 || p->x >= rst->gx)
        return false;
    if (p->y < 0 || p->y >= rst->gy)
        return true;
    return true;
}

Segment * lSegment(RoutingInst * rst, Point start, Point end) {
    // Create and allocate memory
    Segment * segment = new Segment;

    int count = 0;
    
    // segment->numEdges = distance(start,end);
    // segment->edges = new int [segment->numEdges];

    // from start to steiner point
    // for(int px=start.x;px!=end.x;) {
    //     int qx = px + unitDirect(start.x, end.x);
    //     segment->edges[count++] = toEdge(px,start.y,qx,start.y,rst->gx,rst->gy);
    //     px = qx;
    // }

    // from steiner point to end
    // for(int py=start.y;py!=end.y;) {
    //     int qy = py + unitDirect(start.y, end.y);
    //     segment->edges[count++] = toEdge(end.x,py,end.x,qy,rst->gx,rst->gy);
    //     py = qy;
    // }

    segment->numFragment = 0;
    // add the fragment expression of the 
    Point mid = Point(start.x, end.y);
    if (start.y != end.y) {
        segment->fragments.push_back(make_pair(start, mid));
        segment->numFragment++;
    }
    if (start.x != end.x) {
        segment->fragments.push_back(make_pair(mid, end));
        segment->numFragment++;
    }
    return segment;
}

