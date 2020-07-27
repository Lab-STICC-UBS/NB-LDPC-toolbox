#include "nb_ldpc_toolbox.h"

int main(int argc, char * argv[]){

    printf("argc is %d and argv is ", argc);
    for(int i = 0 ; i < argc ; i++ ){
        printf("%s ", argv[i]);
    }
    printf("\n");

    return EXIT_SUCCESS;
}