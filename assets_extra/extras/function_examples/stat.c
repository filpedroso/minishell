#include "headers.h"

void	print_stat_struct(const struct stat *st);

int	main(void)
{
	struct stat	buf;
	char		path[] = "/Users/filpedroso/EstudosCS/42/42_projects/common_core/minishell/functions.txt";

	errno = 0;
	if (stat(path, &buf) == -1)
	{
		perror("");
		return (1);
	}
	print_stat_struct(&buf);
}

void	print_stat_struct(const struct stat *st)
{
	/* Device and inode */
	printf("st_dev: %lld\n", (long long)st->st_dev);
	printf("st_ino: %llu\n", (unsigned long long)st->st_ino);
	/* Mode and links */
	printf("st_mode: 0%o\n", (unsigned int)st->st_mode);
	printf("st_nlink: %u\n", (unsigned int)st->st_nlink);
	/* Ownership */
	printf("st_uid: %u\n", (unsigned int)st->st_uid);
	printf("st_gid: %u\n", (unsigned int)st->st_gid);
	/* Special device */
	printf("st_rdev: %lld\n", (long long)st->st_rdev);
	/* Times (timespec) */
	printf("st_atimespec.tv_sec: %lld\n", (long long)st->st_atimespec.tv_sec);
	printf("st_atimespec.tv_nsec: %ld\n", (long)st->st_atimespec.tv_nsec);
	printf("st_mtimespec.tv_sec: %lld\n", (long long)st->st_mtimespec.tv_sec);
	printf("st_mtimespec.tv_nsec: %ld\n", (long)st->st_mtimespec.tv_nsec);
	printf("st_ctimespec.tv_sec: %lld\n", (long long)st->st_ctimespec.tv_sec);
	printf("st_ctimespec.tv_nsec: %ld\n", (long)st->st_ctimespec.tv_nsec);
	/* Size and storage */
	printf("st_size: %lld\n", (long long)st->st_size);
	printf("st_blocks: %lld\n", (long long)st->st_blocks);
	printf("st_blksize: %lu\n", (unsigned long)st->st_blksize);
	/* Flags and generation */
	printf("st_flags: 0x%lx\n", (unsigned long)st->st_flags);
	printf("st_gen: %lu\n", (unsigned long)st->st_gen);
}

// struct stat { /* when _DARWIN_FEATURE_64_BIT_INODE is NOT defined */
// 	dev_t    st_dev;    /* device inode resides on */
// 	ino_t    st_ino;    /* inode's number */
// 	mode_t   st_mode;   /* inode protection mode */
// 	nlink_t  st_nlink;  /* number of hard links to the file */
// 	uid_t    st_uid;    /* user-id of owner */
// 	gid_t    st_gid;    /* group-id of owner */
// 	dev_t    st_rdev;   /* device type, for special file inode */
// 	struct timespec st_atimespec;  /* time of last access */
// 	struct timespec st_mtimespec;  /* time of last data modification */
// 	struct timespec st_ctimespec;  /* time of last file status change */
// 	off_t    st_size;   /* file size, in bytes */
// 	quad_t   st_blocks; /* blocks allocated for file */
// 	u_long   st_blksize;/* optimal file sys I/O ops blocksize */
// 	u_long   st_flags;  /* user defined flags for file */
// 	u_long   st_gen;    /* file generation number */
// };
