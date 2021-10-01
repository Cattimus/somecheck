#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void printHelpText();
int checkMd5(char* filePath, char* hash, int ignoreCase);
int checkHash(char* hashA, char* hashB, int ignoreCase);
void getHash(char* dest, char* commandString, int sz);
void sanitize(char* dest, char* src, int sz);

//intended usage: somecheck [checksum_version] [filename] [hash] --options
//supported checksums: md5 sha1 sha256 sha512
int main(int argc, char* argv[])
{
	//display help text with no error if --help is provded as argument
	if(argc > 1)
	{
		int charMax = 7;
		if(strlen(argv[1]) < charMax)
		{
			charMax = strlen(argv[1]);
		}

		if(!strncmp("--help", argv[1], charMax))
		{
			printHelpText();
			exit(0);
		}
	}

	//in any other scenario 3 arguments are required
	if(argc < 4)
	{
		fprintf(stderr, "Insufficient arguments provided.\n");
		printHelpText();
		exit(1);
	}

	//basic options
	char* checksumVersion = argv[1];
	char filePath[256];
	sanitize(filePath, argv[2], 256);
	char* hash = argv[3];

	//extended options
	int ignoreCase = 0;

	//grab extended options
	if(argc > 4)
	{
		for(int i = 3; i < argc; i++)
		{
			if(!strcmp(argv[i], "--nocase"))
			{
				ignoreCase = 1;
			}
		}
	}

	int results = 0;

	//md5 hash check
	if(!strcmp(checksumVersion, "md5"))
	{
		results = checkMd5(filePath, hash, ignoreCase);
	}

	if(results)
	{
		printf("Hashes match.\n");
	}
	else
	{
		printf("Hashes do not match.\n");
	}

	return 0;
}

//check md5
int checkMd5(char* filePath, char* hash, int ignoreCase)
{
	//assemble command
	char command[512];
	memset(command, 0, 512);

	int maxInput = 512 - 8;
	if(strlen(filePath) < maxInput)
	{
		maxInput = strlen(filePath);
	}
	
	strncat(command, "md5sum ", 8);
	strncat(command, filePath, maxInput);

	//get md5 hash
	char results[512];
	memset(results, 0, 512);
	getHash(results, command, 512);

	printf("raw output: %s", results);
	printf("input hash: %s\n", hash);

	return checkHash(results, hash, ignoreCase);
}

void getHash(char* dest, char* command, int sz)
{
	FILE* process = popen(command, "r");
	if(process == NULL)
	{
		fprintf(stderr, "Failed to open process md5sum.\n");
		exit(1);
	}
	
	//read result into buffer
	fgets(dest, sz, process);
	pclose(process);
}

//check if the two hashes are identical
int checkHash(char* hashA, char* hashB, int ignoreCase)
{
	int maxlen = strlen(hashA);
	if(strlen(hashB) < maxlen)
	{
		maxlen = strlen(hashB);
	}

	if(!ignoreCase)
	{
		return !strncmp(hashA, hashB, maxlen);
	}
	else
	{
		for(int i = 0; i < maxlen; i++)
		{
			//if any characters do not match we know  we can stop comparing
			if(tolower(hashA[i]) != tolower(hashB[i]))
			{
				return 0;
			}
		}

		//if no mismatches were found, strings must be equal
		return 1;
	}
}

void printHelpText()
{
	printf("[USAGE]\n");
	printf("somecheck [checksum_name] [file_path] [hash_string]\n\n");

	printf("[SUPPORTED HASHES]\n");
	printf("md5\n");
	printf("sha1\n");
	printf("sha256\n");
	printf("sha512\n");
}

//sanitize string (escape with quotes to prevent attacks)
void sanitize(char* dest, char* src, int sz)
{
	memset(dest, 0, sz);
	strcat(dest, "\"");
	strncat(dest, src, sz - 3);
	strcat(dest, "\"");
}