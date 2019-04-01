//
// Created by Felipe Dominguez Claro on 2019-03-25.
//

#pragma once

typedef enum
{
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
    int runs;
    int interruptions;
    int turnaround_time;
    int response_time;
    int waiting_time;
} Process;


Process* process_init(int PID, int priority, int start_time, int length, int* bursts, char* name);

int work(Process* process);

void set_state(Process* process, states state);

void set_response_time(Process* process, int time);

void increment_waiting_time(Process* process);