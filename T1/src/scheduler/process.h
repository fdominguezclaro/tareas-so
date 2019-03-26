//
// Created by Felipe Dominguez Claro on 2019-03-25.
//

#pragma once

typedef struct process {
    int PID;
    int priority;
    int start_time;
    int finish_time;
    int length;
    char name[257];
    char status[9];
} Process;