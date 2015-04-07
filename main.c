#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

struct fields
{
	char name[80];
	long int length;
};	

int main (int argc, char *argv[])
{
	int i=0;
	DIR *dp;
	char *dir = argv[1];
	struct dirent *entry;
	struct stat statbuf;
	off_t size; 
	FILE *fout;
	FILE *fin;
	struct fields test;	
	size_t p;
	char *buf;	
			
	if ((dp = opendir(dir)) == NULL) 
	{
		fprintf(stderr, "cannot open directory: %s\n", dir);
	}
	
	chdir(dir);
	
	fout = fopen("out", "wb");
	while (entry = readdir(dp))
	{
		if (strcmp(".", entry->d_name) == 0 || strcmp ("..", entry->d_name) == 0)
			continue;
		fin = fopen(entry->d_name, "rb");//открытие файла
		printf("%s\n", entry->d_name);
		strcpy(test.name, entry->d_name);//имя
		//printf ("%s--\n", test.name);
		stat(entry->d_name, &statbuf);//размер
		size=statbuf.st_size;
		test.length=(int)size;
		printf("%ld\n", (long)size);
		//printf ("%ld--\n", test.length);
		buf = malloc(size);
		fread(buf, 1, size, fin);
		fwrite(&test, sizeof(test), 1, fout);
		fwrite(buf, 1, size, fout);
	}
	//fwrite(&test, sizeof(test), 1, fout);
	//test.name="qwe";
	//test.length=0;
	//fclose (fout);
	//fout = fopen("out", "r");
	//fread(&test, sizeof(test), 1, fout);
	fclose (fout);
	fclose (fin);
	
	
	
	//printf("%s,  %ld", test.name, test.length);
	return 0;
}
