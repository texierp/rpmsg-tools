OBJ = rpmsg-test.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $<

rpmsg-test: $(OBJ)
	$(CC) ${LDFLAGS} -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f rpmsg-test $(OBJ) 
