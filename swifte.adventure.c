#include "dirent.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "unistd.h"
#include "stdio.h"
#include "string.h"
#include "time.h"

int main() {

	struct dirent * entry;
	struct stat stats;

	char roomdir[32];
	int i = 0;
	time_t filetime = 0;


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

	return 0;
}
