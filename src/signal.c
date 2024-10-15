#include <signal.h>

#define SIG_BLOCK	0
#define SIG_UNBLOCK	1
#define SIG_SETMASK	2

#define SA_NOCLDSTOP	1
#define SA_NOCLDWAIT	2
#define SA_SIGINFO	4
#define SA_ONSTACK	0x08000000
#define SA_RESTART	0x10000000
#define SA_NODEFER	0x40000000
#define SA_RESETHAND	0x80000000
#define SA_RESTORER	0x04000000

int sigreturn(unsigned long n);

struct ksa {
	void *handler;
	unsigned long flags;
	int (*restorer)(unsigned long n);
	sigset_t mask;
};

int __sigaction(int sig, const struct sigaction *sa, struct sigaction *old)
{
	struct ksa ksa, _ksa;
	if (sa) {
		ksa.handler = sa->sa_handler;
		ksa.flags = sa->sa_flags | SA_RESTORER;
		ksa.restorer = sigreturn;
		ksa.mask = sa->sa_mask;
	}

	if (sigaction(sig, sa ? (void*) &ksa : 0, old ? (void*) &_ksa : 0, 8))
		return -1;
	if (old) {
		old->sa_handler = _ksa.handler;
		old->sa_flags = _ksa.flags;
		old->sa_mask = _ksa.mask;
	}
	return 0;
}

sighandler_t signal(int sig, sighandler_t func)
{
	struct sigaction sa = {.sa_handler = func, .sa_flags = SA_RESTART};
	if (__sigaction(sig, &sa, &sa) < 0)
		return SIG_ERR;
	return sa.sa_handler;
}
