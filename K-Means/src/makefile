src = $(wildcard *.c)
obj = $(src:.c=.o)

LDFLAGS = -lGL -lGLU -lglut -lm

kmeans: $(obj)
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) kmeans
