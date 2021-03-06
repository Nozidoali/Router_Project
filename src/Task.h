#ifndef EDGE_H
#define EDGE_H

#include "Point.h"

typedef int EDGE;
typedef set<EDGE, less<EDGE> > EDGES;

void            Edg_Free            ( EDGES * edges );

struct Task 
{

    int difficulty;
    Point start, end;
    EDGES * edges;

};

typedef vector<Task> Tasks;

//=======================================Memory Control======================//
Task            Tsk_Init            ( Point start, Point end );
void            Tsk_Free            ( Tasks * pTasks );

//=======================================Features============================//
bool            Tsk_HasResult       ( const Task & task );
EDGES *         Tsk_GetResult       ( const Task & task );
int             Tsk_GetWirelength   ( const Task & task );
int             Tsk_GetScale        ( const Task & task );
int             Tsk_GetSize         ( const Task & task );
int             Tsk_GetArea         ( const Task & task );
bool            Tsk_PointIsValid    ( const Task & task, Point point );
void            Tsk_SetDifficulty   ( Task & task, int difficulty );

//=======================================Results=============================//
void            Tsk_Append          ( Task & task, EDGE edge );
void            Tsk_CleanResult     ( Task & task );
EDGES *         Tsk_CollectResult   ( Tasks * pTasks );
#endif