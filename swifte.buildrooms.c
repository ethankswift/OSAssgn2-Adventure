#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "time.h"

struct room
{
	int roomType, roomID, connNum;
	int conns[6];
};

const char * titles[] = { "ODIN", "THOR", "FREYA", "TYR", "NJORD","BALDR", "LOKI" };

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
		sprintf(fname, "%s/%s'S_HALL", dirname, titles[i]);
		scribe = fopen (fname,"w");
		fprintf (scribe, "ROOM NAME: %s'S_HALL\n", titles[i]);
		for (j = 0; j < rooms[i].connNum; j++) {
			fprintf (scribe, "CONNECTION %d: %s'S_HALL\n", j, titles[rooms[i].conns[j]]);
		}
	}

}

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

struct room GetRandomRoom(struct room * rooms){
	srand(time(0));
	return rooms[(rand() % 7)];
}

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

	printf("connecting %d and %d\n", a.roomID, b.roomID);	

	Connect(rooms, a, b);
	
}


int CanAddFrom(struct room a){
	if (a.connNum > 5)
		return 0;
	else
		return 1;
}

int ConnExists(struct room a, struct room b) {
	int i;
	for (i = 0; i < 6; i++) {
		if (a.roomID == b.conns[i]) {
			return 1;
		}
	}
	return 0;
}

int Connect(struct room * rooms, struct room a, struct room b) {
	rooms[a.roomID].conns[rooms[a.roomID].connNum] = b.roomID;
	rooms[b.roomID].conns[rooms[b.roomID].connNum] = a.roomID;

	rooms[a.roomID].connNum++;
	rooms[b.roomID].connNum++;
}

int AreSame(struct room a, struct room b) {
	return a.roomID == b.roomID;
}

int main() {
	int i, j;
	struct room rooms[7];


	for (i = 0; i < 7; i++) {
		rooms[i].roomID = i;
		rooms[i].roomType = 0;
		rooms[i].connNum = 0;
		for (j = 0; j < 6; j++) {
			rooms[i].conns[j] = -1;
		}
	}


	while (IsGraphFull(rooms) == 0) {
		printf("adding one more random connection\n");
		AddRandom(rooms);
	}
	author(rooms);

}
