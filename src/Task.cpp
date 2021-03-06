#include "Task.h"

Task Tsk_Init( Point start, Point end ) {
    Task task;
    task.start = start;
    task.end   = end;
    task.edges = NULL;
    task.difficulty = 0;
    return task;
}

bool Tsk_HasResult( const Task & task ) {
    return ( task.edges != NULL );
}

EDGES * Tsk_GetResult( const Task & task ) {
    return task.edges;
}

void Tsk_Append( Task & task, EDGE edge ) {
    assert( task.edges != NULL );
    task.edges->insert( edge );
}

int Tsk_GetWirelength( const Task & task ) {
    return (int)task.edges->size();
}

void Tsk_CleanResult( Task & task ) {
    if ( task.edges == NULL )
        return;
    task.edges->clear();
    delete task.edges; task.edges = NULL;
}

void Tsk_Free( Tasks * pTasks ) {
    if ( pTasks == NULL )
        return;
    for ( auto & task : *pTasks ) {
        Tsk_CleanResult( task );
    }
    pTasks->clear();
    delete pTasks; pTasks = NULL;
}

bool Tsk_PointIsValid( const Task & task, Point point ) {
    return ( point >= ( (task.start -= task.end) - Point(task.difficulty, task.difficulty)) ) 
        && ( point <= ( (task.start += task.end) + Point(task.difficulty, task.difficulty)) );
}

void Tsk_SetDifficulty ( Task & task, int difficulty ) {

    // deal with AUTO command
    if ( difficulty == -1 ) {
        task.difficulty = Tsk_GetScale( task ) > 10 ?  1 : 4;
        return;
    }

    task.difficulty = difficulty;
}

EDGES * Tsk_CollectResult ( Tasks * pTasks ) {

    assert( pTasks != NULL );
    EDGES * edges = new EDGES;
    for ( auto & task : *pTasks ) {
        assert( Tsk_HasResult( task ) );
        for ( auto & edge : *(task.edges) ) {
            edges->insert( edge );
        }
    }
    return edges;

}

void Edg_Free ( EDGES * edges ) {
    if ( edges == NULL )
        return;
    edges->clear();
    delete edges; edges = NULL;
}

int Tsk_GetScale ( const Task & task ) {
    return task.start ^ task.end;
}

int Tsk_GetSize ( const Task & task ) {
    return min( abs(task.start.x - task.end.x), abs(task.start.y - task.end.y)  ) + 1;
}

int Tsk_GetArea ( const Task & task ) {
    int x = abs( task.start.x - task.end.x ) + 1;
    int y = abs( task.start.y - task.end.y ) + 1; 
    return 2*x*y - x - y;
}