#ifndef NN_RENDER_C_
#define NN_RENDER_C_

#include <nn_render.h>

/* ----------
 * Constants:
 * ---------- */
const float frame_hpad_multiplier       = 0.02f;
const float frame_vpad_multiplier       = 0.02f;
const float default_obj_vpad_multiplier = 0.1f;
const float default_obj_hpad_multiplier = 0.1f;
const float default_line_thickness_multiplier = 1.4f;
const float default_nn_frame_thickness = 2.f;

/* ----------------------
 * Static Function Decls:
 * ---------------------- */
static int find_max_nof_neuron_circles(nn_t nn);


/* -------------------------------
 * Library Method Implementations:
 * ------------------------------- */
void nn_render(nn_t nn, Rectangle nn_frame)
{
    DrawRectangleLinesEx(nn_frame, default_nn_frame_thickness, GRAY);

    for (size_t l = 0; l <= nn.nof_layers; l++) {
        nn_render_neuron_layer_circles(nn, l, nn_frame);
        nn_render_neuron_layer_connections(nn, l, nn_frame);
    }
}

void nn_render_your_mom(nn_t nn, Rectangle frame)
{
    
    nn_render(nn, frame);
    DrawText("<3 Your Mom <3", GetRenderWidth()/4, 20, 80, PINK);
}

void nn_render_neuron_layer_connections(nn_t nn, size_t layer, Rectangle frame)
{
    Color                   line_color;
    Vector2                 line_start_pos, line_end_pos;
    nn_r_layer_circ_info    curr_circ_info, next_circ_info;

    NN_ASSERT(layer <= nn.nof_layers);

    if (layer == nn.nof_layers)
        return;

    curr_circ_info      = get_layer_circle_info(nn, layer, frame);
    next_circ_info      = get_layer_circle_info(nn, layer + 1, frame);
    line_start_pos.x    = curr_circ_info.start_pos.x + curr_circ_info.radius;
    line_start_pos.y    = curr_circ_info.start_pos.y;
    line_end_pos.x      = next_circ_info.start_pos.x - next_circ_info.radius;
    line_end_pos.y      = next_circ_info.start_pos.y;

    for (size_t i = 0; i < curr_circ_info.nof_vcircles; i++) {
        Color line_color_high   = NN_RENDER_LINE_COLOR_HIGH;
        Color line_color_low    = NN_RENDER_LINE_COLOR_LOW;

        for (size_t j = 0; j < next_circ_info.nof_vcircles; j++) {
            float line_weight_val = 
                sigmoidf(NN_MATRIX_AT(nn.weights[layer], i, j));
            
            line_color_high.a   = floorf(255.f * line_weight_val);
            line_color          = ColorAlphaBlend(line_color_low, line_color_high, WHITE);

            DrawLineEx(line_start_pos, line_end_pos, 1, line_color);
            line_end_pos.y -= next_circ_info.offset.y;
        }

        line_start_pos.y -= curr_circ_info.offset.y;
        line_end_pos.y = next_circ_info.start_pos.y;
    }
}

void nn_render_neuron_layer_circles(nn_t nn, size_t layer, Rectangle frame)
{
    nn_r_layer_circ_info circ_info;
    NN_ASSERT(layer <= nn.nof_layers);

    circ_info   = get_layer_circle_info(nn, layer, frame);

    for (size_t i = 0; i < circ_info.nof_vcircles; i++) {
        if (layer) {
            Color circ_color_high = NN_RENDER_CIRCLE_COLOR_HIGH;
            Color circ_color_low = NN_RENDER_CIRCLE_COLOR_LOW;

            circ_color_high.a = floorf(255.f * sigmoidf( 
                NN_MATRIX_AT(nn.biases[layer-1], 0, i) 
            ));
            circ_info.init_color = ColorAlphaBlend(circ_color_low, circ_color_high, WHITE);
        }
        
        DrawCircleV(circ_info.start_pos, circ_info.radius, circ_info.init_color);
        circ_info.start_pos.y -= circ_info.offset.y;
    }
}

float calc_neuron_circle_radius(nn_t nn, Rectangle frame)
{
    float   circ_hspace             = get_relative_object_hspace(frame, nn.nof_layers + 1);
    float   circ_vspace             = get_relative_object_vspace(frame, find_max_nof_neuron_circles(nn));
    float   circ_vpad               = get_object_vpad(circ_vspace);
    float   circ_hpad               = get_object_hpad(circ_hspace);

    return (
        circ_vspace < circ_hspace ?
        ( (circ_vspace - circ_vpad * 2) / 2 ) :
        ( (circ_hspace - circ_hpad * 2) / 2 )
    );
}

nn_r_layer_circ_info get_layer_circle_info(
    nn_t nn, size_t layer, Rectangle frame
)
{
    nn_r_layer_circ_info info;

    NN_ASSERT(layer <= nn.nof_layers);
    
    info.nof_vcircles   = (layer ? nn.biases[layer-1].cols: NN_INPUT(nn).cols);
    info.nof_hcircles   = nn.nof_layers + 1;
    info.vspace = get_relative_object_vspace(frame, info.nof_vcircles);
    info.hspace = get_relative_object_hspace(frame, info.nof_hcircles);
    info.radius = calc_neuron_circle_radius(nn, frame);
    info.hpad   = (info.hspace - info.radius * 2) / 2;
    info.vpad   = (info.vspace - info.radius * 2) / 2;
    info.offset.x = info.hpad * 2 + info.radius * 2;
    info.offset.y = info.vpad * 2 + info.radius * 2;
    info.start_pos.x = (
        frame.x + get_frame_hpad(frame) + info.hpad + info.radius +
        layer * info.offset.x
    );
    info.start_pos.y = ( 
        (frame.y + frame.height) - 
            get_frame_vpad(frame) - info.vpad - info.radius 
    );
    info.init_color = (layer ? NN_RENDER_CIRCLE_COLOR_HIGH: NN_RENDER_CIRCLE_COLOR_OFF);

    return info;
}

float get_frame_hpad(Rectangle frame)
{
    return (frame.width * frame_hpad_multiplier);
}

float get_frame_vpad(Rectangle frame)
{
    return (frame.height * frame_vpad_multiplier);
}

float get_frame_hspace(Rectangle frame)
{
    return (frame.width - get_frame_hpad(frame) * 2);
}

float get_frame_vspace(Rectangle frame)
{
    return (frame.height - get_frame_vpad(frame) * 2);
}

float get_relative_object_vspace(Rectangle frame, int nof_vertical_objects)
{
    NN_ASSERT(nof_vertical_objects);

    return (get_frame_vspace(frame) / nof_vertical_objects);
}

float get_relative_object_hspace(Rectangle frame, int nof_horizontal_objects)
{
    NN_ASSERT(nof_horizontal_objects);

    return (get_frame_hspace(frame) / nof_horizontal_objects);
}

float get_object_vpad(float relative_object_vspace)
{
    return (relative_object_vspace * default_obj_vpad_multiplier);
}

float get_object_hpad(float relative_object_hspace)
{
    return (relative_object_hspace * default_obj_hpad_multiplier);
}


/* ----------------------
 * Static Function Impls:
 * ---------------------- */
static int find_max_nof_neuron_circles(nn_t nn)
{
    int max = (
        NN_INPUT(nn).cols > NN_OUTPUT(nn).cols ? 
            NN_INPUT(nn).cols : NN_OUTPUT(nn).cols
    );

    for (size_t l = 0; l < nn.nof_layers; l++) {
        if (max < nn.biases[l].cols)
            max = nn.biases[l].cols;
    }

    return max;
}


#endif /* NN_RENDER_C_ */
