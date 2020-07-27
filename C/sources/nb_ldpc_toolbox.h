#ifndef NB_LDPC_TOOLBOX_H_
#define NB_LDPC_TOOLBOX_H_

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "struct.h"

void LoadCode(const char * FileMatrix, code_t * code);

void FreeCode(code_t * code);

void LoadTables(table_t * table, int GF, int logGF);

void FreeTable(table_t * table);

void GaussianElimination(code_t * code, table_t * table);

void DumpHlayered(const code_t * code);

#endif
