CC=gcc 

snfr_d: snfr_d.o
#cli_snfr: cli_snfr.o

clean:
	rm -f snfr snfr.o
	rm -f /sock_file
	rm -f /outlog
