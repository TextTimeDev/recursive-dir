#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define MAX_SIZE 1024

void print_func(char *str, void *arg) {
	printf ("%s\n", str);
}

void read_dir(char *dirname,
	      void (function)(char *str, void *arg),
	      void *argument) {
	DIR *dir = opendir(dirname);
	struct dirent *ds;

	if (!dir) {
		return;
	}

	char *path;
	int not_self;
	int not_parent;
	int cond;

	while ((ds = readdir (dir)) != NULL) {
		not_self = strcmp (ds->d_name, ".") != 0;
		not_parent = strcmp (ds->d_name, "..");
	
		if ((not_self
		    && not_parent) != 0) {
			path = malloc ((strlen (ds->d_name)
					+ strlen (dirname) + 2)
					* sizeof(char));

			strcpy (path, dirname);
			strcat (path, "\\");
			strcat (path, ds->d_name);

			function (path, argument);

			read_dir (path, function, argument);

			free (path);
		}
	}
		
		closedir (dir);
}

int main(int argc, char **argv) {
	if (argv [1])
		read_dir (argv [1], print_func, argv [0]);
	else
		read_dir (".", print_func, argv [0]);

	return 0;
}