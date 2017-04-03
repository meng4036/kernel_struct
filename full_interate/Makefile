
OBJS = mysample.o

%.o: %.c
	gcc -c $<

sample:$(OBJS)
	gcc -o $@ $(OBJS)

clean:
	rm *.o *.gch sample
