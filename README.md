# somecheck

### What is it?:

Somecheck is a checksum verification tool used to partially automate file integrity checks (which should be done a lot more often than they are). The name is somecheck because it's like checksum (but backwards and with some instead of sum). I am very clever I know.

### Usage:

There are three primary arguments for the program: checksum string, filename, existing hash. **checksum string** is just the name of the checksum (md5, sha1, sha256, or sha512). **filename** is the name of the file in the current directory or a path to a file. **existing hash** is given to you by the website you're downloading the file from in order to verify the integrity of the data recieved.
