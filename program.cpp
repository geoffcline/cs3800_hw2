#include <cstdlib>
#include <iostream>
#include <cerrno>
#include <unistd.h>

using namespace std;

#include "mpi.h"

int main ( int argc, char *argv[] ) 
{
  int id; //my MPI ID
  int p;  //total MPI processes
  MPI::Status status;
  int tag = 1;
  int q_start = 0;
  int q_held = 0;

  bool* forks;
  int*  queue;

  //  Initialize MPI.
  MPI::Init ( argc, argv );

  //  Get the number of processes.
  p = MPI::COMM_WORLD.Get_size ( );

  //  Determine the rank of this process.
  id = MPI::COMM_WORLD.Get_rank ( );
  
  //Safety check - need at least 2 philosophers to make sense
  if (p < 3) {
	    MPI::Finalize ( );
	    std::cerr << "Need at least 2 philosophers! Try again" << std::endl;
	    return 1; //non-normal exit
  }

  //setup arrays
  forks = new bool[p];
  queue = new int[p];

  for (int i = 0; i < p; i++)
	  forks[i] = false;

  srand(id + time(NULL)); //ensure different seeds...
  
  //  Setup Fork Master (Ombudsman) and Philosophers
  if ( id == 0 ) //Master
  {
	int msgIn; //messages are integers
	  
	//let the philosophers check in
    for (int i = 1; i < p; i++) {
		MPI::COMM_WORLD.Recv ( &msgIn, 1, MPI::INT, MPI::ANY_SOURCE, tag, status );
		std::cout << "Receiving message " << msgIn << " from Philosopher ";
		std::cout << status.Get_source() << std::endl;
	}
  }
  else //I'm a philosopher
  {

    eat_quota = rand();

    while(eat_quota > 0)
    {
      sleep(rand());

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
