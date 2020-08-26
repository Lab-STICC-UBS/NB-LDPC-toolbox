# NB-LDPC-toolbox

toolbox for Non Binary LDPC 

## C Codes

- [ ] Document cmake instructions and C-Library

## Matlab Codes

Can be found in [Matlab directory](nb_ldpc_mat_toolbox).

### Functions

Self-documented set of functions which used the next GF symbols equivalence:
|GF Symbol  | Code Index  | Matlab Decimal value     |
|:---:      |:---:        |:---:                     |
|0_gf       | 1           | 1                        |
|alpha^k    | k + 2       | (true decimal value) + 1 |

*this for 0 <= k < q - 1, q being the GF order.*

> **Example with q = 8:**
> | GF Symbol | Code Index | Matlab Decimal Value |
> |:---------:|:----------:|:--------------------:|
> | 0_gf      | 1          | 1                    |
> | alpha^0   | 2          | 2                    |
> | alpha^1   | 3          | 3                    |
> | alpha^2   | 4          | 5                    |
> | alpha^3   | 5          | 4                    |
> | alpha^4   | 6          | 7                    |
> | alpha^5   | 7          | 8                    |
> | alpha^6   | 8          | 6                    |

Use built-in Octave/Matlab help to print function documentation
Ex:

    octave> help buildgf
    'buildgf' is a function from the file NB-LDPC-toolbox/nb_ldpc_mat_toolbox/functions/buildgf.m

    BUILDGF Initialize tools for NB-LDPC
         Authors :  Cédric Marchand, Camille Monière
         Build finite field vectors and initialize
         dec2gf, gf2dec, multgf_gf, multgf_dec,
         divgf_gf and divgf_dec functions.

     %     GF symbols correspondance :
     %     GF Symbol  => Code Index  => Matlab Decimal value
     %     ------------------------------------------
     %     0_gf       => 1           => 1
     %     alpha^k    => k + 2       => (true decimal value) + 1
     %         this for k >=  0
     %
     %     Example with q = 8:
     %     > GF Symbol => Code Index  => Matlab Decimal Value
     %     > ------------------------------------------
     %     > 0_gf      => 1           => 1
     %     > alpha^0   => 2           => 2
     %     > alpha^1   => 3           => 3
     %     > alpha^2   => 4           => 5
     %     > alpha^3   => 5           => 4
     %     > alpha^4   => 6           => 7
     %     > alpha^5   => 7           => 8
     %     > alpha^6   => 8           => 6

         See also  DEC2GF, GF2DEC, MULTGF_GF, MULTGF_DEC,
                   DIVGF_GF, DIVGF_DEC.


    Additional help for built-in functions and operators is
    available in the online version of the manual.  Use the command
    'doc <topic>' to search the manual index.

    Help and information about Octave is also available on the WWW
    at https://www.octave.org and via the help@octave.org
    mailing list.

### Legacy Scripts

Scripts to manipulate alist and sparse matrix. 

* **show_matrix.m**: function to show in colour a NB LDPC matrix 
* **matrix2alist.m**: Matlab function to convert from matrix to alist 
* **alist2matrix.m**: Matlab function to convert from alist to matrix 
* **DVB_T2_QAM.m**: Matlab function to construct DVB-T2 constellation 
* **build_GF.m**: Matlab function to build finite field vectors 
* **shannon.m**: Matlab script to show Shannon limit 
