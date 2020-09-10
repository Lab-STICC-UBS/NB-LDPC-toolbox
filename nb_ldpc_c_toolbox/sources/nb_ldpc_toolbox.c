#include "nb_ldpc_toolbox.h"

#include <errno.h>
#include <error.h>
#include <math.h>

#define STR_MAXSIZE 512

static void check_l_f(int value, int expected, const FILE * f, int line, const char * file) {
	if (value < expected) {
		if (value == EOF) {
			if (f != NULL) {
				perror(NULL);
			} else {
				fprintf(stderr, "Error line %d in file %s: File not opened.", line, file);
			}
		} else {
			fprintf(stderr, "Error line %d in file %s: Not enough items have been read.", line, file);
		}

		exit(EXIT_FAILURE);
	}
}
#define check(value, expected, f) check_l_f(value, expected, f, __LINE__, __FILE__)

/**
 * \fn LoadCode
 * \brief Open a parity-check matrix file. Allocate and initialize the code structures
 * Parameters    :
 * Inputs        :
 * 	- FileMatrix  : Name of the parity-check matrix file
 * 	- code_t code : Structure that describes the code
 * Outputs       :
 */
void LoadCode2Dump(const char * FileMatrix, code_t * code) {
	int	   M		= 0;
	int	   N		= 0;
	int	   GF		= 0;
	int	   logGF	= 0;
	int	   dvMax	= 0;
	int	   dcMax	= 0;
	int	   n		= 0;
	int	   m		= 0;
	int	   k		= 0;
	char * FileName = malloc(STR_MAXSIZE);
	double temp		= 0.0;

	// Load the files corresponding to code (graph, size, GF)
	strncpy(FileName, FileMatrix, STR_MAXSIZE - 1);
	FILE * f = fopen(FileName, "r");
	if (f == NULL) {
		printf("Erreur a l'ouverture du fichier... (%s)\n", FileMatrix);
		exit(EXIT_FAILURE);
	}

	check(fscanf(f, "%d", &N), 1, f);
	check(fscanf(f, "%d", &M), 1, f);
	check(fscanf(f, "%d", &GF), 1, f);
	check(fscanf(f, "%d", &dvMax), 1, f);
	check(fscanf(f, "%d", &dcMax), 1, f);
	temp		= log((double)(GF));
	temp		= temp / log((double)2.0);
	temp		= rint(temp);
	logGF		= (int)temp;
	code->N		= N;
	code->M		= M;
	code->K		= N - M;
	code->rate	= (float)(N - M) / N;
	code->GF	= GF;
	code->logGF = logGF;
	code->dvMax = dvMax;
	code->dcMax = dcMax;

	printf("Parameters [N = %d, M = %d, NmK = %d, GF = %d, dv max = %d, dc max = %d]\n", N, M, N - M, GF, dvMax, dcMax);

	code->columnDegree = calloc(N, sizeof(int));
	code->rowDegree	   = calloc(M, sizeof(int));

	if (strstr(FileName, ".ref") == NULL) {

		for (n = 0; n < N; n++) {
			check(fscanf(f, "%d", &code->columnDegree[n]), 1, f);
		}
		for (n = 0; n < N; n++) {
			printf("%d, ", code->columnDegree[n]);
		}
		printf("\n");

		for (m = 0; m < M; m++) {
			check(fscanf(f, "%d", &code->rowDegree[m]), 1, f);
		}
		for (m = 0; m < M; m++) {
			printf("%d, ", code->rowDegree[m]);
		}
		printf("\n");

	} else {

		int rowdeg, coldeg;
		check(fscanf(f, "%d", &coldeg), 1, f);
		check(fscanf(f, "%d", &rowdeg), 1, f);
		for (int n = 0; n < N; n++) {
			code->columnDegree[n] = coldeg;
		}
		for (int m = 0; m < M; m++) {
			code->rowDegree[m] = rowdeg;
		}
	}

	code->mat = calloc(M, sizeof(int *));
	if (code->mat == NULL) {
		fprintf(stderr, "%s:%d > calloc failed !", __FILE__, __LINE__);
		exit(EXIT_FAILURE);
	}
	for (m = 0; m < M; m++) {
		code->mat[m] = calloc(code->rowDegree[m], sizeof(int));
		if ((code->mat[m] == NULL) || (code->mat[m][0] + code->mat[m][1] + code->mat[m][2] != 0)) {
			fprintf(stderr, "%s:%d > calloc failed !", __FILE__, __LINE__);
			exit(EXIT_FAILURE);
		}
	}
	code->matValue = calloc(M, sizeof(int *));
	for (m = 0; m < M; m++) {
		code->matValue[m] = calloc(code->rowDegree[m], sizeof(int));
		if (code->matValue[m] == NULL) {
			fprintf(stderr, "%s:%d > calloc failed !", __FILE__, __LINE__);
			exit(EXIT_FAILURE);
		}
	}

	if (strstr(FileName, ".kn") != NULL) {
		printf(" \n KN alist format is used! \n");
		for (m = 0; m < M; m++) {
			for (k = 0; k < code->rowDegree[m]; k++) {
				check(fscanf(f, "%d", &code->mat[m][k]), 1, f);
				code->mat[m][k] -= 1; // Pour E. Boutillon
			}
		}

		for (m = 0; m < M; m++)
			for (k = 0; k < code->rowDegree[m]; k++) {
				check(fscanf(f, "%d", &code->matValue[m][k]), 1, f);
				code->matValue[m][k] += 1; // Pour E. Boutillon
			}

	} else if (strstr(FileName, ".matlab") != NULL) {

		printf(" \n MATLAB alist format is used! \n");
		for (m = 0; m < M; m++) {
			for (k = 0; k < code->rowDegree[m]; k++) {
				check(fscanf(f, "%d", &code->mat[m][k]), 1, f);
				code->mat[m][k] -= 1;
			}
		}
		for (m = 0; m < M; m++)
			for (k = 0; k < code->rowDegree[m]; k++) {
				check(fscanf(f, "%d", &code->matValue[m][k]), 1, f);
			}

	} else if (strstr(FileName, ".ubs.bis") != NULL) {

		printf(" \nUBS alist format is used!\n\n");
		int temp_int;
		for (m = 0; m < M; m++) {
			for (k = 0; k < code->rowDegree[m]; k++) {
				check(fscanf(f, "%d", &temp_int), 1, f);
				assert((temp_int - 1) < N);
				code->mat[m][k] = temp_int - 1;

				check(fscanf(f, "%d", &temp_int), 1, f);
				code->matValue[m][k] = temp_int;

				//                  printf("%d | ", code->matValue[m][k]); fflush( stdout ); //temp_int, GF - temp_int);
				assert(code->matValue[m][k] < GF);
				assert(code->matValue[m][k] != 0);
			}
			printf("\n");
		}

	} else if (strstr(FileName, ".ubs") != NULL) {
		printf(" \nUBS alist format is used!\nIt is as follows:\n");
		int temp_int = 0;
		for (m = 0; m < M; m++) {
			for (k = 0; k < code->rowDegree[m]; k++) {
				check(fscanf(f, "%d", &temp_int), 1, f);
				code->mat[m][k] = temp_int - 1;
				printf("%d ", temp_int - 1);
				check(fscanf(f, "%d", &temp_int), 1, f);
				code->matValue[m][k] = (temp_int + 1); // % 128;
				printf("%d ", temp_int + 1);
			}
			printf("\n");
		}
		printf("\n");
	} else if (strstr(FileName, ".ref") != NULL) {
		printf(" \n REF alist format is used! \n");
		int temp_int;
		for (m = 0; m < M; m++) {
			printf("CN %2d : ", m);
			for (k = 0; k < code->rowDegree[m]; k++) {
				check(fscanf(f, "%d", &temp_int), 1, f);
				code->mat[m][k] = temp_int;

				printf("%2d ", code->mat[m][k]);

				code->matValue[m][k] = rand() % (GF - 6) + 3;
			}
			printf("\n");
		}

	} else if (strstr(FileName, ".ccsds") != NULL) {
		printf(" \n CCSDS alist format is used! \n");
		int temp_int;
		for (m = 0; m < M; m++) {
			for (k = 0; k < code->rowDegree[m]; k++) {
				check(fscanf(f, "%d", &temp_int), 1, f); // POSITION DU CN, USELESS !
				check(fscanf(f, "%d", &temp_int), 1, f);
				code->mat[m][k] = (temp_int - 1); // POSITION DU VN

				check(fscanf(f, "%d", &temp_int), 1, f);
				code->matValue[m][k] = (temp_int + 1);
			}
		}

	} else if (strstr(FileName, ".nasa") != NULL) {
		printf(" \n NASA alist format is used! \n");
		//
		// ON RETRANCHE (1) A LA VALEUR DES COLONNES ET ON AJOUTE
		// (1) A LA VALEUR DES ALPHAS
		//
		int temp_int;
		for (m = 0; m < M; m++) {
			printf("ROW %2d : ", m);
			for (k = 0; k < code->rowDegree[m]; k++) {
				check(fscanf(f, "%d", &temp_int), 1, f);
				code->mat[m][k] = temp_int - 1;

				check(fscanf(f, "%d", &temp_int), 1, f);
				code->matValue[m][k] = (temp_int + 1);

				printf("%2d (%3d) ", code->mat[m][k], code->matValue[m][k]);
			}
			printf("\n");
		}

	} else if (strstr(FileName, ".fake") != NULL) {
		printf(" \n FAKE alist format is used! \n");
		//
		// ON RETRANCHE (1) A LA VALEUR DES COLONNES ET ON AJOUTE
		// (1) A LA VALEUR DES ALPHAS
		//
		int	 temp_int;
		int	 offset = 0;
		char buffer[256];
		for (m = 0; m < M; m++) {
			printf("ROW %2d : ", m);
			for (k = 0; k < code->rowDegree[m]; k++) {
				check(fscanf(f, "%s", buffer), 1, f);
				if (strcmp(buffer, "#") == 0) {
					check(fscanf(f, "%d", &offset), 1, f);
					//printf("Offset = %d\n", offset);
					k = k - 1;
				} else {
					temp_int = atoi(buffer);
					//                    check(fscanf(f,"%d",&temp_int), 1, f);
					code->mat[m][k] = temp_int - 1 + offset;

					check(fscanf(f, "%d", &temp_int), 1, f);
					code->matValue[m][k] = (temp_int + 1);

					printf("%2d (%3d) ", code->mat[m][k], code->matValue[m][k]);
				}
			}
			printf("\n");
		}

	} else {
		printf("Error file format not supported yet !\n");
		exit(0);
	}

	fclose(f);

	code->nbBranch = 0;
	for (m = 0; m < M; m++)
		code->nbBranch += code->rowDegree[m];

	if (code->dcMax == 0) {
		code->dcMax = code->rowDegree[0];
	}

	if (code->dvMax == 0) {
		code->dvMax = code->columnDegree[0];
	}

	free(FileName);

	printf("LDPC code parameters: \n");
	printf(" \t N \t:%d \n \t K \t:%d \n \t M\t:%d \n \t CR\t:%g \n \t GF \t:%d \n \t logGF \t:%d\n", N, N - M, M, code->rate, GF, logGF);
	fflush(stdout);
}

/**
 * \fn LoadCode
 * \brief Open a parity-check matrix file. Allocate and initialize the code structures
 * Parameters    :
 * Inputs        :
 * 	- FileMatrix  : Name of the parity-check matrix file
 * 	- code_t code : Structure that describes the code
 * Outputs       :
 */
void LoadCode(const char * FileMatrix, code_t * code) {
	int	   M;
	int	   N;
	int	   GF;
	int	   logGF;
	int	   dvMax;
	int	   dcMax;
	int	   n;
	int	   m;
	int	   k;
	char   FileName[STR_MAXSIZE];
	double temp;

	FILE * f;
	/*
     * Load the files corresponding to code (graph, size, GF)
     */
	strncpy(FileName, FileMatrix, STR_MAXSIZE - 1);
	f = fopen(FileName, "r");

	if (f == NULL) {
		perror("Can't open the alist.");
		exit(EXIT_FAILURE);
	}

	check(fscanf(f, "%d", &N), 1, f);
	check(fscanf(f, "%d", &M), 1, f);
	check(fscanf(f, "%d", &GF), 1, f);
	check(fscanf(f, "%d", &dvMax), 1, f);
	check(fscanf(f, "%d", &dcMax), 1, f);
	temp		= log((double)(GF));
	temp		= temp / log((double)2.0);
	temp		= rint(temp);
	logGF		= (int)temp;
	code->N		= N;
	code->M		= M;
	code->K		= N - M;
	code->rate	= (float)(N - M) / N;
	code->GF	= GF;
	code->logGF = logGF;
	code->dvMax = dvMax;
	code->dcMax = dcMax;

	code->columnDegree = calloc(N, sizeof(int));
	//if ( code->columnDegree == NULL ) err(EXIT_FAILURE,"%s:%d > malloc failed !",__FILE__,__LINE__);
	for (n = 0; n < N; n++) {
		check(fscanf(f, "%d", &code->columnDegree[n]), 1, f);
	}

	code->rowDegree = calloc(M, sizeof(int));
	//if ( code->rowDegree == NULL ) err(EXIT_FAILURE,"%s:%d > malloc failed !",__FILE__,__LINE__);
	for (m = 0; m < M; m++) {
		check(fscanf(f, "%d", &code->rowDegree[m]), 1, f);
	}

	code->mat = calloc(M, sizeof(int *));
	//if ( code->mat == NULL ) err(EXIT_FAILURE,"%s:%d > malloc failed !",__FILE__,__LINE__);
	for (m = 0; m < M; m++) {
		code->mat[m] = calloc(code->rowDegree[m], sizeof(int));
		//if ( code->mat[m] == NULL ) err(EXIT_FAILURE,"%s:%d > malloc failed !",__FILE__,__LINE__);
	}
	code->matValue = calloc(M, sizeof(int *));
	for (m = 0; m < M; m++) {
		code->matValue[m] = calloc(code->rowDegree[m], sizeof(int));
		//if ( code->matValue [m] == NULL ) err(EXIT_FAILURE,"%s:%d > malloc failed !",__FILE__,__LINE__);
	}

	printf(" \n Normal alist format is used! \n");
	int temp_int;
	for (m = 0; m < M; m++) {
		for (k = 0; k < code->rowDegree[m]; k++) {
			check(fscanf(f, "%d", &temp_int), 1, f);
			code->mat[m][k] = temp_int - 1;
			check(fscanf(f, "%d", &temp_int), 1, f);
			code->matValue[m][k] = temp_int + 1;
		}
	}

	fclose(f);

	code->nbBranch = 0;
	for (m = 0; m < M; m++) {
		code->nbBranch += code->rowDegree[m];
	}

	if (code->dcMax == 0) {
		code->dcMax = code->rowDegree[0];
	}

	if (code->dvMax == 0) {
		code->dvMax = code->columnDegree[0];
	}

	printf("LDPC code parameters: \n");
	printf(" \t N \t:%d \n \t K \t:%d \n \t M\t:%d \n \t CR\t:%g \n \t GF \t:%d \n \t logGF \t:%d\n", N, N - M, M, code->rate, GF, logGF);
	fflush(stdout);
}

void FreeCode(code_t * code) {
	int m;
	for (m = 0; m < code->M; m++) {
		free(code->mat[m]);
		free(code->matValue[m]);
	}
	free(code->matUT[0]);
	free(code->matUT);
	free(code->mat);
	free(code->matValue);
	free(code->rowDegree);
	free(code->columnDegree);
	free(code->Perm);
}

// /*!
//  * \fn Table_Add_GF
//  * \brief Compute the addition table in GF(q)
//  * Parameters    :
//  * Inputs        :
//  * 	- table  : structure containing an allocated pointer to the addition table
//  * 	- int logGF : logGF = log2 (GF)
//  * 	- int GF    : order of the field
//  * Outputs       :
//  */
// static void Table_Add_GF(table_t * table, int GF, int logGF) {
// 	int i, j, k;
// 	int temp[12];

// 	for (j = 0; j < GF; j++) {
// 		for (k = 0; k < GF; k++) {
// 			for (i = 0; i < logGF; i++) {
// 				temp[i] = (table->BINGF[j][i]) ^ (table->BINGF[k][i]);
// 			}
// 			table->ADDGF[j][k] = Bin2GF(temp, GF, logGF, table->BINGF);
// 		}
// 		printf("%d", j);
// 	}
// }

// multiply GF values and output decimal
static void Table_Mul_DEC(table_t * table, int GF) {

	int i, j;
	for (i = 0; i < GF; i++) {
		for (j = 0; j < GF; j++) {
			//table->MULDEC[table->DECGF[i]][table->DECGF[j]]=table->DECGF[table->MULGF[i][j]];
			table->MULDEC[i][j] = table->DECGF[table->MULGF[i][j]];
		}
	}

	//    for(i=0; i<GF; i++)
	//    {
	//        for(j=0; j<GF; j++)
	//        {
	//          printf("%d ",table->MULDEC[i][j]);
	//        }
	//        printf(" \n ");
	//    }
	//    getchar();
}

//divide decimal by GF and output GF
static void Table_Div_DEC(table_t * table, int GF) {

	int i, j;
	for (i = 0; i < GF; i++) {
		for (j = 0; j < GF; j++) {
			//table->DIVDEC[table->DECGF[i]][table->DECGF[j]]=table->DECGF[table->DIVGF[i][j]];
			table->DIVDEC[table->DECGF[i]][j] = table->DIVGF[i][j];
		}
	}

	//    for(i=0; i<GF; i++)
	//    {
	//        for(j=0; j<GF; j++)
	//        {
	//          printf("%d ",table->DIVDEC[i][j]);
	//        }
	//        printf(" \n ");
	//    }
	//    getchar();
}

static void Table_dec_GF(table_t * table, int GF, int logGF) {
	int i, j;

	//bin2dec
	int sum;
	int tmp;
	for (j = 0; j < GF; j++) {
		sum = 0;
		for (i = 0; i < logGF; i++) {
			tmp = table->BINGF[j][i];
			//printf("%d",tmp);
			sum = sum + (tmp << i);
		}
		table->DECGF[j]	  = sum;
		table->GFDEC[sum] = j;
		//printf(" \n bin2dec of GF %d is %d \n",j,sum);
	}
	//getchar();
}

/*!
 * \fn Table_Mul_GF
 * \brief compute the multiplication table in GF(q)
 * Parameters    :
 * Inputs        :
 * 	- table  : structure containing an allocated pointer to the addition table
 * 	- int logGF : logGF = log2 (GF)
 * 	- int GF    : order of the field
 * Outputs       :
 */
static void Table_Mul_GF(int ** MULGF, int GF) {
	int i, j, temp;
	for (i = 0; i < GF; i++) {
		for (j = 0; j < GF; j++) {
			if (i == 0 || j == 0)
				MULGF[i][j] = 0;
			else if (i == 1)
				MULGF[i][j] = j;
			else if (j == 1)
				MULGF[i][j] = i;
			else {
				temp = i + j - 2;
				if (temp < GF - 1)
					MULGF[i][j] = temp + 1;
				else
					MULGF[i][j] = (temp % (GF - 1)) + 1;
			}
		}
	}
	//    for(i=0; i<GF; i++)
	//    {
	//        for(j=0; j<GF; j++)
	//        {
	//          printf("%d ",MULGF[i][j]);
	//        }
	//        printf(" \n ");
	//    }
	//    getchar();
}

/*!
 * \fn Table_Div_GF
 * \brief compute the division table in GF(q)
 * Parameters    :
 * Inputs        :
 * 	- table  : structure containing an allocated pointer to the addition table
 * 	- int logGF : logGF = log2 (GF)
 * 	- int GF    : order of the field
 * Outputs       :
 */
static void Table_Div_GF(int ** DIVGF, int GF) {
	int i;
	int j;
	int nb = GF - 1;

	for (i = 0; i < GF; i++) {
		for (j = 0; j < GF; j++) {
			if (j == 0) {
				DIVGF[i][j] = 0;
			} else if (i == 0) {
				DIVGF[i][j] = 0;
			} else if (j == 1) {
				DIVGF[i][j] = i;
			} else {
				DIVGF[i][j] = nb--;
			};
			if (nb < 1) {
				nb = GF - 1;
			}
		}
	}
	//    for(i=0; i<GF; i++)
	//    {
	//        for(j=0; j<GF; j++)
	//        {
	//          printf("%d ",DIVGF[i][j]);
	//        }
	//        printf(" \n ");
	//    }
	//    getchar();
}

/*!
 * \fn Bin2GF
 * \brief compute a GF(q) symbol corresponding to a frame of log_2(GF) bits
 * Parameters    :
 * Inputs        :
 * 	- int *U    : array representing logGF bits
 * 	- int logGF : size of the array U. logGF = log2 (GF)
 * 	- int GF    : order of the field
 * 	- int ** BINGF: binary mapping table
 * Outputs       :
 *      - index of the non-binary symbol
 */

static int Bin2GF(int * U, int GF, int logGF, int ** BINGF) {
	int k;

	for (k = 0; k < GF; k++) {
		if (memcmp(U, BINGF[k], sizeof(int) * logGF) == 0)
			break;
	}
	return (k);
}

/*!
 * \fn Table_MUL_GF
 * \brief Compute the addition table in GF(q)
 * Parameters    :
 * Inputs        :
 * 	- table  : structure containing an allocated pointer to the addition table
 * 	- int logGF : logGF = log2 (GF)
 * 	- int GF    : order of the field
 * Outputs       :
 */
static void Table_Add_GF(table_t * table, int GF, int logGF) {
	int i, j, k;
	int temp[8];

	for (j = 0; j < GF; j++) {
		for (k = 0; k < GF; k++) {
			for (i = 0; i < logGF; i++) {
				temp[i] = (table->BINGF[j][i]) ^ (table->BINGF[k][i]);
			}
			table->ADDGF[j][k] = Bin2GF(temp, GF, logGF, table->BINGF);
			//            printf("ADD(%2d,%2d) => %2d\n", j, k, table->ADDGF[j][k]);
		}
	}

//#define DUMP_GF
#ifdef DUMP_GF
	printf("const int GF%d_BINGF[%d][%d]=\n{\n", GF, GF, logGF);
	for (j = 0; j < GF; j++) {
		printf("  {");
		for (k = 0; k < logGF; k++) {
			printf("%d", table->BINGF[j][k]);
			if ((k + 1) != logGF)
				printf(", ");
		}
		printf("}");
		if ((j + 1) != GF)
			printf(",\n");
	}
	printf("\n};\n\n");

	//////////////////////////////////////////////////////////////////////////////

	printf("const int GF%d_S2N[%d]=\n{", GF, GF);
	int table_back[256];
	for (j = 0; j < GF; j++) {
		if ((j % 8) == 0)
			printf("\n  ");
		int v = 0;
		for (k = 0; k < logGF; k++) {
			v = (v << 1) + table->BINGF[j][k];
		}
		table_back[v] = j;
		printf("%3d", v);
		if ((j + 1) != GF)
			printf(", ");
	}
	printf("\n};\n\n");

	printf("const int GF%d_N2S[%d]=\n{", GF, GF);
	for (j = 0; j < GF; j++) {
		if ((j % 8) == 0)
			printf("\n  ");
		printf("%3d", table_back[j]);
		if ((j + 1) != GF)
			printf(", ");
	}
	printf("\n};\n\n");

	printf("const int GF%d_ADD[%d][%d]=\n{", GF, GF, GF);
	for (j = 0; j < GF; j++) {
		printf("  {");
		for (k = 0; k < GF; k++) {
			printf("%2d", table->ADDGF[j][k]);
			if ((k + 1) != GF)
				printf(", ");
		}
		printf("}");
		if ((j + 1) != GF)
			printf(",\n");
	}
	printf("\n}\n");

	printf("// SYMBOL INDICE OF PRODUCED DATA\n");
	printf("const int GF%d_ADD_Natural[%d][%d]=\n{", GF, GF, GF);
	unsigned short tab[256][256];
	for (j = 0; j < GF; j++) {
		printf("  {");
		for (k = 0; k < GF; k++) {
			tab[j][k] = j ^ k;
			if (GF < 10)
				printf("%1u", tab[j][k]);
			else if (GF < 100)
				printf("%2u", tab[j][k]);
			else
				printf("%3u", tab[j][k]);
			if ((k + 1) != GF)
				printf(", ");
		}
		printf("}");
		if ((j + 1) != GF)
			printf(",\n");
	}
	printf("\n}\n");

	printf("// SYMBOL INDICE OF PRODUCED DATA\n");
	printf("const int GF%d_ADD_fast_table_avx[%d * %d]={\n", GF, GF, GF);
	for (j = 1; j < GF; j++) {
		printf("  ");
		for (k = 0; k < GF; k++) {
			unsigned int v	 = tab[j][k];
			unsigned int pos = -1;
			for (int z = 0; z < GF; z += 1) {
				if (tab[j - 1][z] == v) {
					pos = z;
					break;
				}
			}

			if (GF < 10)
				printf("%1u", pos);
			else if (GF < 100)
				printf("%2u", pos);
			else
				printf("%3u", pos);
			if ((k + 1) != GF)
				printf(", ");
		}
		printf(",\n");
	}
	{
		printf("  ");
		for (k = 0; k < GF; k++) {
			unsigned int v	 = tab[0][k];
			unsigned int pos = -1;
			for (int z = 0; z < GF; z += 1) {
				if (tab[GF - 1][z] == v) {
					pos = z;
					break;
				}
			}

			if (GF < 10)
				printf("%1u", pos);
			else if (GF < 100)
				printf("%2u", pos);
			else
				printf("%3u", pos);
			if ((k + 1) != GF)
				printf(", ");
		}
	}
	printf("\n}\n");
#endif
}

/**
 * \fn void LoadTables (table_t *table, int GF, int logGF)
 * \brief Memory allocation for the tables and Initialization of the tables.
 * Inputs        :
 * 	- table_t table : Structure that contains pointers to the tables
 * 	- int GF    : order of the field
 * 	- int logGF : logGF = log2(GF)
 * Outputs       :
 */
void LoadTables(table_t * table, int GF, int logGF) {
	int nbRow, nbCol, g, k, l;

	if (GF != 16 && GF != 32 && GF != 64 && GF != 256 && GF != 4096) {
		printf("The binary image of GF(%d) is not available in this version of the program. Please try GF(64) or GF(256)\n", GF);
		exit(EXIT_FAILURE);
	}

	nbRow = GF;
	nbCol = logGF;
	/* BINGF [GF][logGF] */
	table->BINGF = calloc((size_t)nbRow, sizeof(int *));
	//if (table->BINGF  == NULL) err(EXIT_FAILURE,"%s:%d > malloc failed !",__FILE__,__LINE__);
	table->BINGF[0] = calloc((size_t)nbRow * nbCol, sizeof(int));
	//if (table->BINGF [0] == NULL) err(EXIT_FAILURE,"%s:%d > malloc failed !",__FILE__,__LINE__);
	for (k = 1; k < nbRow; k++) {
		table->BINGF[k] = table->BINGF[0] + k * nbCol;
	}

	nbRow = GF;
	nbCol = GF;
	/* ADDGF [GF][GF] */
	table->ADDGF = calloc((size_t)nbRow, sizeof(int *));
	//if (table->ADDGF  == NULL) err(EXIT_FAILURE,"%s:%d > malloc failed !",__FILE__,__LINE__);
	table->ADDGF[0] = calloc((size_t)nbRow * nbCol, sizeof(int));
	//if (table->ADDGF [0] == NULL) err(EXIT_FAILURE,"%s:%d > malloc failed !",__FILE__,__LINE__);
	for (k = 1; k < nbRow; k++) {
		table->ADDGF[k] = table->ADDGF[0] + k * nbCol;
	}

	/*DECGF [GF] */
	table->DECGF = calloc(GF, sizeof(int));
	/*GFDEC [GF] */
	table->GFDEC = calloc(GF, sizeof(int));

	/* MULGF [GF][GF] */
	table->MULGF = calloc((size_t)nbRow, sizeof(int *));
	//if (table->MULGF  == NULL) err(EXIT_FAILURE,"%s:%d > malloc failed !",__FILE__,__LINE__);
	table->MULGF[0] = calloc((size_t)nbRow * nbCol, sizeof(int));
	//if (table->MULGF [0] == NULL) err(EXIT_FAILURE,"%s:%d > malloc failed !",__FILE__,__LINE__);
	for (k = 1; k < nbRow; k++) {
		table->MULGF[k] = table->MULGF[0] + k * nbCol;
	}
	/* DIVGF [GF][GF] */
	table->DIVGF = calloc((size_t)nbRow, sizeof(int *));
	//if (table->DIVGF  == NULL) err(EXIT_FAILURE,"%s:%d > malloc failed !",__FILE__,__LINE__);
	table->DIVGF[0] = calloc((size_t)nbRow * nbCol, sizeof(int));
	//if (table->DIVGF [0] == NULL) err(EXIT_FAILURE,"%s:%d > malloc failed !",__FILE__,__LINE__);
	for (k = 1; k < nbRow; k++) {
		table->DIVGF[k] = table->DIVGF[0] + k * nbCol;
	}

	/* MULDEC [GF][GF] */
	table->MULDEC	 = calloc((size_t)nbRow, sizeof(int *));
	table->MULDEC[0] = calloc((size_t)nbRow * nbCol, sizeof(int));
	for (k = 1; k < nbRow; k++) {
		table->MULDEC[k] = table->MULDEC[0] + k * nbCol;
	}

	/* DIVDEC [GF][GF] */
	table->DIVDEC	 = calloc((size_t)nbRow, sizeof(int *));
	table->DIVDEC[0] = calloc((size_t)nbRow * nbCol, sizeof(int));
	for (k = 1; k < nbRow; k++) {
		table->DIVDEC[k] = table->DIVDEC[0] + k * nbCol;
	}

	if (GF == 16) {
		for (g = 0; g < GF; g++) {
			for (l = 0; l < logGF; l++) {
				table->BINGF[g][l] = BinGF_16[g][l];
			}
		}
		//printf("Loading of the binary image of GF(64): Success\n");
		//fflush(stdout);
	}

	if (GF == 32) {
		for (g = 0; g < GF; g++) {
			for (l = 0; l < logGF; l++) {
				table->BINGF[g][l] = BinGF_32[g][l];
			}
		}
		//printf("Loading of the binary image of GF(64): Success\n");
		//fflush(stdout);
	}

	if (GF == 64) {
		for (g = 0; g < GF; g++) {
			for (l = 0; l < logGF; l++) {
				table->BINGF[g][l] = BinGF_64[g][l];
			}
		}
		//printf("Loading of the binary image of GF(64): Success\n");
		//fflush(stdout);
	}

	if (GF == 256) {
		for (g = 0; g < GF; g++) {
			for (l = 0; l < logGF; l++) {
				table->BINGF[g][l] = BinGF_256[g][l];
			}
		}
		//printf("Loading of the binary image of GF(256): Success\n");
		//fflush(stdout);
	}

	if (GF == 4096) {
		for (g = 0; g < GF; g++) {
			for (l = 0; l < logGF; l++) {
				table->BINGF[g][l] = BinGF_4096[g][l];
			}
		}
		//printf("Loading of the binary image of GF(256): Success\n");
		//fflush(stdout);
	}

	/*
     * Build the addition, multiplication and division tables (corresponding to GF[q])
     */
	// Table_Add_GF(table,GF,logGF);
	Table_dec_GF(table, GF, logGF);
	Table_Mul_GF(table->MULGF, GF);
	Table_Div_GF(table->DIVGF, GF);
	Table_Mul_DEC(table, GF);
	Table_Div_DEC(table, GF);
}

/**
 * \fn void LoadTables (table_t *table, int GF, int logGF)
 * \brief Memory allocation for the tables and Initialization of the tables.
 * Inputs        :
 * 	- table_t table : Structure that contains pointers to the tables
 * 	- int GF    : order of the field
 * 	- int logGF : logGF = log2(GF)
 * Outputs       :
 */
void LoadTables2Dump(table_t * table, int GF, int logGF) {
	int nbRow, nbCol, g, k, l;

	if (GF != 16 && GF != 64 && GF != 32 && GF != 128 && GF != 256) {
		printf("The binary image of GF(%d) is not available in this version of the program. Please try GF(64) or GF(256)\n", GF);
		exit(EXIT_FAILURE);
	}
#if (DEBUG > 0)
	printf("GF configuration = %d\n", GF);
#endif
	nbRow = GF;
	nbCol = logGF;

#if (DEBUG > 0)
	printf("GF Matrix (%d, %d)\n", GF, nbCol);
#endif
	/* BINGF [GF][logGF] */
	table->BINGF = calloc((size_t)nbRow, sizeof(int *));
	//if (table->BINGF  == NULL) err(EXIT_FAILURE,"%s:%d > malloc failed !",__FILE__,__LINE__);
	table->BINGF[0] = calloc((size_t)nbRow * nbCol, sizeof(int));
	//if (table->BINGF [0] == NULL) err(EXIT_FAILURE,"%s:%d > malloc failed !",__FILE__,__LINE__);
	for (k = 1; k < nbRow; k++)
		table->BINGF[k] = table->BINGF[0] + k * nbCol;

	nbRow = GF;
	nbCol = GF;
	/* ADDGF [GF][logGF] */
	table->ADDGF = calloc((size_t)nbRow, sizeof(int *));
	//if (table->ADDGF  == NULL) err(EXIT_FAILURE,"%s:%d > malloc failed !",__FILE__,__LINE__);
	table->ADDGF[0] = calloc((size_t)nbRow * nbCol, sizeof(int));
	//if (table->ADDGF [0] == NULL) err(EXIT_FAILURE,"%s:%d > malloc failed !",__FILE__,__LINE__);
	for (k = 1; k < nbRow; k++)
		table->ADDGF[k] = table->ADDGF[0] + k * nbCol;

	/* MULGF [GF][logGF] */
	table->MULGF = calloc((size_t)nbRow, sizeof(int *));
	//if (table->MULGF  == NULL) err(EXIT_FAILURE,"%s:%d > malloc failed !",__FILE__,__LINE__);
	table->MULGF[0] = calloc((size_t)nbRow * nbCol, sizeof(int));
	//if (table->MULGF [0] == NULL) err(EXIT_FAILURE,"%s:%d > malloc failed !",__FILE__,__LINE__);
	for (k = 1; k < nbRow; k++)
		table->MULGF[k] = table->MULGF[0] + k * nbCol;

	/* DIVGF [GF][logGF] */
	table->DIVGF = calloc((size_t)nbRow, sizeof(int *));
	//if (table->DIVGF  == NULL) err(EXIT_FAILURE,"%s:%d > malloc failed !",__FILE__,__LINE__);
	table->DIVGF[0] = calloc((size_t)nbRow * nbCol, sizeof(int));
	//if (table->DIVGF [0] == NULL) err(EXIT_FAILURE,"%s:%d > malloc failed !",__FILE__,__LINE__);
	for (k = 1; k < nbRow; k++)
		table->DIVGF[k] = table->DIVGF[0] + k * nbCol;

	if (GF == 16) {
		for (g = 0; g < GF; g++)
			for (l = 0; l < logGF; l++)
				table->BINGF[g][l] = BinGF_16[g][l];
		//printf("Loading of the binary image of GF(64): Success\n");
		//fflush(stdout);
	}

	if (GF == 32) {
		for (g = 0; g < GF; g++)
			for (l = 0; l < logGF; l++)
				table->BINGF[g][l] = BinGF_32[g][l];
	}

	if (GF == 64) {
		for (g = 0; g < GF; g++)
			for (l = 0; l < logGF; l++)
				table->BINGF[g][l] = BinGF_64[g][l];
				//printf("Loading of the binary image of GF(64): Success\n");
				//fflush(stdout);
#if 0
        int i, j;
        printf("const int GF64_BINGF[%d][%d]=\n{\n", GF, logGF);
        for(i=0; i<GF; i++)
        {
            printf("  {");
            for(j=0; j<logGF; j++)
            {
              printf("%d", BinGF_64[i][j]);
              if((j+1) != logGF) printf(", ");
            }
            printf("}");
            if((i+1) != GF) printf(",\n");
          }
          printf("\n};\n");
#endif
	}

	if (GF == 128) {
		for (g = 0; g < GF; g++)
			for (l = 0; l < logGF; l++)
				table->BINGF[g][l] = BinGF_128[g][l];
	}

	if (GF == 256) {
		for (g = 0; g < GF; g++)
			for (l = 0; l < logGF; l++)
				table->BINGF[g][l] = BinGF_256[g][l];
		//printf("Loading of the binary image of GF(256): Success\n");
		//fflush(stdout);
	}

	/*
     * Build the addition, multiplication and division tables (corresponding to GF[q])
     */
	Table_Add_GF(table, GF, logGF);
	Table_Mul_GF(table->MULGF, GF);
	Table_Div_GF(table->DIVGF, GF);
}

/*!
 * \fn FreeTable(table_t *table)
 * \brief Free the memory in a table_t structure
 * Inputs        :
 * 	- table_t table : Structure that contains pointers to the tables
 * Outputs       :
 */
void FreeTable(table_t * table) {
	free(table->ADDGF[0]);
	free(table->MULGF[0]);
	free(table->DIVGF[0]);
	free(table->BINGF[0]);
	free(table->ADDGF);
	free(table->MULGF);
	free(table->DIVGF);
	free(table->BINGF);
}

void GaussianElimination(code_t * code, table_t * table) {
	const int N = code->N;
	const int M = code->M;
	int		  n, m, k, m1, ind, buf;
	
#if (DEBUG > 0)
	printf("MatUT (%d, %d)\n", M, N);
#endif

	code->matUT = (int **)calloc((size_t)M, sizeof(int *));
	if (code->matUT == NULL) {
		fprintf(stderr, "%s:%d > calloc failed !", __FILE__, __LINE__);
		exit(EXIT_FAILURE);
	}
#if 0
	code->matUT[0] = (int *) calloc((size_t)M * N, sizeof(int));
	//if (code->matUT[0] == NULL) err(EXIT_FAILURE,"%s:%d > malloc failed !",__FILE__,__LINE__);
	for (k = 1; k < M; k++)
		code->matUT[k] = code->matUT[0] + k * N;
#else
	for (k = 0; k < M; k++) {
		code->matUT[k] = (int *)calloc((size_t)N, sizeof(int));
	}
#endif

	code->Perm = (int *)calloc(N, sizeof(int));
	if (code->Perm == NULL) {
		fprintf(stderr, "%s:%d > calloc failed !", __FILE__, __LINE__);
		exit(EXIT_FAILURE);
	}

#if (DEBUG > 2)
	printf("Perm = [ ");
	for (n = 0; n < N; n++) {
		code->Perm[n] = n;
		printf("%d, ", code->Perm[n]);
	}
	printf("]\n");
#endif

	for (m = 0; m < M; m++) {
		for (k = 0; k < code->rowDegree[m]; k++) {
			code->matUT[m][code->mat[m][k]] = code->matValue[m][k];
		}
	}
#if (DEBUG > 1)
	printf("matUT = [ \n");
	for (m = 0; m < M; m++) {
		printf("[ ");
		for (k = 0; k < N; k++) {
			printf("%d, ", code->matUT[m][k]);
		}
		printf("],\n");
	}
	printf("]\n\n");
#endif

	for (m = 0; m < M; m++) {
		for (ind = m; ind < N; ind++) {
			if (code->matUT[m][ind] != 0)
				break;
		}
		if (ind == N) {
			printf("The matrix is not full rank (%d,%d)\n", m, ind);
			exit(EXIT_FAILURE);
		}
		buf				= code->Perm[ind];
		code->Perm[ind] = code->Perm[m];
		code->Perm[m]	= buf;
		for (m1 = 0; m1 < M; m1++) {
			buf					 = code->matUT[m1][m];
			code->matUT[m1][m]	 = code->matUT[m1][ind];
			code->matUT[m1][ind] = buf;
		}

		for (m1 = m + 1; m1 < M; m1++) {
			if (code->matUT[m1][m] != 0) {
				buf = code->matUT[m1][m];
				for (n = m; n < N; n++) {
					if (code->matUT[m1][n] != 0) {
						const int pos = code->matUT[m1][n];
						//printf("%p - %p - %p : code->matUT[%d][%d] = table->DIVGF[%d][%d] (%d)\n", code->matUT, code->matUT[m1], &code->matUT[m1][n], m1, n, pos, buf, table->DIVGF[pos][buf]);
						code->matUT[m1][n] = table->DIVGF[pos][buf];
					}
				}
				for (n = m; n < N; n++) {
					if (code->matUT[m1][n] != 0)
						code->matUT[m1][n] = table->MULGF[code->matUT[m1][n]][code->matUT[m][m]];
				}
				for (n = m; n < N; n++) {
					code->matUT[m1][n] = table->ADDGF[code->matUT[m1][n]][code->matUT[m][n]];
				}
			}
		}
	}
#if (DEBUG > 1)
	printf("matUT = [ \n");
	for (m = 0; m < M; m++) {
		printf("[ ");
		for (k = 0; k < N; k++) {
			printf("%d, ", code->matUT[m][k]);
		}
		printf("],\n");
	}
	printf("]\n\n");
#endif
}
void DumpHlayered(const code_t * code) {
	char buffer[256];

	sprintf(buffer, "././%db_%dx%d_GF%d", code->logGF * code->N, code->N, code->M, code->GF);
	//    sprintf(buffer, "./%dx%d_GF%d", code->N, code->M, code->GF);
	struct stat st = {0};
	if (stat(buffer, &st) == -1) {
		mkdir(buffer, 0700);
	}

	sprintf(buffer, "././%db_%dx%d_GF%d/hlayered.h", code->logGF * code->N, code->N, code->M, code->GF);
	FILE * f = fopen(buffer, "w");

	fprintf(f, "#ifndef _GEN_PAR_CPP_H_\n");
	fprintf(f, "#define _GEN_PAR_CPP_H_\n");
	fprintf(f, "\n");

	////////////////////////////////////////////////////////////
	fprintf(f, "const int CHECK_DEGREE[%d]=\n{", code->M);
	for (int i = 0; i < code->M; i++) {
		if ((i % 16) == 0)
			fprintf(f, "\n  ");
		fprintf(f, "%d", code->rowDegree[0]);
		if ((i + 1) != code->M)
			fprintf(f, ", ");
	}
	fprintf(f, "\n};\n\n");
	////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////
	fprintf(f, "const int LINK_NODES[%d]=\n{\n  ", code->rowDegree[0] * code->M);
	for (int node = 0; node < code->M; node++) {
		for (int i = 0; i < code->rowDegree[node]; i++) {
			fprintf(f, "%3d", code->mat[node][i]);
			if ((i + 1) != code->rowDegree[node])
				fprintf(f, ", ");
		}
		if ((node + 1) != code->M)
			fprintf(f, ", /* [%2d] */\n  ", node);
	}
	fprintf(f, "  /* [%2d] */\n};\n", code->M - 1);
	////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////
	fprintf(f, "const int ALPHA[%d]=\n{\n  ", code->rowDegree[0] * code->M);
	for (int node = 0; node < code->M; node++) {
		for (int i = 0; i < code->rowDegree[node]; i++) {
			const int alpha = code->matValue[node][i];
			assert(alpha < code->GF);
			fprintf(f, "%3d", alpha);
			if ((i + 1) != code->rowDegree[node])
				fprintf(f, ", ");
		}
		if ((node + 1) != code->M)
			fprintf(f, ", /* [%2d] */\n  ", node);
	}
	fprintf(f, "  /* [%2d] */\n};\n", code->M - 1);
	fprintf(f, "\n#endif\n");
	fclose(f);
}

void DumpConstants(const code_t * code) {
	char buffer[256];

	sprintf(buffer, "././%db_%dx%d_GF%d", code->logGF * code->N, code->N, code->M, code->GF);
	//    sprintf(buffer, "./%dx%d_GF%d", code->N, code->M, code->GF);
	struct stat st = {0};
	if (stat(buffer, &st) == -1) {
		mkdir(buffer, 0700);
	}

	sprintf(buffer, "././%db_%dx%d_GF%d/constantes.h", code->logGF * code->N, code->N, code->M, code->GF);
	//                    sprintf(buffer, "././%dx%d_GF%d/constantes.h", code->N, code->M, code->GF);
	FILE * f = fopen(buffer, "w");
	fprintf(f, "#ifndef CONSTANTES\n");
	fprintf(f, "#define CONSTANTES\n");
	fprintf(f, "\n");
	fprintf(f, "#include <math.h>\n");
	fprintf(f, "\n");
	fprintf(f, "#define _GF_        		%3d\n", code->GF);
	fprintf(f, "#define _LOG2GF_    		%3d\n", code->logGF);
	fprintf(f, "\n");
	fprintf(f, "#define _KSYMBOL_   		%3d\n", code->N - code->M);
	fprintf(f, "#define _NSYMBOL_   		%3d\n", code->N);
	fprintf(f, "#define _MESSAGE_   		%3d\n", code->M * code->dcMax); // REGULAR CODE ONLY !!!
	fprintf(f, "\n");
	fprintf(f, "#define _DEG_CN_             %3d\n", code->dcMax);
	fprintf(f, "#define _DEG_COMPUTATIONS_   %3d\n", code->M);
	fprintf(f, "\n");
	fprintf(f, "#define _KBIT_      		(_KSYMBOL_*_LOG2GF_)\n");
	fprintf(f, "#define _NBIT_      		(_NSYMBOL_*_LOG2GF_)\n");
	fprintf(f, "\n");
	fprintf(f, "#define _NmKSYMBOL_ 	 	(_NSYMBOL_-_KSYMBOL_)\n");
	fprintf(f, "#define _NmKBIT_     		(_NBIT_-_KBIT_)\n");
	fprintf(f, "\n");
	fprintf(f, "#endif\n");
	fclose(f);
}

void DumpEncoder(const code_t * code) {
	char buffer[256];

	sprintf(buffer, "././%db_%dx%d_GF%d", code->logGF * code->N, code->N, code->M, code->GF);
	//    sprintf(buffer, "./%dx%d_GF%d", code->N, code->M, code->GF);
	struct stat st = {0};
	if (stat(buffer, &st) == -1) {
		mkdir(buffer, 0700);
	}

	sprintf(buffer, "././%db_%dx%d_GF%d/encoder.h", code->logGF * code->N, code->N, code->M, code->GF);
	//    sprintf(buffer, "././%dx%d_GF%d/encoder.h", code->N, code->M, code->GF);
	FILE * f = fopen(buffer, "w");

	fprintf(f, "#ifndef _EncoderTable_\n");
	fprintf(f, "#define _EncoderTable_\n");
	fprintf(f, "\n");
	fprintf(f, "const int GF%d_ENC[%d][%d]=\n{\n", code->GF, code->M, code->N);
	for (int m = 0; m < code->M; m++) {
		fprintf(f, "  {");
		for (int n = 0; n < code->N; n++) {
			fprintf(f, "%2d", code->matUT[m][n]);
			if ((n + 1) != code->N)
				fprintf(f, ", ");
		}
		fprintf(f, "}");
		if ((m + 1) != code->M)
			fprintf(f, ",\n");
	}
	fprintf(f, "\n};\n");
	fprintf(f, "\n");
	fprintf(f, "const int GF%d_PERM[%d]=\n{", code->GF, code->N);
	for (int i = 0; i < code->N; i++) {
		if ((i % 16) == 0)
			fprintf(f, "\n  ");
		fprintf(f, "%3d", code->Perm[i]);
		if ((i + 1) != code->N)
			fprintf(f, ", ");
	}
	fprintf(f, "\n};\n");
	fprintf(f, "\n#endif\n");
	fclose(f);
}
