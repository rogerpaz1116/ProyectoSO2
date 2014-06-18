#include "FS.h"

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

void createDir(char * filePath,char * fileName)
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
		 return;
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
	directoryBlock dirSource;
	directoryBlock dirDestiny; 
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

	for(i=0; i<directoryCount;i++) 
	{

		 if(dirSource.entries[i].fileType == IS_DIRECTORY) 
		 {

		  newBlock = createDir(pathDestiny,dirSource.entries[i].fileName);
		  pathDestiny = mystrcat(pathDestiny,dirSource.entries[i].fileName);
		  for(x=0; x <= directoryCount;x++)
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
		 else if(dirSource.entries[i].fileType == IS_FILE) 
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