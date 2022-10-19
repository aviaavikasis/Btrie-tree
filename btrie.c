#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */
#include <stdio.h>
#include <string.h> /* memset */

#include "btrie.h"

#define CHILDRENS_NUM 2
#define INT(input) input - 48
#define ZERO 48


typedef struct node
{
	struct node *parent;
	bool_ty is_full;
	struct node *childrens[CHILDRENS_NUM];
}node_ty;

struct btrie
{
	node_ty *root;
};

typedef enum
{
	LEFT = 0,
	RIGHT = 1
}btrie_children_ty;



/* getters and setters */
static node_ty *GetLeft(node_ty *node);
static node_ty *GetRight(node_ty *node);
static void SetLeft(node_ty *parent, node_ty *child);
static void SetRight(node_ty *parent, node_ty *child);
static node_ty *GetRoot(btrie_ty *btrie);
static node_ty **GetChildrens(node_ty *node);
static void SetIsFull(node_ty *node, bool_ty status);

/* help static functions */
static node_ty *CreateNode(node_ty *parent);
static status_ty RecursiveInsert(node_ty *root, const char *exp, int new_path);
/*static bool_ty UpdateIsFull(node_ty *root);*/
static bool_ty HaveOneChildren(node_ty *node);
static bool_ty IsLeaf(node_ty *node);
static bool_ty HaveTwoChildrens(node_ty *node);
static void DestroyNode(node_ty *node);
static btrie_children_ty WhichSide(node_ty *child, node_ty *parent);
static status_ty RecursiveRemove(node_ty *root, const char *exp);
static bool_ty IsFull(node_ty *node);


/*----------- getters and setters------------*/

static node_ty *GetLeft(node_ty *node)
{
	assert(NULL != node);
	return (node->childrens[LEFT]);
}

static node_ty *GetRight(node_ty *node)
{
	assert(NULL != node);
	return (node->childrens[RIGHT]);
}


static void SetLeft(node_ty *parent, node_ty *child)
{
	assert(NULL != parent);
	
	parent->childrens[LEFT] = child;
}

static void SetRight(node_ty *parent, node_ty *child)
{
	assert(NULL != parent);
	
	parent->childrens[RIGHT] = child;
}

static void SetIsFull(node_ty *node, bool_ty status)
{
	assert(NULL != node);
	
	node->is_full = status;
}


static node_ty *GetRoot(btrie_ty *btrie)
{
	assert(NULL != btrie);
	
	return btrie->root;
}

static node_ty **GetChildrens(node_ty *node)    
{
	assert(NULL != node);
	
	return node->childrens;
}

static node_ty *GetParent(node_ty *node)
{
	assert(NULL != node);
	
	return node->parent;
}


/*-------------- BTrie functions --------------*/


btrie_ty *BTrieCreate(void)
{
	btrie_ty *btrie = (btrie_ty *)malloc(sizeof(btrie_ty));
	if(NULL == btrie)
	{
		return NULL;
	}
	
	btrie->root = CreateNode(NULL);
	if(NULL == btrie->root)
	{
		free(btrie);
		btrie = NULL;
		return NULL;
	}

	return btrie;
}


static void RecursionDestroy(node_ty *root)
{
	if(NULL != GetLeft(root))
	{
		RecursionDestroy(GetLeft(root));
	}	
	if(NULL != GetRight(root))
	{
		RecursionDestroy(GetRight(root));
	}
	DestroyNode(root);
	
}


void BTrieDestroy(btrie_ty *btrie)
{
	assert(NULL != btrie);

	RecursionDestroy(GetRoot(btrie));
	free(btrie);
	btrie = NULL;
}


static status_ty RecursiveInsert(node_ty *root, const char *exp, int new_path)
{
	status_ty status = 0;
	
	if('\0' == *exp && TRUE == new_path) 
	{
		SetIsFull(root, TRUE);
		return SUCCESS;
	}
	
	if('\0' == *exp && FALSE == new_path)
	{
		return FAIL;
	}
	
	if(NULL == GetChildrens(root)[INT(*exp)])
	{
		GetChildrens(root)[INT(*exp)] = CreateNode(root);
		if(NULL == GetChildrens(root)[INT(*exp)])
		{
			return FAIL;
		}

		new_path = TRUE;
	}
	
	status = RecursiveInsert(GetChildrens(root)[INT(*exp)], exp + 1, new_path);
	
	if(SUCCESS == status)
	{
		if(IsFull(GetLeft(root)) && IsFull(GetRight(root)))
		{
			SetIsFull(root, TRUE);
		}
	}
	
	return status;
	
}


status_ty BTrieInsert(btrie_ty *btrie, const char *expression)
{
	assert(NULL != btrie);
	assert(NULL != expression);
	
	return RecursiveInsert(GetRoot(btrie), expression, FALSE);
	
}


static status_ty RecursiveRemove(node_ty *root, const char *exp)
{
	assert(NULL != root);
	
	if('\0' != *exp)
	{
		
		if(NULL != GetChildrens(root)[INT(*exp)])
		{
			return RecursiveRemove(GetChildrens(root)[INT(*exp)], exp + 1);	
		}
		else
		{
			return FAIL;
		}
	}
	
	if(IsLeaf(root))
	{
		GetChildrens(GetParent(root))[WhichSide(root, GetParent(root))] = NULL;
		free(root);
	}
	else
	{
		SetIsFull(root, FALSE);
	}
	
	return SUCCESS;
}


status_ty BTrieRemove(btrie_ty *btrie, const char *exp)
{
	assert(NULL != btrie);
	assert(NULL != exp);
	
	return RecursiveRemove(GetRoot(btrie), exp);
}


return_status_ty RecursiveFindStr(node_ty *root, char *result, char *request)
{
	status_ty insert_status = 0;
	return_status_ty ret_status = 0;
	
	if('\0' != *request && NULL != GetChildrens(root)[INT(*request)]
					 && !IsFull(GetChildrens(root)[INT(*request)]))
	{
		*result = *request;
		ret_status = RecursiveFindStr(GetChildrens(root)[INT(*request)],
												 result + 1, request + 1);
		return ret_status;
	}

	else if(IsFull(GetChildrens(root)[INT(*request)]))
	{
		if(0 == INT(*request) && NULL != GetChildrens(root)[1] &&
									 !IsFull(GetChildrens(root)[1]))
		{
			*result = '1';	
			ret_status = RecursiveFindStr(GetChildrens(root)[INT(*result)],
													 result + 1, request + 1);
			return ret_status;
		}
		
		else if (0 == INT(*request) && NULL == GetChildrens(root)[1])
		{
			*result = '1';
			memset(result + 1, ZERO, strlen(request + 1));
			return ALLOCATED_AVAILABLE_ADDRESS;
		}
	}
	
	else if ('\0' != *request && NULL == GetChildrens(root)[INT(*request)])
	{
		strcpy(result, request);
		return ALLOCATED_REQUSTED_ADDRESS;
	}
		
}


return_status_ty BTrieFindEqualOrLargeStr(btrie_ty *btrie, char *o_str, char*expression)
{
	assert(NULL != btrie);
	assert(NULL != expression);
	
	if(IsFull(GetRoot(btrie)))
	{
		return FAILED_TO_ALLOCATE;
	}
	
	return RecursiveFindStr(GetRoot(btrie), o_str, expression);

}

static size_t RecursiveSize(node_ty *root)
{
	
	if(NULL == root)
	{
		return 0;
	}
	
	if(IsLeaf(root))
	{
		return 1;
	}
	
	return RecursiveSize(GetLeft(root)) + RecursiveSize(GetRight(root));

}


size_t BTrieSize(const btrie_ty *btrie)
{
	assert(NULL != btrie);
	
	if(IsLeaf(btrie->root))
	{
		return 0;
	}
	
	return  RecursiveSize(btrie->root);

}



/*--------------- Static functions ------------*/

static node_ty *CreateNode(node_ty *parent)
{
	node_ty *new_node = (node_ty *)malloc(sizeof(node_ty));
	if(NULL == new_node)
	{
		return NULL;
	}
	
	new_node->parent = parent;
	new_node->is_full = FALSE;
	SetLeft(new_node, NULL);
	SetRight(new_node, NULL);
	
	return new_node;
}

static void DestroyNode(node_ty *node)
{
	SetLeft(node, NULL);
	SetRight(node, NULL);
	
	free(node);
	node = NULL;
}

static bool_ty HaveOneChildren(node_ty *node)
{
	assert(NULL != node);
	
	return ((NULL == GetLeft(node) && NULL != GetRight(node))
			||
		   (NULL != GetLeft(node) && NULL == GetRight(node)));

}

static bool_ty HaveTwoChildrens(node_ty *node)
{
	assert(NULL != node);
	
	return (NULL != GetLeft(node) && NULL != GetRight(node));
}

static bool_ty IsLeaf(node_ty *node)
{
	assert(NULL != node);
	
	return (NULL == GetLeft(node) && NULL == GetRight(node));
}


static btrie_children_ty WhichSide(node_ty *child, node_ty *parent)
{
	assert(NULL != child);
	assert(NULL != parent);
	
	return (child == GetLeft(parent)) ? LEFT : RIGHT;

}

static bool_ty IsFull(node_ty *node)
{
	
	return (NULL != node && TRUE == node->is_full);
}









