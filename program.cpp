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
  MPI::Init ( argc, argv );
  
  //Safety check - need at least 2 philosophers to make sense
  if (MPI::COMM_WORLD.Get_size ( ) < 3) {
	    MPI::Finalize ( );
	    std::cerr << "Need at least 2 philosophers! Try again" << std::endl;
	    return 1; //non-normal exit
  }

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
      cout << "done wating" << endl;

      //request forks
      msgOut = eat_quota;
      std::cout << "This is Philosopher " << id << " requesting forks with message " << msgOut << std::endl;
      MPI::COMM_WORLD.Send ( &msgOut, 1, MPI::INT, 0, tag ); 
      
      //wait for forks
      MPI::COMM_WORLD.Recv ( &msgIn, 1, MPI::INT, 0, tag, status );
      std::cout << "This is Philosopher " << id << " recieving message of " << msgIn << std::endl;

      //eat 
      toeat = rand() % eat_quota;
      sleep(toeat);
      eat_quota -= toeat;

      //return forks
      msgOut = -1;
      std::cout << "This is Philosopher " << id << " sending message of " << msgOut << std::endl;
      MPI::COMM_WORLD.Send ( &msgOut, 1, MPI::INT, 0, tag ); 
    }
	
    std::cout << "This is Philosopher " << id << " and I am full. ";
    //return forks
    msgOut = -1;
    std::cout << "This is Philosopher " << id << " sending message of " << msgOut << std::endl;
    MPI::COMM_WORLD.Send ( &msgOut, 1, MPI::INT, 0, tag ); 

  }
  
  //  Terminate MPI.
  MPI::Finalize ( );
  return 0;
}
