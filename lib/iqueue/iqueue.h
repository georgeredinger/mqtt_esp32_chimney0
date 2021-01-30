#ifndef __IQUEUE__
#define __IQUEUE__
#define N 5
//queue of integers, n long

void printq(int *, int);
bool qEmpty();
void qInit();
int qHead();
int qLength();
void qEnqueue(int);
int dQueue();
#endif