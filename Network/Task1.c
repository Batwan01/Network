#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#define BUF_SIZE 100

int main(int argc, char* argv[]) {
	int src, dst;  // source and destination
	int read_cnt = 0;  // the number of bytes 
	char buf[BUF_SIZE];

	/* file open for read-only: O_RDONLY */
	src = open("src.txt", O_RDONLY);

	/* file open for writing: O_CREAT|O_WRONLY|O_TRUNC */
	dst = open("dst.txt", O_CREAT | O_WRONLY | O_TRUNC);

	if (src == -1 || dst == -1) {
		puts("file open error");
		return -1;
	}

	/* read data from the source and write the data to the destination*/
	if (write(dst, buf, read_cnt == -1))
		error_handling("write() error!");

	close(src);
	close(dst);
	return 0;
}
