#include "nb_ldpc_toolbox.h"

#define STR_MAXSIZE 256

int main(int argc, char * argv[]) {

	printf("argc is %d and argv is ", argc);
	for (int i = 0; i < argc; i++) {
		printf("%s ", argv[i]);
	}
	printf("\n");

    if(argc < 2){
        fprintf(stderr, "Wrong use. No alist file provided.\n use `file-generator </path/to/alist.txt>`\n");
        exit(EXIT_FAILURE);
    }else if(argc > 2)
    {
        fprintf(stderr, "Wrong use. More than one argument is not supported.\n use `file-generator </path/to/alist.txt>`\n");
        exit(EXIT_FAILURE);
    }

	code_t	  code;
	table_t	  table;

	char FileMatrix[STR_MAXSIZE];

	strncpy(FileMatrix, argv[1], STR_MAXSIZE);

	LoadCode2Dump(FileMatrix, &code);
	LoadTables2Dump(&table, code.GF, code.logGF);
	GaussianElimination(&code, &table);

	DumpEncoder(&code);
	DumpHlayered(&code);
	DumpConstants(&code);

    FreeTable(&table);
    FreeCode(&code);

	return EXIT_SUCCESS;
}