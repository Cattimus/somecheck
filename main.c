#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define BUFF_SIZE 1024


int checkHash(char* commandString, char* filePath, char* hash);
int checkMatch(char* hashA, char* hashB);
void sanitize(char* dest, char* src, int sz);
void printHelpText();

int ignoreCase = 0;
int showOutput = 0;

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

	//grab extended options
	if(argc > 4)
	{
		for(int i = 3; i < argc; i++)
		{
			if(!strcmp(argv[i], "--nocase"))
			{
				ignoreCase = 1;
			}

			if(!strcmp(argv[i], "--show"))
			{
				showOutput = 1;
			}
		}
	}

	int results = 0;

	//hash checks
	if(!strcmp(checksumVersion, "md5"))
	{
		results = checkHash("md5sum ", filePath, hash);
	}
	else if(!strcmp(checksumVersion, "sha1"))
	{
		results = checkHash("sha1sum ", filePath, hash);
	}
	else if(!strcmp(checksumVersion, "sha256"))
	{
		results = checkHash("sha256sum ", filePath, hash);
	}
	else if(!strcmp(checksumVersion, "sha512"))
	{
		results = checkHash("sha512sum ", filePath, hash);
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

//check with a given hash function if the inputted hash matches
int checkHash(char* commandString, char* filePath, char* hash)
{
	//assemble command
	char command[BUFF_SIZE];
	memset(command, 0, BUFF_SIZE);

	int maxInput = BUFF_SIZE - strlen(commandString);
	if(strlen(filePath) < maxInput)
	{
		maxInput = strlen(filePath);
	}
	
	strcat(command, commandString);
	strncat(command, filePath, maxInput);

	//get md5 hash
	char results[BUFF_SIZE];
	memset(results, 0, BUFF_SIZE);

	FILE* process = popen(command, "r");
	if(process == NULL)
	{
		fprintf(stderr, "Failed to open process %s.\n", commandString);
		exit(1);
	}
	
	//read result into buffer
	fgets(results, BUFF_SIZE, process);
	pclose(process);

	if(showOutput)
	{
		printf("Raw output: %s", results);
		printf("User Input: %s\n", hash);
	}

	return checkMatch(results, hash);
}

//check if the two hashes are identical
int checkMatch(char* hashA, char* hashB)
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

//sanitize string (escape with quotes to hopefully prevent attacks)
void sanitize(char* dest, char* src, int sz)
{
	memset(dest, 0, sz);
	strcat(dest, "\"");
	strncat(dest, src, sz - 3);
	strcat(dest, "\"");
}