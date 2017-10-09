CFLAGS=-I.
OBJ = rpmsg-test.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

rpmsg-test: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f rpmsg-test $(OBJ) 
