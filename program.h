/*
 * @author  Geoffrey Cline
 * @date    10/25/2015
 * @file    program.h
 * @course  cs3800, section B
 * @brief   header file for dining philosophers problem
 *          includes constants relating to eating/waiting
 *          times
 */

#include <cstdlib>
#include <iostream>
#include <cerrno>
#include <unistd.h>
#include <list>
#include "mpi.h"

using namespace std;

//MPI Message Codes
const int MSG_ALOC = -2;
const int MSG_REL  = -1;
const int MSG_FULL = 0;

//Philosopher Timing Constants
const int MAX_TOTAL_EAT = 10;
const int MAX_EAT_SESSION = 10;
const int MIN_TOTAL_EAT = 1;
const int EAT_SES_MAX = 10;
const int SLEEP_MAX = 4;

void ombudsman();

void philosopher();

bool assignforks(int* forks, const int p, const int rid);
bool forksavail(int* forks, const int p, const int rid); 
void returnforks(int* forks, const int p, const int rid);
