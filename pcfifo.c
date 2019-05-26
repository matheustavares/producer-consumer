#include "pcfifo.h"
#include <stdint.h>
#include <errno.h>

void pcfifo_init(struct pcfifo *pcf)
{
	pcf->head = 0;
	pcf->tail = 0;
	sem_init(&pcf->empty, 0, PCFIFO_BUF_SIZE);
	sem_init(&pcf->full, 0, 0);
	pthread_mutex_init(&pcf->lock, NULL);
}

void pcfifo_destroy(struct pcfifo *pcf, void *(*destroyer)(void *))
{
	pthread_mutex_destroy(&pcf->lock);
	sem_destroy(&pcf->full);
	sem_destroy(&pcf->empty);

	if (destroyer) {
		for (uint16_t i = pcf->tail; i != pcf->head; ++i)
			destroyer(pcf->buffer[i]);
	}
}

void *pcfifo_get(struct pcfifo *pcf)
{
	void *ret;

	sem_wait(&pcf->full);
	pthread_mutex_lock(&pcf->lock);
	ret = pcf->buffer[pcf->tail];
	pcf->tail++;
	pthread_mutex_unlock(&pcf->lock);
	sem_post(&pcf->empty);

	return ret;
}

void pcfifo_put(struct pcfifo *pcf, void *element)
{
	sem_wait(&pcf->empty);
	pthread_mutex_lock(&pcf->lock);
	pcf->buffer[pcf->head] = element;
	pcf->head++;
	pthread_mutex_unlock(&pcf->lock);
	sem_post(&pcf->full);
}
