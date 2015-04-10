#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>


struct fields
{
	char name[256];
	long int length;
	int flag;
	int count;
};

int count_files(char *dir)
{
	int file_count = 0;
	int dir_count = 0;
	int total = 0;

	DIR * dirp;
	struct dirent * entry;

	dirp = opendir(dir); 
	while ((entry = readdir(dirp)) != NULL) 
	{
    /*total++;
    if (entry->d_type == DT_REG) 
    {
  		file_count++;
    }*/
    printf("%s\n", entry->d_name);
    printf("%d\n", entry->d_ino);
	}
	dir_count = total - file_count;
	closedir(dirp);
	return file_count;
}

/*void pack1(char *dir, FILE *fout)
{
	DIR *dp, *dp1;
	struct dirent *entry, *entry1;
	struct stat statbuf;
	off_t size; 
	FILE *fin;
	struct fields test;	
	char *buf;
	char *str;
	mode_t modes;
	int i = 0;
	
	stat(dir, &statbuf);
	modes = statbuf.st_mode;
	
	if (S_ISDIR(modes))
	{
//		chdir(dir);
		if ((dp = opendir(dir)) == NULL) 
		{
			//fprintf(stderr, "cannot open directory: %s\n", dir);
			printf("cannot open directory: %s\n", dir);
			exit(0);
			//chdir(dir);
		}
		
	if (S_ISDIR(modes))
	{
//		chdir(dir);
		if ((dp = opendir(dir)) == NULL) 
		{
			//fprintf(stderr, "cannot open directory: %s\n", dir);
			printf("cannot open directory: %s\n", dir);
			exit(0);
			//chdir(dir);
		}	
//		chdir(dir);
		if ((entry = readdir(dp)) == NULL) {return;}
		if (strcmp(".", entry->d_name) == 0 || strcmp ("..", entry->d_name) == 0){pack1(entry -> d_name, fout);}
		
	 	stat(entry->d_name, &statbuf);
		modes = statbuf.st_mode;
		
		if (S_ISDIR(modes))
	{
//		chdir(dir);
		if ((dp = opendir(dir)) == NULL) 
		{
			//fprintf(stderr, "cannot open directory: %s\n", dir);
			printf("cannot open directory: %s\n", dir);
			exit(0);
			//chdir(dir);
		}
	 	strcpy(test.name, entry->d_name);
	 	test.flag = 1;
	 	//test.length = 4096;
	 	//dp = opendir(entry -> d_name);
	 	test.count = count_files(dir);
	 	
	 	fwrite(&test, sizeof(test), 1, fout);
	 	printf("1111\n");
	 	/*
	 	if ((dp = opendir(dir)) == NULL) 
		{
			//fprintf(stderr, "cannot open directory: %s\n", dir);
			printf("cannot open directory: %s\n", dir);
			exit(0);
		}
	 	pack1(test.name, fout);
	 	}
	}
	else 
	{
		if ((entry = readdir(dp)) == NULL) {return;}
		printf("222\n");
		test.flag = 0;
		if ((fin = fopen(entry->d_name, "rb")) == NULL);
		{
			printf("cannot create output file %s\n", entry->d_name);
			exit(0);
		}
		printf("%s\n", entry->d_name);
		strcpy(test.name, entry->d_name);
		stat(entry->d_name, &statbuf);
		size=statbuf.st_size;
		test.length=(int)size;
		//printf("%ld\n", (long)size);
		buf = malloc(size);
		fread(buf, 1, size, fin);
		fwrite(&test, sizeof(test), 1, fout);
		fwrite(buf, 1, size, fout);
		free(buf);
		fclose (fin);
		pack1(dir, fout);
	}
}*/

void pack (char *dir)
{
	DIR *dp, *dp1;
	struct dirent *entry, *entry1;
	struct stat statbuf;
	off_t size; 
	FILE *fin, *fout;
	struct fields test;	
	char *buf;
	char *str;
	mode_t modes;
	int i = 0;
	
	if ((fout = fopen("out", "wb")) == NULL)
	{
		printf("cannot create output file %s\n", dir);
		exit(0);
	}
	if ((dp = opendir(dir)) == NULL) 
	{
		//fprintf(stderr, "cannot open directory: %s\n", dir);
		printf("cannot open directory: %s\n", dir);
		return; 
	}
	chdir(dir);
	
	while (entry = readdir(dp))
	{
		if (strcmp(".", entry->d_name) == 0 || strcmp ("..", entry->d_name) == 0 ){continue;}
		if (entry->d_type == DT_DIR)
		{
			strcpy(test.name, entry->d_name);
	  	printf("%s\n", entry->d_name);
	  	test.flag = 1;
	  	fwrite(&test, sizeof(test), 1, fout);
	  	pack(test.name);
		}
		else 
		{
			test.flag = 0;
			fin = fopen(entry->d_name, "rb");
			printf("%s\n", entry->d_name);
			strcpy(test.name, entry->d_name);
			stat(entry->d_name, &statbuf);
			size=statbuf.st_size;
			test.length=(int)size;
			buf = malloc(size);
			fread(buf, 1, size, fin);
			fwrite(&test, sizeof(test), 1, fout);
			fwrite(buf, 1, size, fout);
			free(buf);
			fclose (fin);
			printf("%s\n", entry->d_name);
		}
	}
	fclose(fout);
	closedir(dp);
	return;
}

/*
void pack(char *dir, FILE *fout)
{
	int papka, file;
		DIR *dp, *dp1;
	struct dirent *entry, *entry1;
	struct stat statbuf;
	off_t size; 
	FILE *fin;
	struct fields test;	
	char *buf;
	char *str;
	mode_t modes;
	int i = 0;
	
	if ((dp = opendir(dir)) == NULL) 
	{
		//fprintf(stderr, "cannot open directory: %s\n", dir);
		printf("cannot open directory: %s\n", dir);
		return; 
	}
	chdir(dir);
	
	while (entry = readdir(dp))
	{
		if (strcmp(".", entry->d_name) == 0 || strcmp ("..", entry->d_name) == 0 /*|| strcmp (dir, entry->d_name) == 0){continue;}*/
		
		/*stat(entry -> d_name, &statbuf);
		modes = statbuf.st_mode;
		if (S_ISDIR(modes))
		{
			papka++;
			continue;
		}
		else
		{
			file++;
			continue;
		}
	}
	printf("Papok:%d, faylov:%d", papka, files);
	return;
}
		
/*
void pack(char *dir, FILE *fout)
{
	DIR *dp, *dp1;
	struct dirent *entry, *entry1;
	struct stat statbuf;
	off_t size; 
	FILE *fin;
	struct fields test;	
	char *buf;
	char *str;
	mode_t modes;
	int i = 0;
	
	printf("QWE\n");
	if ((dp = opendir(dir)) == NULL) 
	{
		//fprintf(stderr, "cannot open directory: %s\n", dir);
		printf("cannot open directory: %s\n", dir);
		return; 
	}
	chdir(dir);

	while (entry = readdir(dp))
	{
		if (strcmp(".", entry->d_name) == 0 || strcmp ("..", entry->d_name) == 0 /*|| strcmp (dir, entry->d_name) == 0){continue;}*/
		
		/*stat(entry -> d_name, &statbuf);
		modes = statbuf.st_mode;
	  
	  if (S_ISDIR(modes))
	  {
	  	i = 0;
	  	strcpy(test.name, entry->d_name);
	  	printf("%s\n", entry->d_name);
	  	test.flag = 1;
	  	test.length = 4096;
	  	//dp1 = opendir(entry -> d_name);
	  	//while(entry1 = readdir(dp)) {i++;}
	  	//printf("I:%d\n", i);
	  	i = count_files(entry->d_name);
			printf("I:%d\n", i);
	  	test.count = i;
	  	fwrite(&test, sizeof(test), 1, fout);
	  	printf("1111\n");
	  	pack(test.name, fout);
	  }
	  else 
	  {
	  	printf("222\n");
			test.flag = 0;
			fin = fopen(entry->d_name, "rb");
			printf("%s\n", entry->d_name);
			strcpy(test.name, entry->d_name);
			stat(entry->d_name, &statbuf);
			size=statbuf.st_size;
			test.length=(int)size;
			//printf("%ld\n", (long)size);
			buf = malloc(size);
			fread(buf, 1, size, fin);
			fwrite(&test, sizeof(test), 1, fout);
			fwrite(buf, 1, size, fout);
			free(buf);
			fclose (fin);
		}
	}
	chdir("..");
	closedir(dp);
	return;
}*/


void unpack(char *file)
{
	FILE *fout;
	FILE *fin;
	struct fields test;
	char *buf;

	fout = fopen("out", "rb");
	mkdir("unpack", 0700);
	chdir("unpack");
	while (fread(&test, sizeof(test), 1, fout))
	{
		if (test.flag == 1)
		{
			unpack(test.name);
		}
		else
		{
			fin = fopen(test.name, "wb");
			buf = malloc(test.length);
			fread(buf, 1, test.length, fout);
			strcpy(test.name, buf);
			fwrite(buf, 1, test.length, fin);
			fclose(fin);
			free(buf);
		}
	}
	return;
}
/*void unpack(char *file)
{
	FILE *fout;
	FILE *fin;
	struct fields test;
	char *buf;
	int i=0, d;
	
	fout = fopen(file, "rb");
	mkdir("unpack", 0700);
	chdir("unpack");
	while (fread(&test, sizeof(test), 1, fout))
	{
		printf("AD\n");
		if (test.flag == 1)
		{
			printf("asd\n");
			mkdir(test.name, 0700);
			d = test.count;
			chdir(test.name);
			for (i = 0; i < d; i++)
			{
				printf("%d\n", d);
				fread(&test, sizeof(test), 1, fout);
				fin = fopen(test.name, "wb");
				buf = malloc(test.length);
				fread(buf, 1, test.length, fout);
				strcpy(test.name, buf);
				fwrite(buf, 1, test.length, fin);
				fclose(fin);
				free(buf);		
			}
			chdir("..");
			//closedir(dp);
			continue;
			
		}			
		fin = fopen(test.name, "wb");
		buf = malloc(test.length);
		fread(buf, 1, test.length, fout);
		strcpy(test.name, buf);
		fwrite(buf, 1, test.length, fin);
		
		fclose(fin);
		
		free(buf);
	}
	return;
}*/
/*
void unpack(char *file, FILE *fin)
{
	FILE *fout;
	FILE *fin;
	struct fields test;
	char *buf;
	int i=0, d;
	
/*	fout = fopen(file, "rb");
	mkdir("unpack", 0700);
	chdir("unpack");
	fread(&test, sizeof(test), 1, fout)
	if (test.flag == 1)
	{
		mkdir(test.name, 0700);
		chdir(test.name);
		unpack(test.name, fin)
		return;
	}
	else 
	{
		in = fopen(test.name, "wb");
		buf = malloc(test.length);
		fread(buf, 1, test.length, fout);
		strcpy(test.name, buf);
		fwrite(buf, 1, test.length, fin);
		fclose(fin);
		free(buf);
*/



int main (int argc, char *argv[])
{
	char *dir = argv[1];
	struct stat statbuf;
	mode_t modes;
	int kol;
	FILE *fout;
	
	/*if ((fout = fopen("out", "wb")) == NULL)
	{
		printf("cannot create output file %s\n", dir);
		exit(0);
	}*/
	/*kol = count_files(dir);
	printf("%d\n", kol);*/
	
	/*stat(dir, &statbuf);
	modes = statbuf.st_mode;
	if (S_ISDIR(modes))
	{
		if ((fout = fopen("out", "wb")) == NULL)
		{
			printf("cannot create output file %s\n", dir);
			exit(0);
		}
		pack(dir, fout);
	}
	else unpack(dir);*/
	//kol = count_files(dir);
	//printf ("%d\n", kol);
	//fclose (fout);
	//pack(dir);
	unpack(dir);
	return 0;
}
