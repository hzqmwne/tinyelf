#ifndef TINYLIB_H_
#define TINYLIB_H_

// https://git.musl-libc.org/cgit/musl/tree/arch/x86_64/syscall_arch.h

typedef unsigned long size_t;
typedef signed long ssize_t;
typedef signed long off_t;

typedef unsigned long long uint64_t;
typedef signed long long int64_t;
typedef unsigned int uint32_t;
typedef signed int int32_t;
typedef unsigned short uint16_t;
typedef signed short int16_t;
typedef unsigned char uint8_t;
typedef signed char int8_t;

#define NULL ((void *)0)

static __inline long __syscall0(long n)
{
	unsigned long ret;
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n) : "rcx", "r11", "memory");
	return ret;
}

static __inline long __syscall1(long n, long a1)
{
	unsigned long ret;
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1) : "rcx", "r11", "memory");
	return ret;
}

static __inline long __syscall2(long n, long a1, long a2)
{
	unsigned long ret;
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2)
						  : "rcx", "r11", "memory");
	return ret;
}

static __inline long __syscall3(long n, long a1, long a2, long a3)
{
	unsigned long ret;
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
						  "d"(a3) : "rcx", "r11", "memory");
	return ret;
}

static __inline long __syscall4(long n, long a1, long a2, long a3, long a4)
{
	unsigned long ret;
	register long r10 __asm__("r10") = a4;
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
						  "d"(a3), "r"(r10): "rcx", "r11", "memory");
	return ret;
}

static __inline long __syscall5(long n, long a1, long a2, long a3, long a4, long a5)
{
	unsigned long ret;
	register long r10 __asm__("r10") = a4;
	register long r8 __asm__("r8") = a5;
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
						  "d"(a3), "r"(r10), "r"(r8) : "rcx", "r11", "memory");
	return ret;
}

static __inline long __syscall6(long n, long a1, long a2, long a3, long a4, long a5, long a6)
{
	unsigned long ret;
	register long r10 __asm__("r10") = a4;
	register long r8 __asm__("r8") = a5;
	register long r9 __asm__("r9") = a6;
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
						  "d"(a3), "r"(r10), "r"(r8), "r"(r9) : "rcx", "r11", "memory");
	return ret;
}

// =============================================================================

static inline ssize_t read(int fd, void *buf, size_t count) {
	return (ssize_t)__syscall3(0, (long)fd, (long)buf, (long)count);
}

static inline ssize_t write(int fd, const void *buf, size_t count) {
	return (ssize_t)__syscall3(1, (long)fd, (long)buf, (long)count);
}

// https://git.musl-libc.org/cgit/musl/tree/include/fcntl.h
#define O_RDONLY 0
#define O_WRONLY 1
#define O_RDWR 2

#define O_CREAT 00000100
#define O_TRUNC 00001000

static inline int open(const char *pathname, int flags) {
	return (int)__syscall2(2, (long)pathname, flags);
}

static inline int openat(int dirfd, const char *pathname, int flags) {
	return (int)__syscall3(257, (long)dirfd, (long)pathname, flags);
}

static inline int close(int fd) {
	return (int)__syscall1(3, (long)fd);
}

// https://git.musl-libc.org/cgit/musl/tree/include/sys/mman.h
#define MAP_SHARED     0x01
#define MAP_PRIVATE    0x02
#define MAP_ANONYMOUS  0x20
#define MAP_NORESERVE  0x4000
#define PROT_NONE      0
#define PROT_READ      1
#define PROT_WRITE     2
#define PROT_EXEC      4

static inline void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset) {
	return (void *)__syscall6(9, (long)addr, (long)length, (long)prot, (long)flags, (long)fd, (long)offset);
}

static inline int munmap(void *addr, size_t length) {
	return (int)__syscall2(11, (long)addr, (long)length);
}

static inline int execve(const char *pathname, char *const argv[], char *const envp[]) {
	return (int)__syscall3(59, (long)pathname, (long)argv, (long)envp);
}

static inline void exit(int status) {
	__syscall1(60, (long)status);
}

static inline int ioctl(int fd, unsigned long request, unsigned long data) {
	return __syscall3(16, (long)fd, (long)request, (long)data);
}

static inline int symlink(const char *target, const char *linkpath) {
	return (int)__syscall2(88, (long)target, (long)linkpath);
}

#define SEEK_SET 0

static inline off_t lseek(int fd, off_t offset, int whence) {
	return __syscall3(8, (long)fd, (long)offset, (long)whence);
}

#define AT_EMPTY_PATH 0x1000

static inline int execveat(int dirfd, const char *pathname, char *const argv[], char *const envp[], int flags) {
	return __syscall5(322, (long)dirfd, (long)pathname, (long)argv, (long)envp, (long)flags);
}

static inline int memfd_create(const char *name, unsigned int flags) {
	return __syscall2(319, (long)name, flags);
}


#define AF_ALG 38
#define SOCK_SEQPACKET 5

typedef uint32_t socklen_t;

struct sockaddr {
	uint16_t sa_family;
	char     sa_data[];
};

struct sockaddr_alg {
	uint16_t salg_family;
	uint8_t	 salg_type[14];
	uint32_t salg_feat;
	uint32_t salg_mask;
	uint8_t	 salg_name[64];
};

static inline int socket(int domain, int type, int protocol) {
	return __syscall3(41, (long)domain, (long)type, (long)protocol);
}

static inline int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
	return __syscall3(49, (long)sockfd, (long)addr, (long)addrlen);
}

static inline int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen) {
	return __syscall3(43, (long)sockfd, (long)addr, (long)addrlen);
}



// https://unix.superglobalmegacorp.com/Net2/newsrc/sys/ipc.h.html
#define	IPC_CREAT	01000	/* create entry if key does not exist */
#define	IPC_EXCL	02000	/* fail if key exists */
typedef long key_t;

static inline int shmget(key_t key, size_t size, int shmflg) {
	return __syscall3(29, (long)key, (long)size, (long)shmflg);
}

static inline void *shmat(int shmid, const void *shmaddr, int shmflg) {
	return (void *)__syscall3(30, (long)shmid, (long)shmaddr, (long)shmflg);
}

static inline int shmdt(const void *shmaddr) {
	return __syscall1(67, (long)shmaddr);
}



typedef uint32_t uid_t;
typedef uint32_t gid_t;

static inline int setresuid(uid_t ruid, uid_t euid, uid_t suid) {
	return __syscall3(117, (long)ruid, (long)euid, (long)suid);
}

static inline int setresgid(gid_t rgid, gid_t egid, gid_t sgid) {
	return __syscall3(119, (long)rgid, (long)egid, (long)sgid);
}


static inline int chdir(const char *path) {
	return __syscall1(80, (long)path);
}

static inline int fchdir(int fd) {
	return __syscall1(81, (long)fd);
}

static inline int dup2(int oldfd, int newfd) {
	return __syscall2(33, (long)oldfd, (long)newfd);
}

static inline int chroot(const char *path) {
	return __syscall1(161, (long)path);
}

#define	R_OK	4		/* Test for read permission.  */
#define	W_OK	2		/* Test for write permission.  */
#define	X_OK	1		/* Test for execute permission.  */
#define	F_OK	0		/* Test for existence.  */

static inline int access(const char *pathname, int mode) {
	return __syscall2(21, (long)pathname, (long)mode);
}

typedef uint32_t mode_t;

static inline int mkdir(const char *pathname, mode_t mode) {
	return __syscall2(83, (long)pathname, (long)mode);
}

#endif
