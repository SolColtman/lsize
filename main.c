#include <dirent.h> 
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>

struct File{
    char name[64];
    double size;
};

char * getPath();
long getFileSize(char * fileName);
char * calculateSize(long size);

int main(){
    DIR *directory;
    struct dirent *dir;
    struct File files[64];
    int c = 0;
    double size;
    char buf[256];
    char * slash = "/";
    char * s;

    char * path = getPath();

    directory = opendir(path);
    if (directory){
        while ((dir = readdir(directory)) != NULL){
            strcpy(files[c].name, dir->d_name);
            snprintf(buf, sizeof(buf), "%s%s%s", path, slash, dir->d_name);
            files[c].size = getFileSize(buf);
            c++;
        }
        closedir(directory);
    }

    for (int i=0; i<c; i++){
        if (files[i].name == NULL)
            break;

        s = calculateSize(files[i].size);
        if (strcmp(s, "kB") == 0)
            printf("%s - %.2f kB\n", files[i].name, (files[i].size/1000));
        else if (strcmp(s, "MB") == 0)
            printf("%s - %.2f MB\n", files[i].name, (files[i].size/1000000));
        else if (strcmp(s, "GB") == 0)
            printf("%s - %.2f GB\n", files[i].name, (files[i].size/1000000));
        else
            printf("%s - %.0f bytes\n", files[i].name, files[i].size);
    }

    free(path);
    return 0;
}

char * getPath(){
    char *path;
    char *dot = "./";
    char name[64];

    printf("enter relative directory e.g. sub_folder/sub_sub_folder\n   : ");
    scanf("%s", name);
    printf("\n");

    path = malloc(strlen(name)+strlen(dot)+1);
    strcpy(path, dot);
    strcat(path, name);

    return path;
}

long getFileSize(char * fileName){
    FILE *fp;
    long size;

    fp = fopen(fileName, "r");
    if (fseek(fp, 0, SEEK_END) < 0){
        fclose(fp);
        return -1;
    }

    size = ftell(fp);
    fclose(fp);
    return size;
}

char * calculateSize(long size){
    if (size >= 0 && size <= 999) return "B";
    if (size >= 1000 && size < 1000000) return "kB";
    if (size >= 1000001 && size < 1000000000) return "MB";
    if (size >= 1000000001) return "GB";
}