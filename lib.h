
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




void readsector(int, char*);
void writesector(int, char*);
void putchar(char);
char getchar();
void printstring(char*);
void printnumber(int);
void readstring(char*);
void readfile(char*, char*);
void writefile(char*, char*, int);
void deletefile(char*);
void exit();
void executeprogram(char*, int,char*);
void allow_preemption();
int mod(int,int);
int div(int,int);
void setvideo(int);
void setpixel(int,int,int);
void clearscreen();
void setcursor(int,int);
void setchar(char,char,int,int);
void setstring(char*,char,int,int);
void getnumberstring(char*,int);



//Funciones utilerías
char *strcpy(char* , char*);

int strCmp (const char *, const char *);

int stlen(char * cadena, int start, char delim);

int qtytoken(char * token, char delim);

char * strtok(char * str, char delim, int pos);

int mystrlen(char *cadena);

char * mistrcat(char * destino,char * fuente);


//Funciones de Basic
void read_sector(int, char*);
void write_sector(int, char*);



//funciones del FS



DirectoryBlock createEmptyDirectory();
DirectoryBlock loadRootDirectory();
int findFileInDirectory(DirectoryBlock ,char * );
unsigned int findFileBlockInPath(char * );
unsigned int allocateBlock();

char * getPathName(char * path, int type);
void setFreeBlock(unsigned int blockNumber);

unsigned int createDir(char* filePath,char* fileName);
DirectoryBlock listDir(char * filePath);
void mkdirFS(char *name);
void renameDir(char *path, char * newName);
void copyDir(char *sourcePath, char *destinationPath);
void deleteDirCascade(int block);
void copyDirCascade(int blockSource, int blockDestination, char * destinationPath);
void move (char* sourcePath, char* destinyPath);



void ps();
