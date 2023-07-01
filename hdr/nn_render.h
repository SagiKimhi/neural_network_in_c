#ifndef NN_RENDER_H_
#define NN_RENDER_H_

#include <raylib.h>
#include <nn.h>

#ifndef NN_RENDER_CIRCLE_COLOR_HIGH
    #define NN_RENDER_CIRCLE_COLOR_HIGH GREEN
#endif

#ifndef NN_RENDER_CIRCLE_COLOR_OFF
    #define NN_RENDER_CIRCLE_COLOR_OFF GRAY
#endif

#ifndef NN_RENDER_CIRCLE_COLOR_LOW
    #define NN_RENDER_CIRCLE_COLOR_LOW PINK
#endif

#ifndef NN_RENDER_LINE_COLOR_HIGH
    #define NN_RENDER_LINE_COLOR_HIGH GREEN
#endif

#ifndef NN_RENDER_LINE_COLOR_OFF
    #define NN_RENDER_LINE_COLOR_OFF GRAY
#endif

#ifndef NN_RENDER_LINE_COLOR_LOW
    #define NN_RENDER_LINE_COLOR_LOW PINK
#endif

#define COST_INFO_INIT_CAP 256

#define cost_info_append(cost_info, item)                                                          \
    do {                                                                             \
        if ((cost_info)->count >= (cost_info)->capacity) {                                         \
            (cost_info)->capacity = (cost_info)->capacity == 0 ? COST_INFO_INIT_CAP : (cost_info)->capacity*2;   \
            (cost_info)->items = realloc((cost_info)->items, (cost_info)->capacity*sizeof(*(cost_info)->items)); \
            NN_ASSERT((cost_info)->items != NULL && "Buy more RAM lol");                   \
        }                                                                            \
                                                                                     \
        (cost_info)->items[(cost_info)->count++] = (item);                                         \
    } while (0)

typedef struct {
    int     nof_hcircles;
    int     nof_vcircles;
    float   radius;
    float   vspace;
    float   hspace;
    float   vpad;
    float   hpad;
    Vector2 offset;
    Vector2 start_pos;
    Color   init_color;
} nn_r_layer_circ_info;

typedef struct {
    float   *items;
    size_t  count;
    size_t  capacity;
} nn_render_cost_info;

typedef struct {
    size_t arch_len;
    size_t *arch;
} nn_arch_t;

typedef void nn_print_func_t(nn_t nn, nn_matrix_t ts_in, nn_matrix_t ts_out);
typedef void nn_load_data_func_t(char *data_filepath, nn_matrix_t *ts_in, nn_matrix_t *ts_out, size_t range);

extern float get_frame_vpad(Rectangle frame);
extern float get_frame_hpad(Rectangle frame);
extern float get_frame_vspace(Rectangle frame);
extern float get_frame_hspace(Rectangle frame);
extern float get_relative_object_vspace(Rectangle frame, int nof_vertical_objects);
extern float get_relative_object_hspace(Rectangle frame, int nof_horizontal_objects);
extern float get_object_vpad(float relative_object_vspace);
extern float get_object_hpad(float relative_object_hspace);
extern float calc_neuron_circle_radius(nn_t nn, Rectangle frame);
extern nn_r_layer_circ_info get_layer_circle_info( nn_t nn, size_t layer, Rectangle frame);
extern void nn_render_model_information_text(Rectangle frame, nn_render_cost_info cost_info, float rate, char buf[256]);
extern void nn_render_x_y_axis_internal(Rectangle frame);
extern void nn_render_cost_graph(Rectangle frame, nn_render_cost_info cost);
extern void nn_render_neuron_layer_circles(nn_t nn, size_t layer, Rectangle frame);
extern void nn_render_neuron_layer_connections(nn_t nn, size_t layer, Rectangle frame);
extern void nn_render_your_mom(nn_t nn, Rectangle nn_frame);
extern void nn_render_network(nn_t nn, Rectangle nn_frame);
extern void nn_render_with_default_frames(
    nn_arch_t arch, float rand_low, float rand_high, 
    nn_print_func_t optional_print_func_ptr, nn_load_data_func_t load_data
);

#endif /* NN_RENDER_H_ */
