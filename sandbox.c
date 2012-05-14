#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>

#define __USE_GNU
#include <ucontext.h>

void *readPage = 0x0000000000401000;
void *mainPage = 0x0000000000402000;
long return_address = NULL;

void sig_hand(int signo, siginfo_t * si, void *context)
{
    //printf("Inside sighandler\n");
    ucontext_t *ucp = (ucontext_t *) context;
    void *curr_rbp = *(void **) (ucp->uc_mcontext.gregs[REG_RSP]);
    if (return_address == NULL) {
	return_address = (long) curr_rbp;
	mprotect(readPage, 4096, PROT_READ | PROT_WRITE | PROT_EXEC);
	mprotect(mainPage, 4096, PROT_NONE);
    } else {
	if (return_address != (long) si->si_addr) {
	    printf("malicious buffer overflow detected, will exit!\n");
	    exit(0);
	} else {
	    mprotect(readPage, 4096, PROT_NONE);
	    mprotect(mainPage, 4096, PROT_READ | PROT_WRITE | PROT_EXEC);
	    return_address = NULL;
	}
    }
    return;
}


void init_sandbox()
{
    struct sigaction sa;
    sa.sa_sigaction = sig_hand;
    sa.sa_flags = SA_RESTART | SA_SIGINFO;
    sigaction(SIGSEGV, &sa, NULL);

    sigset_t ss;
    sigemptyset(&ss);
    sigaddset(&ss, SIGSEGV);
    sigprocmask(SIG_UNBLOCK, &ss, NULL);
    mprotect(readPage, 4096, PROT_NONE);
}
