#include "program.h"

int main ( int argc, char *argv[] ) 
{
  int id;
  MPI::Status status;

  MPI::Init ( argc, argv );
  
  //Safety check - need at least 2 philosophers to make sense
  if (MPI::COMM_WORLD.Get_size ( ) < 3) 
  {
	    MPI::Finalize ( );
	    std::cerr << "Need at least 2 philosophers! Try again" << std::endl;
	    return 1; //non-normal exit
  }

  id = MPI::COMM_WORLD.Get_rank ( );

  srand(id + time(NULL)); //ensure different seeds...
  
  //  Setup Fork Master (Ombudsman) and Philosophers
  if ( id == 0 ) //Master
    ombudsman();
  else //I'm a philosopher
    philosopher();
  
  //  Terminate MPI.
  MPI::Finalize ( );
  return 0;
}
