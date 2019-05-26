# PCFIFO - A C Producer-Consumer FIFO

This is a simple C lib to be used as a FIFO for producer-consumer
implementations. It allows multiple producers and/or consumers.

## Usage

1) Copy `pcfifo.c` and `pcfifo.h` to your project's directory
2) Include `pcfifo.h` where you want to use the API
3) Include pcfifo in your Makefile and don't forget to use `-lpthread` when
linking

## Example

A tipical code that uses pcfifo would be implemented as:

```c
struct pcfifo pcf;

void producer()
{
        while (wants_to_produce) {
                element_t *e = produce();
                pcfifo_put(&pcf, e);
        }

	// Tell consumer threads to stop
        for (each consumer thread)
                pcfifo_put(&pcf, NULL);
}

void consumer()
{
        while(1) {
                element_t *e = pcfifo_get(&pcf);

                if (e == NULL) //Nothing more to consume
                        return;

                consume(e);
        }
}

int main() {
        pcfifo_init(&pcf);

        for(each producer)
                pthread_create(producer);

        for(each consumer)
                pthread_create(consumer);

	join_all_threads();
}
```

### Authors

Written by [Matheus Tavares](https://github.com/matheustavares) and
[Giuliano Belinassi](https://github.com/giulianobelinassi/)
