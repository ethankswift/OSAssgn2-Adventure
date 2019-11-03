#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "time.h"

//data storage for the room object
struct room
{
	int roomType, roomID, connNum;
	int conns[6];
};

//names for the rooms
const char * titles[] = { "ODIN", "THOR", "FREYA", "TYR", "NJORD","BALDR", "LOKI" };

//function to write the file, takes a pointer to the array of room structs
void author(struct room * rooms) {
	int pid = getpid();
	char dirname[32];
	char fname[32];
	FILE * scribe;
	int i, j;

// assigning the pid to the directory name
	sprintf(dirname, "swifte.rooms.%d", pid);
// making the directory
	mkdir(dirname, 0777);
// making the files
	for (i = 0; i < 7; i++) {
		sprintf(fname, "%s/%s", dirname, titles[i]);
		scribe = fopen (fname,"w");
		fprintf (scribe, "ROOM NAME: %s\n", titles[i]);
		for (j = 0; j < rooms[i].connNum; j++) {
			fprintf (scribe, "CONNECTION %d: %s\n", j+1, titles[rooms[i].conns[j]]);
		}
		if (rooms[i].roomType == 1)
			fprintf (scribe, "ROOM TYPE: START_ROOM\n");
		else if (rooms[i].roomType == 2)
			fprintf (scribe, "ROOM TYPE: END_ROOM\n");
		else
			fprintf (scribe, "ROOM TYPE: MID_ROOM\n");
	}
	fclose(scribe);
}

//this function checks if there is an adequate number of connections at each node
int IsGraphFull(struct room * rooms) {
	int i, check;
	check = 0;
	for (i = 0; i < 7; i++) {
		if (rooms[i].connNum > 2) {
			check++;
		}
	}
	if (check > 6)
		return 1;
	else
		return 0;
}

//returns a random node from the graph
struct room GetRandomRoom(struct room * rooms){
	srand(time(0));
	return rooms[(rand() % 7)];
}

//adds a single random new connection to the graph
void AddRandom(struct room * rooms) {
	struct room a;
	struct room b;

	while(1) {
		a = GetRandomRoom(rooms);
		
		if (CanAddFrom(a) == 1 )
			break;
	}


	do {
		b = GetRandomRoom(rooms);
	} while ( CanAddFrom(b) == 0 || AreSame(a,b) == 1 || ConnExists(a,b) == 1 );

//actual connection happens here
	Connect(rooms, a, b);
	
}

//checks the number of connections from a node
int CanAddFrom(struct room a){
	if (a.connNum > 5)
		return 0;
	else
		return 1;
}

//checks if a specific connection exists
int ConnExists(struct room a, struct room b) {
	int i;
	for (i = 0; i < 6; i++) {
		if (a.roomID == b.conns[i]) {
			return 1;
		}
	}
	return 0;
}

//function that takes a pointer to the original array of structs and adds the new connection
int Connect(struct room * rooms, struct room a, struct room b) {
	rooms[a.roomID].conns[rooms[a.roomID].connNum] = b.roomID;
	rooms[b.roomID].conns[rooms[b.roomID].connNum] = a.roomID;

	rooms[a.roomID].connNum++;
	rooms[b.roomID].connNum++;
}

//checks if two nodes are the same node
int AreSame(struct room a, struct room b) {
	return a.roomID == b.roomID;
}

int main() {
	int i, j;
	struct room rooms[7];

//initializing empty rooms
	for (i = 0; i < 7; i++) {
		rooms[i].roomID = i;
		rooms[i].roomType = 0;
		rooms[i].connNum = 0;
		for (j = 0; j < 6; j++) {
			rooms[i].conns[j] = -1;
		}
	}

//fills connections until done
	while (IsGraphFull(rooms) == 0) {
		AddRandom(rooms);
	}

//select two random different numbers to be the start and end
	i = rand()%7;
	do {
	j = rand()%7;
	} while (i == j);
//assigns start and end
	rooms[i].roomType = 1;
	rooms[j].roomType = 2;	
//writes the directory and files
	author(rooms);
return 0;
}
