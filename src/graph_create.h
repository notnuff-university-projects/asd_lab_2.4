#ifndef INC_2_3_GRAPH_CREATE_H
#define INC_2_3_GRAPH_CREATE_H

#include <stdio.h>
#include <stdlib.h>
#include "consts_n_structures.h"

double **randm(int size1, int size2);
int **mulmr(double coefficient, double **double_mat, int **target_mat, int size1, int size2, int oriented);
int **mat_create(int size1, int size2);

point_t *tri_graph_create(point_t *tar_graph, int graph_size);
void free_mat (size_t **target_mat, int size);
void print_mat(int **target_mat, int size1, int size2);
void print_power (int **target_mat, int size, int oriented);
void mat_composition (int **target_mat, int **source_mat, int size);
void mat_union (int **target_mat, int **source_mat, int size);
void mat_2path (int **target_mat, int **source_mat, int **pow2_mat, int size);
void mat_to_pow(int **target_mat, int n, int size);
void mat_to_bool (int **target_mat, int size);
void mat_cpy (int **target_mat, int **source_mat, int size);
void mat_reach (int **target_mat, int **source_mat,  int size);
void mat_transpose (int **target_mat,  int size);
void mat_elem_by_elem (int **target_mat, int **source_mat, int size);
void mat_strong (int **target_mat, int **source_mat, int size);
void cond_graph_matrix_create(int ***target_mat, int **strong_mat, int **rel_mat, int **cond_matrix, int size,
                              int *cond_size);
void cond_graph_create(point_t **cond_graph, point_t *simple_graph, int **cond_matrix, int size, int rel_mat_size);
#endif //INC_2_3_GRAPH_CREATE_H
