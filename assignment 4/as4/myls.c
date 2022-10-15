#define _GNU_SOURCE
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>

#define MAX_FILE_LIST_SIZE 100
#define MAX_DIR_FILE_SIZE 1024
#define MAX_FILENAME_SIZE 256


int parseOptions(char* options, int result[3]);
void printDate(time_t* time);
int isValid(const char* path);
int isFile(const char* path);
int isDirectory(const char* path);
int isPathEndWithDirChar(const char* dir);
void concatPath(char* parentName, char* filename, char* result);
void printFilePermission(mode_t mode);
void sortFilenames(char str[MAX_DIR_FILE_SIZE][MAX_FILENAME_SIZE], int length);

int printInfo(char* parentName, char* filename, int options[3]);
void printFileInfo(char* dirName, char* filename, int options[3]);
int printDirectoryInfo(char* dirName, char str[MAX_DIR_FILE_SIZE][MAX_FILENAME_SIZE], int size, int options[3]);

int parseOptions(char* options, int result[3]) {
	if(options == NULL || options[0] != '-') {
		return 1;
	}

	char* cur = options + 1;
	if(*cur == '\0') {
		return 1;
	}
	while(*cur != '\0') {
		if(*cur == 'i' && result[0] == 0) {
			result[0] = 1;
		} else if (*cur == 'l' && result[1] == 0) {
			result[1] = 1;
		} else if (*cur == 'R' && result[2] == 0) {
			result[2] = 1;
		} else {
			return 1;
		}
		cur = cur+1;
	}
	return 0;
}

void printDate(time_t* time) {
	char buf[50];
	struct tm* pTm = localtime(time);
	strftime(buf, sizeof(buf), "%b %e %Y %H:%M", pTm);
	printf("%s ", buf);
}

int isValid(const char* path) {
	struct stat buf;
	if(lstat(path, &buf) == -1) {
		return 0;
	}
	return 1;
}

int isFile(const char* path) {
	struct stat buf;
	lstat(path, &buf);
	return S_ISREG(buf.st_mode);
}

int isDirectory(const char* path) {
	struct stat buf;
	lstat(path, &buf);
	return S_ISDIR(buf.st_mode);
}

int isPathEndWithDirChar(const char* dir) {
	if(dir == NULL) return 0;
	char lastChar = dir[0];
	while(*dir != '\0') {
		lastChar = *dir;
		dir = dir + 1;
	}
	return (lastChar == '/') ? 1 : 0;
}

void concatPath(char* parentName, char* filename, char* result) {
	if(filename == NULL || result == NULL) return ;
	if(parentName) {
		strcpy(result, parentName);
		if(!isPathEndWithDirChar(result)) {
			strcat(result, "/");
		}
		strcat(result, filename);
	} else {
		strcpy(result, filename);
	} 
}

void printFilePermission(mode_t mode) {
	char perm[11];
	perm[0] = S_ISLNK(mode) ? 'l' : (S_ISDIR(mode) ? 'd' : '-');
	perm[1] = (mode & S_IRUSR) ? 'r' : '-';
	perm[2] = (mode & S_IWUSR) ? 'w' : '-';
	perm[3] = (mode & S_IXUSR) ? 'x' : '-';
	perm[4] = (mode & S_IRGRP) ? 'r' : '-';
	perm[5] = (mode & S_IWGRP) ? 'w' : '-';
	perm[6] = (mode & S_IXGRP) ? 'x' : '-';
	perm[7] = (mode & S_IROTH) ? 'r' : '-';
	perm[8] = (mode & S_IWOTH) ? 'w' : '-';
	perm[9] = (mode & S_IXOTH) ? 'x' : '-';
	perm[10] = '\0';
	printf("%s ", perm);
}

void printFileInfo(char* dirName, char* filename, int options[3]) {

	// Get entry's info
	struct stat statbuf;
	char fullname[MAX_FILENAME_SIZE];
	concatPath(dirName, filename, fullname);
	if(lstat(fullname, &statbuf) == -1) {
		printf("Error: get stat of filename '%s' failed\n", fullname);
		return ;
	}

	if(options[0] == 1) {
		printf("%lu ", statbuf.st_ino);
	}

	if(options[1] == 1) {
		printFilePermission(statbuf.st_mode);

		printf("%ld ", statbuf.st_nlink);

		struct passwd* pw = getpwuid(statbuf.st_uid);
		printf("%s ", pw->pw_name);

		struct group* grp = getgrgid(statbuf.st_uid);
		printf("%s ", grp->gr_name);

		printf("%9ld ", statbuf.st_size);
		printDate(&statbuf.st_mtime);
	}
	
	printf("%s", filename);
	if(options[1] == 1 && S_ISLNK(statbuf.st_mode)) {
		char linkfile[MAX_FILENAME_SIZE];
		ssize_t size = readlink(fullname, linkfile, sizeof(linkfile));
		if(size != -1) {
			linkfile[size] = '\0';
			printf(" -> %s", linkfile);
		}
	}
	printf("\n");
}


int printDirectoryInfo(char* dirName, char filenames[MAX_DIR_FILE_SIZE][MAX_FILENAME_SIZE], int size, int options[3]) {
	if(options[2] == 1) {
		printf("%s:\n", dirName);
	}

	// print all files/dirs in the cur directory
	for(int i = 0; i < size; ++i) {
		printFileInfo(dirName, filenames[i], options);
	}

	if(options[2] == 1) {
		printf("\n");
		for(int i = 0; i < size; ++i) {

			char subDirName[MAX_FILENAME_SIZE];
			concatPath(dirName, filenames[i], subDirName);
			if(isDirectory(subDirName)) {
				if(printInfo(dirName, filenames[i], options)) {
					printf("Print dir/file '%s' failed\n", filenames[i]);
					return 1;
				}
			}
		}
	}

	return 0;

}

void sortFilenames(char str[MAX_DIR_FILE_SIZE][MAX_FILENAME_SIZE], int length) {
	for(int i = 0; i <= length; ++i) {
		for(int j = i+1; j < length; ++j) {
			if(strcmp(str[i], str[j]) > 0) {
				char tmp[MAX_FILENAME_SIZE];
				strcpy(tmp, str[i]);
				strcpy(str[i], str[j]);
				strcpy(str[j], tmp);
			}
		}
	}
}


int printInfo(char* parentName, char* filename, int options[3]) {
	char fullname[MAX_FILENAME_SIZE];
	concatPath(parentName, filename, fullname);

	if(!isValid(fullname)) {
		printf("Error: cannot find file or directory '%s'\n", filename);
		return 1;	
	}

	// If this is a file
	if(isFile(fullname)) {
		printFileInfo(parentName, filename, options);
		return 0;
	}


	// If this is a dir
	DIR* dir;
	struct dirent* dp;

	if((dir = opendir(fullname)) == NULL) {
		printf("Cannot open directory %s\n", fullname);
		return 1;
	}

	char filenames[MAX_DIR_FILE_SIZE][MAX_FILENAME_SIZE];
	int size = 0;
	while((dp = readdir(dir)) != NULL) {
		if(strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0
			|| dp->d_name[0] == '.') {
			continue;
		}
		strcpy(filenames[size], dp->d_name);
		++size;
	}
	closedir(dir);

	sortFilenames(filenames, size);

	if(printDirectoryInfo(fullname, filenames, size, options)) {
		return 1;
	}

	return 0;
}


int main(int argc, char** argv) {

	// corresponds to options i, l and R, respectively
	// 1 indicates the options is specified, otherwise 0
	int options[3] = {0, 0, 0};
	char fileList[MAX_DIR_FILE_SIZE][MAX_FILENAME_SIZE];
	int fileListLength = 0;
	for(int i = 1; i < argc; ++i) {
		if(argv[i][0] == '-') {
			if(fileListLength > 0) {
				printf("Invlid options: '%s'. Note: options should be specified before file list\n", argv[i]);
				return 1;
			}
			if(parseOptions(argv[i], options)) {
				printf("Invalid option: '%s'\n", argv[i]);
				return 1;
			}
		} else {
			strcpy(fileList[fileListLength], argv[i]);
			++fileListLength;
		}
	}

	if(fileListLength > 0) {
		sortFilenames(fileList, fileListLength);
		for(int i = 0; i < fileListLength; ++i) {
			printInfo(NULL, fileList[i], options);
		}
	} else if(fileListLength == 0) {
		printInfo(NULL, ".", options);
	}

	return 0;
}

