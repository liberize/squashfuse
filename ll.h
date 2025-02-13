/*
 * Copyright (c) 2012 Dave Vasilevsky <dave@vasilevsky.ca>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR(S) ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR(S) BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef SQFS_LL_H
#define SQFS_LL_H

#include "squashfuse.h"

#include "squashfuse_dlopen.h"
#ifndef ENABLE_DLOPEN
#	include <fuse_lowlevel.h>
#endif

typedef struct sqfs_ll sqfs_ll;
struct sqfs_ll {
	sqfs fs;
	
	/* Converting inodes between squashfs and fuse */
	fuse_ino_t (*ino_fuse)(sqfs_ll *ll, sqfs_inode_id i);
	sqfs_inode_id (*ino_sqfs)(sqfs_ll *ll, fuse_ino_t i);
	
	/* Register a new inode, returning the fuse ID for it */
	fuse_ino_t (*ino_register)(sqfs_ll *ll, sqfs_dir_entry *e);
	void (*ino_forget)(sqfs_ll *ll, fuse_ino_t i, size_t refs);
	
	/* Like register, but don't actually remember it */
	fuse_ino_t (*ino_fuse_num)(sqfs_ll *ll, sqfs_dir_entry *e);
	
	/* Private data, and how to destroy it */
	void *ino_data;
	void (*ino_destroy)(sqfs_ll *ll);	
};

sqfs_err sqfs_ll_init(sqfs_ll *ll);
void sqfs_ll_destroy(sqfs_ll *ll);


/* Get an inode from an sqfs_ll */
sqfs_err sqfs_ll_inode(sqfs_ll *ll, sqfs_inode *inode, fuse_ino_t i);

/* Convenience function: Get both ll and inode, and handle errors */
#define SQFS_FUSE_INODE_NONE 0
typedef struct {
	sqfs_ll *ll;
	sqfs_inode inode;
} sqfs_ll_i;
sqfs_err sqfs_ll_iget(fuse_req_t req, sqfs_ll_i *lli, fuse_ino_t i);


int sqfs_ll_daemonize(int fg);

#endif
