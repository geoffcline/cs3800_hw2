#include <cstdlib>
#include <iostream>
#include <cerrno>
#include <unistd.h>
#include <queue>

using namespace std;

#include "mpi.h"

void ombudsman();

void philosopher();

int main ( int argc, char *argv[] ) 
{
  int id, eat_quota;
  int tag = 1;
  MPI::Status status;
  int msgOut, msgIn;
  int toeat;

  MPI::Init ( argc, argv );
  
  //Safety check - need at least 2 philosophers to make sense
  if (MPI::COMM_WORLD.Get_size ( ) < 3) {
	    MPI::Finalize ( );
	    std::cerr << "Need at least 2 philosophers! Try again" << std::endl;
	    return 1; //non-normal exit
  }

  id = MPI::COMM_WORLD.Get_rank ( );

  srand(id + time(NULL)); //ensure different seeds...
  
  //  Setup Fork Master (Ombudsman) and Philosophers
  if ( id == 0 ) //Master
  {
    ombudsman();
    
	}
  else //I'm a philosopher
  {

    eat_quota = rand() % 30;

    while(eat_quota > 0)
    {
      cout << "wating : " << id << endl; 
      sleep(rand() % 10);
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
      std::cout << "Return: " << id << " | M: " << msgOut << std::endl;
      MPI::COMM_WORLD.Send ( &msgOut, 1, MPI::INT, 0, tag ); 
    }
	
    
    //return forks
    msgOut = -1;
    std::cout << "Full: " << id << " | M: " << msgOut << std::endl;
    MPI::COMM_WORLD.Send ( &msgOut, 1, MPI::INT, 0, tag ); 

  }
  
  //  Terminate MPI.
  MPI::Finalize ( );
  return 0;
}
