#include "../graph_create.h"

double **randm(int size1, int size2) {
    double **result_mat = (double **) malloc(sizeof (double *) * size1);

    for (int i = 0; i < size1; i++) {
        result_mat[i] = (double *) malloc(sizeof(double ) * size2);

        for (int j = 0; j < size2; j++) {
            result_mat[i][j] = 2.0 * rand() / RAND_MAX;
        }
    }
    return result_mat;
}

int **mat_create(int size1, int size2) {
    int **result_mat = (int **) malloc(sizeof(int *) * size1);
    for (int i = 0; i < size1; i++) {
        result_mat[i] = (int *) malloc(sizeof(int) * size2);
    }

    for (int i = 0; i < size1; i++) {
        for (int j = 0; j < size2; j++) {
            result_mat[i][j] = 0;
        }
    }

    return result_mat;
}

point_t *tri_graph_create(point_t *tar_graph, int graph_size) {
    double as_ratio = (double) SCREEN_HEIGHT / SCREEN_WIDTH;
    tar_graph = malloc(sizeof (point_t) * graph_size);
    serv_point_t *serv_points = malloc(sizeof (serv_point_t) * graph_size);
    double indent_x = (double) 100 / SCREEN_WIDTH;
    double indent_y = (double) 400 / SCREEN_HEIGHT;

    int point_1 = 0;
    int point_2 = graph_size / 3;
    int point_3 = 2 * graph_size / 3;

    serv_points[point_1] = (serv_point_t) {0, 1 - indent_y / 5};
    serv_points[point_2] = (serv_point_t) {1 - indent_x, -1 + indent_y};
    serv_points[point_3] = (serv_point_t) {-1 + indent_x, -1 + indent_y};

    //interpolation (I guess?) for non-anchor points of triangle
    for (int i = point_1 + 1, j = 1 ; i < point_2; i++, j++) {
        serv_points[i].x = serv_points[point_1].x +
                           j * (serv_points[point_2].x - serv_points[point_1].x) / (point_2 - point_1);
        serv_points[i].y = serv_points[point_1].y +
                           j * (serv_points[point_2].y - serv_points[point_1].y) / (point_2 - point_1);
    }
    for (int i = point_2 + 1, j = 1; i < point_3; i++, j++) {
        serv_points[i].x = serv_points[point_2].x +
                           j * (serv_points[point_3].x - serv_points[point_2].x) / (point_3 - point_2);
        serv_points[i].y = serv_points[point_2].y +
                           j * (serv_points[point_3].y - serv_points[point_2].y) / (point_3 - point_2);
    }
    for (int i = point_3 + 1, j = 1; i < graph_size; i++, j++) {
        serv_points[i].x = serv_points[point_3].x +
                           j * (serv_points[point_1].x - serv_points[point_3].x) / (graph_size - point_3);
        serv_points[i].y = serv_points[point_3].y +
                           j * (serv_points[point_1].y - serv_points[point_3].y) / (graph_size - point_3);
    }
    //translating into the screen space
    for (int i = 0; i < graph_size; i++) {
        tar_graph[i].x =  SCREEN_WIDTH + (int) (serv_points[i].x * SCREEN_WIDTH * as_ratio - SCREEN_WIDTH) / 2;
        tar_graph[i].y =  SCREEN_HEIGHT - (int) (serv_points[i].y * SCREEN_HEIGHT + SCREEN_HEIGHT) / 2 ;
        sprintf(tar_graph[i].number, "%d", i + 1);
    }
    free(serv_points);
    return tar_graph;
}

int **mulmr(double coefficient, double **double_mat, int **target_mat, int size1, int size2, int oriented) {
    for (int i = 0; i < size1; i++) {
        for (int j = 0; j < size2; j++) {
            if (!oriented) {
                if (coefficient * double_mat[i][j] < 1) {
                    if (target_mat[i][j] != 1)
                        target_mat[i][j] = 0;
                }
                else {
                    target_mat[i][j] = 1;
                    target_mat[j][i] = 1;
                }
            }
            else {
                target_mat[i][j] = coefficient * double_mat[i][j] < 1 ?
                                   0 : 1;
            }
        }
    }
    return target_mat;
}


void free_mat (size_t **target_mat, int size) {
    for (int i = 0; i < size; i++) {
        free(target_mat[i]);
    }
    free(target_mat);
}

void print_power (int **target_mat, int size, int oriented) {

    int powr_mat[size][2];

    for (int i = 0; i < size; i++) {
        int positive = 0;
        int negative = 0;
        for (int j = 0; j < size; j++) {
            if (target_mat[i][j]) positive++;
            if (target_mat[j][i]) negative++;
        }
        powr_mat[i][0] = positive;
        powr_mat[i][1] = negative;
    }
    int homogeneous = 1;
    for (int i = 0; i < size; i++) {
        if (oriented) {
            printf("вершина %2i: ", i + 1);
            printf("додатня степінь %2i | ", powr_mat[i][0]);
            printf("від'ємна степінь %2i", powr_mat[i][1]);
            printf("\n");
        }
        else {
            printf("степінь вершини %2i: %2i", i + 1,  powr_mat[i][0]);
            printf("\n");
        }
        if (powr_mat[i][0] != powr_mat[(i + 1) % (size - 1)][0] ||
            powr_mat[i][1] != powr_mat[(i + 1) % (size - 1)][1]) homogeneous = 0;
    }
    if (homogeneous) {
        if (oriented)
            printf("граф однорідний і має %i додатню та %i від'ємну степінь\n", powr_mat[0][0], powr_mat[0][1]);
        else
            printf("граф однорідний і має %i степінь\n", powr_mat[0][0]);
    }
}

void mat_composition (int **target_mat, int **source_mat, int size) {
    int **result = mat_create(size, size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            for (int k = 0; k < size; k++) {
                result[i][j] += target_mat[i][k] * source_mat[k][j];
            }
        }
    }
    mat_cpy(target_mat, result, size);
    free_mat(result, size);
}

void mat_union (int **target_mat, int **source_mat, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            target_mat[i][j] += source_mat[i][j];
        }
    }
}

void mat_to_pow(int **target_mat, int n, int size) {
    int **source_mat = mat_create(size, size);
    mat_cpy(source_mat, target_mat, size);
    for (int i = 0; i < n-1; i++) {
        mat_composition(target_mat, source_mat, size);
    }
    free_mat(source_mat, size);
}

void mat_cpy (int **target_mat, int **source_mat, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            target_mat[i][j] = source_mat[i][j];
        }
    }
}

void mat_to_bool (int **target_mat, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            target_mat[i][j] = target_mat[i][j] == 0 ? 0 : 1;
        }
    }
}

void mat_2path (int **target_mat, int **source_mat, int **pow2_mat, int size) {
    int **result = mat_create(size, size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (pow2_mat[i][j]) {
                for (int k = 0; k < size; k++) {
                    result[i][k] += source_mat[i][k] * source_mat[k][j];
                    result[k][j] += source_mat[i][k] * source_mat[k][j];
                }
            }
        }
    }
    mat_cpy(target_mat, result, size);
    mat_to_bool(target_mat, size);
    free_mat(result, size);
}

void mat_reach(int **target_mat, int **source_mat, int size) {
    int **result = mat_create(size, size);
    mat_cpy(target_mat, source_mat, size);

    mat_union(result, target_mat, size);
    for (int i = 0; i < size; i++) {
        mat_to_pow(target_mat, i, size);
        mat_union(result, target_mat, size);
        result[i][i] += 1;
    }
    mat_cpy(target_mat, result, size);
    mat_to_bool(target_mat, size);
    free_mat(result, size);
}
void mat_transpose(int **target_mat,  int size) {
    int **result = mat_create(size, size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            result[i][j] = target_mat[j][i];
        }
    }
    mat_cpy(target_mat, result, size);
    free_mat(result, size);
};
void mat_elem_by_elem (int **target_mat, int **source_mat, int size){
    int **result = mat_create(size, size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            result[i][j] += target_mat[i][j] * source_mat[i][j];
        }
    }
    mat_cpy(target_mat, result, size);
    free_mat(result, size);
}

void mat_strong (int **target_mat, int **source_mat, int size) {
    int **reach_mat = mat_create(size, size);
    mat_reach(reach_mat, source_mat, size);
    mat_cpy(target_mat, reach_mat, size);
    mat_transpose(reach_mat, size);
    mat_elem_by_elem(target_mat, reach_mat, size);
    mat_to_bool(target_mat, size);
}

void print_mat(int **target_mat, int size1, int size2) {
    for (int i = 0; i < size1; i++) {
        for (int j = 0; j < size2; j++) {
            printf("%2i ", target_mat[i][j]);
        }
        printf("\n");
    }
};
