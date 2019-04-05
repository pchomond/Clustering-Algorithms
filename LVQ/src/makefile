src = $(wildcard *.c)
obj = $(src:.c=.o)

LDFLAGS = -lGL -lGLU -lglut -lm

lvq: $(obj)
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) lvq
