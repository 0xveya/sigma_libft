#pragma once

/* Linux x86-64 syscall catalog and wrapper declarations.
 * Source:
 * https://www.chromium.org/chromium-os/developer-library/reference/linux-constants/syscalls/
 */
#define SIGMA_LINUX_SYSCALLS(X)                                                \
  /** Calls Linux read(unsigned int fd, char *buf, size_t count). See x86_64_0 \
   * in the source table. */                                                   \
  X(0, read, 3, NORMAL, sigma_read_result_t, USIZE, (i32, fd), (void *, data), \
    (usize, size))                                                             \
  /** Calls Linux write(unsigned int fd, const char *buf, size_t count). See   \
   * x86_64_1 in the source table. */                                          \
  X(1, write, 3, NORMAL, sigma_write_result_t, USIZE, (i32, fd),               \
    (const void *, data), (usize, size))                                       \
  /** Calls Linux open(const char *filename, int flags, umode_t mode). See     \
   * x86_64_2 in the source table. */                                          \
  X(2, open, 3, NORMAL, sigma_sys_result_t, IPTR, (const void *, filename),    \
    (i32, flags), (syscall_arg_t, mode))                                       \
  /** Calls Linux close(unsigned int fd). See x86_64_3 in the source table. */ \
  X(3, close, 1, NORMAL, sigma_sys_result_t, IPTR, (u32, fd))                  \
  /** Calls Linux stat(const char *filename, struct __old_kernel_stat          \
   * *statbuf). See x86_64_4 in the source table. */                           \
  X(4, stat, 2, NORMAL, sigma_sys_result_t, IPTR, (const void *, filename),    \
    (void *, statbuf))                                                         \
  /** Calls Linux fstat(unsigned int fd, struct __old_kernel_stat *statbuf).   \
   * See x86_64_5 in the source table. */                                      \
  X(5, fstat, 2, NORMAL, sigma_sys_result_t, IPTR, (u32, fd),                  \
    (void *, statbuf))                                                         \
  /** Calls Linux lstat(const char *filename, struct __old_kernel_stat         \
   * *statbuf). See x86_64_6 in the source table. */                           \
  X(6, lstat, 2, NORMAL, sigma_sys_result_t, IPTR, (const void *, filename),   \
    (void *, statbuf))                                                         \
  /** Calls Linux poll(struct pollfd *ufds, unsigned int nfds, int timeout).   \
   * See x86_64_7 in the source table. */                                      \
  X(7, poll, 3, NORMAL, sigma_sys_result_t, IPTR, (void *, ufds), (u32, nfds), \
    (i32, timeout))                                                            \
  /** Calls Linux lseek(unsigned int fd, off_t offset, unsigned int whence).   \
   * See x86_64_8 in the source table. */                                      \
  X(8, lseek, 3, NORMAL, sigma_sys_result_t, IPTR, (u32, fd),                  \
    (syscall_arg_t, offset), (u32, whence))                                    \
  /** Calls Linux mmap(?, ?, ?, ?, ?, ?). See x86_64_9 in the source table. */ \
  X(9, mmap, 6, NORMAL, sigma_mmap_result_t, POINTER, (void *, address),       \
    (usize, size), (sigma_mmap_protection_t, protection),                      \
    (sigma_mmap_flags_t, flags), (i32, fd), (usize, offset))                   \
  /** Calls Linux mprotect(unsigned long start, size_t len, unsigned long      \
   * prot). See x86_64_10 in the source table. */                              \
  X(10, mprotect, 3, NORMAL, sigma_sys_result_t, IPTR, (usize, start),         \
    (usize, len), (usize, prot))                                               \
  /** Calls Linux munmap(unsigned long addr, size_t len). See x86_64_11 in the \
   * source table. */                                                          \
  X(11, munmap, 2, NORMAL, sigma_munmap_result_t, STATUS, (void *, address),   \
    (usize, size))                                                             \
  /** Calls Linux brk(unsigned long brk). See x86_64_12 in the source table.   \
   */                                                                          \
  X(12, brk, 1, NORMAL, sigma_sys_result_t, IPTR, (usize, brk))                \
  /** Calls Linux rt_sigaction(int, const struct sigaction *, struct sigaction \
   * *, size_t). See x86_64_13 in the source table. */                         \
  X(13, rt_sigaction, 4, NORMAL, sigma_sys_result_t, IPTR, (i32, arg0),        \
    (const void *, sigaction), (void *, sigaction_2), (usize, arg3))           \
  /** Calls Linux rt_sigprocmask(int how, sigset_t *set, sigset_t *oset,       \
   * size_t sigsetsize). See x86_64_14 in the source table. */                 \
  X(14, rt_sigprocmask, 4, NORMAL, sigma_sys_result_t, IPTR, (i32, how),       \
    (void *, set), (void *, oset), (usize, sigsetsize))                        \
  /** Calls Linux rt_sigreturn(?, ?, ?, ?, ?, ?). See x86_64_15 in the source  \
   * table. */                                                                 \
  X(15, rt_sigreturn, 6, NORMAL, sigma_sys_result_t, IPTR,                     \
    (syscall_arg_t, arg0), (syscall_arg_t, arg1), (syscall_arg_t, arg2),       \
    (syscall_arg_t, arg3), (syscall_arg_t, arg4), (syscall_arg_t, arg5))       \
  /** Calls Linux ioctl(unsigned int fd, unsigned int cmd, unsigned long arg). \
   * See x86_64_16 in the source table. */                                     \
  X(16, ioctl, 3, NORMAL, sigma_sys_result_t, IPTR, (u32, fd), (u32, cmd),     \
    (usize, arg))                                                              \
  /** Calls Linux pread64(unsigned int fd, char *buf, size_t count, loff_t     \
   * pos). See x86_64_17 in the source table. */                               \
  X(17, pread64, 4, NORMAL, sigma_sys_result_t, IPTR, (u32, fd),               \
    (void *, buf), (usize, count), (syscall_arg_t, pos))                       \
  /** Calls Linux pwrite64(unsigned int fd, const char *buf, size_t count,     \
   * loff_t pos). See x86_64_18 in the source table. */                        \
  X(18, pwrite64, 4, NORMAL, sigma_sys_result_t, IPTR, (u32, fd),              \
    (const void *, buf), (usize, count), (syscall_arg_t, pos))                 \
  /** Calls Linux readv(unsigned long fd, const struct iovec *vec, unsigned    \
   * long vlen). See x86_64_19 in the source table. */                         \
  X(19, readv, 3, NORMAL, sigma_sys_result_t, IPTR, (usize, fd),               \
    (const void *, vec), (usize, vlen))                                        \
  /** Calls Linux writev(unsigned long fd, const struct iovec *vec, unsigned   \
   * long vlen). See x86_64_20 in the source table. */                         \
  X(20, writev, 3, NORMAL, sigma_sys_result_t, IPTR, (usize, fd),              \
    (const void *, vec), (usize, vlen))                                        \
  /** Calls Linux access(const char *filename, int mode). See x86_64_21 in the \
   * source table. */                                                          \
  X(21, access, 2, NORMAL, sigma_sys_result_t, IPTR, (const void *, filename), \
    (i32, mode))                                                               \
  /** Calls Linux pipe(int *fildes). See x86_64_22 in the source table. */     \
  X(22, pipe, 1, NORMAL, sigma_sys_result_t, IPTR, (void *, fildes))           \
  /** Calls Linux select(int n, fd_set *inp, fd_set *outp, fd_set *exp, struct \
   * __kernel_old_timeval *tvp). See x86_64_23 in the source table. */         \
  X(23, select, 5, NORMAL, sigma_sys_result_t, IPTR, (i32, n), (void *, inp),  \
    (void *, outp), (void *, exp), (void *, tvp))                              \
  /** Calls Linux sched_yield(void). See x86_64_24 in the source table. */     \
  X(24, sched_yield, 0, NORMAL, sigma_sys_result_t, IPTR)                      \
  /** Calls Linux mremap(unsigned long addr, unsigned long old_len, unsigned   \
   * long new_len, unsigned long flags, unsigned long new_addr). See x86_64_25 \
   * in the source table. */                                                   \
  X(25, mremap, 5, NORMAL, sigma_sys_result_t, IPTR, (usize, addr),            \
    (usize, old_len), (usize, new_len), (usize, flags), (usize, new_addr))     \
  /** Calls Linux msync(unsigned long start, size_t len, int flags). See       \
   * x86_64_26 in the source table. */                                         \
  X(26, msync, 3, NORMAL, sigma_sys_result_t, IPTR, (usize, start),            \
    (usize, len), (i32, flags))                                                \
  /** Calls Linux mincore(unsigned long start, size_t len, unsigned char *     \
   * vec). See x86_64_27 in the source table. */                               \
  X(27, mincore, 3, NORMAL, sigma_sys_result_t, IPTR, (usize, start),          \
    (usize, len), (void *, vec))                                               \
  /** Calls Linux madvise(unsigned long start, size_t len, int behavior). See  \
   * x86_64_28 in the source table. */                                         \
  X(28, madvise, 3, NORMAL, sigma_sys_result_t, IPTR, (usize, start),          \
    (usize, len), (i32, behavior))                                             \
  /** Calls Linux shmget(key_t key, size_t size, int flag). See x86_64_29 in   \
   * the source table. */                                                      \
  X(29, shmget, 3, NORMAL, sigma_sys_result_t, IPTR, (syscall_arg_t, key),     \
    (usize, size), (i32, flag))                                                \
  /** Calls Linux shmat(int shmid, char *shmaddr, int shmflg). See x86_64_30   \
   * in the source table. */                                                   \
  X(30, shmat, 3, NORMAL, sigma_sys_result_t, IPTR, (i32, shmid),              \
    (void *, shmaddr), (i32, shmflg))                                          \
  /** Calls Linux shmctl(int shmid, int cmd, struct shmid_ds *buf). See        \
   * x86_64_31 in the source table. */                                         \
  X(31, shmctl, 3, NORMAL, sigma_sys_result_t, IPTR, (i32, shmid), (i32, cmd), \
    (void *, buf))                                                             \
  /** Calls Linux dup(unsigned int fildes). See x86_64_32 in the source table. \
   */                                                                          \
  X(32, dup, 1, NORMAL, sigma_sys_result_t, IPTR, (u32, fildes))               \
  /** Calls Linux dup2(unsigned int oldfd, unsigned int newfd). See x86_64_33  \
   * in the source table. */                                                   \
  X(33, dup2, 2, NORMAL, sigma_sys_result_t, IPTR, (u32, oldfd), (u32, newfd)) \
  /** Calls Linux pause(void). See x86_64_34 in the source table. */           \
  X(34, pause, 0, NORMAL, sigma_sys_result_t, IPTR)                            \
  /** Calls Linux nanosleep(struct __kernel_timespec *rqtp, struct             \
   * __kernel_timespec *rmtp). See x86_64_35 in the source table. */           \
  X(35, nanosleep, 2, NORMAL, sigma_sys_result_t, IPTR, (void *, rqtp),        \
    (void *, rmtp))                                                            \
  /** Calls Linux getitimer(int which, struct __kernel_old_itimerval *value).  \
   * See x86_64_36 in the source table. */                                     \
  X(36, getitimer, 2, NORMAL, sigma_sys_result_t, IPTR, (i32, which),          \
    (void *, value))                                                           \
  /** Calls Linux alarm(unsigned int seconds). See x86_64_37 in the source     \
   * table. */                                                                 \
  X(37, alarm, 1, NORMAL, sigma_sys_result_t, IPTR, (u32, seconds))            \
  /** Calls Linux setitimer(int which, struct __kernel_old_itimerval *value,   \
   * struct __kernel_old_itimerval *ovalue). See x86_64_38 in the source       \
   * table. */                                                                 \
  X(38, setitimer, 3, NORMAL, sigma_sys_result_t, IPTR, (i32, which),          \
    (void *, value), (void *, ovalue))                                         \
  /** Calls Linux getpid(void). See x86_64_39 in the source table. */          \
  X(39, getpid, 0, NORMAL, sigma_sys_result_t, IPTR)                           \
  /** Calls Linux sendfile(int out_fd, int in_fd, off_t *offset, size_t        \
   * count). See x86_64_40 in the source table. */                             \
  X(40, sendfile, 4, NORMAL, sigma_sys_result_t, IPTR, (i32, out_fd),          \
    (i32, in_fd), (void *, offset), (usize, count))                            \
  /** Calls Linux socket(int, int, int). See x86_64_41 in the source table. */ \
  X(41, socket, 3, NORMAL, sigma_sys_result_t, IPTR, (i32, arg0), (i32, arg1), \
    (i32, arg2))                                                               \
  /** Calls Linux connect(int, struct sockaddr *, int). See x86_64_42 in the   \
   * source table. */                                                          \
  X(42, connect, 3, NORMAL, sigma_sys_result_t, IPTR, (i32, arg0),             \
    (void *, sockaddr), (i32, arg2))                                           \
  /** Calls Linux accept(int, struct sockaddr *, int *). See x86_64_43 in the  \
   * source table. */                                                          \
  X(43, accept, 3, NORMAL, sigma_sys_result_t, IPTR, (i32, arg0),              \
    (void *, sockaddr), (void *, arg2))                                        \
  /** Calls Linux sendto(int, void *, size_t, unsigned, struct sockaddr *,     \
   * int). See x86_64_44 in the source table. */                               \
  X(44, sendto, 6, NORMAL, sigma_sys_result_t, IPTR, (i32, arg0),              \
    (void *, arg1), (usize, arg2), (syscall_arg_t, arg3), (void *, sockaddr),  \
    (i32, arg5))                                                               \
  /** Calls Linux recvfrom(int, void *, size_t, unsigned, struct sockaddr *,   \
   * int *). See x86_64_45 in the source table. */                             \
  X(45, recvfrom, 6, NORMAL, sigma_sys_result_t, IPTR, (i32, arg0),            \
    (void *, arg1), (usize, arg2), (syscall_arg_t, arg3), (void *, sockaddr),  \
    (void *, arg5))                                                            \
  /** Calls Linux sendmsg(int fd, struct user_msghdr *msg, unsigned flags).    \
   * See x86_64_46 in the source table. */                                     \
  X(46, sendmsg, 3, NORMAL, sigma_sys_result_t, IPTR, (i32, fd),               \
    (void *, msg), (syscall_arg_t, flags))                                     \
  /** Calls Linux recvmsg(int fd, struct user_msghdr *msg, unsigned flags).    \
   * See x86_64_47 in the source table. */                                     \
  X(47, recvmsg, 3, NORMAL, sigma_sys_result_t, IPTR, (i32, fd),               \
    (void *, msg), (syscall_arg_t, flags))                                     \
  /** Calls Linux shutdown(int, int). See x86_64_48 in the source table. */    \
  X(48, shutdown, 2, NORMAL, sigma_sys_result_t, IPTR, (i32, arg0),            \
    (i32, arg1))                                                               \
  /** Calls Linux bind(int, struct sockaddr *, int). See x86_64_49 in the      \
   * source table. */                                                          \
  X(49, bind, 3, NORMAL, sigma_sys_result_t, IPTR, (i32, arg0),                \
    (void *, sockaddr), (i32, arg2))                                           \
  /** Calls Linux listen(int, int). See x86_64_50 in the source table. */      \
  X(50, listen, 2, NORMAL, sigma_sys_result_t, IPTR, (i32, arg0), (i32, arg1)) \
  /** Calls Linux getsockname(int, struct sockaddr *, int *). See x86_64_51 in \
   * the source table. */                                                      \
  X(51, getsockname, 3, NORMAL, sigma_sys_result_t, IPTR, (i32, arg0),         \
    (void *, sockaddr), (void *, arg2))                                        \
  /** Calls Linux getpeername(int, struct sockaddr *, int *). See x86_64_52 in \
   * the source table. */                                                      \
  X(52, getpeername, 3, NORMAL, sigma_sys_result_t, IPTR, (i32, arg0),         \
    (void *, sockaddr), (void *, arg2))                                        \
  /** Calls Linux socketpair(int, int, int, int *). See x86_64_53 in the       \
   * source table. */                                                          \
  X(53, socketpair, 4, NORMAL, sigma_sys_result_t, IPTR, (i32, arg0),          \
    (i32, arg1), (i32, arg2), (void *, arg3))                                  \
  /** Calls Linux setsockopt(int fd, int level, int optname, char *optval, int \
   * optlen). See x86_64_54 in the source table. */                            \
  X(54, setsockopt, 5, NORMAL, sigma_sys_result_t, IPTR, (i32, fd),            \
    (i32, level), (i32, optname), (void *, optval), (i32, optlen))             \
  /** Calls Linux getsockopt(int fd, int level, int optname, char *optval, int \
   * *optlen). See x86_64_55 in the source table. */                           \
  X(55, getsockopt, 5, NORMAL, sigma_sys_result_t, IPTR, (i32, fd),            \
    (i32, level), (i32, optname), (void *, optval), (void *, optlen))          \
  /** Calls Linux clone(unsigned long, unsigned long, int *, int *, unsigned   \
   * long). See x86_64_56 in the source table. */                              \
  X(56, clone, 5, NORMAL, sigma_sys_result_t, IPTR, (usize, arg0),             \
    (usize, arg1), (void *, arg2), (void *, arg3), (usize, arg4))              \
  /** Calls Linux fork(void). See x86_64_57 in the source table. */            \
  X(57, fork, 0, NORMAL, sigma_sys_result_t, IPTR)                             \
  /** Calls Linux vfork(void). See x86_64_58 in the source table. */           \
  X(58, vfork, 0, NORMAL, sigma_sys_result_t, IPTR)                            \
  /** Calls Linux execve(const char *filename, const char *const *argv, const  \
   * char *const *envp). See x86_64_59 in the source table. */                 \
  X(59, execve, 3, NORMAL, sigma_sys_result_t, IPTR, (const void *, filename), \
    (const void *, argv), (const void *, envp))                                \
  /** Calls Linux exit(int error_code). See x86_64_60 in the source table. */  \
  X(60, exit, 1, NORETURN, void, NONE, (i32, status))                          \
  /** Calls Linux wait4(pid_t pid, int *stat_addr, int options, struct rusage  \
   * *ru). See x86_64_61 in the source table. */                               \
  X(61, wait4, 4, NORMAL, sigma_sys_result_t, IPTR, (syscall_arg_t, pid),      \
    (void *, stat_addr), (i32, options), (void *, ru))                         \
  /** Calls Linux kill(pid_t pid, int sig). See x86_64_62 in the source table. \
   */                                                                          \
  X(62, kill, 2, NORMAL, sigma_sys_result_t, IPTR, (syscall_arg_t, pid),       \
    (i32, sig))                                                                \
  /** Calls Linux uname(struct old_utsname *). See x86_64_63 in the source     \
   * table. */                                                                 \
  X(63, uname, 1, NORMAL, sigma_sys_result_t, IPTR, (void *, old_utsname))     \
  /** Calls Linux semget(key_t key, int nsems, int semflg). See x86_64_64 in   \
   * the source table. */                                                      \
  X(64, semget, 3, NORMAL, sigma_sys_result_t, IPTR, (syscall_arg_t, key),     \
    (i32, nsems), (i32, semflg))                                               \
  /** Calls Linux semop(int semid, struct sembuf *sops, unsigned nsops). See   \
   * x86_64_65 in the source table. */                                         \
  X(65, semop, 3, NORMAL, sigma_sys_result_t, IPTR, (i32, semid),              \
    (void *, sops), (syscall_arg_t, nsops))                                    \
  /** Calls Linux semctl(int semid, int semnum, int cmd, unsigned long arg).   \
   * See x86_64_66 in the source table. */                                     \
  X(66, semctl, 4, NORMAL, sigma_sys_result_t, IPTR, (i32, semid),             \
    (i32, semnum), (i32, cmd), (usize, arg))                                   \
  /** Calls Linux shmdt(char *shmaddr). See x86_64_67 in the source table. */  \
  X(67, shmdt, 1, NORMAL, sigma_sys_result_t, IPTR, (void *, shmaddr))         \
  /** Calls Linux msgget(key_t key, int msgflg). See x86_64_68 in the source   \
   * table. */                                                                 \
  X(68, msgget, 2, NORMAL, sigma_sys_result_t, IPTR, (syscall_arg_t, key),     \
    (i32, msgflg))                                                             \
  /** Calls Linux msgsnd(int msqid, struct msgbuf *msgp, size_t msgsz, int     \
   * msgflg). See x86_64_69 in the source table. */                            \
  X(69, msgsnd, 4, NORMAL, sigma_sys_result_t, IPTR, (i32, msqid),             \
    (void *, msgp), (usize, msgsz), (i32, msgflg))                             \
  /** Calls Linux msgrcv(int msqid, struct msgbuf *msgp, size_t msgsz, long    \
   * msgtyp, int msgflg). See x86_64_70 in the source table. */                \
  X(70, msgrcv, 5, NORMAL, sigma_sys_result_t, IPTR, (i32, msqid),             \
    (void *, msgp), (usize, msgsz), (i64, msgtyp), (i32, msgflg))              \
  /** Calls Linux msgctl(int msqid, int cmd, struct msqid_ds *buf). See        \
   * x86_64_71 in the source table. */                                         \
  X(71, msgctl, 3, NORMAL, sigma_sys_result_t, IPTR, (i32, msqid), (i32, cmd), \
    (void *, buf))                                                             \
  /** Calls Linux fcntl(unsigned int fd, unsigned int cmd, unsigned long arg). \
   * See x86_64_72 in the source table. */                                     \
  X(72, fcntl, 3, NORMAL, sigma_sys_result_t, IPTR, (u32, fd), (u32, cmd),     \
    (usize, arg))                                                              \
  /** Calls Linux flock(unsigned int fd, unsigned int cmd). See x86_64_73 in   \
   * the source table. */                                                      \
  X(73, flock, 2, NORMAL, sigma_sys_result_t, IPTR, (u32, fd), (u32, cmd))     \
  /** Calls Linux fsync(unsigned int fd). See x86_64_74 in the source table.   \
   */                                                                          \
  X(74, fsync, 1, NORMAL, sigma_sys_result_t, IPTR, (u32, fd))                 \
  /** Calls Linux fdatasync(unsigned int fd). See x86_64_75 in the source      \
   * table. */                                                                 \
  X(75, fdatasync, 1, NORMAL, sigma_sys_result_t, IPTR, (u32, fd))             \
  /** Calls Linux truncate(const char *path, long length). See x86_64_76 in    \
   * the source table. */                                                      \
  X(76, truncate, 2, NORMAL, sigma_sys_result_t, IPTR, (const void *, path),   \
    (i64, length))                                                             \
  /** Calls Linux ftruncate(unsigned int fd, off_t length). See x86_64_77 in   \
   * the source table. */                                                      \
  X(77, ftruncate, 2, NORMAL, sigma_sys_result_t, IPTR, (u32, fd),             \
    (syscall_arg_t, length))                                                   \
  /** Calls Linux getdents(unsigned int fd, struct linux_dirent *dirent,       \
   * unsigned int count). See x86_64_78 in the source table. */                \
  X(78, getdents, 3, NORMAL, sigma_sys_result_t, IPTR, (u32, fd),              \
    (void *, dirent), (u32, count))                                            \
  /** Calls Linux getcwd(char *buf, unsigned long size). See x86_64_79 in the  \
   * source table. */                                                          \
  X(79, getcwd, 2, NORMAL, sigma_sys_result_t, IPTR, (void *, buf),            \
    (usize, size))                                                             \
  /** Calls Linux chdir(const char *filename). See x86_64_80 in the source     \
   * table. */                                                                 \
  X(80, chdir, 1, NORMAL, sigma_sys_result_t, IPTR, (const void *, filename))  \
  /** Calls Linux fchdir(unsigned int fd). See x86_64_81 in the source table.  \
   */                                                                          \
  X(81, fchdir, 1, NORMAL, sigma_sys_result_t, IPTR, (u32, fd))                \
  /** Calls Linux rename(const char *oldname, const char *newname). See        \
   * x86_64_82 in the source table. */                                         \
  X(82, rename, 2, NORMAL, sigma_sys_result_t, IPTR, (const void *, oldname),  \
    (const void *, newname))                                                   \
  /** Calls Linux mkdir(const char *pathname, umode_t mode). See x86_64_83 in  \
   * the source table. */                                                      \
  X(83, mkdir, 2, NORMAL, sigma_sys_result_t, IPTR, (const void *, pathname),  \
    (syscall_arg_t, mode))                                                     \
  /** Calls Linux rmdir(const char *pathname). See x86_64_84 in the source     \
   * table. */                                                                 \
  X(84, rmdir, 1, NORMAL, sigma_sys_result_t, IPTR, (const void *, pathname))  \
  /** Calls Linux creat(const char *pathname, umode_t mode). See x86_64_85 in  \
   * the source table. */                                                      \
  X(85, creat, 2, NORMAL, sigma_sys_result_t, IPTR, (const void *, pathname),  \
    (syscall_arg_t, mode))                                                     \
  /** Calls Linux link(const char *oldname, const char *newname). See          \
   * x86_64_86 in the source table. */                                         \
  X(86, link, 2, NORMAL, sigma_sys_result_t, IPTR, (const void *, oldname),    \
    (const void *, newname))                                                   \
  /** Calls Linux unlink(const char *pathname). See x86_64_87 in the source    \
   * table. */                                                                 \
  X(87, unlink, 1, NORMAL, sigma_sys_result_t, IPTR, (const void *, pathname)) \
  /** Calls Linux symlink(const char *old, const char *new). See x86_64_88 in  \
   * the source table. */                                                      \
  X(88, symlink, 2, NORMAL, sigma_sys_result_t, IPTR, (const void *, old),     \
    (const void *, new))                                                       \
  /** Calls Linux readlink(const char *path, char *buf, int bufsiz). See       \
   * x86_64_89 in the source table. */                                         \
  X(89, readlink, 3, NORMAL, sigma_sys_result_t, IPTR, (const void *, path),   \
    (void *, buf), (i32, bufsiz))                                              \
  /** Calls Linux chmod(const char *filename, umode_t mode). See x86_64_90 in  \
   * the source table. */                                                      \
  X(90, chmod, 2, NORMAL, sigma_sys_result_t, IPTR, (const void *, filename),  \
    (syscall_arg_t, mode))                                                     \
  /** Calls Linux fchmod(unsigned int fd, umode_t mode). See x86_64_91 in the  \
   * source table. */                                                          \
  X(91, fchmod, 2, NORMAL, sigma_sys_result_t, IPTR, (u32, fd),                \
    (syscall_arg_t, mode))                                                     \
  /** Calls Linux chown(const char *filename, uid_t user, gid_t group). See    \
   * x86_64_92 in the source table. */                                         \
  X(92, chown, 3, NORMAL, sigma_sys_result_t, IPTR, (const void *, filename),  \
    (syscall_arg_t, arg1), (syscall_arg_t, group))                             \
  /** Calls Linux fchown(unsigned int fd, uid_t user, gid_t group). See        \
   * x86_64_93 in the source table. */                                         \
  X(93, fchown, 3, NORMAL, sigma_sys_result_t, IPTR, (u32, fd),                \
    (syscall_arg_t, arg1), (syscall_arg_t, group))                             \
  /** Calls Linux lchown(const char *filename, uid_t user, gid_t group). See   \
   * x86_64_94 in the source table. */                                         \
  X(94, lchown, 3, NORMAL, sigma_sys_result_t, IPTR, (const void *, filename), \
    (syscall_arg_t, arg1), (syscall_arg_t, group))                             \
  /** Calls Linux umask(int mask). See x86_64_95 in the source table. */       \
  X(95, umask, 1, NORMAL, sigma_sys_result_t, IPTR, (i32, mask))               \
  /** Calls Linux gettimeofday(struct __kernel_old_timeval *tv, struct         \
   * timezone *tz). See x86_64_96 in the source table. */                      \
  X(96, gettimeofday, 2, NORMAL, sigma_sys_result_t, IPTR, (void *, tv),       \
    (void *, tz))                                                              \
  /** Calls Linux getrlimit(unsigned int resource, struct rlimit *rlim). See   \
   * x86_64_97 in the source table. */                                         \
  X(97, getrlimit, 2, NORMAL, sigma_sys_result_t, IPTR, (u32, resource),       \
    (void *, rlim))                                                            \
  /** Calls Linux getrusage(int who, struct rusage *ru). See x86_64_98 in the  \
   * source table. */                                                          \
  X(98, getrusage, 2, NORMAL, sigma_sys_result_t, IPTR, (i32, who),            \
    (void *, ru))                                                              \
  /** Calls Linux sysinfo(struct sysinfo *info). See x86_64_99 in the source   \
   * table. */                                                                 \
  X(99, sysinfo, 1, NORMAL, sigma_sys_result_t, IPTR, (void *, info))          \
  /** Calls Linux times(struct tms *tbuf). See x86_64_100 in the source table. \
   */                                                                          \
  X(100, times, 1, NORMAL, sigma_sys_result_t, IPTR, (void *, tbuf))           \
  /** Calls Linux ptrace(long request, long pid, unsigned long addr, unsigned  \
   * long data). See x86_64_101 in the source table. */                        \
  X(101, ptrace, 4, NORMAL, sigma_sys_result_t, IPTR, (i64, request),          \
    (i64, pid), (usize, addr), (usize, data))                                  \
  /** Calls Linux getuid(void). See x86_64_102 in the source table. */         \
  X(102, getuid, 0, NORMAL, sigma_sys_result_t, IPTR)                          \
  /** Calls Linux syslog(int type, char *buf, int len). See x86_64_103 in the  \
   * source table. */                                                          \
  X(103, syslog, 3, NORMAL, sigma_sys_result_t, IPTR, (i32, type),             \
    (void *, buf), (i32, len))                                                 \
  /** Calls Linux getgid(void). See x86_64_104 in the source table. */         \
  X(104, getgid, 0, NORMAL, sigma_sys_result_t, IPTR)                          \
  /** Calls Linux setuid(uid_t uid). See x86_64_105 in the source table. */    \
  X(105, setuid, 1, NORMAL, sigma_sys_result_t, IPTR, (syscall_arg_t, uid))    \
  /** Calls Linux setgid(gid_t gid). See x86_64_106 in the source table. */    \
  X(106, setgid, 1, NORMAL, sigma_sys_result_t, IPTR, (syscall_arg_t, gid))    \
  /** Calls Linux geteuid(void). See x86_64_107 in the source table. */        \
  X(107, geteuid, 0, NORMAL, sigma_sys_result_t, IPTR)                         \
  /** Calls Linux getegid(void). See x86_64_108 in the source table. */        \
  X(108, getegid, 0, NORMAL, sigma_sys_result_t, IPTR)                         \
  /** Calls Linux setpgid(pid_t pid, pid_t pgid). See x86_64_109 in the source \
   * table. */                                                                 \
  X(109, setpgid, 2, NORMAL, sigma_sys_result_t, IPTR, (syscall_arg_t, pid),   \
    (syscall_arg_t, pgid))                                                     \
  /** Calls Linux getppid(void). See x86_64_110 in the source table. */        \
  X(110, getppid, 0, NORMAL, sigma_sys_result_t, IPTR)                         \
  /** Calls Linux getpgrp(void). See x86_64_111 in the source table. */        \
  X(111, getpgrp, 0, NORMAL, sigma_sys_result_t, IPTR)                         \
  /** Calls Linux setsid(void). See x86_64_112 in the source table. */         \
  X(112, setsid, 0, NORMAL, sigma_sys_result_t, IPTR)                          \
  /** Calls Linux setreuid(uid_t ruid, uid_t euid). See x86_64_113 in the      \
   * source table. */                                                          \
  X(113, setreuid, 2, NORMAL, sigma_sys_result_t, IPTR, (syscall_arg_t, ruid), \
    (syscall_arg_t, euid))                                                     \
  /** Calls Linux setregid(gid_t rgid, gid_t egid). See x86_64_114 in the      \
   * source table. */                                                          \
  X(114, setregid, 2, NORMAL, sigma_sys_result_t, IPTR, (syscall_arg_t, rgid), \
    (syscall_arg_t, egid))                                                     \
  /** Calls Linux getgroups(int gidsetsize, gid_t *grouplist). See x86_64_115  \
   * in the source table. */                                                   \
  X(115, getgroups, 2, NORMAL, sigma_sys_result_t, IPTR, (i32, gidsetsize),    \
    (void *, grouplist))                                                       \
  /** Calls Linux setgroups(int gidsetsize, gid_t *grouplist). See x86_64_116  \
   * in the source table. */                                                   \
  X(116, setgroups, 2, NORMAL, sigma_sys_result_t, IPTR, (i32, gidsetsize),    \
    (void *, grouplist))                                                       \
  /** Calls Linux setresuid(uid_t ruid, uid_t euid, uid_t suid). See           \
   * x86_64_117 in the source table. */                                        \
  X(117, setresuid, 3, NORMAL, sigma_sys_result_t, IPTR,                       \
    (syscall_arg_t, ruid), (syscall_arg_t, euid), (syscall_arg_t, suid))       \
  /** Calls Linux getresuid(uid_t *ruid, uid_t *euid, uid_t *suid). See        \
   * x86_64_118 in the source table. */                                        \
  X(118, getresuid, 3, NORMAL, sigma_sys_result_t, IPTR, (void *, ruid),       \
    (void *, euid), (void *, suid))                                            \
  /** Calls Linux setresgid(gid_t rgid, gid_t egid, gid_t sgid). See           \
   * x86_64_119 in the source table. */                                        \
  X(119, setresgid, 3, NORMAL, sigma_sys_result_t, IPTR,                       \
    (syscall_arg_t, rgid), (syscall_arg_t, egid), (syscall_arg_t, sgid))       \
  /** Calls Linux getresgid(gid_t *rgid, gid_t *egid, gid_t *sgid). See        \
   * x86_64_120 in the source table. */                                        \
  X(120, getresgid, 3, NORMAL, sigma_sys_result_t, IPTR, (void *, rgid),       \
    (void *, egid), (void *, sgid))                                            \
  /** Calls Linux getpgid(pid_t pid). See x86_64_121 in the source table. */   \
  X(121, getpgid, 1, NORMAL, sigma_sys_result_t, IPTR, (syscall_arg_t, pid))   \
  /** Calls Linux setfsuid(uid_t uid). See x86_64_122 in the source table. */  \
  X(122, setfsuid, 1, NORMAL, sigma_sys_result_t, IPTR, (syscall_arg_t, uid))  \
  /** Calls Linux setfsgid(gid_t gid). See x86_64_123 in the source table. */  \
  X(123, setfsgid, 1, NORMAL, sigma_sys_result_t, IPTR, (syscall_arg_t, gid))  \
  /** Calls Linux getsid(pid_t pid). See x86_64_124 in the source table. */    \
  X(124, getsid, 1, NORMAL, sigma_sys_result_t, IPTR, (syscall_arg_t, pid))    \
  /** Calls Linux capget(cap_user_header_t header, cap_user_data_t dataptr).   \
   * See x86_64_125 in the source table. */                                    \
  X(125, capget, 2, NORMAL, sigma_sys_result_t, IPTR, (syscall_arg_t, header), \
    (syscall_arg_t, dataptr))                                                  \
  /** Calls Linux capset(cap_user_header_t header, const cap_user_data_t       \
   * data). See x86_64_126 in the source table. */                             \
  X(126, capset, 2, NORMAL, sigma_sys_result_t, IPTR, (syscall_arg_t, header), \
    (syscall_arg_t, data))                                                     \
  /** Calls Linux rt_sigpending(sigset_t *set, size_t sigsetsize). See         \
   * x86_64_127 in the source table. */                                        \
  X(127, rt_sigpending, 2, NORMAL, sigma_sys_result_t, IPTR, (void *, set),    \
    (usize, sigsetsize))                                                       \
  /** Calls Linux rt_sigtimedwait(const sigset_t *uthese, siginfo_t *uinfo,    \
   * const struct __kernel_timespec *uts, size_t sigsetsize). See x86_64_128   \
   * in the source table. */                                                   \
  X(128, rt_sigtimedwait, 4, NORMAL, sigma_sys_result_t, IPTR,                 \
    (const void *, uthese), (void *, uinfo), (const void *, uts),              \
    (usize, sigsetsize))                                                       \
  /** Calls Linux rt_sigqueueinfo(pid_t pid, int sig, siginfo_t *uinfo). See   \
   * x86_64_129 in the source table. */                                        \
  X(129, rt_sigqueueinfo, 3, NORMAL, sigma_sys_result_t, IPTR,                 \
    (syscall_arg_t, pid), (i32, sig), (void *, uinfo))                         \
  /** Calls Linux rt_sigsuspend(sigset_t *unewset, size_t sigsetsize). See     \
   * x86_64_130 in the source table. */                                        \
  X(130, rt_sigsuspend, 2, NORMAL, sigma_sys_result_t, IPTR,                   \
    (void *, unewset), (usize, sigsetsize))                                    \
  /** Calls Linux sigaltstack(const struct sigaltstack *uss, struct            \
   * sigaltstack *uoss). See x86_64_131 in the source table. */                \
  X(131, sigaltstack, 2, NORMAL, sigma_sys_result_t, IPTR,                     \
    (const void *, uss), (void *, uoss))                                       \
  /** Calls Linux utime(char *filename, struct utimbuf *times). See x86_64_132 \
   * in the source table. */                                                   \
  X(132, utime, 2, NORMAL, sigma_sys_result_t, IPTR, (void *, filename),       \
    (void *, times))                                                           \
  /** Calls Linux mknod(const char *filename, umode_t mode, unsigned dev). See \
   * x86_64_133 in the source table. */                                        \
  X(133, mknod, 3, NORMAL, sigma_sys_result_t, IPTR, (const void *, filename), \
    (syscall_arg_t, mode), (syscall_arg_t, dev))                               \
  /** Calls Linux uselib(const char *library). See x86_64_134 in the source    \
   * table. */                                                                 \
  X(134, uselib, 1, NORMAL, sigma_sys_result_t, IPTR, (const void *, library)) \
  /** Calls Linux personality(unsigned int personality). See x86_64_135 in the \
   * source table. */                                                          \
  X(135, personality, 1, NORMAL, sigma_sys_result_t, IPTR, (u32, personality)) \
  /** Calls Linux ustat(unsigned dev, struct ustat *ubuf). See x86_64_136 in   \
   * the source table. */                                                      \
  X(136, ustat, 2, NORMAL, sigma_sys_result_t, IPTR, (syscall_arg_t, dev),     \
    (void *, ubuf))                                                            \
  /** Calls Linux statfs(const char * path, struct statfs *buf). See           \
   * x86_64_137 in the source table. */                                        \
  X(137, statfs, 2, NORMAL, sigma_sys_result_t, IPTR, (const void *, path),    \
    (void *, buf))                                                             \
  /** Calls Linux fstatfs(unsigned int fd, struct statfs *buf). See x86_64_138 \
   * in the source table. */                                                   \
  X(138, fstatfs, 2, NORMAL, sigma_sys_result_t, IPTR, (u32, fd),              \
    (void *, buf))                                                             \
  /** Calls Linux sysfs(int option, unsigned long arg1, unsigned long arg2).   \
   * See x86_64_139 in the source table. */                                    \
  X(139, sysfs, 3, NORMAL, sigma_sys_result_t, IPTR, (i32, option),            \
    (usize, arg1), (usize, arg2))                                              \
  /** Calls Linux getpriority(int which, int who). See x86_64_140 in the       \
   * source table. */                                                          \
  X(140, getpriority, 2, NORMAL, sigma_sys_result_t, IPTR, (i32, which),       \
    (i32, who))                                                                \
  /** Calls Linux setpriority(int which, int who, int niceval). See x86_64_141 \
   * in the source table. */                                                   \
  X(141, setpriority, 3, NORMAL, sigma_sys_result_t, IPTR, (i32, which),       \
    (i32, who), (i32, niceval))                                                \
  /** Calls Linux sched_setparam(pid_t pid, struct sched_param *param). See    \
   * x86_64_142 in the source table. */                                        \
  X(142, sched_setparam, 2, NORMAL, sigma_sys_result_t, IPTR,                  \
    (syscall_arg_t, pid), (void *, param))                                     \
  /** Calls Linux sched_getparam(pid_t pid, struct sched_param *param). See    \
   * x86_64_143 in the source table. */                                        \
  X(143, sched_getparam, 2, NORMAL, sigma_sys_result_t, IPTR,                  \
    (syscall_arg_t, pid), (void *, param))                                     \
  /** Calls Linux sched_setscheduler(pid_t pid, int policy, struct sched_param \
   * *param). See x86_64_144 in the source table. */                           \
  X(144, sched_setscheduler, 3, NORMAL, sigma_sys_result_t, IPTR,              \
    (syscall_arg_t, pid), (i32, policy), (void *, param))                      \
  /** Calls Linux sched_getscheduler(pid_t pid). See x86_64_145 in the source  \
   * table. */                                                                 \
  X(145, sched_getscheduler, 1, NORMAL, sigma_sys_result_t, IPTR,              \
    (syscall_arg_t, pid))                                                      \
  /** Calls Linux sched_get_priority_max(int policy). See x86_64_146 in the    \
   * source table. */                                                          \
  X(146, sched_get_priority_max, 1, NORMAL, sigma_sys_result_t, IPTR,          \
    (i32, policy))                                                             \
  /** Calls Linux sched_get_priority_min(int policy). See x86_64_147 in the    \
   * source table. */                                                          \
  X(147, sched_get_priority_min, 1, NORMAL, sigma_sys_result_t, IPTR,          \
    (i32, policy))                                                             \
  /** Calls Linux sched_rr_get_interval(pid_t pid, struct __kernel_timespec    \
   * *interval). See x86_64_148 in the source table. */                        \
  X(148, sched_rr_get_interval, 2, NORMAL, sigma_sys_result_t, IPTR,           \
    (syscall_arg_t, pid), (void *, interval))                                  \
  /** Calls Linux mlock(unsigned long start, size_t len). See x86_64_149 in    \
   * the source table. */                                                      \
  X(149, mlock, 2, NORMAL, sigma_sys_result_t, IPTR, (usize, start),           \
    (usize, len))                                                              \
  /** Calls Linux munlock(unsigned long start, size_t len). See x86_64_150 in  \
   * the source table. */                                                      \
  X(150, munlock, 2, NORMAL, sigma_sys_result_t, IPTR, (usize, start),         \
    (usize, len))                                                              \
  /** Calls Linux mlockall(int flags). See x86_64_151 in the source table. */  \
  X(151, mlockall, 1, NORMAL, sigma_sys_result_t, IPTR, (i32, flags))          \
  /** Calls Linux munlockall(void). See x86_64_152 in the source table. */     \
  X(152, munlockall, 0, NORMAL, sigma_sys_result_t, IPTR)                      \
  /** Calls Linux vhangup(void). See x86_64_153 in the source table. */        \
  X(153, vhangup, 0, NORMAL, sigma_sys_result_t, IPTR)                         \
  /** Calls Linux modify_ldt(?, ?, ?, ?, ?, ?). See x86_64_154 in the source   \
   * table. */                                                                 \
  X(154, modify_ldt, 6, NORMAL, sigma_sys_result_t, IPTR,                      \
    (syscall_arg_t, arg0), (syscall_arg_t, arg1), (syscall_arg_t, arg2),       \
    (syscall_arg_t, arg3), (syscall_arg_t, arg4), (syscall_arg_t, arg5))       \
  /** Calls Linux pivot_root(const char *new_root, const char *put_old). See   \
   * x86_64_155 in the source table. */                                        \
  X(155, pivot_root, 2, NORMAL, sigma_sys_result_t, IPTR,                      \
    (const void *, new_root), (const void *, put_old))                         \
  /** Calls Linux _sysctl(?, ?, ?, ?, ?, ?). See x86_64_156 in the source      \
   * table. */                                                                 \
  X(156, _sysctl, 6, NORMAL, sigma_sys_result_t, IPTR, (syscall_arg_t, arg0),  \
    (syscall_arg_t, arg1), (syscall_arg_t, arg2), (syscall_arg_t, arg3),       \
    (syscall_arg_t, arg4), (syscall_arg_t, arg5))                              \
  /** Calls Linux prctl(int option, unsigned long arg2, unsigned long arg3,    \
   * unsigned long arg4, unsigned long arg5). See x86_64_157 in the source     \
   * table. */                                                                 \
  X(157, prctl, 5, NORMAL, sigma_sys_result_t, IPTR, (i32, option),            \
    (usize, arg2), (usize, arg3), (usize, arg4), (usize, arg5))                \
  /** Calls Linux arch_prctl(?, ?, ?, ?, ?, ?). See x86_64_158 in the source   \
   * table. */                                                                 \
  X(158, arch_prctl, 2, NORMAL, sigma_status_result_t, STATUS,                 \
    (sigma_arch_operation_t, operation), (uptr, address))                      \
  /** Calls Linux adjtimex(struct __kernel_timex *txc_p). See x86_64_159 in    \
   * the source table. */                                                      \
  X(159, adjtimex, 1, NORMAL, sigma_sys_result_t, IPTR, (void *, txc_p))       \
  /** Calls Linux setrlimit(unsigned int resource, struct rlimit *rlim). See   \
   * x86_64_160 in the source table. */                                        \
  X(160, setrlimit, 2, NORMAL, sigma_sys_result_t, IPTR, (u32, resource),      \
    (void *, rlim))                                                            \
  /** Calls Linux chroot(const char *filename). See x86_64_161 in the source   \
   * table. */                                                                 \
  X(161, chroot, 1, NORMAL, sigma_sys_result_t, IPTR,                          \
    (const void *, filename))                                                  \
  /** Calls Linux sync(void). See x86_64_162 in the source table. */           \
  X(162, sync, 0, NORMAL, sigma_sys_result_t, IPTR)                            \
  /** Calls Linux acct(const char *name). See x86_64_163 in the source table.  \
   */                                                                          \
  X(163, acct, 1, NORMAL, sigma_sys_result_t, IPTR, (const void *, name))      \
  /** Calls Linux settimeofday(struct __kernel_old_timeval *tv, struct         \
   * timezone *tz). See x86_64_164 in the source table. */                     \
  X(164, settimeofday, 2, NORMAL, sigma_sys_result_t, IPTR, (void *, tv),      \
    (void *, tz))                                                              \
  /** Calls Linux mount(char *dev_name, char *dir_name, char *type, unsigned   \
   * long flags, void *data). See x86_64_165 in the source table. */           \
  X(165, mount, 5, NORMAL, sigma_sys_result_t, IPTR, (void *, dev_name),       \
    (void *, dir_name), (void *, type), (usize, flags), (void *, data))        \
  /** Calls Linux umount2(?, ?, ?, ?, ?, ?). See x86_64_166 in the source      \
   * table. */                                                                 \
  X(166, umount2, 6, NORMAL, sigma_sys_result_t, IPTR, (syscall_arg_t, arg0),  \
    (syscall_arg_t, arg1), (syscall_arg_t, arg2), (syscall_arg_t, arg3),       \
    (syscall_arg_t, arg4), (syscall_arg_t, arg5))                              \
  /** Calls Linux swapon(const char *specialfile, int swap_flags). See         \
   * x86_64_167 in the source table. */                                        \
  X(167, swapon, 2, NORMAL, sigma_sys_result_t, IPTR,                          \
    (const void *, specialfile), (i32, swap_flags))                            \
  /** Calls Linux swapoff(const char *specialfile). See x86_64_168 in the      \
   * source table. */                                                          \
  X(168, swapoff, 1, NORMAL, sigma_sys_result_t, IPTR,                         \
    (const void *, specialfile))                                               \
  /** Calls Linux reboot(int magic1, int magic2, unsigned int cmd, void *arg). \
   * See x86_64_169 in the source table. */                                    \
  X(169, reboot, 4, NORMAL, sigma_sys_result_t, IPTR, (i32, magic1),           \
    (i32, magic2), (u32, cmd), (void *, arg))                                  \
  /** Calls Linux sethostname(char *name, int len). See x86_64_170 in the      \
   * source table. */                                                          \
  X(170, sethostname, 2, NORMAL, sigma_sys_result_t, IPTR, (void *, name),     \
    (i32, len))                                                                \
  /** Calls Linux setdomainname(char *name, int len). See x86_64_171 in the    \
   * source table. */                                                          \
  X(171, setdomainname, 2, NORMAL, sigma_sys_result_t, IPTR, (void *, name),   \
    (i32, len))                                                                \
  /** Calls Linux iopl(?, ?, ?, ?, ?, ?). See x86_64_172 in the source table.  \
   */                                                                          \
  X(172, iopl, 6, NORMAL, sigma_sys_result_t, IPTR, (syscall_arg_t, arg0),     \
    (syscall_arg_t, arg1), (syscall_arg_t, arg2), (syscall_arg_t, arg3),       \
    (syscall_arg_t, arg4), (syscall_arg_t, arg5))                              \
  /** Calls Linux ioperm(unsigned long from, unsigned long num, int on). See   \
   * x86_64_173 in the source table. */                                        \
  X(173, ioperm, 3, NORMAL, sigma_sys_result_t, IPTR, (usize, from),           \
    (usize, num), (i32, on))                                                   \
  /** Calls Linux create_module(?, ?, ?, ?, ?, ?). See x86_64_174 in the       \
   * source table. */                                                          \
  X(174, create_module, 6, NORMAL, sigma_sys_result_t, IPTR,                   \
    (syscall_arg_t, arg0), (syscall_arg_t, arg1), (syscall_arg_t, arg2),       \
    (syscall_arg_t, arg3), (syscall_arg_t, arg4), (syscall_arg_t, arg5))       \
  /** Calls Linux init_module(void *umod, unsigned long len, const char        \
   * *uargs). See x86_64_175 in the source table. */                           \
  X(175, init_module, 3, NORMAL, sigma_sys_result_t, IPTR, (void *, umod),     \
    (usize, len), (const void *, uargs))                                       \
  /** Calls Linux delete_module(const char *name_user, unsigned int flags).    \
   * See x86_64_176 in the source table. */                                    \
  X(176, delete_module, 2, NORMAL, sigma_sys_result_t, IPTR,                   \
    (const void *, name_user), (u32, flags))                                   \
  /** Calls Linux get_kernel_syms(?, ?, ?, ?, ?, ?). See x86_64_177 in the     \
   * source table. */                                                          \
  X(177, get_kernel_syms, 6, NORMAL, sigma_sys_result_t, IPTR,                 \
    (syscall_arg_t, arg0), (syscall_arg_t, arg1), (syscall_arg_t, arg2),       \
    (syscall_arg_t, arg3), (syscall_arg_t, arg4), (syscall_arg_t, arg5))       \
  /** Calls Linux query_module(?, ?, ?, ?, ?, ?). See x86_64_178 in the source \
   * table. */                                                                 \
  X(178, query_module, 6, NORMAL, sigma_sys_result_t, IPTR,                    \
    (syscall_arg_t, arg0), (syscall_arg_t, arg1), (syscall_arg_t, arg2),       \
    (syscall_arg_t, arg3), (syscall_arg_t, arg4), (syscall_arg_t, arg5))       \
  /** Calls Linux quotactl(unsigned int cmd, const char *special, qid_t id,    \
   * void *addr). See x86_64_179 in the source table. */                       \
  X(179, quotactl, 4, NORMAL, sigma_sys_result_t, IPTR, (u32, cmd),            \
    (const void *, special), (syscall_arg_t, id), (void *, addr))              \
  /** Calls Linux nfsservctl(?, ?, ?, ?, ?, ?). See x86_64_180 in the source   \
   * table. */                                                                 \
  X(180, nfsservctl, 6, NORMAL, sigma_sys_result_t, IPTR,                      \
    (syscall_arg_t, arg0), (syscall_arg_t, arg1), (syscall_arg_t, arg2),       \
    (syscall_arg_t, arg3), (syscall_arg_t, arg4), (syscall_arg_t, arg5))       \
  /** Calls Linux getpmsg(?, ?, ?, ?, ?, ?). See x86_64_181 in the source      \
   * table. */                                                                 \
  X(181, getpmsg, 6, NORMAL, sigma_sys_result_t, IPTR, (syscall_arg_t, arg0),  \
    (syscall_arg_t, arg1), (syscall_arg_t, arg2), (syscall_arg_t, arg3),       \
    (syscall_arg_t, arg4), (syscall_arg_t, arg5))                              \
  /** Calls Linux putpmsg(?, ?, ?, ?, ?, ?). See x86_64_182 in the source      \
   * table. */                                                                 \
  X(182, putpmsg, 6, NORMAL, sigma_sys_result_t, IPTR, (syscall_arg_t, arg0),  \
    (syscall_arg_t, arg1), (syscall_arg_t, arg2), (syscall_arg_t, arg3),       \
    (syscall_arg_t, arg4), (syscall_arg_t, arg5))                              \
  /** Calls Linux afs_syscall(?, ?, ?, ?, ?, ?). See x86_64_183 in the source  \
   * table. */                                                                 \
  X(183, afs_syscall, 6, NORMAL, sigma_sys_result_t, IPTR,                     \
    (syscall_arg_t, arg0), (syscall_arg_t, arg1), (syscall_arg_t, arg2),       \
    (syscall_arg_t, arg3), (syscall_arg_t, arg4), (syscall_arg_t, arg5))       \
  /** Calls Linux tuxcall(?, ?, ?, ?, ?, ?). See x86_64_184 in the source      \
   * table. */                                                                 \
  X(184, tuxcall, 6, NORMAL, sigma_sys_result_t, IPTR, (syscall_arg_t, arg0),  \
    (syscall_arg_t, arg1), (syscall_arg_t, arg2), (syscall_arg_t, arg3),       \
    (syscall_arg_t, arg4), (syscall_arg_t, arg5))                              \
  /** Calls Linux security(?, ?, ?, ?, ?, ?). See x86_64_185 in the source     \
   * table. */                                                                 \
  X(185, security, 6, NORMAL, sigma_sys_result_t, IPTR, (syscall_arg_t, arg0), \
    (syscall_arg_t, arg1), (syscall_arg_t, arg2), (syscall_arg_t, arg3),       \
    (syscall_arg_t, arg4), (syscall_arg_t, arg5))                              \
  /** Calls Linux gettid(void). See x86_64_186 in the source table. */         \
  X(186, gettid, 0, NORMAL, sigma_sys_result_t, IPTR)                          \
  /** Calls Linux readahead(int fd, loff_t offset, size_t count). See          \
   * x86_64_187 in the source table. */                                        \
  X(187, readahead, 3, NORMAL, sigma_sys_result_t, IPTR, (i32, fd),            \
    (syscall_arg_t, offset), (usize, count))                                   \
  /** Calls Linux setxattr(const char *path, const char *name, const void      \
   * *value, size_t size, int flags). See x86_64_188 in the source table. */   \
  X(188, setxattr, 5, NORMAL, sigma_sys_result_t, IPTR, (const void *, path),  \
    (const void *, name), (const void *, value), (usize, size), (i32, flags))  \
  /** Calls Linux lsetxattr(const char *path, const char *name, const void     \
   * *value, size_t size, int flags). See x86_64_189 in the source table. */   \
  X(189, lsetxattr, 5, NORMAL, sigma_sys_result_t, IPTR, (const void *, path), \
    (const void *, name), (const void *, value), (usize, size), (i32, flags))  \
  /** Calls Linux fsetxattr(int fd, const char *name, const void *value,       \
   * size_t size, int flags). See x86_64_190 in the source table. */           \
  X(190, fsetxattr, 5, NORMAL, sigma_sys_result_t, IPTR, (i32, fd),            \
    (const void *, name), (const void *, value), (usize, size), (i32, flags))  \
  /** Calls Linux getxattr(const char *path, const char *name, void *value,    \
   * size_t size). See x86_64_191 in the source table. */                      \
  X(191, getxattr, 4, NORMAL, sigma_sys_result_t, IPTR, (const void *, path),  \
    (const void *, name), (void *, value), (usize, size))                      \
  /** Calls Linux lgetxattr(const char *path, const char *name, void *value,   \
   * size_t size). See x86_64_192 in the source table. */                      \
  X(192, lgetxattr, 4, NORMAL, sigma_sys_result_t, IPTR, (const void *, path), \
    (const void *, name), (void *, value), (usize, size))                      \
  /** Calls Linux fgetxattr(int fd, const char *name, void *value, size_t      \
   * size). See x86_64_193 in the source table. */                             \
  X(193, fgetxattr, 4, NORMAL, sigma_sys_result_t, IPTR, (i32, fd),            \
    (const void *, name), (void *, value), (usize, size))                      \
  /** Calls Linux listxattr(const char *path, char *list, size_t size). See    \
   * x86_64_194 in the source table. */                                        \
  X(194, listxattr, 3, NORMAL, sigma_sys_result_t, IPTR, (const void *, path), \
    (void *, list), (usize, size))                                             \
  /** Calls Linux llistxattr(const char *path, char *list, size_t size). See   \
   * x86_64_195 in the source table. */                                        \
  X(195, llistxattr, 3, NORMAL, sigma_sys_result_t, IPTR,                      \
    (const void *, path), (void *, list), (usize, size))                       \
  /** Calls Linux flistxattr(int fd, char *list, size_t size). See x86_64_196  \
   * in the source table. */                                                   \
  X(196, flistxattr, 3, NORMAL, sigma_sys_result_t, IPTR, (i32, fd),           \
    (void *, list), (usize, size))                                             \
  /** Calls Linux removexattr(const char *path, const char *name). See         \
   * x86_64_197 in the source table. */                                        \
  X(197, removexattr, 2, NORMAL, sigma_sys_result_t, IPTR,                     \
    (const void *, path), (const void *, name))                                \
  /** Calls Linux lremovexattr(const char *path, const char *name). See        \
   * x86_64_198 in the source table. */                                        \
  X(198, lremovexattr, 2, NORMAL, sigma_sys_result_t, IPTR,                    \
    (const void *, path), (const void *, name))                                \
  /** Calls Linux fremovexattr(int fd, const char *name). See x86_64_199 in    \
   * the source table. */                                                      \
  X(199, fremovexattr, 2, NORMAL, sigma_sys_result_t, IPTR, (i32, fd),         \
    (const void *, name))                                                      \
  /** Calls Linux tkill(pid_t pid, int sig). See x86_64_200 in the source      \
   * table. */                                                                 \
  X(200, tkill, 2, NORMAL, sigma_sys_result_t, IPTR, (syscall_arg_t, pid),     \
    (i32, sig))                                                                \
  /** Calls Linux time(__kernel_old_time_t *tloc). See x86_64_201 in the       \
   * source table. */                                                          \
  X(201, time, 1, NORMAL, sigma_sys_result_t, IPTR, (void *, tloc))            \
  /** Calls Linux futex(u32 *uaddr, int op, u32 val, const struct              \
   * __kernel_timespec *utime, u32 *uaddr2, u32 val3). See x86_64_202 in the   \
   * source table. */                                                          \
  X(202, futex, 6, NORMAL, sigma_sys_result_t, IPTR, (void *, uaddr),          \
    (i32, op), (syscall_arg_t, val), (const void *, utime), (void *, uaddr2),  \
    (syscall_arg_t, val3))                                                     \
  /** Calls Linux sched_setaffinity(pid_t pid, unsigned int len, unsigned long \
   * *user_mask_ptr). See x86_64_203 in the source table. */                   \
  X(203, sched_setaffinity, 3, NORMAL, sigma_sys_result_t, IPTR,               \
    (syscall_arg_t, pid), (u32, len), (void *, user_mask_ptr))                 \
  /** Calls Linux sched_getaffinity(pid_t pid, unsigned int len, unsigned long \
   * *user_mask_ptr). See x86_64_204 in the source table. */                   \
  X(204, sched_getaffinity, 3, NORMAL, sigma_sys_result_t, IPTR,               \
    (syscall_arg_t, pid), (u32, len), (void *, user_mask_ptr))                 \
  /** Calls Linux set_thread_area(?, ?, ?, ?, ?, ?). See x86_64_205 in the     \
   * source table. */                                                          \
  X(205, set_thread_area, 6, NORMAL, sigma_sys_result_t, IPTR,                 \
    (syscall_arg_t, arg0), (syscall_arg_t, arg1), (syscall_arg_t, arg2),       \
    (syscall_arg_t, arg3), (syscall_arg_t, arg4), (syscall_arg_t, arg5))       \
  /** Calls Linux io_setup(unsigned nr_reqs, aio_context_t *ctx). See          \
   * x86_64_206 in the source table. */                                        \
  X(206, io_setup, 2, NORMAL, sigma_sys_result_t, IPTR,                        \
    (syscall_arg_t, nr_reqs), (void *, ctx))                                   \
  /** Calls Linux io_destroy(aio_context_t ctx). See x86_64_207 in the source  \
   * table. */                                                                 \
  X(207, io_destroy, 1, NORMAL, sigma_sys_result_t, IPTR,                      \
    (syscall_arg_t, ctx))                                                      \
  /** Calls Linux io_getevents(aio_context_t ctx_id, long min_nr, long nr,     \
   * struct io_event *events, struct __kernel_timespec *timeout). See          \
   * x86_64_208 in the source table. */                                        \
  X(208, io_getevents, 5, NORMAL, sigma_sys_result_t, IPTR,                    \
    (syscall_arg_t, ctx_id), (i64, min_nr), (i64, nr), (void *, events),       \
    (void *, timeout))                                                         \
  /** Calls Linux io_submit(aio_context_t, long, struct iocb * *). See         \
   * x86_64_209 in the source table. */                                        \
  X(209, io_submit, 3, NORMAL, sigma_sys_result_t, IPTR,                       \
    (syscall_arg_t, arg0), (i64, arg1), (void *, iocb))                        \
  /** Calls Linux io_cancel(aio_context_t ctx_id, struct iocb *iocb, struct    \
   * io_event *result). See x86_64_210 in the source table. */                 \
  X(210, io_cancel, 3, NORMAL, sigma_sys_result_t, IPTR,                       \
    (syscall_arg_t, ctx_id), (void *, iocb), (void *, result))                 \
  /** Calls Linux get_thread_area(?, ?, ?, ?, ?, ?). See x86_64_211 in the     \
   * source table. */                                                          \
  X(211, get_thread_area, 6, NORMAL, sigma_sys_result_t, IPTR,                 \
    (syscall_arg_t, arg0), (syscall_arg_t, arg1), (syscall_arg_t, arg2),       \
    (syscall_arg_t, arg3), (syscall_arg_t, arg4), (syscall_arg_t, arg5))       \
  /** Calls Linux lookup_dcookie(?, ?, ?, ?, ?, ?). See x86_64_212 in the      \
   * source table. */                                                          \
  X(212, lookup_dcookie, 6, NORMAL, sigma_sys_result_t, IPTR,                  \
    (syscall_arg_t, arg0), (syscall_arg_t, arg1), (syscall_arg_t, arg2),       \
    (syscall_arg_t, arg3), (syscall_arg_t, arg4), (syscall_arg_t, arg5))       \
  /** Calls Linux epoll_create(int size). See x86_64_213 in the source table.  \
   */                                                                          \
  X(213, epoll_create, 1, NORMAL, sigma_sys_result_t, IPTR, (i32, size))       \
  /** Calls Linux epoll_ctl_old(?, ?, ?, ?, ?, ?). See x86_64_214 in the       \
   * source table. */                                                          \
  X(214, epoll_ctl_old, 6, NORMAL, sigma_sys_result_t, IPTR,                   \
    (syscall_arg_t, arg0), (syscall_arg_t, arg1), (syscall_arg_t, arg2),       \
    (syscall_arg_t, arg3), (syscall_arg_t, arg4), (syscall_arg_t, arg5))       \
  /** Calls Linux epoll_wait_old(?, ?, ?, ?, ?, ?). See x86_64_215 in the      \
   * source table. */                                                          \
  X(215, epoll_wait_old, 6, NORMAL, sigma_sys_result_t, IPTR,                  \
    (syscall_arg_t, arg0), (syscall_arg_t, arg1), (syscall_arg_t, arg2),       \
    (syscall_arg_t, arg3), (syscall_arg_t, arg4), (syscall_arg_t, arg5))       \
  /** Calls Linux remap_file_pages(unsigned long start, unsigned long size,    \
   * unsigned long prot, unsigned long pgoff, unsigned long flags). See        \
   * x86_64_216 in the source table. */                                        \
  X(216, remap_file_pages, 5, NORMAL, sigma_sys_result_t, IPTR,                \
    (usize, start), (usize, size), (usize, prot), (usize, pgoff),              \
    (usize, flags))                                                            \
  /** Calls Linux getdents64(unsigned int fd, struct linux_dirent64 *dirent,   \
   * unsigned int count). See x86_64_217 in the source table. */               \
  X(217, getdents64, 3, NORMAL, sigma_sys_result_t, IPTR, (u32, fd),           \
    (void *, dirent), (u32, count))                                            \
  /** Calls Linux set_tid_address(int *tidptr). See x86_64_218 in the source   \
   * table. */                                                                 \
  X(218, set_tid_address, 1, NORMAL, sigma_sys_result_t, IPTR,                 \
    (void *, tidptr))                                                          \
  /** Calls Linux restart_syscall(void). See x86_64_219 in the source table.   \
   */                                                                          \
  X(219, restart_syscall, 0, NORMAL, sigma_sys_result_t, IPTR)                 \
  /** Calls Linux semtimedop(int semid, struct sembuf *sops, unsigned nsops,   \
   * const struct __kernel_timespec *timeout). See x86_64_220 in the source    \
   * table. */                                                                 \
  X(220, semtimedop, 4, NORMAL, sigma_sys_result_t, IPTR, (i32, semid),        \
    (void *, sops), (syscall_arg_t, nsops), (const void *, timeout))           \
  /** Calls Linux fadvise64(int fd, loff_t offset, size_t len, int advice).    \
   * See x86_64_221 in the source table. */                                    \
  X(221, fadvise64, 4, NORMAL, sigma_sys_result_t, IPTR, (i32, fd),            \
    (syscall_arg_t, offset), (usize, len), (i32, advice))                      \
  /** Calls Linux timer_create(clockid_t which_clock, struct sigevent          \
   * *timer_event_spec, timer_t * created_timer_id). See x86_64_222 in the     \
   * source table. */                                                          \
  X(222, timer_create, 3, NORMAL, sigma_sys_result_t, IPTR,                    \
    (syscall_arg_t, which_clock), (void *, timer_event_spec),                  \
    (void *, created_timer_id))                                                \
  /** Calls Linux timer_settime(timer_t timer_id, int flags, const struct      \
   * __kernel_itimerspec *new_setting, struct __kernel_itimerspec              \
   * *old_setting). See x86_64_223 in the source table. */                     \
  X(223, timer_settime, 4, NORMAL, sigma_sys_result_t, IPTR,                   \
    (syscall_arg_t, timer_id), (i32, flags), (const void *, new_setting),      \
    (void *, old_setting))                                                     \
  /** Calls Linux timer_gettime(timer_t timer_id, struct __kernel_itimerspec   \
   * *setting). See x86_64_224 in the source table. */                         \
  X(224, timer_gettime, 2, NORMAL, sigma_sys_result_t, IPTR,                   \
    (syscall_arg_t, timer_id), (void *, setting))                              \
  /** Calls Linux timer_getoverrun(timer_t timer_id). See x86_64_225 in the    \
   * source table. */                                                          \
  X(225, timer_getoverrun, 1, NORMAL, sigma_sys_result_t, IPTR,                \
    (syscall_arg_t, timer_id))                                                 \
  /** Calls Linux timer_delete(timer_t timer_id). See x86_64_226 in the source \
   * table. */                                                                 \
  X(226, timer_delete, 1, NORMAL, sigma_sys_result_t, IPTR,                    \
    (syscall_arg_t, timer_id))                                                 \
  /** Calls Linux clock_settime(clockid_t which_clock, const struct            \
   * __kernel_timespec *tp). See x86_64_227 in the source table. */            \
  X(227, clock_settime, 2, NORMAL, sigma_sys_result_t, IPTR,                   \
    (syscall_arg_t, which_clock), (const void *, tp))                          \
  /** Calls Linux clock_gettime(clockid_t which_clock, struct                  \
   * __kernel_timespec *tp). See x86_64_228 in the source table. */            \
  X(228, clock_gettime, 2, NORMAL, sigma_sys_result_t, IPTR,                   \
    (syscall_arg_t, which_clock), (void *, tp))                                \
  /** Calls Linux clock_getres(clockid_t which_clock, struct __kernel_timespec \
   * *tp). See x86_64_229 in the source table. */                              \
  X(229, clock_getres, 2, NORMAL, sigma_sys_result_t, IPTR,                    \
    (syscall_arg_t, which_clock), (void *, tp))                                \
  /** Calls Linux clock_nanosleep(clockid_t which_clock, int flags, const      \
   * struct __kernel_timespec *rqtp, struct __kernel_timespec *rmtp). See      \
   * x86_64_230 in the source table. */                                        \
  X(230, clock_nanosleep, 4, NORMAL, sigma_sys_result_t, IPTR,                 \
    (syscall_arg_t, which_clock), (i32, flags), (const void *, rqtp),          \
    (void *, rmtp))                                                            \
  /** Calls Linux exit_group(int error_code). See x86_64_231 in the source     \
   * table. */                                                                 \
  X(231, exit_group, 1, NORETURN, void, NONE, (i32, status))                   \
  /** Calls Linux epoll_wait(int epfd, struct epoll_event *events, int         \
   * maxevents, int timeout). See x86_64_232 in the source table. */           \
  X(232, epoll_wait, 4, NORMAL, sigma_sys_result_t, IPTR, (i32, epfd),         \
    (void *, events), (i32, maxevents), (i32, timeout))                        \
  /** Calls Linux epoll_ctl(int epfd, int op, int fd, struct epoll_event       \
   * *event). See x86_64_233 in the source table. */                           \
  X(233, epoll_ctl, 4, NORMAL, sigma_sys_result_t, IPTR, (i32, epfd),          \
    (i32, op), (i32, fd), (void *, event))                                     \
  /** Calls Linux tgkill(pid_t tgid, pid_t pid, int sig). See x86_64_234 in    \
   * the source table. */                                                      \
  X(234, tgkill, 3, NORMAL, sigma_sys_result_t, IPTR, (syscall_arg_t, tgid),   \
    (syscall_arg_t, pid), (i32, sig))                                          \
  /** Calls Linux utimes(char *filename, struct __kernel_old_timeval *utimes). \
   * See x86_64_235 in the source table. */                                    \
  X(235, utimes, 2, NORMAL, sigma_sys_result_t, IPTR, (void *, filename),      \
    (void *, utimes))                                                          \
  /** Calls Linux vserver(?, ?, ?, ?, ?, ?). See x86_64_236 in the source      \
   * table. */                                                                 \
  X(236, vserver, 6, NORMAL, sigma_sys_result_t, IPTR, (syscall_arg_t, arg0),  \
    (syscall_arg_t, arg1), (syscall_arg_t, arg2), (syscall_arg_t, arg3),       \
    (syscall_arg_t, arg4), (syscall_arg_t, arg5))                              \
  /** Calls Linux mbind(unsigned long start, unsigned long len, unsigned long  \
   * mode, const unsigned long *nmask, unsigned long maxnode, unsigned flags). \
   * See x86_64_237 in the source table. */                                    \
  X(237, mbind, 6, NORMAL, sigma_sys_result_t, IPTR, (usize, start),           \
    (usize, len), (usize, mode), (const void *, nmask), (usize, maxnode),      \
    (syscall_arg_t, flags))                                                    \
  /** Calls Linux set_mempolicy(int mode, const unsigned long *nmask, unsigned \
   * long maxnode). See x86_64_238 in the source table. */                     \
  X(238, set_mempolicy, 3, NORMAL, sigma_sys_result_t, IPTR, (i32, mode),      \
    (const void *, nmask), (usize, maxnode))                                   \
  /** Calls Linux get_mempolicy(int *policy, unsigned long *nmask, unsigned    \
   * long maxnode, unsigned long addr, unsigned long flags). See x86_64_239 in \
   * the source table. */                                                      \
  X(239, get_mempolicy, 5, NORMAL, sigma_sys_result_t, IPTR, (void *, policy), \
    (void *, nmask), (usize, maxnode), (usize, addr), (usize, flags))          \
  /** Calls Linux mq_open(const char *name, int oflag, umode_t mode, struct    \
   * mq_attr *attr). See x86_64_240 in the source table. */                    \
  X(240, mq_open, 4, NORMAL, sigma_sys_result_t, IPTR, (const void *, name),   \
    (i32, oflag), (syscall_arg_t, mode), (void *, attr))                       \
  /** Calls Linux mq_unlink(const char *name). See x86_64_241 in the source    \
   * table. */                                                                 \
  X(241, mq_unlink, 1, NORMAL, sigma_sys_result_t, IPTR, (const void *, name)) \
  /** Calls Linux mq_timedsend(mqd_t mqdes, const char *msg_ptr, size_t        \
   * msg_len, unsigned int msg_prio, const struct __kernel_timespec            \
   * *abs_timeout). See x86_64_242 in the source table. */                     \
  X(242, mq_timedsend, 5, NORMAL, sigma_sys_result_t, IPTR,                    \
    (syscall_arg_t, mqdes), (const void *, msg_ptr), (usize, msg_len),         \
    (u32, msg_prio), (const void *, abs_timeout))                              \
  /** Calls Linux mq_timedreceive(mqd_t mqdes, char *msg_ptr, size_t msg_len,  \
   * unsigned int *msg_prio, const struct __kernel_timespec *abs_timeout). See \
   * x86_64_243 in the source table. */                                        \
  X(243, mq_timedreceive, 5, NORMAL, sigma_sys_result_t, IPTR,                 \
    (syscall_arg_t, mqdes), (void *, msg_ptr), (usize, msg_len),               \
    (void *, msg_prio), (const void *, abs_timeout))                           \
  /** Calls Linux mq_notify(mqd_t mqdes, const struct sigevent *notification). \
   * See x86_64_244 in the source table. */                                    \
  X(244, mq_notify, 2, NORMAL, sigma_sys_result_t, IPTR,                       \
    (syscall_arg_t, mqdes), (const void *, notification))                      \
  /** Calls Linux mq_getsetattr(mqd_t mqdes, const struct mq_attr *mqstat,     \
   * struct mq_attr *omqstat). See x86_64_245 in the source table. */          \
  X(245, mq_getsetattr, 3, NORMAL, sigma_sys_result_t, IPTR,                   \
    (syscall_arg_t, mqdes), (const void *, mqstat), (void *, omqstat))         \
  /** Calls Linux kexec_load(unsigned long entry, unsigned long nr_segments,   \
   * struct kexec_segment *segments, unsigned long flags). See x86_64_246 in   \
   * the source table. */                                                      \
  X(246, kexec_load, 4, NORMAL, sigma_sys_result_t, IPTR, (usize, entry),      \
    (usize, nr_segments), (void *, segments), (usize, flags))                  \
  /** Calls Linux waitid(int which, pid_t pid, struct siginfo *infop, int      \
   * options, struct rusage *ru). See x86_64_247 in the source table. */       \
  X(247, waitid, 5, NORMAL, sigma_sys_result_t, IPTR, (i32, which),            \
    (syscall_arg_t, pid), (void *, infop), (i32, options), (void *, ru))       \
  /** Calls Linux add_key(const char *_type, const char *_description, const   \
   * void *_payload, size_t plen, key_serial_t destringid). See x86_64_248 in  \
   * the source table. */                                                      \
  X(248, add_key, 5, NORMAL, sigma_sys_result_t, IPTR, (const void *, _type),  \
    (const void *, _description), (const void *, _payload), (usize, plen),     \
    (syscall_arg_t, destringid))                                               \
  /** Calls Linux request_key(const char *_type, const char *_description,     \
   * const char *_callout_info, key_serial_t destringid). See x86_64_249 in    \
   * the source table. */                                                      \
  X(249, request_key, 4, NORMAL, sigma_sys_result_t, IPTR,                     \
    (const void *, _type), (const void *, _description),                       \
    (const void *, _callout_info), (syscall_arg_t, destringid))                \
  /** Calls Linux keyctl(int cmd, unsigned long arg2, unsigned long arg3,      \
   * unsigned long arg4, unsigned long arg5). See x86_64_250 in the source     \
   * table. */                                                                 \
  X(250, keyctl, 5, NORMAL, sigma_sys_result_t, IPTR, (i32, cmd),              \
    (usize, arg2), (usize, arg3), (usize, arg4), (usize, arg5))                \
  /** Calls Linux ioprio_set(int which, int who, int ioprio). See x86_64_251   \
   * in the source table. */                                                   \
  X(251, ioprio_set, 3, NORMAL, sigma_sys_result_t, IPTR, (i32, which),        \
    (i32, who), (i32, ioprio))                                                 \
  /** Calls Linux ioprio_get(int which, int who). See x86_64_252 in the source \
   * table. */                                                                 \
  X(252, ioprio_get, 2, NORMAL, sigma_sys_result_t, IPTR, (i32, which),        \
    (i32, who))                                                                \
  /** Calls Linux inotify_init(void). See x86_64_253 in the source table. */   \
  X(253, inotify_init, 0, NORMAL, sigma_sys_result_t, IPTR)                    \
  /** Calls Linux inotify_add_watch(int fd, const char *path, u32 mask). See   \
   * x86_64_254 in the source table. */                                        \
  X(254, inotify_add_watch, 3, NORMAL, sigma_sys_result_t, IPTR, (i32, fd),    \
    (const void *, path), (syscall_arg_t, mask))                               \
  /** Calls Linux inotify_rm_watch(int fd, __s32 wd). See x86_64_255 in the    \
   * source table. */                                                          \
  X(255, inotify_rm_watch, 2, NORMAL, sigma_sys_result_t, IPTR, (i32, fd),     \
    (syscall_arg_t, wd))                                                       \
  /** Calls Linux migrate_pages(pid_t pid, unsigned long maxnode, const        \
   * unsigned long *from, const unsigned long *to). See x86_64_256 in the      \
   * source table. */                                                          \
  X(256, migrate_pages, 4, NORMAL, sigma_sys_result_t, IPTR,                   \
    (syscall_arg_t, pid), (usize, maxnode), (const void *, from),              \
    (const void *, to))                                                        \
  /** Calls Linux openat(int dfd, const char *filename, int flags, umode_t     \
   * mode). See x86_64_257 in the source table. */                             \
  X(257, openat, 4, NORMAL, sigma_sys_result_t, IPTR, (i32, dfd),              \
    (const void *, filename), (i32, flags), (syscall_arg_t, mode))             \
  /** Calls Linux mkdirat(int dfd, const char * pathname, umode_t mode). See   \
   * x86_64_258 in the source table. */                                        \
  X(258, mkdirat, 3, NORMAL, sigma_sys_result_t, IPTR, (i32, dfd),             \
    (const void *, pathname), (syscall_arg_t, mode))                           \
  /** Calls Linux mknodat(int dfd, const char * filename, umode_t mode,        \
   * unsigned dev). See x86_64_259 in the source table. */                     \
  X(259, mknodat, 4, NORMAL, sigma_sys_result_t, IPTR, (i32, dfd),             \
    (const void *, filename), (syscall_arg_t, mode), (syscall_arg_t, dev))     \
  /** Calls Linux fchownat(int dfd, const char *filename, uid_t user, gid_t    \
   * group, int flag). See x86_64_260 in the source table. */                  \
  X(260, fchownat, 5, NORMAL, sigma_sys_result_t, IPTR, (i32, dfd),            \
    (const void *, filename), (syscall_arg_t, arg2), (syscall_arg_t, group),   \
    (i32, flag))                                                               \
  /** Calls Linux futimesat(int dfd, const char *filename, struct              \
   * __kernel_old_timeval *utimes). See x86_64_261 in the source table. */     \
  X(261, futimesat, 3, NORMAL, sigma_sys_result_t, IPTR, (i32, dfd),           \
    (const void *, filename), (void *, utimes))                                \
  /** Calls Linux newfstatat(int dfd, const char *filename, struct stat        \
   * *statbuf, int flag). See x86_64_262 in the source table. */               \
  X(262, newfstatat, 4, NORMAL, sigma_sys_result_t, IPTR, (i32, dfd),          \
    (const void *, filename), (void *, statbuf), (i32, flag))                  \
  /** Calls Linux unlinkat(int dfd, const char * pathname, int flag). See      \
   * x86_64_263 in the source table. */                                        \
  X(263, unlinkat, 3, NORMAL, sigma_sys_result_t, IPTR, (i32, dfd),            \
    (const void *, pathname), (i32, flag))                                     \
  /** Calls Linux renameat(int olddfd, const char * oldname, int newdfd, const \
   * char * newname). See x86_64_264 in the source table. */                   \
  X(264, renameat, 4, NORMAL, sigma_sys_result_t, IPTR, (i32, olddfd),         \
    (const void *, oldname), (i32, newdfd), (const void *, newname))           \
  /** Calls Linux linkat(int olddfd, const char *oldname, int newdfd, const    \
   * char *newname, int flags). See x86_64_265 in the source table. */         \
  X(265, linkat, 5, NORMAL, sigma_sys_result_t, IPTR, (i32, olddfd),           \
    (const void *, oldname), (i32, newdfd), (const void *, newname),           \
    (i32, flags))                                                              \
  /** Calls Linux symlinkat(const char * oldname, int newdfd, const char *     \
   * newname). See x86_64_266 in the source table. */                          \
  X(266, symlinkat, 3, NORMAL, sigma_sys_result_t, IPTR,                       \
    (const void *, oldname), (i32, newdfd), (const void *, newname))           \
  /** Calls Linux readlinkat(int dfd, const char *path, char *buf, int         \
   * bufsiz). See x86_64_267 in the source table. */                           \
  X(267, readlinkat, 4, NORMAL, sigma_sys_result_t, IPTR, (i32, dfd),          \
    (const void *, path), (void *, buf), (i32, bufsiz))                        \
  /** Calls Linux fchmodat(int dfd, const char *filename, umode_t mode). See   \
   * x86_64_268 in the source table. */                                        \
  X(268, fchmodat, 3, NORMAL, sigma_sys_result_t, IPTR, (i32, dfd),            \
    (const void *, filename), (syscall_arg_t, mode))                           \
  /** Calls Linux faccessat(int dfd, const char *filename, int mode). See      \
   * x86_64_269 in the source table. */                                        \
  X(269, faccessat, 3, NORMAL, sigma_sys_result_t, IPTR, (i32, dfd),           \
    (const void *, filename), (i32, mode))                                     \
  /** Calls Linux pselect6(int, fd_set *, fd_set *, fd_set *, struct           \
   * __kernel_timespec *, void *). See x86_64_270 in the source table. */      \
  X(270, pselect6, 6, NORMAL, sigma_sys_result_t, IPTR, (i32, arg0),           \
    (void *, fd_set), (void *, fd_set_2), (void *, fd_set_3),                  \
    (void *, __kernel_timespec), (void *, arg5))                               \
  /** Calls Linux ppoll(struct pollfd *, unsigned int, struct                  \
   * __kernel_timespec *, const sigset_t *, size_t). See x86_64_271 in the     \
   * source table. */                                                          \
  X(271, ppoll, 5, NORMAL, sigma_sys_result_t, IPTR, (void *, pollfd),         \
    (u32, arg1), (void *, __kernel_timespec), (const void *, sigset_t),        \
    (usize, arg4))                                                             \
  /** Calls Linux unshare(unsigned long unshare_flags). See x86_64_272 in the  \
   * source table. */                                                          \
  X(272, unshare, 1, NORMAL, sigma_sys_result_t, IPTR, (usize, unshare_flags)) \
  /** Calls Linux set_robust_list(struct robust_list_head *head, size_t len).  \
   * See x86_64_273 in the source table. */                                    \
  X(273, set_robust_list, 2, NORMAL, sigma_sys_result_t, IPTR, (void *, head), \
    (usize, len))                                                              \
  /** Calls Linux get_robust_list(int pid, struct robust_list_head *           \
   * *head_ptr, size_t *len_ptr). See x86_64_274 in the source table. */       \
  X(274, get_robust_list, 3, NORMAL, sigma_sys_result_t, IPTR, (i32, pid),     \
    (void *, head_ptr), (void *, len_ptr))                                     \
  /** Calls Linux splice(int fd_in, loff_t *off_in, int fd_out, loff_t         \
   * *off_out, size_t len, unsigned int flags). See x86_64_275 in the source   \
   * table. */                                                                 \
  X(275, splice, 6, NORMAL, sigma_sys_result_t, IPTR, (i32, fd_in),            \
    (void *, off_in), (i32, fd_out), (void *, off_out), (usize, len),          \
    (u32, flags))                                                              \
  /** Calls Linux tee(int fdin, int fdout, size_t len, unsigned int flags).    \
   * See x86_64_276 in the source table. */                                    \
  X(276, tee, 4, NORMAL, sigma_sys_result_t, IPTR, (i32, fdin), (i32, fdout),  \
    (usize, len), (u32, flags))                                                \
  /** Calls Linux sync_file_range(int fd, loff_t offset, loff_t nbytes,        \
   * unsigned int flags). See x86_64_277 in the source table. */               \
  X(277, sync_file_range, 4, NORMAL, sigma_sys_result_t, IPTR, (i32, fd),      \
    (syscall_arg_t, offset), (syscall_arg_t, nbytes), (u32, flags))            \
  /** Calls Linux vmsplice(int fd, const struct iovec *iov, unsigned long      \
   * nr_segs, unsigned int flags). See x86_64_278 in the source table. */      \
  X(278, vmsplice, 4, NORMAL, sigma_sys_result_t, IPTR, (i32, fd),             \
    (const void *, iov), (usize, nr_segs), (u32, flags))                       \
  /** Calls Linux move_pages(pid_t pid, unsigned long nr_pages, const void *   \
   * *pages, const int *nodes, int *status, int flags). See x86_64_279 in the  \
   * source table. */                                                          \
  X(279, move_pages, 6, NORMAL, sigma_sys_result_t, IPTR,                      \
    (syscall_arg_t, pid), (usize, nr_pages), (const void *, pages),            \
    (const void *, nodes), (void *, status), (i32, flags))                     \
  /** Calls Linux utimensat(int dfd, const char *filename, struct              \
   * __kernel_timespec *utimes, int flags). See x86_64_280 in the source       \
   * table. */                                                                 \
  X(280, utimensat, 4, NORMAL, sigma_sys_result_t, IPTR, (i32, dfd),           \
    (const void *, filename), (void *, utimes), (i32, flags))                  \
  /** Calls Linux epoll_pwait(int epfd, struct epoll_event *events, int        \
   * maxevents, int timeout, const sigset_t *sigmask, size_t sigsetsize). See  \
   * x86_64_281 in the source table. */                                        \
  X(281, epoll_pwait, 6, NORMAL, sigma_sys_result_t, IPTR, (i32, epfd),        \
    (void *, events), (i32, maxevents), (i32, timeout),                        \
    (const void *, sigmask), (usize, sigsetsize))                              \
  /** Calls Linux signalfd(int ufd, sigset_t *user_mask, size_t sizemask). See \
   * x86_64_282 in the source table. */                                        \
  X(282, signalfd, 3, NORMAL, sigma_sys_result_t, IPTR, (i32, ufd),            \
    (void *, user_mask), (usize, sizemask))                                    \
  /** Calls Linux timerfd_create(int clockid, int flags). See x86_64_283 in    \
   * the source table. */                                                      \
  X(283, timerfd_create, 2, NORMAL, sigma_sys_result_t, IPTR, (i32, clockid),  \
    (i32, flags))                                                              \
  /** Calls Linux eventfd(unsigned int count). See x86_64_284 in the source    \
   * table. */                                                                 \
  X(284, eventfd, 1, NORMAL, sigma_sys_result_t, IPTR, (u32, count))           \
  /** Calls Linux fallocate(int fd, int mode, loff_t offset, loff_t len). See  \
   * x86_64_285 in the source table. */                                        \
  X(285, fallocate, 4, NORMAL, sigma_sys_result_t, IPTR, (i32, fd),            \
    (i32, mode), (syscall_arg_t, offset), (syscall_arg_t, len))                \
  /** Calls Linux timerfd_settime(int ufd, int flags, const struct             \
   * __kernel_itimerspec *utmr, struct __kernel_itimerspec *otmr). See         \
   * x86_64_286 in the source table. */                                        \
  X(286, timerfd_settime, 4, NORMAL, sigma_sys_result_t, IPTR, (i32, ufd),     \
    (i32, flags), (const void *, utmr), (void *, otmr))                        \
  /** Calls Linux timerfd_gettime(int ufd, struct __kernel_itimerspec *otmr).  \
   * See x86_64_287 in the source table. */                                    \
  X(287, timerfd_gettime, 2, NORMAL, sigma_sys_result_t, IPTR, (i32, ufd),     \
    (void *, otmr))                                                            \
  /** Calls Linux accept4(int, struct sockaddr *, int *, int). See x86_64_288  \
   * in the source table. */                                                   \
  X(288, accept4, 4, NORMAL, sigma_sys_result_t, IPTR, (i32, arg0),            \
    (void *, sockaddr), (void *, arg2), (i32, arg3))                           \
  /** Calls Linux signalfd4(int ufd, sigset_t *user_mask, size_t sizemask, int \
   * flags). See x86_64_289 in the source table. */                            \
  X(289, signalfd4, 4, NORMAL, sigma_sys_result_t, IPTR, (i32, ufd),           \
    (void *, user_mask), (usize, sizemask), (i32, flags))                      \
  /** Calls Linux eventfd2(unsigned int count, int flags). See x86_64_290 in   \
   * the source table. */                                                      \
  X(290, eventfd2, 2, NORMAL, sigma_sys_result_t, IPTR, (u32, count),          \
    (i32, flags))                                                              \
  /** Calls Linux epoll_create1(int flags). See x86_64_291 in the source       \
   * table. */                                                                 \
  X(291, epoll_create1, 1, NORMAL, sigma_sys_result_t, IPTR, (i32, flags))     \
  /** Calls Linux dup3(unsigned int oldfd, unsigned int newfd, int flags). See \
   * x86_64_292 in the source table. */                                        \
  X(292, dup3, 3, NORMAL, sigma_sys_result_t, IPTR, (u32, oldfd),              \
    (u32, newfd), (i32, flags))                                                \
  /** Calls Linux pipe2(int *fildes, int flags). See x86_64_293 in the source  \
   * table. */                                                                 \
  X(293, pipe2, 2, NORMAL, sigma_sys_result_t, IPTR, (void *, fildes),         \
    (i32, flags))                                                              \
  /** Calls Linux inotify_init1(int flags). See x86_64_294 in the source       \
   * table. */                                                                 \
  X(294, inotify_init1, 1, NORMAL, sigma_sys_result_t, IPTR, (i32, flags))     \
  /** Calls Linux preadv(unsigned long fd, const struct iovec *vec, unsigned   \
   * long vlen, unsigned long pos_l, unsigned long pos_h). See x86_64_295 in   \
   * the source table. */                                                      \
  X(295, preadv, 5, NORMAL, sigma_sys_result_t, IPTR, (usize, fd),             \
    (const void *, vec), (usize, vlen), (usize, pos_l), (usize, pos_h))        \
  /** Calls Linux pwritev(unsigned long fd, const struct iovec *vec, unsigned  \
   * long vlen, unsigned long pos_l, unsigned long pos_h). See x86_64_296 in   \
   * the source table. */                                                      \
  X(296, pwritev, 5, NORMAL, sigma_sys_result_t, IPTR, (usize, fd),            \
    (const void *, vec), (usize, vlen), (usize, pos_l), (usize, pos_h))        \
  /** Calls Linux rt_tgsigqueueinfo(pid_t tgid, pid_t pid, int sig, siginfo_t  \
   * *uinfo). See x86_64_297 in the source table. */                           \
  X(297, rt_tgsigqueueinfo, 4, NORMAL, sigma_sys_result_t, IPTR,               \
    (syscall_arg_t, tgid), (syscall_arg_t, pid), (i32, sig), (void *, uinfo))  \
  /** Calls Linux perf_event_open(struct perf_event_attr *attr_uptr, pid_t     \
   * pid, int cpu, int group_fd, unsigned long flags). See x86_64_298 in the   \
   * source table. */                                                          \
  X(298, perf_event_open, 5, NORMAL, sigma_sys_result_t, IPTR,                 \
    (void *, attr_uptr), (syscall_arg_t, pid), (i32, cpu), (i32, group_fd),    \
    (usize, flags))                                                            \
  /** Calls Linux recvmmsg(int fd, struct mmsghdr *msg, unsigned int vlen,     \
   * unsigned flags, struct __kernel_timespec *timeout). See x86_64_299 in the \
   * source table. */                                                          \
  X(299, recvmmsg, 5, NORMAL, sigma_sys_result_t, IPTR, (i32, fd),             \
    (void *, msg), (u32, vlen), (syscall_arg_t, flags), (void *, timeout))     \
  /** Calls Linux fanotify_init(unsigned int flags, unsigned int               \
   * event_f_flags). See x86_64_300 in the source table. */                    \
  X(300, fanotify_init, 2, NORMAL, sigma_sys_result_t, IPTR, (u32, flags),     \
    (u32, event_f_flags))                                                      \
  /** Calls Linux fanotify_mark(int fanotify_fd, unsigned int flags, u64 mask, \
   * int fd, const char *pathname). See x86_64_301 in the source table. */     \
  X(301, fanotify_mark, 5, NORMAL, sigma_sys_result_t, IPTR,                   \
    (i32, fanotify_fd), (u32, flags), (syscall_arg_t, mask), (i32, fd),        \
    (const void *, pathname))                                                  \
  /** Calls Linux prlimit64(pid_t pid, unsigned int resource, const struct     \
   * rlimit64 *new_rlim, struct rlimit64 *old_rlim). See x86_64_302 in the     \
   * source table. */                                                          \
  X(302, prlimit64, 4, NORMAL, sigma_sys_result_t, IPTR, (syscall_arg_t, pid), \
    (u32, resource), (const void *, new_rlim), (void *, old_rlim))             \
  /** Calls Linux name_to_handle_at(int dfd, const char *name, struct          \
   * file_handle *handle, void *mnt_id, int flag). See x86_64_303 in the       \
   * source table. */                                                          \
  X(303, name_to_handle_at, 5, NORMAL, sigma_sys_result_t, IPTR, (i32, dfd),   \
    (const void *, name), (void *, handle), (void *, mnt_id), (i32, flag))     \
  /** Calls Linux open_by_handle_at(int mountdirfd, struct file_handle         \
   * *handle, int flags). See x86_64_304 in the source table. */               \
  X(304, open_by_handle_at, 3, NORMAL, sigma_sys_result_t, IPTR,               \
    (i32, mountdirfd), (void *, handle), (i32, flags))                         \
  /** Calls Linux clock_adjtime(clockid_t which_clock, struct __kernel_timex   \
   * *tx). See x86_64_305 in the source table. */                              \
  X(305, clock_adjtime, 2, NORMAL, sigma_sys_result_t, IPTR,                   \
    (syscall_arg_t, which_clock), (void *, tx))                                \
  /** Calls Linux syncfs(int fd). See x86_64_306 in the source table. */       \
  X(306, syncfs, 1, NORMAL, sigma_sys_result_t, IPTR, (i32, fd))               \
  /** Calls Linux sendmmsg(int fd, struct mmsghdr *msg, unsigned int vlen,     \
   * unsigned flags). See x86_64_307 in the source table. */                   \
  X(307, sendmmsg, 4, NORMAL, sigma_sys_result_t, IPTR, (i32, fd),             \
    (void *, msg), (u32, vlen), (syscall_arg_t, flags))                        \
  /** Calls Linux setns(int fd, int nstype). See x86_64_308 in the source      \
   * table. */                                                                 \
  X(308, setns, 2, NORMAL, sigma_sys_result_t, IPTR, (i32, fd), (i32, nstype)) \
  /** Calls Linux getcpu(unsigned *cpu, unsigned *node, struct getcpu_cache    \
   * *cache). See x86_64_309 in the source table. */                           \
  X(309, getcpu, 3, NORMAL, sigma_sys_result_t, IPTR, (void *, cpu),           \
    (void *, node), (void *, cache))                                           \
  /** Calls Linux process_vm_readv(pid_t pid, const struct iovec *lvec,        \
   * unsigned long liovcnt, const struct iovec *rvec, unsigned long riovcnt,   \
   * unsigned long flags). See x86_64_310 in the source table. */              \
  X(310, process_vm_readv, 6, NORMAL, sigma_sys_result_t, IPTR,                \
    (syscall_arg_t, pid), (const void *, lvec), (usize, liovcnt),              \
    (const void *, rvec), (usize, riovcnt), (usize, flags))                    \
  /** Calls Linux process_vm_writev(pid_t pid, const struct iovec *lvec,       \
   * unsigned long liovcnt, const struct iovec *rvec, unsigned long riovcnt,   \
   * unsigned long flags). See x86_64_311 in the source table. */              \
  X(311, process_vm_writev, 6, NORMAL, sigma_sys_result_t, IPTR,               \
    (syscall_arg_t, pid), (const void *, lvec), (usize, liovcnt),              \
    (const void *, rvec), (usize, riovcnt), (usize, flags))                    \
  /** Calls Linux kcmp(pid_t pid1, pid_t pid2, int type, unsigned long idx1,   \
   * unsigned long idx2). See x86_64_312 in the source table. */               \
  X(312, kcmp, 5, NORMAL, sigma_sys_result_t, IPTR, (syscall_arg_t, pid1),     \
    (syscall_arg_t, pid2), (i32, type), (usize, idx1), (usize, idx2))          \
  /** Calls Linux finit_module(int fd, const char *uargs, int flags). See      \
   * x86_64_313 in the source table. */                                        \
  X(313, finit_module, 3, NORMAL, sigma_sys_result_t, IPTR, (i32, fd),         \
    (const void *, uargs), (i32, flags))                                       \
  /** Calls Linux sched_setattr(pid_t pid, struct sched_attr *attr, unsigned   \
   * int flags). See x86_64_314 in the source table. */                        \
  X(314, sched_setattr, 3, NORMAL, sigma_sys_result_t, IPTR,                   \
    (syscall_arg_t, pid), (void *, attr), (u32, flags))                        \
  /** Calls Linux sched_getattr(pid_t pid, struct sched_attr *attr, unsigned   \
   * int size, unsigned int flags). See x86_64_315 in the source table. */     \
  X(315, sched_getattr, 4, NORMAL, sigma_sys_result_t, IPTR,                   \
    (syscall_arg_t, pid), (void *, attr), (u32, size), (u32, flags))           \
  /** Calls Linux renameat2(int olddfd, const char *oldname, int newdfd, const \
   * char *newname, unsigned int flags). See x86_64_316 in the source table.   \
   */                                                                          \
  X(316, renameat2, 5, NORMAL, sigma_sys_result_t, IPTR, (i32, olddfd),        \
    (const void *, oldname), (i32, newdfd), (const void *, newname),           \
    (u32, flags))                                                              \
  /** Calls Linux seccomp(unsigned int op, unsigned int flags, void *uargs).   \
   * See x86_64_317 in the source table. */                                    \
  X(317, seccomp, 3, NORMAL, sigma_sys_result_t, IPTR, (u32, op),              \
    (u32, flags), (void *, uargs))                                             \
  /** Calls Linux getrandom(char *buf, size_t count, unsigned int flags). See  \
   * x86_64_318 in the source table. */                                        \
  X(318, getrandom, 3, NORMAL, sigma_sys_result_t, IPTR, (void *, buf),        \
    (usize, count), (u32, flags))                                              \
  /** Calls Linux memfd_create(const char *uname_ptr, unsigned int flags). See \
   * x86_64_319 in the source table. */                                        \
  X(319, memfd_create, 2, NORMAL, sigma_sys_result_t, IPTR,                    \
    (const void *, uname_ptr), (u32, flags))                                   \
  /** Calls Linux kexec_file_load(int kernel_fd, int initrd_fd, unsigned long  \
   * cmdline_len, const char *cmdline_ptr, unsigned long flags). See           \
   * x86_64_320 in the source table. */                                        \
  X(320, kexec_file_load, 5, NORMAL, sigma_sys_result_t, IPTR,                 \
    (i32, kernel_fd), (i32, initrd_fd), (usize, cmdline_len),                  \
    (const void *, cmdline_ptr), (usize, flags))                               \
  /** Calls Linux bpf(int cmd, union bpf_attr *attr, unsigned int size). See   \
   * x86_64_321 in the source table. */                                        \
  X(321, bpf, 3, NORMAL, sigma_sys_result_t, IPTR, (i32, cmd), (void *, attr), \
    (u32, size))                                                               \
  /** Calls Linux execveat(int dfd, const char *filename, const char *const    \
   * *argv, const char *const *envp, int flags). See x86_64_322 in the source  \
   * table. */                                                                 \
  X(322, execveat, 5, NORMAL, sigma_sys_result_t, IPTR, (i32, dfd),            \
    (const void *, filename), (const void *, argv), (const void *, envp),      \
    (i32, flags))                                                              \
  /** Calls Linux userfaultfd(int flags). See x86_64_323 in the source table.  \
   */                                                                          \
  X(323, userfaultfd, 1, NORMAL, sigma_sys_result_t, IPTR, (i32, flags))       \
  /** Calls Linux membarrier(int cmd, unsigned int flags, int cpu_id). See     \
   * x86_64_324 in the source table. */                                        \
  X(324, membarrier, 3, NORMAL, sigma_sys_result_t, IPTR, (i32, cmd),          \
    (u32, flags), (i32, cpu_id))                                               \
  /** Calls Linux mlock2(unsigned long start, size_t len, int flags). See      \
   * x86_64_325 in the source table. */                                        \
  X(325, mlock2, 3, NORMAL, sigma_sys_result_t, IPTR, (usize, start),          \
    (usize, len), (i32, flags))                                                \
  /** Calls Linux copy_file_range(int fd_in, loff_t *off_in, int fd_out,       \
   * loff_t *off_out, size_t len, unsigned int flags). See x86_64_326 in the   \
   * source table. */                                                          \
  X(326, copy_file_range, 6, NORMAL, sigma_sys_result_t, IPTR, (i32, fd_in),   \
    (void *, off_in), (i32, fd_out), (void *, off_out), (usize, len),          \
    (u32, flags))                                                              \
  /** Calls Linux preadv2(unsigned long fd, const struct iovec *vec, unsigned  \
   * long vlen, unsigned long pos_l, unsigned long pos_h, rwf_t flags). See    \
   * x86_64_327 in the source table. */                                        \
  X(327, preadv2, 6, NORMAL, sigma_sys_result_t, IPTR, (usize, fd),            \
    (const void *, vec), (usize, vlen), (usize, pos_l), (usize, pos_h),        \
    (syscall_arg_t, flags))                                                    \
  /** Calls Linux pwritev2(unsigned long fd, const struct iovec *vec, unsigned \
   * long vlen, unsigned long pos_l, unsigned long pos_h, rwf_t flags). See    \
   * x86_64_328 in the source table. */                                        \
  X(328, pwritev2, 6, NORMAL, sigma_sys_result_t, IPTR, (usize, fd),           \
    (const void *, vec), (usize, vlen), (usize, pos_l), (usize, pos_h),        \
    (syscall_arg_t, flags))                                                    \
  /** Calls Linux pkey_mprotect(unsigned long start, size_t len, unsigned long \
   * prot, int pkey). See x86_64_329 in the source table. */                   \
  X(329, pkey_mprotect, 4, NORMAL, sigma_sys_result_t, IPTR, (usize, start),   \
    (usize, len), (usize, prot), (i32, pkey))                                  \
  /** Calls Linux pkey_alloc(unsigned long flags, unsigned long init_val). See \
   * x86_64_330 in the source table. */                                        \
  X(330, pkey_alloc, 2, NORMAL, sigma_sys_result_t, IPTR, (usize, flags),      \
    (usize, init_val))                                                         \
  /** Calls Linux pkey_free(int pkey). See x86_64_331 in the source table. */  \
  X(331, pkey_free, 1, NORMAL, sigma_sys_result_t, IPTR, (i32, pkey))          \
  /** Calls Linux statx(int dfd, const char *path, unsigned flags, unsigned    \
   * mask, struct statx *buffer). See x86_64_332 in the source table. */       \
  X(332, statx, 5, NORMAL, sigma_sys_result_t, IPTR, (i32, dfd),               \
    (const void *, path), (syscall_arg_t, flags), (syscall_arg_t, mask),       \
    (void *, buffer))                                                          \
  /** Calls Linux io_pgetevents(aio_context_t ctx_id, long min_nr, long nr,    \
   * struct io_event *events, struct __kernel_timespec *timeout, const struct  \
   * __aio_sigset *sig). See x86_64_333 in the source table. */                \
  X(333, io_pgetevents, 6, NORMAL, sigma_sys_result_t, IPTR,                   \
    (syscall_arg_t, ctx_id), (i64, min_nr), (i64, nr), (void *, events),       \
    (void *, timeout), (const void *, sig))                                    \
  /** Calls Linux rseq(struct rseq *rseq, uint32_t rseq_len, int flags,        \
   * uint32_t sig). See x86_64_334 in the source table. */                     \
  X(334, rseq, 4, NORMAL, sigma_sys_result_t, IPTR, (void *, rseq),            \
    (syscall_arg_t, rseq_len), (i32, flags), (syscall_arg_t, sig))             \
  /** Calls Linux pidfd_send_signal(int pidfd, int sig, siginfo_t *info,       \
   * unsigned int flags). See x86_64_424 in the source table. */               \
  X(424, pidfd_send_signal, 4, NORMAL, sigma_sys_result_t, IPTR, (i32, pidfd), \
    (i32, sig), (void *, info), (u32, flags))                                  \
  /** Calls Linux io_uring_setup(u32 entries, struct io_uring_params *p). See  \
   * x86_64_425 in the source table. */                                        \
  X(425, io_uring_setup, 2, NORMAL, sigma_sys_result_t, IPTR,                  \
    (syscall_arg_t, entries), (void *, p))                                     \
  /** Calls Linux io_uring_enter(unsigned int fd, u32 to_submit, u32           \
   * min_complete, u32 flags, const void *argp, size_t argsz). See x86_64_426  \
   * in the source table. */                                                   \
  X(426, io_uring_enter, 6, NORMAL, sigma_sys_result_t, IPTR, (u32, fd),       \
    (syscall_arg_t, to_submit), (syscall_arg_t, min_complete),                 \
    (syscall_arg_t, flags), (const void *, argp), (usize, argsz))              \
  /** Calls Linux io_uring_register(unsigned int fd, unsigned int op, void     \
   * *arg, unsigned int nr_args). See x86_64_427 in the source table. */       \
  X(427, io_uring_register, 4, NORMAL, sigma_sys_result_t, IPTR, (u32, fd),    \
    (u32, op), (void *, arg), (u32, nr_args))                                  \
  /** Calls Linux open_tree(int dfd, const char *path, unsigned flags). See    \
   * x86_64_428 in the source table. */                                        \
  X(428, open_tree, 3, NORMAL, sigma_sys_result_t, IPTR, (i32, dfd),           \
    (const void *, path), (syscall_arg_t, flags))                              \
  /** Calls Linux move_mount(int from_dfd, const char *from_path, int to_dfd,  \
   * const char *to_path, unsigned int ms_flags). See x86_64_429 in the source \
   * table. */                                                                 \
  X(429, move_mount, 5, NORMAL, sigma_sys_result_t, IPTR, (i32, from_dfd),     \
    (const void *, from_path), (i32, to_dfd), (const void *, to_path),         \
    (u32, ms_flags))                                                           \
  /** Calls Linux fsopen(const char *fs_name, unsigned int flags). See         \
   * x86_64_430 in the source table. */                                        \
  X(430, fsopen, 2, NORMAL, sigma_sys_result_t, IPTR, (const void *, fs_name), \
    (u32, flags))                                                              \
  /** Calls Linux fsconfig(int fs_fd, unsigned int cmd, const char *key, const \
   * void *value, int aux). See x86_64_431 in the source table. */             \
  X(431, fsconfig, 5, NORMAL, sigma_sys_result_t, IPTR, (i32, fs_fd),          \
    (u32, cmd), (const void *, key), (const void *, value), (i32, aux))        \
  /** Calls Linux fsmount(int fs_fd, unsigned int flags, unsigned int          \
   * ms_flags). See x86_64_432 in the source table. */                         \
  X(432, fsmount, 3, NORMAL, sigma_sys_result_t, IPTR, (i32, fs_fd),           \
    (u32, flags), (u32, ms_flags))                                             \
  /** Calls Linux fspick(int dfd, const char *path, unsigned int flags). See   \
   * x86_64_433 in the source table. */                                        \
  X(433, fspick, 3, NORMAL, sigma_sys_result_t, IPTR, (i32, dfd),              \
    (const void *, path), (u32, flags))                                        \
  /** Calls Linux pidfd_open(pid_t pid, unsigned int flags). See x86_64_434 in \
   * the source table. */                                                      \
  X(434, pidfd_open, 2, NORMAL, sigma_sys_result_t, IPTR,                      \
    (syscall_arg_t, pid), (u32, flags))                                        \
  /** Calls Linux clone3(struct clone_args *uargs, size_t size). See           \
   * x86_64_435 in the source table. */                                        \
  X(435, clone3, 2, NORMAL, sigma_sys_result_t, IPTR, (void *, uargs),         \
    (usize, size))                                                             \
  /** Calls Linux close_range(unsigned int fd, unsigned int max_fd, unsigned   \
   * int flags). See x86_64_436 in the source table. */                        \
  X(436, close_range, 3, NORMAL, sigma_sys_result_t, IPTR, (u32, fd),          \
    (u32, max_fd), (u32, flags))                                               \
  /** Calls Linux faccessat2(int dfd, const char *filename, int mode, int      \
   * flags). See x86_64_439 in the source table. */                            \
  X(439, faccessat2, 4, NORMAL, sigma_sys_result_t, IPTR, (i32, dfd),          \
    (const void *, filename), (i32, mode), (i32, flags))
