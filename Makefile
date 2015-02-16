CC=gcc

SRCDIR=src/
BINDIR=bin/

CFLAGS=-O2 -Wall -std=c99 -I$(SRCDIR)/

PROGNAME=spedloader

$(BINDIR)/$(PROGNAME):	$(BINDIR)/main.o $(BINDIR)/serport.o $(BINDIR)/romparser.o $(BINDIR)/supered.o
	$(CC) $(CFLAGS)	$(BINDIR)/main.o $(BINDIR)/serport.o $(BINDIR)/romparser.o $(BINDIR)/supered.o -o $(BINDIR)/$(PROGNAME) 

$(BINDIR)/main.o:	$(SRCDIR)/main.c
	$(CC) $(CFLAGS)	$(SRCDIR)/main.c -c -o $(BINDIR)/main.o

$(BINDIR)/romparser.o:	$(SRCDIR)/romparser/romparser.c
	$(CC) $(CFLAGS)	$(SRCDIR)/romparser/romparser.c -c -o $(BINDIR)/romparser.o

$(BINDIR)/serport.o:	$(SRCDIR)/serport/serport.c
	$(CC) $(CFLAGS)	$(SRCDIR)/serport/serport.c -c -o $(BINDIR)/serport.o

$(BINDIR)/supered.o:	$(SRCDIR)/supered/supered.c
	$(CC) $(CFLAGS)	$(SRCDIR)/supered/supered.c -c -o $(BINDIR)/supered.o

clean:
	rm -rf $(BINDIR)/$(PROGNAME)
	rm -rf $(BINDIR)/*.o

