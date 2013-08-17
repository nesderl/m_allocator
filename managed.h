/*
 * memory.h
 *
 *  Created on: 4 janv. 2013
 *      Author: Pimpy
 */

#ifndef MEMORY_H_
#define MEMORY_H_

#ifdef NDEBUG
	#define malloc(size) 		xmalloc(size, __FILE__, __LINE__)
	#define free(ptr) 			xfree(ptr)
#else
	#define malloc(size) 		malloc(size)
	#define free(ptr) 			free(ptr)
#endif

void *xmalloc(long unsigned int size, const char* file, int line);

void xfree(void* ptr);


#endif /* MEMORY_H_ */
