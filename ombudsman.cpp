#include "program.h"

void ombudsman()
{
	const int p = MPI::COMM_WORLD.Get_size ( );
	int rid, tid;
	int going_p = p - 1; 

	int* forks;

	int msgIn;
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
    if(msgIn == MSG_FULL) //reporting full
    {
      cout << "\t\tFULL RECIEVED: " << rid << endl;
      going_p--;
      cout << "\t\tSTILL RUNNING: " << going_p << endl;
    }
    //REPORTING WAIT RELEASE
    else if (msgIn == MSG_REL) 
    {
      //RELEASE FORKS
      returnforks(forks, p, rid);
      cout << rid << "\tReturned" <<  endl;


      //CHECK FOR NEWLY POSSIBLE ALLOCATIONS
      for (list<int>::iterator it=requests.begin(); it != requests.end(); ++it)
      {
        tid = *it;

        if(assignforks(forks, p, tid))
        {
          cout << tid << "\tHeld Request Fufilled" <<  endl;

          requests.remove(tid);

          //PREVENT ERRORS WITH DELETING LAST/ONLY ITEM
          it=requests.begin();
        }
        
      }

    }
    //INCOMING REQUEST
    else if (msgIn > 0)
    {
      if(!assignforks(forks, p, rid))
      {
        cout << rid << "\tRequest Held" <<  endl;
        requests.push_back(rid);
      }
      else
        cout << rid << "\tRequest Fufilled" <<  endl;
    }
  }


  delete forks;

  return;
}

bool assignforks(int* forks, const int p, const int rid)
{
  int tag = 1;
  bool rv = forksavail(forks, p, rid);

  if(rv)
  {
    forks[rid] = rid;
    forks[(rid + 1) % p] = rid;

    MPI::COMM_WORLD.Send ( &MSG_ALOC, 1, MPI::INT, rid, tag ); 
  }

  return rv;
}

bool forksavail(int* forks, const int p, const int rid)
{
  return (forks[rid] == 0 && forks[(rid + 1) % p] == 0);
}

void returnforks(int* forks, const int p, const int rid)
{
  forks[rid] = 0;
  forks[(rid + 1) % p] = 0;

  return;
}