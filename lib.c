/*lib.c
Michael Black, 2007
This file contains the various library functions called by the shell and 
user applications
*/

/*setup enables multitasking.  it should be called at the beginning of 
any user program*/

#include "lib.h"

void setup()
{
	seti();
}

/*computes a mod b*/
int mod(int a, int b)
{
        while(a>=b)
                a=a-b;
        return a;
}

/*computes a / b using integer division*/
int div(int a, int b)  
{               
        int q=0;
        while (q*b<=a)
                q++;
        return q-1;
}               

/*reads a sector into the buffer*/
void readsector(int sectornumber, char* buffer)
{
        int sec,head,cyl;

	/*convert to CHS*/
        sec=mod(sectornumber,0x12)+1;
        head=mod(div(sectornumber,0x12),2);
        cyl=div(sectornumber,0x24);

        readsect(buffer,sec,head,cyl);
}

/*writes buffer to a sector*/
void writesector(int sectornumber, char* buffer)
{
        int sec,head,cyl;

	/*convert to CHS*/
        sec=mod(sectornumber,0x12)+1;
        head=mod(div(sectornumber,0x12),2);
        cyl=div(sectornumber,0x24);

        writesect(buffer,sec,head,cyl);
}

/*prints a character*/
void putchar(char c)
{
	printc(c);
}

/*reads a character*/
char getchar()
{
	return (char)readc();
}

/*prints a string terminated with 0*/
void printstring(char* string)
{
	int21(1,string);
}

/*prints an integer*/
void printnumber(int number)
{
	char num[12];
	char pnum[12];
	int d=1;
	int i=0;
	int j;

	/*convert the number to a string digit by digit*/
	while(i<10)
	{
		num[i]=mod(div(number,d),10)+0x30;
		i++;
		d=d*10;
		if (div(number,d)==0)
			break;
	}

	/*reverse it to read most significant to least significant*/
	j=0;
	for (d=i-1; d>=0; d--)
		pnum[j++]=num[d];
	pnum[j]=0;
	printstring(pnum);
}

/*read a line from the keyboard and put it in buffer*/
void readstring(char* buffer)
{
	int21(2,buffer);
}

/*read the file name[] into buffer[]*/
void readfile(char* name, char* buffer)
{
	int21(3,name,buffer);
}

/*write buffer[] to a file called name[]*/
void writefile(char* name, char* buffer, int sectorlength)
{
	int21(4,name,buffer,sectorlength);
}

/*delete the file name[]*/
void deletefile(char* name)
{
	int21(5,name);
}

/*execute the program*/
void executeprogram(char* buffer, int bytelength)
{
	int21(8,buffer,bytelength);
}

/*execute the program, but don't make the caller wait*/
void executeprogrambackground(char* buffer, int bytelength)
{
	int21(6,buffer,bytelength);
}

/*terminate program - this must be called by the program before ending*/
void exit()
{
	int21(7);
}

/*sets the video to 1 - graphics, 0 - text*/
void setvideo(int mode)
{
	if (mode==0)
		setvideotext();
	else
		setvideographics();
}

/*sets the pixel at columnxrow to color*/
void setpixel(int color, int column, int row)
{
	drawpixel(color,row*320+column);
}

/*sets the cursor position to row, column*/
void setcursor(int row, int column)
{
	interrupt(0x10,2*256,0,0,row*256+column);
}

/*clear the screen*/
void clearscreen()
{
        int r,c;
        for (c=0; c<80; c++)
                for (r=0; r<25; r++)
                {
                        putInMemory(0xB800,(r*80+c)*2,0x20);
                        putInMemory(0xB800,(r*80+c)*2+1,7);
                }
}

/*prints a character at a certain cursor position*/
void setchar(char c, char color, int row, int column)
{
	putInMemory(0xB800,(row*80+column)*2,c);
	putInMemory(0xB800,(row*80+column)*2+1,color);
}

/*prints a string at a certain cursor position*/
void setstring(char* c, char color, int row, int column)
{
	int i=0;
	while(c[i]!=0x0)
	{
		putInMemory(0xB800,(row*80+column+i)*2,c[i]);
		putInMemory(0xB800,(row*80+column+i)*2+1,color);
		i++;
	}
}

void ps()
{
    int21(17);
}

/*turns an integer into a string*/
void getnumberstring(char* pnum, int number)
{
	char num[12];
	int d=1;
	int i=0;
	int j;

	/*convert the number to a string digit by digit*/
	while(i<10)
	{
		num[i]=mod(div(number,d),10)+0x30;
		i++;
		d=d*10;
		if (div(number,d)==0)
			break;
	}

	/*reverse it to read most significant to least significant*/
	j=0;
	for (d=i-1; d>=0; d--)
		pnum[j++]=num[d];
	pnum[j]=0;
}

void mkdir(char * line)
{
	int21(11,line);
	//printstring(line);
	
}



//funciones utilerías

  

int mystrlen(char *cadena){
    int i = 0;

    while(cadena[i] !=  '\0'){
        i++;
    }
    return i;
}

char * mistrcat(char * destino,char * fuente)
{
 int i=0, j=0, pos = 0, len = mystrlen(destino) + mystrlen(fuente);
 char * temp = malloc(len);
 
    while(destino[i])
    {
         temp[i] = destino[i];
         i++;
    }
  
 while(fuente[j])
 {
  temp[i]=fuente[j];
  i++;
  j++;
 }
 
  destino = malloc(len);
  for(pos;pos<len;pos++)
  destino[pos] = temp[pos];
 return destino;
}

char * strcpy( char *to,char* from)
{
    int i=0;
    while (to[i]!='\0')
    {
       from[i]=to[i];
       i++;
    }
  return from;
}

int strCmp ( char *s1,  char *s2)
{
     unsigned char *p1 = ( unsigned char *)s1;
     unsigned char *p2 = ( unsigned char *)s2;

    while (*p1 != '\0') {
        if (*p2 == '\0') return  1;
        if (*p2 > *p1)   return -1;
        if (*p1 > *p2)   return  1;

        p1++;
        p2++;
    }

    if (*p2 != '\0') return -1;

    return 0;
}

int stlen(char * cadena, int start, char delim)
{
    int count = 0, pos = (cadena[start]==delim) ? start+1 : start;

    while(cadena[pos]!=delim && cadena[pos]!='\0'){
        count++;
        pos++;
    }

    return count;
}



int qtytoken(char * token, char delim)
{
    int totalTokens = 1;
    int pos = 0;

    while(token[pos]!='\0')
    {
        if(token[pos]==delim){
        totalTokens++;
        }
        pos++;
    }
    return totalTokens;
}

char * strtok(char * str, char delim, int pos)
{
    
    int totalTokens = qtytoken(str,delim), count = 1, posStr = 0;
    char * token = (char*)malloc(stlen(str, posStr, delim));
    int posToken = 0;

while(str[posStr]!='\0')
    {
        if(str[posStr]==delim)
        {
            int len = stlen(str, posStr, delim);
            count++;
            token =((count<pos)) ? (char*)malloc(stlen(str, posStr, delim)) : token;
        }
        else if(count==pos){
            token[posToken] = str[posStr];
            posToken++;
        }
        posStr++;
    }   
    return token;
}


//funciones de Basic

/*reads a sector into the buffer*/
void read_sector(int sectornumber, char* buffer)
{
        int sec,head,cyl;

    /*convert to CHS*/
        sec=mod(sectornumber,0x12)+1;
        head=mod(div(sectornumber,0x12),2);
        cyl=div(sectornumber,0x24);

        readsect(buffer,sec,head,cyl);
}

/*writes buffer to a sector*/
void write_sector(int sectornumber, char* buffer)
{
        int sec,head,cyl;

    /*convert to CHS*/
        sec=mod(sectornumber,0x12)+1;
        head=mod(div(sectornumber,0x12),2);
        cyl=div(sectornumber,0x24);

        writesect(buffer,sec,head,cyl);
}


//funciones de FS
DirectoryBlock createEmptyDirectory()
{
	DirectoryBlock newDirectory;
	int i;
	for(i=0;i<DIRECTORYENTRYCOUNT;i++)
	{
		newDirectory.entries[i].fileBlockStart = 0;
		newDirectory.entries[i].fileBlockSize = 0;
	}
	return newDirectory;
}

DirectoryBlock loadRootDirectory()
{
	DirectoryBlock rootBlock;
	read_sector(ROOTBLOCK,(char*)&rootBlock);
	return rootBlock;
}

SuperBlock  loadSuperBlock()
{
	SuperBlock superBlock;
	read_sector(SUPERBLOCK, (char*)&superBlock);
	return superBlock;
}

unsigned int allocateBlock()
{
	SuperBlock superBlock = loadSuperBlock();

	unsigned int allocatedBlockNumber = superBlock.firstFreeBlock;
    UnusedBlock  allocatedBlock;
    unsigned int newFirstBlockNumber;
	read_sector(allocatedBlockNumber, (char*)&allocatedBlock);
	
	newFirstBlockNumber = allocatedBlock.nextFreeBlock;
	superBlock.firstFreeBlock = newFirstBlockNumber;
	superBlock.totalFreeBlocks--;
	write_sector(SUPERBLOCK,(char*)&superBlock);

	return allocatedBlockNumber;
}

int findFileInDirectory(DirectoryBlock directory, char* fileName)
{
	int i;
	for(i=0;i<DIRECTORYENTRYCOUNT;i++)
		if(strCmp(directory.entries[i].fileName,fileName)==0)
			return i;
	return -1;
}
unsigned int findFileBlockInPath(char* path)
{
	int tokens = qtytoken(path,'/'); 
	int posToken=1;
	char *directoryName; 
	
	int directoryEntryNumber; 
	unsigned int directoryBlockNumber; 
	//ubicacion del root en el block 16 
	DirectoryBlock directory;

	directoryName = strtok(path, '/', posToken);
	directoryBlockNumber = ROOTBLOCK;
	directoryEntryNumber=0;

	posToken++; 
	directory = loadRootDirectory(); 
	while (posToken <= tokens) 
		{ 
			
			directoryEntryNumber = findFileInDirectory(directory,directoryName); 
			if(directoryEntryNumber==-1) 
				return 0; 
			directoryBlockNumber = directory.entries[directoryEntryNumber].fileBlockStart;
			read_sector( directoryBlockNumber,(char *)&directory);
			directoryName = strtok(path, '/', posToken); 
			posToken++; 
		} 
	return directoryBlockNumber; 
}

void setFreeBlock(unsigned int blockNumber)
{
	SuperBlock superBlock = loadSuperBlock();
	UnusedBlock newBlock;
	UnusedBlock lastFreeBlock;
	unsigned int secondToLastBlock;

	newBlock.nextFreeBlock = 0;

	
	read_sector(superBlock.lastFreeBlock,&lastFreeBlock);

	lastFreeBlock.nextFreeBlock = blockNumber;
	secondToLastBlock =superBlock.lastFreeBlock;
	superBlock.lastFreeBlock = blockNumber;

	superBlock.totalFreeBlocks++;

	write_sector(SUPERBLOCK, (char*) &superBlock);
	write_sector(secondToLastBlock, (char*)&lastFreeBlock);
	write_sector(blockNumber, (char*)&newBlock);

}

unsigned int createDir(char * filePath,char * fileName)
{
	 unsigned int destinationDirBlockNumber;
	 unsigned int newDirBlockNumber;
	 DirectoryBlock destinationDirectory;
	 DirectoryBlock newDirectory;
	 int i;

	 destinationDirBlockNumber = findFileBlockInPath(filePath);
	 if(destinationDirBlockNumber==0)
	 {
		 //printf("%s\n", "Directory does not exists");
		 return 0;
	 }
	 
	 read_sector(destinationDirBlockNumber,(char*)&destinationDirectory);

	 newDirBlockNumber = allocateBlock();
	 
	 newDirectory = createEmptyDirectory();
	 write_sector(newDirBlockNumber,(char*)&newDirectory);
	 
	 for(i=0;i<DIRECTORYENTRYCOUNT;i++)
	 {
		 if(destinationDirectory.entries[i].fileBlockStart==0)
		 {
			 strcpy(destinationDirectory.entries[i].fileName,fileName);
			 destinationDirectory.entries[i].fileType='D';
			 destinationDirectory.entries[i].fileBlockStart = newDirBlockNumber;
			 destinationDirectory.entries[i].fileBlockSize= 1;
			 break;
		 }
	 }
	 write_sector(destinationDirBlockNumber, (char*)&destinationDirectory);

	 return destinationDirBlockNumber;
}

DirectoryBlock listDir(char * filePath)
{
	DirectoryBlock destinationDirectory;
	unsigned int destinationDirBlockNumber= findFileBlockInPath(filePath);
	if(destinationDirBlockNumber==0)
	{
		return;
	}
	
	read_sector(destinationDirBlockNumber,(char*)&destinationDirectory);

	return destinationDirectory;
}

char * getPathName(char * path, int type)
{
	int lengthName;
    int tokens;
    int posToken;
    int i=0;
    char *dirName;

	if(type==1)
	{
		return path;
	}

	lengthName = mystrlen(path);
    
    path = (char *)malloc(lengthName);

	tokens = qtytoken(path,'/');
    
    posToken=1;

    path = strtok(path, '/', posToken);
    path = mistrcat(path, "/");
    
    posToken++;

    while (posToken <= tokens-1)
    {
    	char *token;
    	token = strtok(path, '/', posToken);
        path = mistrcat(path, token);
        if(posToken != tokens-1)
            path = mistrcat(path, "/");
        posToken++;
    }
    
    dirName = strtok(path, '/', tokens);

	return path;
}

void mkdirFS(char *name)
{
    char * path;
    
    path = getPathName(name, 2);

    name = getPathName(name, 1);
	
    createDir(path, name);
}


void renameDir(char *dirPath, char *newDirName)
{
	DirectoryBlock dir;
	int dirBlockNumber;
	int entryNumber;
	char *oldDirName;
	char *path;
	
    //path = (char *)malloc(lengthdirPath);
	
	path = getPathName(dirPath,1);
    oldDirName = getPathName(dirPath,2);

    dirBlockNumber = findFileBlockInPath(path);
    

	if(dirBlockNumber==0)
	{
		//cout<<"Directory does not exists";
		return;
	}
	
	read_sector(dirBlockNumber,(char*)&dir);
	entryNumber = findFileInDirectory(dir, oldDirName);

	if(dir.entries[entryNumber].fileName == oldDirName)
	{
		strcpy(dir.entries[entryNumber].fileName,newDirName);
		write_sector(dirBlockNumber,(char*)&dir);
	}
}

void copyDir(char *sourcePath, char *destinationPath){
	DirectoryBlock dirSource;
	int blockSource, blockDestination;

	blockSource = findFileBlockInPath(sourcePath);
	blockDestination = findFileBlockInPath(destinationPath);

	if(blockSource == 0 || blockDestination == 0)
	{
		return;
	}

	read_sector(blockSource, (char*)&dirSource);
}

void deleteDirCascade(int block)
{
	DirectoryBlock dir;
	int i,j;
	int fileStart;
	int fileSize;

	read_sector(block, (char *)&dir);

	if(block == 0)
	{
		return;
	}	

	for(i=0; i<DIRECTORYENTRYCOUNT;i++)
	{
		if(dir.entries[i].fileType == ISDIRECTORY)
		{
			//recorrer todo el directorio
			deleteDirCascade(dir.entries[i].fileBlockStart);
			setFreeBlock(i);
		}
		else if(dir.entries[i].fileType == ISFILE)
		{
			fileStart = dir.entries[i].fileBlockStart;
			fileSize = dir.entries[i].fileBlockSize;
			
			for(j=fileStart; j<fileSize; j++)
			{
				setFreeBlock(j);
			}
		}
	}
}

void copyDirCascade(int blockSource, int blockDestiny, char* pathDestiny) 
{ 
	DirectoryBlock dirSource;
	DirectoryBlock dirDestiny; 
	int i,j,x; 
	int fileStart; 
	int newBlock;
	int fileSize; 
	readSector2(blockSource, (char *)&dirSource);
	readsector2(blockDestiny, (char*)&dirDestiny); 

	if(blockSource == 0) 
	{ 
		return; 
	} 

	for(i=0; i<DIRECTORYENTRYCOUNT;i++) 
	{

		 if(dirSource.entries[i].fileType == ISDIRECTORY) 
		 {

		  newBlock = createDir(pathDestiny,dirSource.entries[i].fileName);
		  pathDestiny = mystrcat(pathDestiny,dirSource.entries[i].fileName);
		  for(x=0; x <= DIRECTORYENTRYCOUNT;x++)
		  {
		  	if(dirDestiny.entries[x].fileBlockStart==0)
		  	{
		  		dirDestiny.entries[x].fileBlockStart = newBlock;
		  		break;
		  	}
		  }
		  writesector2(blockDestiny,(char*)&dirDestiny);
		  copyDirCascade(dirDestiny.entries[x].fileBlockStart,newBlock,pathDestiny); 
		   
		 } 
		 else if(dirSource.entries[i].fileType == ISFILE) 
		 	{ 
		 		
		 	} 
		 else
		 	copyDirCascade(dirDestiny.entries[x].fileBlockStart, blockDestiny, pathDestiny);
	} 
}

void move (char* sourcePath, char* destinyPath)
{
	copyDir(sourcePath,destinyPath);
	removeDir(sourcePath);
}
