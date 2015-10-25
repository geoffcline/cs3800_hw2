#include <cstdlib>
#include <iostream>
#include <cerrno>
#include <unistd.h>
#include <list>

using namespace std;

#include "mpi.h"

bool assignforks();
bool forksavail(); 

void ombudsman()
{
	const int p = MPI::COMM_WORLD.Get_size ( );
	int rid, tid;
	int going_p = p - 1; 

	int* forks;

	int msgIn;
	int msgOut;
	int tag = 1;

	MPI::Status status;

	forks = new int[p];

  list<int> requests;

	for (int i = 0; i < p; i++)
    forks[i] = 0;
		

  while(going_p > 0)
  {
    MPI::COMM_WORLD.Recv ( &msgIn, 1, MPI::INT, MPI::ANY_SOURCE, tag, status );
    rid = status.Get_source();

    //REPORTING FULL
    if(msgIn == 0) //reporting full
    {
      cout << "\t\tFULL RECIEVED: " << rid << endl;
      going_p--;
      cout << "\t\tSTILL RUNNING: " << going_p << endl;
    }
    //REPORTING WAIT RELEASE
    else if (msgIn == -1) 
    {
      //RELEASE FORKS
      cout << "Forks Returned:  " << rid <<  endl;
      forks[rid] = 0;
      forks[(rid + 1) % p] = 0;

      //CHECK FOR NEWLY POSSIBLE ALLOCATIONS
      for (list<int>::iterator it=requests.begin(); it != requests.end(); ++it)
      {
        tid = *it;

        cout << "\tChecking:  " << tid;

        if(forks[tid] == 0 && forks[(tid + 1) % p] == 0)
        {
          
          cout << " | YES " << endl;
          forks[tid] = tid;
          forks[(tid + 1) % p] = tid;

          msgOut = -2;
          MPI::COMM_WORLD.Send ( &msgOut, 1, MPI::INT, tid, tag ); 

          cout << "Held Fufilled: " << tid << endl;

          cout << "about to erase, size: " << requests.size() << endl;
          requests.remove(tid);

          //PREVENT ERRORS WITH DELETING LAST/ONLY ITEM
          it=requests.begin();


        }
        else
        {
          cout << " | NO " << endl;
        }
        
      }

      cout << "\tDone Checking " << endl;
    }
    //INCOMING REQUEST
    else if (msgIn > 0)
    {
      //FORKS AVAIL
      if ( forks[rid] == 0 && forks[(rid + 1) % p] == 0)
      {
        forks[rid] = rid;
        forks[(rid + 1) % p] = rid;

        msgOut = -2;
        cout << "Forks Given:  " << rid <<  endl;
        MPI::COMM_WORLD.Send ( &msgOut, 1, MPI::INT, rid, tag ); 

        //cout << " | Forks Given" << endl;
      }
      //FORKS UNAVAIL
      else
      {
        cout << "Queued: " << rid << endl;
        requests.push_back(rid);
      }
    }
    else
    {
      cout << "wut" << endl;
    }
  }


  delete forks;

  return;
}