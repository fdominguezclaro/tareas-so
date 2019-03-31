//
// Created by Felipe Dominguez Claro on 2019-03-25.
//

#pragma once

typedef enum
{
    NOINIT,
    READY,
    RUNNING,
    WAITING,
    FINISHED
} states;

typedef struct process {
    int PID;
    int priority;
    int start_time;
    int finish_time;
    int length;
    int* bursts;
    int actual_burst;
    states status;
    char name[257];
} Process;


Process* process_init(int PID, int priority, int start_time, int length, int* bursts, char* name);

void ready_process(Process* process);

void run_process(Process* process);

void pause_process(Process* process);

void stop_process(Process* process);