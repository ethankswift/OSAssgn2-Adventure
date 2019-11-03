#include "dirent.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "unistd.h"
#include "stdio.h"
#include "string.h"
#include "time.h"
#include "pthread.h"

pthread_mutex_t lock;

char * titles[] = { "ODIN", "THOR", "FREYA", "TYR", "NJORD","BALDR", "LOKI" };

char * timefile = "time.txt";

//prints the time from file
void printTime() {
	char * line = NULL;
	FILE * tfile = NULL;
	size_t len = 0;
	ssize_t read = 0;
	
	tfile = fopen(timefile, "r");

	while ((read = getline(&line, &len, tfile)) != -1) {
	printf("%s\n", line);
	}
	fclose(tfile);
}

//makes the file with the time
void * timeFileMake() {
	
	char timeStr[256];
	struct tm *timeData;
	time_t currT;
	FILE * tfile;
	
	time(&currT);
	timeData = localtime(&currT);
	strftime(timeStr, 256, "%I:%M%P %A, %B %d, %Y", timeData);

	tfile = fopen(timefile, "w");
	fprintf(tfile,"%s\n",timeStr);

	fclose(tfile);
}

//tries to make a thread and returns result
int tryThread()
{
	pthread_t tfilewrite;
	pthread_mutex_lock(&lock);

	if (pthread_create(&tfilewrite, NULL, timeFileMake, NULL) != 0) {
		return 1;
	}

	pthread_mutex_unlock(&lock);
	pthread_join(timeFileMake, NULL);

	return 0;
}

//reads files and checks them for a supplied string
int reader(char * filepath, char * str) {

	char * line = NULL;
	size_t len = 0;
	ssize_t read = 0;
	FILE * file = NULL;

	int check = 0;


	file = fopen(filepath, "r");

	while ((read = getline(&line, &len, file)) != -1) {
		if (strstr(line, str) != NULL )
			check = 1;
	}
	
	fclose(file);	

	return check;
}

//checks if input is a room name
int inputcheck(char * str) {
	int i;

	for (i = 0; i < 7; i++) {
		if (strcmp(str,titles[i]) == 0)
			return 1;
	}
	return 0;
}

//runs the game
int main() {

	struct dirent * entry;
	struct stat stats;

	FILE * file;

	char roomdir[32];
	char filepath[64];
	char start[8];
	char end[8];
	char curr[8];
	char target[8];
	char steps[32][8];
	
	int stepc = 0;
	int i = 0;
	time_t filetime = 0;
	

//checks directory for room directories
	DIR * parentdir = opendir(".");

	while ((entry = readdir(parentdir)) != NULL) {
		if (strncmp(entry->d_name,"swifte.rooms",12) == 0) {
			stat(entry->d_name, &stats);
			if ( difftime( stats.st_mtime, filetime ) > 0) {
				strcpy (roomdir, entry->d_name);
				filetime = stats.st_mtime;
			}
		}
	}
	closedir(parentdir);

	printf("%s\n",roomdir);
//finds start and end
	for (i = 0; i < 7; i++) {
		sprintf(filepath, "%s/%s", roomdir, titles[i]);
		if ( reader(filepath,"START") )
			strcpy(start, titles[i]);
			strcpy(curr, start);
		if ( reader(filepath,"END") )
			strcpy(end, titles[i]);
	}

//main game loop
	while(1) {
	
		sprintf(filepath, "%s/%s", roomdir, curr);
		
		if (reader(filepath,"END")) {
			printf("\nYOU WIN!\n\nIT TOOK: %d STEPS\nTHEY WERE:\n", stepc);
			for (i = 0; i < stepc; i++) {
				printf("%s\n", steps[i]);
			}
			break;
		}

		printf("\nCURRENT LOCATION: %s\nPOSSIBLE CONNECTIONS:", curr);
		for (i = 0; i < 7; i++) {
			if ( reader(filepath,titles[i]) && strcmp(curr,titles[i])  )
				printf(" %s,", titles[i]);
		}
	
		printf("\b.\nWHERE TO? >");
		fgets(target,8,stdin);

		target[strcspn(target, "\r\n")] = 0;

		if ( reader(filepath,target) && strcmp(curr,target) && inputcheck(target) ) {
			strcpy(steps[stepc],target);
			strcpy(curr, target);
			stepc++;
		} else if (strcmp(target,"time") == 0) {
			if(tryThread() == 0) {
				printTime();
			}
		}
		else
			printf("\nHUH? I DON'T UNDERSTAND THAT ROOM. TRY AGAIN.\n\n");
	}

return 0;
}
