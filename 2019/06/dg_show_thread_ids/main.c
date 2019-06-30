#include <sys/kinfo.h>
#include <sys/sysctl.h>
#include <sys/types.h>
#include <fcntl.h>
#include <kvm.h>

#include <err.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        errx(1, "Usage: dg_show_thread_ids pid");
    }

    char errbuf[_POSIX2_LINE_MAX];
    kvm_t* kd = kvm_openfiles(NULL, NULL, NULL, O_RDONLY, errbuf);
    if (!kd)
    {
        errx(1, "%s", errbuf);
    }

    errno = 0;
    char *endptr = NULL;
    long pid = strtol(argv[1], &endptr, 10);
    if (errno || (*endptr != '\0') || (pid > INT_MAX))
    {
        errx(1, "Invalid pid: %s", argv[1]);
    }

    int cnt = 0;
    struct kinfo_proc *proc = kvm_getprocs(
                                kd,
                                KERN_PROC_PID | KERN_PROC_FLAG_LWP,
                                (int)(pid),
                                &cnt);
    if (!proc)
    {
        errx(1, "%s", kvm_geterr(kd));
    }
    else if (!cnt)
    {
        errx(1, "%s doesn't exist", argv[1]);
    }

    printf("Process (%s) has %d threads, and LWP IDs are:\n", argv[1], cnt);
    for (int i = 0; i < cnt; i++)
    {
        printf("%d\n",  proc[i].kp_lwp.kl_tid);
    }

    kvm_close(kd);
    return 0;
}