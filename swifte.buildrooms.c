#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"

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
	int i;

// assigning the pid to the directory name
	sprintf(dirname, "swifte.rooms.%d", pid);
// making the directory
	mkdir(dirname, 0777);
// making the files
	for (i = 0; i < 7; i++) {
		sprintf(fname, "%s/%s'S_HALL", dirname, titles[i]);
		scribe = fopen (fname,"w");
		fprintf (scribe, "ROOM NAME: %s\n", titles[i]);
	}

}


int main() {
	int i;
	struct room rooms[7];


	for (i = 0; i < 7; i++) {
		rooms[i].roomID = i;
		rooms[i].roomType = 0;
		rooms[i].connNum = 0;
	}

	author(rooms);

}
