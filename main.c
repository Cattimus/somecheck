#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void printHelpText();

//intended usage: somecheck [checksum_version] [filename] [hash]
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

	return 0;
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