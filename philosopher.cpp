#include "program.h"

/*
 * @author  Geoffrey Cline
 * @date    10/25/2015
 * @file    philosopher.cpp
 * @course  cs3800, section B
 * @brief   This program simulates the philosopher talking,
 *          requesting to eat, and eating
 */

void philosopher()
{
  //MPI Environment Related Variables
  int tag = 1;
  MPI::Status status;
  int msgIn, msgOut;

  //eat quota related variables
  int toeat;
  int eat_quota = rand() % (MAX_TOTAL_EAT-MIN_TOTAL_EAT);
  eat_quota += MIN_TOTAL_EAT;


  while(eat_quota > 0) //still more left to eat
  {
    sleep(rand() % SLEEP_MAX);

    //request forks
    msgOut = eat_quota;
    MPI::COMM_WORLD.Send ( &msgOut, 1, MPI::INT, 0, tag ); 
    
    //wait for forks
    MPI::COMM_WORLD.Recv ( &msgIn, 1, MPI::INT, 0, tag, status );

    //eat 
    toeat = rand() % EAT_SES_MAX;
    sleep(toeat);
    eat_quota -= toeat;

    //return forks
    msgOut = MSG_REL;
    MPI::COMM_WORLD.Send ( &msgOut, 1, MPI::INT, 0, tag ); 
  }

  //return full
  msgOut = MSG_FULL;
  MPI::COMM_WORLD.Send ( &msgOut, 1, MPI::INT, 0, tag ); 

  return;
}