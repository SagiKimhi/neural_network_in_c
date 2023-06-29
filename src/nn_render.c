#ifndef NN_RENDER_C_
#define NN_RENDER_C_

#include "raylib.h"
#include <nn_render.h>

#define NN_RENDER_GET_COST_GRAPH_POS_Y(y, cost, min, max, vspace)\
    ( (y) + ( 1 - ((cost) - (min)) / ((max) - (min)) ) * (vspace) )

#define NN_RENDER_GET_COST_GRAPH_POS_X(x, i, n, hspace)\
    ( (x) + (hspace) / (n) * (i) )


/* ----------
 * Constants:
 * ---------- */
const int   default_window_width                        = 1200;
const int   default_window_height                       = 800;
const int   default_target_fps                          = 60;
const char  default_window_title[]                      = "nn_gui_deeznuts";
const float frame_hpad_multiplier                       = 0.003f;
const float frame_vpad_multiplier                       = 0.005f;
const float default_obj_vpad_multiplier                 = 0.1f;
const float default_obj_hpad_multiplier                 = 0.1f;
const float default_line_thickness_multiplier           = 1.4f;
const float default_nn_main_frame_thickness             = 2.f;
const float default_nn_graph_frame_thickness            = 1.5f;
const float default_nn_network_frame_thickness          = 1.5f;
const float default_nn_graph_frame_h_multiplier         = 0.95f;
const float default_nn_graph_frame_w_multiplier         = 0.42f;
const float default_nn_network_frame_h_multiplier       = 0.5f;
const float default_nn_network_frame_w_multiplier       = 0.57f;

/* ----------------------
 * Static Function Decls:
 * ---------------------- */
static int nn_render_find_max_nof_neuron_circles(nn_t nn);
static void nn_render_find_max_min_costs(nn_render_cost_info c_info, float *max, float *min);
static void nn_render_update_frames_internal_(
    Rectangle *main_frame, Rectangle *network_frame, Rectangle *graph_frame
);


/* -------------------------------
 * Library Method Implementations:
 * ------------------------------- */
void nn_render_with_default_frames(
    nn_t nn, nn_t gradient, nn_matrix_t ts_in, nn_matrix_t ts_out,
    float rand_low, float rand_high, nn_print_func_t optional_print_func_ptr
)
{
    char buf[256];

    int     i                   = 0;
    int     key                 = 0;
    int     stop_flag           = 0;
    int     auto_stop_triggered = 0;
    int     restart_flag        = 0;
    float   nn_rate             = 5e-1;
    Rectangle main_frame, network_frame, graph_frame;

    nn_render_cost_info cost_info = {
        .items = NN_MALLOC(sizeof(float) * COST_INFO_INIT_CAP),
        .capacity = COST_INFO_INIT_CAP,
        .count = 0,
    };

    nn_rand(nn, rand_low, rand_high);
    NN_ASSERT(cost_info.items);
    cost_info_append(&cost_info, nn_cost(nn, ts_in, ts_out));

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(default_window_width, default_window_height, default_window_title);
    SetTargetFPS(default_target_fps);


    do {
        if (!(i++ % 0x40) && IsWindowReady()) {
            BeginDrawing();
            nn_render_update_frames_internal_(&main_frame, &network_frame, &graph_frame);
            DrawRectangleLinesEx(network_frame, default_nn_network_frame_thickness, GRAY);
            DrawRectangleLinesEx(graph_frame, default_nn_graph_frame_thickness, GRAY);
            ClearBackground(BLACK);
            nn_render_network(nn, network_frame);
            nn_render_cost_graph(graph_frame, cost_info);
            nn_render_model_information_text(main_frame, cost_info, nn_rate, buf);
            EndDrawing();
        }

        while ( (key = GetKeyPressed()) ) {
            switch (key) {
                case KEY_S:
                    stop_flag = !stop_flag;
                    break;

                case KEY_R:
                    restart_flag = 1;
                    break;

                case KEY_DOWN:
                    nn_rate -= 1e-1;
                    break;

                case KEY_UP:
                    nn_rate += 1e-1;
                    break;

                case KEY_P:
                    if (optional_print_func_ptr)
                        optional_print_func_ptr(nn, ts_in, ts_out);
            }

            key = 0;
        }

        if (restart_flag) {
            cost_info.count = 0;
            nn_rand(nn, rand_low, rand_high);
            cost_info_append(&cost_info, nn_cost(nn, ts_in, ts_out));
            restart_flag = 0;
            i = 0;
            continue;
        }

        if (stop_flag)
            continue;

        nn_back_propagation(nn, gradient, ts_in, ts_out);
        nn_learn(nn, gradient, nn_rate);
        cost_info_append(&cost_info, nn_cost(nn, ts_in, ts_out));

        if (!auto_stop_triggered && cost_info.items[cost_info.count-1] <= 5e-4) {
            stop_flag = 1;
            auto_stop_triggered = 1;
        }


    } while (!WindowShouldClose());
    CloseWindow();
    free(cost_info.items);
}

void nn_render_model_information_text(Rectangle frame, nn_render_cost_info cost_info, float rate, char buf[256])
{
    if (!cost_info.count)
        return;


    snprintf(buf, 255, "Rate: %f, Iteration: %zu, Cost: %f", rate, cost_info.count, cost_info.items[cost_info.count-1]);
    DrawText(buf, get_frame_hpad(frame)*3, get_frame_vpad(frame)*3, 20, RAYWHITE);
}

void nn_render_network(nn_t nn, Rectangle nn_frame)
{

    for (size_t l = 0; l <= nn.nof_layers; l++) {
        nn_render_neuron_layer_circles(nn, l, nn_frame);
        nn_render_neuron_layer_connections(nn, l, nn_frame);
    }
}

void nn_render_your_mom(nn_t nn, Rectangle frame)
{
    
    nn_render_network(nn, frame);
    DrawText("<3 Your Mom <3", GetRenderWidth()/4, 20, 80, PINK);
}

void nn_render_cost_graph(Rectangle frame, nn_render_cost_info cost)
{
    const size_t min_count_to_render = 1000;

    size_t  n;
    float   max, min;
    float   frame_hspace, frame_vspace, frame_hpad, frame_vpad;
    Vector2 start_pos, end_pos;

    NN_ASSERT(cost.items);
    NN_ASSERT(cost.count <= cost.capacity);
    
    if (cost.count <= 1)
        return;

    frame_hspace    = get_frame_hspace(frame);
    frame_vspace    = get_frame_vspace(frame);
    frame_hpad      = get_frame_hpad(frame);
    frame_vpad      = get_frame_vpad(frame);
    n = (
        cost.count > min_count_to_render ? 
        cost.count : min_count_to_render 
    );
    nn_render_find_max_min_costs(cost, &max, &min);

    for (size_t i = 0; i + 1 < cost.count; i++) {
        start_pos.x = NN_RENDER_GET_COST_GRAPH_POS_X(
            frame.x+frame_hpad, i, n, frame_hspace
        );
        start_pos.y = NN_RENDER_GET_COST_GRAPH_POS_Y(
            frame.y+frame_vpad, cost.items[i], min, max, frame_vspace
        );
        end_pos.x   = NN_RENDER_GET_COST_GRAPH_POS_X(
            frame.x+frame_hpad, i + 1, n, frame_hspace
        );
        end_pos.y   = NN_RENDER_GET_COST_GRAPH_POS_Y(
            frame.y+frame_vpad, cost.items[i + 1], min, max, frame_vspace
        );

        DrawLineEx(start_pos, end_pos, 1, RED);
    }

    nn_render_x_y_axis_internal(frame);
}

void nn_render_x_y_axis_internal(Rectangle frame)
{
    Vector2 start_pos, end_pos;
    float   frame_hpad, frame_vpad, frame_hspace, frame_vspace;

    frame_hspace    = get_frame_hspace(frame);
    frame_vspace    = get_frame_vspace(frame);
    frame_hpad      = get_frame_hpad(frame);
    frame_vpad      = get_frame_vpad(frame);

    start_pos.x = frame.x + frame_hpad;
    start_pos.y = frame.y + frame_vpad + frame_vspace;
    end_pos.x   = frame.x + frame_hpad + frame_hspace;
    end_pos.y   = start_pos.y;
    DrawLineEx(start_pos, end_pos, 1, WHITE);

    start_pos.x = frame.x + frame_hpad;
    start_pos.y = frame.y + frame_vpad;
    end_pos.x   = start_pos.x;
    end_pos.y   = frame.y + frame_vpad + frame_vspace;
    DrawLineEx(start_pos, end_pos, 1, WHITE);
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
    float   circ_vspace             = get_relative_object_vspace(frame, nn_render_find_max_nof_neuron_circles(nn));
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
static int nn_render_find_max_nof_neuron_circles(nn_t nn)
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

static void nn_render_find_max_min_costs(nn_render_cost_info c_info, float *max, float *min)
{
    *max = FLT_MIN;
    *min = FLT_MAX;

    for (size_t i = 0; i < c_info.count; i++) {
        if (c_info.items[i] > *max) *max = c_info.items[i];
        if (c_info.items[i] < *min) *min = c_info.items[i];
    }

    if (*min > 0) * min = 0;
}

static void nn_render_update_frames_internal_(
    Rectangle *main_frame, Rectangle *network_frame, Rectangle *graph_frame
)
{
    *main_frame = (Rectangle) {
        .x = 0, 
        .y = 0, 
        .width = GetRenderWidth(), 
        .height = GetRenderHeight(),
    };

    *network_frame = (Rectangle) {
        .width  = main_frame->width * default_nn_network_frame_w_multiplier,
        .height = main_frame->height * default_nn_network_frame_h_multiplier,
        .x      = (
            main_frame->width - get_frame_hpad(*main_frame) - 
            main_frame->width * default_nn_network_frame_w_multiplier
        ),
        .y      = (
            main_frame->height - get_frame_vpad(*main_frame) - 
            main_frame->height * default_nn_network_frame_h_multiplier
        ) 
    };

    *graph_frame = (Rectangle) {
        .width  = main_frame->width * default_nn_graph_frame_w_multiplier,
        .height = main_frame->height * default_nn_graph_frame_h_multiplier,
        .x      = get_frame_hpad(*main_frame),
        .y      = (
            main_frame->height - get_frame_vpad(*main_frame) - 
            main_frame->height * default_nn_graph_frame_h_multiplier
        ),
    };

}


#endif /* NN_RENDER_C_ */
