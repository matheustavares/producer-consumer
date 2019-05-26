#ifndef _PCFIFO_H
#define _PCFIFO_H

#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>

/*
 * pcfifo size is 2^16. This way, head and tail can be declared as
 * uint16_t, and we don't have to be update them with modulus operator.
 * (We use the overflow as modulus)
 */
#define PCFIFO_BUF_SIZE (1 << 16)

struct pcfifo {
	void *buffer[PCFIFO_BUF_SIZE];
	uint16_t head, tail;
	sem_t empty;
	sem_t full;
	pthread_mutex_t lock;
};

/* Initialize an empty pcfifo */
void pcfifo_init(struct pcfifo *pcf);

/*
 * Destroy the pcfifo, freeing memory.
 * If destroyer is not NULL, it will be invoked for every element
 * that still resides on the fifo, so that it can be properly freed.
 *
 * Note: This function should not be called if producers or consumers
 * are still waiting at pcfifo_put and pcfifo_get, respectivelly.
 */
void pcfifo_destroy(struct pcfifo *pcf, void *(*destroyer) (void *));

/*
 * Return the next element at pcfifo to be consumed
 * The calling thread will block until there is an element at the fifo
 * to be consumed.
 */
void *pcfifo_get(struct pcfifo *pcf);

/*
 * Put a produced element in the pcfifo
 * The calling thread will block until there is an empty slot at the
 * fifo.
 */
void pcfifo_put(struct pcfifo *pcf, void *element);

#endif
