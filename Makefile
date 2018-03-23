CC=mpicc
CFLAGS=-O2 -Wno-unused-result
INCLUDEDIR   = -I./ 
LDFLAGS      = -lgsl -lgslcblas

diagonalization.o: diagonalization.c  
	${CC} ${CFLAGS} ${INCLUDEDIR}  $*.c -c

GramSchmidt.o: GramSchmidt.c dot.o  
	${CC} ${CFLAGS} ${INCLUDEDIR}  $*.c -c


davidson.o: diagonalization.o GramSchmidt.o dot.o davidson.c  
	${CC} ${CFLAGS} ${INCLUDEDIR}  $*.c -c

dot.o: dot.c  
	${CC} ${CFLAGS} ${INCLUDEDIR}  $*.c -c

davidson_drv: davidson_drv.c davidson.o GramSchmidt.o diagonalization.o dot.o
	${CC} ${CFLAGS} ${INCLUDEDIR}   diagonalization.o davidson.o dot.o GramSchmidt.o davidson_drv.c -o $@ $(CPPFLAGS) $(LDFLAGS)  $(LAPACKLIB) $(BLASLIB) $(F2CLIB) -lm
