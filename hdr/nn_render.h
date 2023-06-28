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
extern void nn_render_neuron_layer_circles(nn_t nn, size_t layer, Rectangle frame);
extern void nn_render_neuron_layer_connections(nn_t nn, size_t layer, Rectangle frame);
extern void nn_render_your_mom(nn_t nn, Rectangle nn_frame);
extern void nn_render(nn_t nn, Rectangle nn_frame);

#endif /* NN_RENDER_H_ */
