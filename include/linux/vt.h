#include <sys/cdefs.h>

#ifndef _LINUX_VT_H_
#define _LINUX_VT_H_

__BEGIN_DECLS

#define VT_OPENQRY	0x5600
#define VT_GETMODE	0x5601
#define VT_SETMODE	0x5602
#define VT_GETSTATE	0x5603
#define VT_SENDSIG	0x5604
#define VT_RELDISP	0x5605
#define VT_ACTIVATE	0x5606
#define VT_WAITACTIVE	0x5607
#define VT_DISALLOCATE	0x5608
#define VT_RESIZE	0x5609
#define VT_RESIZEX	0x560A
#define VT_LOCKSWITCH	0x560B
#define VT_UNLOCKSWITCH	0x560C

#define VT_AUTO		0x00
#define VT_PROCESS	0x01
#define VT_ACKACQ	0x02

struct vt_mode {
	char mode;
	char waitv;
	short relsig;
	short acqsig;
	short frsig;
};

struct vt_stat {
	unsigned short v_active;
	unsigned short v_signal;
	unsigned short v_state;
};

struct vt_sizes {
	unsigned short v_rows;
	unsigned short v_cols;
	unsigned short v_scrollsize;
};

struct vt_consize {
	unsigned short v_rows;
	unsigned short v_cols;
	unsigned short v_vlin;
	unsigned short v_clin;
	unsigned short v_vcol;
	unsigned short v_ccol;
};

__END_DECLS

#endif
