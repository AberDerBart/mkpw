#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>

int wordCount;
char* dictFile;
int camelCase;

unsigned long dictSize;
char** words;
char* dictData;

int parseArgs(int,char**);
int parseDict();
void freeDict();
void printPW();
void printHelp();

unsigned long random29Bit(unsigned long);

int main(int argc,char** argv){
	FILE* dict;
	//set default values
	wordCount=4;
	dictFile="dict.txt";
	camelCase=0;

	//initialize radom number generator
	srand(time(NULL));

	if(parseArgs(argc,argv)){
		return 1;
	}
	
	parseDict();

	printPW();

	freeDict();
	
	return 0;
}

int parseArgs(int argc,char** argv){
	//sets the global variable according to the command line arguments
	int flagChar;

	opterr=0;

	while((flagChar=getopt(argc,argv,"cn:"))!=-1){
		switch(flagChar){
		case 'c':
			camelCase=1;
			break;
		case 'n':
			wordCount=atoi(optarg);
			break;
		default:
			printHelp();
			return 1;
			break;
		}
	}

	if(wordCount<=0){
		fprintf(stderr,"Error: Argument given with the -n flag must be an (unsigned) integer.\n");
		return 1;
	}

	if(optind<argc-1){
		fprintf(stderr,"Error: Too many arguments.\n");
		printHelp();
		return 1;
	}

	if(optind<argc){
		dictFile=argv[optind];
	}

	return 0;
}

int parseDict(){
	//open the dictionary
	FILE* dict=fopen(dictFile,"r");
	unsigned long length;

	unsigned readOffset;
	int wordFound=0;

	int wordIndex=0;

	if(!dict){
		fprintf(stderr,"Error: Could not open dict file: \"%s\".\n",dictFile);
		return 1;
	}

	//read the dictionary into dictData and close it
	fseek(dict,0,SEEK_END);
	length=ftell(dict);
	rewind(dict);

	dictData=malloc(length*sizeof(char));

	fread(dictData,sizeof(char),length,dict);

	fclose(dict);

	//count the words
	dictSize=0;
	wordFound=0;
	for(readOffset=0;readOffset<length;readOffset++){
		if(isgraph(dictData[readOffset]) && !wordFound){
			wordFound=1;
			dictSize++;
		}else if(isspace(dictData[readOffset])){
			wordFound=0;
		}
	}

	//create links to the words
	words=malloc(dictSize*sizeof(char*));
	
	wordIndex=0;
	wordFound=0;
	for(readOffset=0;readOffset<length && wordIndex<dictSize;readOffset++){
		if(isgraph(dictData[readOffset]) && !wordFound){
			wordFound=1;
			words[wordIndex]=&dictData[readOffset];
			wordIndex++;
			if(camelCase){
				dictData[readOffset]=toupper(dictData[readOffset]);
			}
		}else if(isspace(dictData[readOffset])){
			dictData[readOffset]=0;
			wordFound=0;
		}
	}

	return 0;
}

void freeDict(){
	free(dictData);
	free(words);
}
	
void printPW(){
	int i;
	unsigned long wordIndex;
	for(i=0;i<wordCount;i++){
		wordIndex=random29Bit(dictSize);
		printf("%s",words[wordIndex]);
	}
	printf("\n");
}

unsigned long random29Bit(unsigned long max){
	//return a random number between 0 and max. max is restricted to be up to 29 bit
	unsigned long random=0x1fffffff;

	if(max>0x1fffffff){
		return 0xffffffff;
	}
	
	while(random/max == 0x1fffffff/max){
		random=0x00000000;
		random|=rand() & 0x7fff;
		random <<= 15;
		random|=rand() & 0x7fff;
	}

	return random % max;
}

void printHelp(){
	printf("Usage: mkpw [ OPTIONS ] [ DICTFILE ]\n\n");
	printf("Possible Options:\n");
	printf("\t-c\n\t\tenables CamelCase\n");
	printf("\t-n NUMBER\n\t\tsets the number of words in the password\n");
	printf("\t-?\n\t\tprints this help message\n");
}

