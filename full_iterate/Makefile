
OBJS = mysample.o

%.o: %.c
	gcc -c -o $@ $<

sample:$(OBJS)
	gcc -o $@ $^

clean:
	rm -f *.o *.gch sample
