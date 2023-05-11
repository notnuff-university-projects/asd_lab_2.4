
// other libraries
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "src/consts_n_structures.h"
#include "src/graph_create.h"
#include "src/vec_math_lib.h"
#include "src/X_routines.h"
#include "src/graph_draw.h"


int main() {
    int n1 = 2, n2 = 1, n3 = 2, n4 = 5;
    srand(n1 * 1000 + n2 * 100 + n3 * 10 + n4);

    int n = 10 + n3;
    double c;
    int is4 = 0;

    double **service_mat = randm(n, n);
    int **rel_mat = mat_create(n, n);
    point_t *graph = malloc(sizeof (point_t) * n);
    point_t *strong_comp_graph = malloc(sizeof (point_t) * n);
    point_t *cond_graph = NULL;
    int **pow2 = mat_create(n, n);
    int **pow3 = mat_create(n, n);
    int **reach_mat = mat_create(n, n);
    int **strong_mat = mat_create(n, n);
    int **cond_graph_rel_mat = NULL;
    int *cond_graph_size = malloc(sizeof (int));
    int **cond_matrix = mat_create(n, n);

    XEvent event;
    KeySym key;
    char text[255]; //keyboard buffer

    init_x();//window created

    while (1) {
        XNextEvent(dis, &event);
        switch (event.type) {
            case Expose:
                if (event.xexpose.count == 0) redraw_x();
                break;
            case KeyPress:
                if (XLookupString(&event.xkey, text, 255, &key, 0) == 1) {
                    int def = 0;

                    int oriented;
                    switch (text[0]) {
                        case 'q':
                            free_mat(service_mat, n);
                            free_mat(rel_mat, n);
                            free_mat(pow2, n);
                            free_mat(pow3, n);
                            free_mat(reach_mat, n);
                            free_mat(strong_mat, n);
                            free_mat(cond_matrix, n);
                            if (cond_graph_rel_mat) free_mat(cond_graph_rel_mat, *cond_graph_size);
                            free(cond_graph_size);
                            free(strong_comp_graph);
                            free(graph);
                            if (cond_graph) free(cond_graph);
                            close_x();
                            return 0;
                            break;
                        case 'o':
                            redraw_x();
                            oriented = 1;
                            is4 = 0;

                            printf("completing item 1-3 of the lab for oriented graph: \n");
                            printf("1. Printing oriented relation matrix: \n");
                            break;
                        case 'u':
                            redraw_x();
                            oriented = 0;
                            is4 = 0;

                            printf("completing item 1-3 of the lab for unoriented graph: \n");
                            printf("1. Printing unoriented relation matrix: \n");
                            break;
                        case '4':
                            redraw_x();
                            printf("completing item 4 of the lab: \n");
                            is4 = 1;
                            c = 1.0 - n3 * 0.005 - n4 * 0.005 - 0.27;

                            oriented = 1;

                            printf("4.1 Printing oriented relation matrix: \n");
                            rel_mat = mulmr(c, service_mat, rel_mat, n, n, oriented);
                            print_mat(rel_mat, n, n);
                            tri_graph_create(graph, n);
                            draw_graph(graph, rel_mat, n, oriented);

                            XSetForeground(dis, gc, 0xFFFFFF);
                            XDrawString(dis, win, gc, 20, 20, "4.1 Drawing oriented graph:", 27);
                            draw_graph_vertices(graph, n);

                            printf("4.2 Printing powers of vertices: \n");
                            print_power(rel_mat, n, oriented);

                            printf("4.3 Printing ways with length of 2: \n");
                            mat_cpy(pow2, rel_mat, n);
                            mat_to_pow(pow2, 2, n);

                            print_ways_2(rel_mat, pow2, n);

                            printf("4.3 Printing ways with length of 3: \n");
                            mat_cpy(pow3, rel_mat, n);
                            mat_to_pow(pow3, 3, n);
                            print_ways_3(rel_mat, pow3, n);

                            printf("4.4 Printing reach matrix: \n");
                            mat_reach(reach_mat, rel_mat, n);
                            print_mat(reach_mat, n, n);

                            printf("4.6 Printing strongly connected matrix: \n");
                            mat_strong(strong_mat, rel_mat, n);
                            print_mat(strong_mat, n, n);

                            printf("4.5 to look at strongly connected elements in graph type 's'\n");

                            printf("4.7 to look at condensation graph type 'c'\n");

                            break;
                        case 's':
                            redraw_x();
                            if (!is4) {
                                printf("press 4 v1\n");
                                break;
                            }
                            XSetForeground(dis, gc, 0xFFFFFF);
                            XDrawString(dis, win, gc, 20, 20, "4.5 Drawing strongly connected elements:", 40);

                            mat_strong(strong_mat, rel_mat, n);
                            cond_graph_matrix_create(&cond_graph_rel_mat, strong_mat, rel_mat, cond_matrix, n,
                                                     cond_graph_size);

                            tri_graph_create(strong_comp_graph, n);
                            strong_graph_create(strong_comp_graph, n, cond_matrix);
                            draw_graph_vertices(strong_comp_graph, n);

                            break;
                        case 'c':
                            redraw_x();
                            if (!is4) {
                                printf("press 4 v1\n");
                                break;
                            }
                            printf("4.7 printing relation matrix between condensation elements: \n");

                            XSetForeground(dis, gc, 0xFFFFFF);
                            XDrawString(dis, win, gc, 20, 20, "4.7 Drawing condensation graph:", 31);

                            cond_graph_matrix_create(&cond_graph_rel_mat, strong_mat, rel_mat, cond_matrix, n,
                                                     cond_graph_size);
                            cond_graph_create(&cond_graph, strong_comp_graph, cond_matrix, *cond_graph_size, n);
                            print_mat(cond_graph_rel_mat, *cond_graph_size, *cond_graph_size);

                            draw_graph(cond_graph, cond_graph_rel_mat, *cond_graph_size, 1);
                            draw_graph_vertices(cond_graph, *cond_graph_size);
                            break;
                        default:
                            redraw_x();
                            def = 1;

                            break;
                    }
                    if (!is4 && !def) {
                        c = 1.0 - n3 * 0.01 - n4 * 0.01 - 0.3;
                        rel_mat = mulmr(c, service_mat, rel_mat, n, n, oriented);
                        print_mat(rel_mat, n, n);

                        printf("2. Printing powers of vertices: \n");
                        print_power(rel_mat, n, oriented);

                        printf("3. Printing dangling and isolated vertices: \n");
                        print_isolated(rel_mat, n, oriented);

                        tri_graph_create(graph, n);
                        draw_graph(graph, rel_mat, n, oriented);
                        draw_graph_vertices(graph, n);
                    }
                    break;
                }
        }
    }
}