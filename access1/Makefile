
OBJS = list_entry.o

%.o: %.c
	gcc -c $<

sample:$(OBJS)
	gcc -o $@ $(OBJS)

clean:
	rm -f *.o *.gch sample
