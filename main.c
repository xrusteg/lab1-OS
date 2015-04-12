#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>

#define N 80
#define BUFSIZE 1024

struct fields {
	char nameOfFile[N];
	int sizeOfFile;
};

int pack(char *dir)
{
	int out, in;
	struct fields field;
	int check;
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;
	mode_t modes;
	char buffer[BUFSIZE];
	int n;

	out = open("out", O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
	if (out == -1)
	{
		printf("ERROR: %s\n", strerror(errno));
		return 0;
	}
	
	dp = opendir(dir);
	if(dp == NULL)
	{
		printf("Unable to open directore%s\n", dir);
		return 0;
	}
	
	check = chdir(dir);
	if (check == -1)
	{
		printf("ERROR: %s\n", strerror(errno));
		return 0;
	}
	
	while(entry = readdir(dp))
	{
		if (strcmp(".", entry->d_name) == 0 || strcmp ("..", entry->d_name) == 0 ){continue;}
		strcpy(field.nameOfFile, entry->d_name);
		stat(entry->d_name, &statbuf);
		field.sizeOfFile=(int)statbuf.st_size;
		check = write(out, &field, sizeof(field));
		if (check == -1)
		{
			printf("ERROR: %s\n", strerror(errno));
			return 0;
		}
	
		in = open(field.nameOfFile, O_RDONLY, S_IRUSR|S_IWUSR);
		if (in == -1)
		{
			printf("ERROR: %s\n", strerror(errno));
			return 0;
		}
		while ((n = read(in, buffer, sizeof(buffer))) > 0) {
			if (n == -1){
				printf("ERROR(read): %s\n", strerror(errno));
				return 0;
			}
			check = write(out, buffer, n);
			if (check == -1){
				printf("ERROR(read): %s\n", strerror(errno));
				return 0;
			}
		}
		close(in);
	}
	check = close(out);
	if (check == -1)
	{
		printf("ERROR: %s\n", strerror(errno));
		return 0;
	}
	return 1;
}

int unpack(char *file)
{
	int check, in, out, n;
	struct fields field;
	char buffer[BUFSIZE];
	
	in = open(file, O_RDONLY, S_IRUSR|S_IWUSR);
	if (in == -1) {
		printf("ERROR1: %s\n", strerror(errno));
		return 0;
	}
	mkdir("unpack", 0700);
	chdir("unpack");
	
	while(read(in, &field, sizeof(field)))
	{
		printf("%s\n", field.nameOfFile); 
		out = open(field.nameOfFile, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
		if (out == -1)
		{
			printf("ERROR2: %s\n", strerror(errno));
			return 0;
		}
		if(field.sizeOfFile >= BUFSIZE){
			n = BUFSIZE;
			//printf("%d\n", n);
		}
		else {
			n = field.sizeOfFile;
			//printf("%d\n", n);
		}
		while (n) {
			//printf("%d\n", n);
			if (n < BUFSIZE) {break;}
			n = read(in, buffer, n);
			//printf("1\n");
			check = write(out, buffer, n);
			if (check == -1){
				//printf("ERROR(write): %s\n", strerror(errno));
				return 0;
			}
			if (n < BUFSIZE) {break;}
			if((field.sizeOfFile - n) >= BUFSIZE) {
				field.sizeOfFile = field.sizeOfFile - n;
				n = BUFSIZE;
			}
			else {
				n = field.sizeOfFile - n;
				field.sizeOfFile = field.sizeOfFile - n;
			}
			//printf("%d\n", n);
		}
		close(out);
	}
	close(in);
	return 1;
}
		


int main(int argc, char *argv[])
{
	char *dir = argv[1];
	struct stat statbuf;
	mode_t modes;
	int check;
		
	stat(dir, &statbuf);
	modes = statbuf.st_mode;
	if (S_ISDIR(modes))
	{
		check = pack(dir);
		if(check == 0)	{
			printf("Pack was unsuccessfull\n");
		}
	}
	else {
		unpack(dir);
		if(check == 0)	{
			printf("Pack was unsuccessfull\n");
		}
	}
	return 0;
}
	
