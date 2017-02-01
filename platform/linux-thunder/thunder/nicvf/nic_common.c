/***********************license start***************
 * Copyright (c) 2003-2014  Cavium Inc. (support@cavium.com). All rights
 * reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *
 *   * Neither the name of Cavium Inc. nor the names of
 *     its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written
 *     permission.
 *
 * This Software, including technical data, may be subject to U.S. export  control
 * laws, including the U.S. Export Administration Act and its  associated
 * regulations, and may be subject to export or import  regulations in other
 * countries.
 *
 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 * AND WITH ALL FAULTS AND CAVIUM INC. MAKES NO PROMISES, REPRESENTATIONS OR
 * WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO
 * THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY REPRESENTATION OR
 * DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT DEFECTS, AND CAVIUM
 * SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES OF TITLE,
 * MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF
 * VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
 * CORRESPONDENCE TO DESCRIPTION. THE ENTIRE  RISK ARISING OUT OF USE OR
 * PERFORMANCE OF THE SOFTWARE LIES WITH YOU.
 ***********************license end**************************************/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <errno.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <stddef.h>
#include <assert.h>
#include <pthread.h>

#include "thunder/nicvf/nic.h"
#include "thunder/nicvf/nic_queues.h"
#include "odp_shm_internal.h"
#include "odp/api/system_info.h"

#include "thunder/nicvf/nic_common.h"

int nic_dma_map(struct nicvf *nic, void* uva, size_t size, uint64_t *iova)
{
	return nic->nicvf_ops->nic_dma_map(nic, uva, size, iova);
}

int nic_dma_unmap(struct nicvf *nic, void* uva, size_t size, uint64_t iova)
{
	return nic->nicvf_ops->nic_dma_unmap(nic, uva, size, iova);
}

int nic_dma_free(void* mem)
{
	odp_shm_free_addr(mem);
	return 0;
}

void* nic_dma_alloc(size_t size, size_t aligment, const char *name)
{
	DBG("%s: size=%zu aligment=%zu\n", __func__, size, aligment);

	odp_shm_t shm = odp_shm_reserve(name, size, aligment, 0);
	if (shm == ODP_SHM_INVALID)
		return NULL;
	return odp_shm_addr(shm);
}

void* nic_mem_alloc(size_t size)
{
	DBG("%s: size=%lu\n", __func__, size);

	odp_shm_t shm = odp_shm_reserve(NULL, size, odp_sys_page_size(), ODP_SHM_SW_ONLY);
	if (shm == ODP_SHM_INVALID)
		return NULL;
	return odp_shm_addr(shm);
}

void nic_mem_free(void *mem)
{
	odp_shm_free_addr(mem);
}

