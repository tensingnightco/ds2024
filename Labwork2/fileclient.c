#include"errno.h"
#include"rpc/rpc.h"
#include"filetrans.h"
#include"stdio.h"
#include"stdlib.h"
#include <string.h>

int main(int argc, char const *argv[])
{
	if(argc!=3)
	{
		printf("\n\n error:insufficient arguments!!!");
		exit(-1);
	}

	/* Get host. */
	char *host = argv[1];
	/* Create client. */
	CLIENT *cl;
	cl=clnt_create(host,FILETRANSFER_PROG,FILETRANSFER_VERS,"tcp");
	if (cl == NULL) {
		/*
		 * Couldn't establish connection with server.
		 * Print error message and stop.
		 */
		 clnt_pcreateerror(host);
		 exit(1);
	}
	/* Get filename. */
	char *name = argv[2];

	FILE *file;
	file = fopen(name, "r");

	/* 
	 * Create buffer 
	 * Change file name that save on server
	 * ex: abc.txt will be save as abc.txtsv
	 */
	buffer buff;
	buff.name = strcat(name,"sv");

	int read_bytes;
	int *result;
  	long total_bytes = 0;
	
	/* Call the remote procedure readdir on the server */
	while (1) {
		/* Read a BLOCK of bytes from file to data. */
		read_bytes = fread(buff.data, 1, BLOCK, file);
		total_bytes += read_bytes;
		buff.numbytes = read_bytes;
		result = filetransfer_proc_1(&buff, cl);

		if (result == NULL) {
			/*
			 * An RPC error occurred while calling the server.
			 * Print error message and stop.
			 */
			clnt_perror(cl, host);
			exit(1);
		}

		/* Successfully called the remote procedure. */
		if (*result == 0) {
			/*
			 * A remote system error occurred.
			 * Print error message and stop.
			 */
			errno = *result;
			perror(name);
			exit(1);
		}

		/*
		 * Successfully got a block of the file.
		 * Write into our local file.
		 */
		if (read_bytes < BLOCK) 
			break;
	}

	fclose(file);

	printf("File Sent: %s\n",buff.name );
	printf("Bytes Sent: %ld\n", total_bytes);

	return 0;
}
