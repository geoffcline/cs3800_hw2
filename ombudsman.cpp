#include <cstdlib>
#include <iostream>
#include <cerrno>
#include <unistd.h>
#include <list>

using namespace std;

#include "mpi.h"

void ombudsman()
{
	const int p = MPI::COMM_WORLD.Get_size ( );
	const int id MPI::COMM_WORLD.Get_rank ( );
	int p;
	int rid; 
	int going_p = p; 

	int* forks;
	int* requests;
	int q_start = 0;
	int q_held = 0;

	int msgIN;
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

    if(msgIn == -1) //reporting full
    {
      cout << "Philos " << rid << " is full." << endl;
      going_p--;
    }
    else if (msgIn == 0) //returning forks
    {
      cout << "Philos " << rid << " is returning forks." << rid << "and " << (rid + 1) % p << endl;
      forks[rid] = 0;
      forks[(rid + 1) % p] = 0;

      for (list<int>::iterator it=requests.begin(); it != requests.end(); ++it)
      {
        tid = *it;

        if(forks[tid] == 0 && forks[(tid + 1) % p] == 0)
        {
          forks[tid] = tid;
          forks[(tid + 1) % p] = tid;

          msgOut = -2;
          MPI::COMM_WORLD.Send ( &msgOut, 1, MPI::INT, tid, tag ); 

          cout << tid << " | Forks Given" << endl;

          request.erase(it);
        }
      }
    }
    else if (msgIn > 0)
    {
      cout << "Philos " << rid << " is hungry for: " << msgIn;
      if ( forks[rid] == 0 && forks[(rid + 1) % p] == 0)
      {
        forks[rid] = rid;
        forks[(rid + 1) % p] = rid;

        msgOut = -2;
        MPI::COMM_WORLD.Send ( &msgOut, 1, MPI::INT, rid, tag ); 

        cout << " | Forks Given" << endl;
      }
      else
      {
        cout << " | queued " << endl;
        requests.push_back(rid);
      }
    }
    else
    {
      cout << "wut" << endl;
    }
  }
}