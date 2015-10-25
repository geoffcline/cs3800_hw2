#include <cstdlib>
#include <iostream>
#include <cerrno>
#include <unistd.h>
#include <list>

using namespace std;

#include "mpi.h"

const int MAX_TOTAL_EAT = 10;
const int MAX_EAT_SESSION = 10;

void philosopher()
{
  int eat_quota = rand() % MAX_TOTAL_EAT;
  int msgIn, msgOut, toeat;
  int tag = 1;
  const int id = MPI::COMM_WORLD.Get_rank ( );
  MPI::Status status;

  while(eat_quota > 0)
  {
    cout << "wating : " << id << endl; 
    sleep(rand() % 4);
    cout << "done wating : " << id << endl; 

    //request forks
    msgOut = eat_quota;
    std::cout << "Request: " << id << " | M: " << msgOut << std::endl;
    MPI::COMM_WORLD.Send ( &msgOut, 1, MPI::INT, 0, tag ); 
    
    //wait for forks
    MPI::COMM_WORLD.Recv ( &msgIn, 1, MPI::INT, 0, tag, status );
    std::cout << "Fufilled: " << id << " | M: " << msgOut << std::endl;

    //eat 
    cout << "eating : " << id << endl; 
    toeat = rand() % 10;
    sleep(toeat);
    eat_quota -= toeat;
    cout << "done eating : " << id << endl;

    //return forks
    msgOut = -1;
    std::cout << "Reporting Return: " << id << " | M: " << msgOut << std::endl;
    MPI::COMM_WORLD.Send ( &msgOut, 1, MPI::INT, 0, tag ); 
  }

  return;
}