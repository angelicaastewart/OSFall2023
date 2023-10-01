signal: signal.c
	gcc signal.c -o signalab -std=c99

clean:
	rm -f signal