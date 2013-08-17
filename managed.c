/*
 * memory.c
 *
 *  Created on: 4 janv. 2013
 *      Author: Pimpy
 */

#include "../headers/managed.h"

#include <stdio.h>
#include <stdlib.h>

//----------------------------------------------------------
// Statics
//----------------------------------------------------------

static struct Node* nodeList;

static char initialized = 0;

static void add(void* nodePtr);
static void delete(void* ptr);

static void tryInitialize();
static void clean();
static void* getDataAdress(struct Node* meta);

//----------------------------------------------------------
// Linked List
//----------------------------------------------------------
struct Node
{
	const char* file;
	int line;

	size_t size;
	struct Node* next;
}; // total size = 16 bytes

static void add(void* nodePtr)
{
	struct Node* node = (struct Node*)nodePtr;

	//printf("Malloc (%p) %d bytes\n", (node+1), node->size);

	node->next = nodeList->next;
	nodeList->next = node;
}

static void delete(void* ptr)
{
	struct Node* it;
	void* nextData;

	for ( it = nodeList; it->next != NULL; it = it->next)
	{
		nextData = getDataAdress(it->next);
		if ( nextData == ptr)
		{
			struct Node* tofree = it->next;
			it->next = it->next->next;

			//printf("Free (%p) %d bytes\n", ptr, tofree->size);

			free(tofree);

			break;
		}
	}
}

static void tryInitialize()
{
	if ( initialized < 1)
	{
		nodeList = malloc(sizeof(struct Node));
		nodeList->next = NULL;

		atexit(clean);
		initialized = 1;
	}
}

static void* getDataAdress(struct Node* meta)
{
	return (meta + 1);
}

//----------------------------------------------------------
// Memory
//----------------------------------------------------------

void* xmalloc(long unsigned int size, const char* file, int line)
{
	tryInitialize();

	void* ptr = malloc(size + sizeof(struct Node));

	if ( ptr != NULL)
	{
		struct Node* next = ptr;

		next->size = size;
		next->file = file;
		next->line = line;

		ptr = getDataAdress(next);
		add(next);
	}
	else
	{
		ptr = NULL;
	}

	return (ptr);
}

void xfree(void* ptr)
{
	if ( ptr != NULL)
	{
		delete(ptr);
	}
}

void clean()
{
	//printf("MEMORY LEAKS REPORT\n");

	// if the tail is NULL, the list is empty and thus there is no memory leak.
	if ( nodeList->next == NULL)
	{
		//printf("\tCongratulations, your program has no memory leaks !\n");
	}
	else
	{
		size_t totalSize = 0;

		while ( nodeList->next != NULL)
		{
			totalSize += nodeList->next->size;
			//printf("\t- %d bytes allocated in file \"%s\", line %d.\n", nodeList->next->size, nodeList->next->file, nodeList->next->line);
			delete( getDataAdress(nodeList->next) );
		}

		//printf("TOTAL LEAKS = %d bytes.\n", totalSize);
	}

	free(nodeList);

	nodeList = NULL;
	initialized = 0;
}
