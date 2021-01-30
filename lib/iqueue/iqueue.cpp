#include <string.h>
#include <stdio.h>
#include "iqueue.h"
//queue of integers, n long
int q[N];
int tail;
void printq(int *q, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d,", q[i]);
    }
    printf("\r\n");
}
bool qEmpty() {
    return true;
}
void qInit() {
    for (int i = 0; i < N; i++) {
        q[i] = 0;
    }
    tail = 0;
}

int qHead() {
    return q[0];
}

int qLength() {
    return tail;
}

int qTail() {
    return q[N - 1];
}

void qEnqueue(int i) {
    if (tail >= (N)) {
        memcpy(&q[0], &q[1], sizeof(int) * (N));
        q[N - 1] = i;
        tail = N - 1;
    }
    q[tail++] = i;
}

int dQueue() {
    int d = q[0];
    tail--;
    memcpy(&q[0], &q[1], sizeof(int) * (N));
    if (tail < 0)
        tail = 0;
    return d;
}
