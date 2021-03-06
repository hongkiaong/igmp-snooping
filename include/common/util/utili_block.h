/*******************************************************************************
  [File name]	: utili_avlt.h
  [Author]     	: bain.wang@outlook.com
  [Version]    	: 1.0
  [Date]       	: 2017-04-04
  [Description]	:
	define avlt api
  [Others]     	:
	NULL
  [Function List]:  
    1. ....
    2. ....
  [History]:
	 Date       	Modification 							 Initials
	---------- 	--------------                                        ----------
	2017-04-04  	Created								 bain.wang@outlook.com
*******************************************************************************/

#ifndef _UTILI_BLOCK_H_
#define _UTILI_BLOCK_H_

#include "bc_common_defs.h"

typedef struct _utili_block_list_t
{
	struct _utili_block_list_t		*next;
} utili_block_list_t;

/* red-black tree struct */
typedef struct
{
	/* list for block node management  */
	utili_block_list_t		*list_head;			/* the freelist head */
	void					*base;				/* the start address of memory */
	void					*end;				/* the end address of memory */
	bc_uint32				node_num;			/* total node number */
	bc_uint32				free_count;			/* free counter */
} utili_block_t;


/*************************************************
MACRO NAME:
	UTILI_BLOCK_IS_EMPTY

DESCRIPTION:
	check a the block is empty

INPUT:
	block		: the block struct pointer

RETURN:
	TRUE		:  empty
	FALSE		:  not empty

*************************************************/
#define UTILI_BLOCK_IS_EMPTY(_block)		((_block)->list_head == NULL)

/*************************************************
FUNCTION NAME:
	utili_block_init

DESCRIPTION:
	Initialize the red-black tree node array as free-list

INPUT:
	block		: the block struct pointer
	nodes		: the node array start pointer
	node_size	: the node size (include red-black node and real-data)
	nodes_num	: the number of nodes

*************************************************/
void utili_block_init(utili_block_t *block, void *nodes, bc_uint32 node_size, bc_uint32 nodes_num);

/*************************************************
FUNCTION NAME:
	utili_block_alloc

DESCRIPTION:
	allocate a node from nodes free-list

INPUT:
	block		: the block struct pointer

RETURN:
	NULL		: no node for allocatte
	pointer		: a free node

*************************************************/
void  *utili_block_alloc(utili_block_t *block);

/*************************************************
FUNCTION NAME:
	utili_block_free

DESCRIPTION:
	free a node

INPUT:
	tree			: the block struct pointer
	ptr			: the node pointer

*************************************************/
void utili_block_free(utili_block_t *block, void *ptr);

#endif

