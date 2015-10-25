#include "program.h"

void philosopher()
{
  int eat_quota = rand() % (MAX_TOTAL_EAT-MIN_TOTAL_EAT);
  eat_quota += MIN_TOTAL_EAT;
  int msgIn, msgOut, toeat;
  int tag = 1;
  MPI::Status status;

  while(eat_quota > 0)
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