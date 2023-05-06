
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

    int n = 6+ n3;
    double c = 1.0 - n3 * 0.01 - n4 * 0.01 - 0.3;
    double **service_mat;
    service_mat = randm(n, n);

    int **rel_mat;
    rel_mat = mat_create(n, n);
    point_t *graph;

    int **pow2 = mat_create(n, n);
    int **path2 = mat_create(n, n);
    int **pow3 = mat_create(n, n);
    int **path3 = mat_create(n, n);
    int **reach_mat = mat_create(n, n);
    int **strong_mat = mat_create(n, n);

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
                    int oriented;
                    switch (text[0]) {
                        case 'q':
                            free_mat(service_mat, n);
                            free_mat(rel_mat, n);
                            free(graph);
                            close_x();
                            return 0;
                            break;
                        case 'o':
                            redraw_x();

                            oriented = 1;
                            printf("drawing oriented relation matrix: \n");
                            rel_mat = mulmr(c, service_mat, rel_mat, n, n, oriented);
                            print_mat(rel_mat, n, n);
                            //print_power(rel_mat, n, oriented);
                            graph = tri_graph_create(graph, n);
                            draw_graph(graph, rel_mat, n, oriented);
                            draw_graph_vertices(graph, n);
                            break;
                        case 'u':
                            redraw_x();

                            oriented = 0;
                            printf("drawing unoriented relation matrix: \n");
                            rel_mat = mulmr(c, service_mat, rel_mat, n, n, oriented);
                            print_mat(rel_mat, n, n);
                            print_power(rel_mat, n, oriented);
                            graph = tri_graph_create(graph, n);
                            draw_graph(graph, rel_mat, n, oriented);
                            draw_graph_vertices(graph, n);
                            break;
                        case '2':
                            redraw_x();

                            mat_cpy(pow2, rel_mat, n);
                            mat_to_pow(pow2, 2, n);
                            mat_2path(path2, rel_mat, pow2, n);
                            printf("printing relation matrix to power of 2: \n");
                            print_mat(pow2, n, n);
                            printf("printing paths of 2: \n");
                            print_mat(path2, n, n);

                            draw_graph(graph, path2, n, oriented);
                            draw_graph_vertices(graph, n);
                            break;
                        case '3':
                            redraw_x();

                            if (!pow2) break;
                            mat_cpy(pow3, rel_mat, n);
                            mat_to_pow(pow3, 3, n);
                            printf("printing relation matrix to power of 3: \n");
                            print_mat(pow3, n, n);
                            draw_graph(graph, pow3, n, oriented);
                            draw_graph_vertices(graph, n);
                            break;
                        case 't':
                            redraw_x();

                            mat_reach(reach_mat, rel_mat, n);
                            printf("printing reach matrix (transition closure U diagonal matrix): \n");
                            print_mat(reach_mat, n, n);
                            draw_graph(graph, reach_mat, n, 1);
                            draw_graph_vertices(graph, n);
                            break;
                        case 's':
                            redraw_x();

                            mat_strong(strong_mat, rel_mat, n);

                            printf("printing strongly connected matrix: \n");
                            print_mat(strong_mat, n, n);
                            draw_graph(graph, strong_mat, n, 0);
                            draw_graph_vertices(graph, n);
                            break;
                        default:
                            redraw_x();
                            break;
                    }
                    break;
                }
        }
    }
}