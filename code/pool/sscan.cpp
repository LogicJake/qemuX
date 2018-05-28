#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/stat.h> 

int count = 0;

bool isFolder(char *path)
{
    struct stat s_buf; 
	stat(path,&s_buf);  
	if(S_ISDIR(s_buf.st_mode))   
		return true;
	return false;
}

bool isHeaderFile(char *path){
	if(path[strlen(path)-1] == 'h' && path[strlen(path)-2] == '.')
		return true;
	return false;
}

void countWords(char *path){
	char line[1024];
	FILE *fp;
	if((fp=fopen(path,"r"))==NULL)
	{
		printf("can not open file %s",path);
		exit(0);
	}
	else
	{
		while(fgets(line,1024,fp)!=NULL)
		{
			if(strstr(line,"define"))
				count++;
		}
	}
}

char* concatPath(char *path,char *filename){
	char *tempPath = (char *)malloc(sizeof(char)*(strlen(path)+strlen(filename)+3));
	strcpy(tempPath,path);
	strcat(tempPath,"\\");		//ÐèÒªÐÞ¸Ä 
	strcat(tempPath,filename);
	return tempPath;
}

void scan(char *path){
	int error;
    DIR *dir;
    struct dirent entry;
    struct dirent *result;

    dir = opendir(path);
    while(1)
	{
        result = readdir(dir);
        if (error != 0) {
            perror("readdir");
            return;
        }

        if (result == NULL)
            break;
        char *fullPath = concatPath(path,result->d_name);

		if(result->d_name[0] != '.' && isFolder(fullPath))
		{
			scan(fullPath);
		}
		else if(isHeaderFile(fullPath)){
			countWords(fullPath);
		}   
    }
    closedir(dir);
}

int main(){
	scan("C:\\Users\\ASUS\\Desktop\\test");
	printf("%d",count);
}
