#include "basic.h"

#define BLOCKSIZE 512
#define MAGICNUMBER 1837083 
#define DIRECTORYENTRYCOUNT 16
#define ISDIRECTORY 1
#define ISFILE 2
#define ROOTBLOCK 35
#define SUPERBLOCK 34
#define FsStart 15

typedef struct{
	int magicNumber;
	unsigned int totalBlocksInDisk;
	unsigned int totalFreeBlocks;
	unsigned int firstFreeBlock;
	unsigned int lastFreeBlock;
	char wastedSpace[492];
}SuperBlock;


typedef struct
{
	unsigned int nextFreeBlock;
	char wastedSpace[492];
}UnusedBlock;

typedef struct
{
	char  fileName[20];
	char fileType;
	//unsigned int fileBlock;
	unsigned int fileBlockStart;
	unsigned int fileBlockSize;

}DirectoryEntry;

typedef struct 
{
	DirectoryEntry entries[16];
}DirectoryBlock;


DirectoryBlock createEmptyDirectory();
DirectoryBlock loadRootDirectory();
int findFileInDirectory(DirectoryBlock ,char * );
unsigned int findFileBlockInPath(char * );
unsigned int allocateBlock();

char * getPathName(char * path, int type);
void setFreeBlock(unsigned int blockNumber);

void createDir(char* filePath,char* fileName);
DirectoryBlock listDir(char * filePath);
void mkdirFS(char *name);
void renameDir(char *path, char * newName);
void copyDir(char *sourcePath, char *destinationPath);
void deleteDirCascade(int block);
void copyDirCascade(int blockSource, int blockDestination, char * destinationPath);
void move (char* sourcePath, char* destinyPath);

/*
#define BLOCKSIZE 512
#define MAGICNUMBER 1837083 
#define DIRECTORYENTRYCOUNT 16
#define ISDIRECRY 1
#define ISFILE 2
#define FsStart 15




typedef struct{
	int magicNumber;
	unsigned int totalBlocks;
	unsigned int totalFreeBlocks;
	unsigned int firstFreeBlock;
	unsigned int lastFreeBlock;
	char wastedSpace[492];
}SuperBlock;

typedef struct
{
	char  fileName[20];
	char fileType;
	unsigned int fileBlock;
	unsigned int fileBlockStart;
	unsigned int fileBlockSize;
}DirectoryEntry;

typedef struct 
{
	DirectoryEntry entries[16];
}DirectoryBlock;
*/