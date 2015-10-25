#include "program.h"

/*
 * @author  Geoffrey Cline
 * @date    10/25/2015
 * @file    main.cpp
 * @course  cs3800, section B
 * @brief   This program sets up the MPI environment
 *          and launches either the obudsman or philosopher 
 *          function
 */

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
  
  if ( id == 0 )
    ombudsman();
  else           
    philosopher();
  
  //  Terminate MPI.
  MPI::Finalize ( );
  return 0;
}
