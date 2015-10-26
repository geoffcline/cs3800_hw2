# CS3800, HW2: Dining Philosophers

## Structure
Overall, the MPI setup means that each instance will have their own seperate process, with own main and global variables. Messaging passing is the **only** way to communicate between processes. 

## Ombudsman
THhe ombudsman loops while there are still philosophers that have not yet indicated they are full. Each loop essentially waits for a message, then processes the message. 

The possible messages to be recieved are:

1. Request For Forks, can either be immediately fufilled or added to the list
2. Relese Forks, after release processed, checks list starting with oldest for possible fufillment
3. Full Notice, always possible

## Philosopher
First, determines a random number that is eating quota. 

The loops while this quota has not been met, doing:

1. Sleep for random short time, talking
2. Request forks
3. Wait for fork allocation message
4. Eat for random time, decreasing eat quota
5. Releasing forks with message

After this, the loop ends the message indicating this philosopher is full is sent. This occurs after a complete loop operation, and the last operation was fork release. 
