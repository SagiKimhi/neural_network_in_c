#ifndef NN_RENDER_H_
#define NN_RENDER_H_

/* ---------
 * Includes:
 * --------- */
#include <raylib.h>

#ifndef STANDALONE_NN_RENDER_IMPLEMENTATION

# ifndef NN_IMPLEMENTATION
#  define NN_IMPLEMENTATION
#  include "nn.h"
# endif

#endif /* STANDALONE_NN_RENDER_IMPLEMENTATION */

/* --------
 * Defines:
 * -------- */
/* User May define his own print and data loading functions
 * and function args to be called when P/L keys are pressed
 * when running the gui application */
#ifndef NN_RENDER_RESULTS_FUNC_T
typedef void
nn_render_results_func_t(Rectangle frame, nn_t nn);

# define NN_RENDER_RESULTS_FUNC_T nn_render_results_func_t

# define NN_RENDER_RESULTS_FUNC_ARGS results_frame, nn

#elif !defined NN_RENDER_RESULTS_FUNC_ARGS
# error "Please define NN_RENDER_RESULTS_FUNC_ARGS for passing arguments"\
            "to the user defined nn_render_results_func_t function"
#endif /* NN_RENDER_RESULTS_FUNC_T */

#ifndef NN_PRINT_RESULTS_FUNC_T
typedef void
nn_print_func_t(nn_t nn, nn_matrix_t ts_in, nn_matrix_t ts_out);

# define NN_PRINT_RESULTS_FUNC_T nn_print_func_t

# define NN_PRINT_RESULTS_FUNC_ARGS *nn, ts_in, ts_out

#elif !defined NN_PRINT_RESULTS_FUNC_ARGS
# error "Please define NN_PRINT_RESULTS_ARGS for passing arguments"\
            "to a user defined nn_print_func_t function"

#endif /* NN_PRING_RESULTS_FUNC_T */

#ifndef NN_LOAD_DATA_FUNC_T
typedef void
nn_load_data_func_t(nn_matrix_t *ts_inout);

# define NN_LOAD_DATA_FUNC_T nn_load_data_func_t

# define NN_LOAD_DATA_FUNC_ARGS ts_inout

#elif !defined NN_LOAD_DATA_FUNC_ARGS
# error "Please define NN_LOAD_DATA_FUNC_ARGS for passing arguments"\
            "to user defined function of type nn_load_data_func_t"

#endif /* NN_LOAD_DATA_FUNC_T */

/* Default Circle and Weight lines colors - user may define
 * other colors instead before including the header */
#ifndef NN_RENDER_CIRCLE_COLOR_HIGH
# define NN_RENDER_CIRCLE_COLOR_HIGH GREEN
#endif

#ifndef NN_RENDER_CIRCLE_COLOR_OFF
# define NN_RENDER_CIRCLE_COLOR_OFF GRAY
#endif

#ifndef NN_RENDER_CIRCLE_COLOR_LOW
# define NN_RENDER_CIRCLE_COLOR_LOW PINK
#endif

#ifndef NN_RENDER_LINE_COLOR_HIGH
# define NN_RENDER_LINE_COLOR_HIGH GREEN
#endif

#ifndef NN_RENDER_LINE_COLOR_OFF
# define NN_RENDER_LINE_COLOR_OFF GRAY
#endif

#ifndef NN_RENDER_LINE_COLOR_LOW
# define NN_RENDER_LINE_COLOR_LOW PINK
#endif

#ifndef COST_INFO_INIT_CAP
# define COST_INFO_INIT_CAP 256
#endif

#define cost_info_append(cost_info, item)                                   \
    do {                                                                    \
        if ((cost_info)->count >= (cost_info)->capacity) {                  \
            (cost_info)->capacity                                           \
                = ((cost_info)->capacity == 0 ? COST_INFO_INIT_CAP          \
                                              : (cost_info)->capacity * 2); \
            (cost_info)->items = realloc(                                   \
                (cost_info)->items,                                         \
                (cost_info)->capacity * sizeof(*(cost_info)->items));       \
            NN_ASSERT((cost_info)->items != NULL && "Buy more RAM lol");    \
        }                                                                   \
                                                                            \
        (cost_info)->items[(cost_info)->count++] = (item);                  \
    } while (0)

/* ---------
 * Typedefs:
 * --------- */
typedef struct {
    int nof_hcircles;
    int nof_vcircles;
    float radius;
    float vspace;
    float hspace;
    float vpad;
    float hpad;
    Vector2 offset;
    Vector2 start_pos;
    Color init_color;
} nn_r_layer_circ_info;

typedef struct {
    float *items;
    size_t count;
    size_t capacity;
} nn_render_cost_info;

typedef struct {
    nn_arch_t nn_arch;
    float nn_rate;
    float rand_low;
    float rand_high;
    size_t batch_size;
    size_t batches_per_frame;
    bool shuffle_rows;
    NN_LOAD_DATA_FUNC_T *load_data_fn;
    NN_RENDER_RESULTS_FUNC_T *optional_render_results_fn;
    NN_PRINT_RESULTS_FUNC_T *optional_print_results_fn;
} nn_render_cfg_t;

/* ----------------------
 * Function Declarations:
 * ---------------------- */

/* Get Methods */
extern float
get_frame_vpad(Rectangle frame);
extern float
get_frame_hpad(Rectangle frame);
extern float
get_frame_vspace(Rectangle frame);
extern float
get_frame_hspace(Rectangle frame);
extern float
get_relative_object_vspace(Rectangle frame, int nof_vertical_objects);
extern float
get_relative_object_hspace(Rectangle frame, int nof_horizontal_objects);
extern float
get_object_vpad(float relative_object_vspace);
extern float
get_object_hpad(float relative_object_hspace);
extern nn_r_layer_circ_info
get_layer_circle_info(nn_t nn, size_t layer, Rectangle frame);

/* Render Methods */
extern void
nn_render_model_information_text(
    Rectangle frame,
    nn_render_cfg_t cfg,
    nn_render_cost_info cost_info,
    float rate);
extern void
nn_render_x_y_axis_internal(Rectangle frame);
extern void
nn_render_cost_graph(Rectangle frame, nn_render_cost_info cost);
extern void
nn_render_neuron_layer_circles(nn_t nn, size_t layer, Rectangle frame);
extern void
nn_render_neuron_layer_connections(nn_t nn, size_t layer, Rectangle frame);
extern void
nn_render_your_mom(nn_t nn, Rectangle nn_frame);
extern void
nn_render_network(nn_t nn, Rectangle nn_frame);
extern void
nn_render_draw_gui_frames(
    nn_render_cfg_t cfg,
    nn_t nn,
    nn_matrix_t *ts_inout,
    nn_render_cost_info cost_info);
extern void
nn_render_gui(nn_render_cfg_t cfg);

/* Gui Menu Methods */

extern void
nn_render_perform_interactive_gui_menu_actions(
    int key,
    uint8_t *gui_flags,
    nn_render_cfg_t *cfg,
    nn_t *nn,
    nn_matrix_t *ts_inout,
    nn_render_cost_info *cost_info);
extern void
nn_render_gui_menu_adjust_rate(
    int key, uint8_t *gui_flags, nn_render_cfg_t *cfg);
extern void
nn_render_gui_menu_print_training_data(nn_t nn, nn_matrix_t ts_inout);
extern void
nn_render_gui_menu_print_model(nn_t nn);
extern void
nn_render_gui_menu_load_training_data(
    nn_render_cfg_t *cfg, nn_matrix_t *ts_inout);
extern void
nn_render_gui_menu_save_nn_model(nn_t *nn);
extern void
nn_render_gui_menu_load_nn_model(
    nn_t *nn, nn_render_cost_info *cost_info, nn_matrix_t *ts_inout);
extern void
nn_render_gui_menu_print_help_menu(void);

#endif /* NN_RENDER_H_ */

#ifdef NN_RENDER_IMPLEMENTATION

/* -----------------------------------------------------------------------------
 * Constants:
 * ----------------------------------------------------------------------------- */

const int default_window_width = 1200;
const int default_window_height = 800;
const int default_target_fps = 60;
const char default_window_title[] = "nn_gui_deez_nuts";
const float frame_hpad_multiplier = 0.003f;
const float frame_vpad_multiplier = 0.005f;
const float default_obj_vpad_multiplier = 0.1f;
const float default_obj_hpad_multiplier = 0.1f;
const float default_line_thickness_multiplier = 1.4f;
const float default_nn_main_frame_thickness = 2.f;
const float default_nn_graph_frame_thickness = 1.5f;
const float default_nn_network_frame_thickness = 1.5f;
const float default_nn_graph_frame_h_multiplier = 0.95f;
const float default_nn_graph_frame_w_multiplier = 0.42f;
const float default_nn_network_frame_h_multiplier = 0.5f;
const float default_nn_network_frame_w_multiplier = 0.57f;
const float default_nn_results_frame_h_multiplier = 0.45f;
const float default_nn_learning_rate = 0.1;

/* -----------------
 * Internal Defines:
 * ----------------- */
#define NN_RENDER_GET_COST_GRAPH_POS_Y(y, cost, min, max, vspace) \
    ((y) + (1 - ((cost) - (min)) / ((max) - (min))) * (vspace))

#define NN_RENDER_GET_COST_GRAPH_POS_X(x, i, n, hspace) \
    ((x) + (hspace) / (n) * (i))

/* -----------------------------------------------------------------------------
 * Internal Typedefs:
 * ----------------------------------------------------------------------------- */
typedef enum {
    NN_RENDER_RESTART_FLAG = (1 << 0),
    NN_RENDER_PAUSE_FLAG = (1 << 1),
    NN_RENDER_AUTO_PAUSE_TRIGGERED_FLAG = (1 << 2),
} nn_render_gui_flags_internal_t;

/* -----------------------------------------------------------------------------
 * Static Function Decls:
 * ----------------------------------------------------------------------------- */

static int
nn_render_find_max_nof_neuron_circles(nn_t nn);

static float
nn_render_calc_neuron_circle_radius(nn_t nn, Rectangle frame);

static void
nn_render_find_max_min_costs(nn_render_cost_info c_info, float *max, float *min);

static void
nn_render_update_frames_internal_(
    Rectangle *main_frame,
    Rectangle *network_frame,
    Rectangle *graph_frame,
    Rectangle *results_frame);

/* -----------------------------------------------------------------------------
 * Library Method Implementations:
 * ----------------------------------------------------------------------------- */

void
nn_render_gui(nn_render_cfg_t cfg)
{
    NN_ASSERT(cfg.load_data_fn);

    int key_pressed = 0;
    uint8_t gui_flags = (0 | NN_RENDER_PAUSE_FLAG);
    nn_render_cost_info cost_info = {.count = 0, .capacity = 0, .items = 0};
    nn_t nn = {.nof_layers = 0, .biases = 0, .weights = 0, .activations = 0};
    nn_t gradient
        = {.nof_layers = 0, .biases = 0, .weights = 0, .activations = 0};
    nn_matrix_t *ts_inout
        = &(nn_matrix_t){.cols = 0, .rows = 0, .stride = 0, .data = 0};
    nn_batch_t batch = {.begin = 0, .cost = 0, .finished = false};
    nn_matrix_t ts_in, ts_out;

    nn = nn_alloc(cfg.nn_arch.arch, cfg.nn_arch.arch_len);
    gradient = nn_alloc(cfg.nn_arch.arch, cfg.nn_arch.arch_len);
    nn_rand(nn, cfg.rand_low, cfg.rand_high);

    cfg.load_data_fn(NN_LOAD_DATA_FUNC_ARGS);

    if (cfg.shuffle_rows)
        nn_matrix_shuffle_rows(*ts_inout);

    ts_in.rows = ts_inout->rows;
    ts_out.rows = ts_inout->rows;
    ts_in.cols = NN_INPUT(nn).cols;
    ts_out.cols = NN_OUTPUT(nn).cols;
    ts_in.stride = ts_inout->cols;
    ts_out.stride = ts_inout->cols;
    ts_in.data = &NN_MATRIX_AT(*ts_inout, 0, 0);
    ts_out.data = &NN_MATRIX_AT(*ts_inout, 0, NN_INPUT(nn).cols);

    cost_info_append(&cost_info, nn_cost(nn, ts_in, ts_out));

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(
        default_window_width, default_window_height, default_window_title);
    SetTargetFPS(default_target_fps);

    while (!WindowShouldClose()) {
        nn_render_draw_gui_frames(cfg, nn, ts_inout, cost_info);

        while ((key_pressed = GetKeyPressed())) {
            nn_render_perform_interactive_gui_menu_actions(
                key_pressed, &gui_flags, &cfg, &nn, ts_inout, &cost_info);
            key_pressed = 0;
        }

        if (gui_flags & NN_RENDER_RESTART_FLAG) {
            gui_flags ^= NN_RENDER_RESTART_FLAG;
            cost_info.count = 0;
            nn_rand(nn, cfg.rand_low, cfg.rand_high);
            cost_info_append(&cost_info, nn_cost(nn, ts_in, ts_out));
            continue;
        }

        if (gui_flags & NN_RENDER_PAUSE_FLAG)
            continue;

        for (size_t i = 0; i < cfg.batches_per_frame; i++) {
            nn_mini_batch_process(
                &batch, cfg.batch_size, nn, gradient, *ts_inout, cfg.nn_rate);

            if (batch.finished) {
                if (cfg.shuffle_rows)
                    nn_matrix_shuffle_rows(*ts_inout);

                cost_info_append(&cost_info, batch.cost);
            }
        }

        if (gui_flags & NN_RENDER_AUTO_PAUSE_TRIGGERED_FLAG)
            continue;

        if (cost_info.items[cost_info.count - 1] <= 5e-4) {
            gui_flags |= NN_RENDER_PAUSE_FLAG;
            gui_flags |= NN_RENDER_AUTO_PAUSE_TRIGGERED_FLAG;
        }
    }

    CloseWindow();
    nn_free(nn);
    nn_free(gradient);
    nn_matrix_free(*ts_inout);
    free(cost_info.items);
}

void
nn_render_draw_gui_frames(
    nn_render_cfg_t cfg,
    nn_t nn,
    nn_matrix_t *ts_inout,
    nn_render_cost_info cost_info)
{
    Rectangle main_frame, network_frame, graph_frame, results_frame;

    if (!IsWindowReady())
        return;

    BeginDrawing();

    ClearBackground(BLACK);
    nn_render_update_frames_internal_(
        &main_frame, &network_frame, &graph_frame, &results_frame);
    DrawRectangleLinesEx(
        network_frame, default_nn_network_frame_thickness, GRAY);
    DrawRectangleLinesEx(graph_frame, default_nn_graph_frame_thickness, GRAY);
    DrawRectangleLinesEx(results_frame, default_nn_graph_frame_thickness, GRAY);
    nn_render_network(nn, network_frame);
    nn_render_cost_graph(graph_frame, cost_info);
    nn_render_model_information_text(main_frame, cfg, cost_info, cfg.nn_rate);

    if (cfg.optional_render_results_fn != NULL) {
        nn_matrix_t ts_in, ts_out;

        ts_in.rows = ts_inout->rows;
        ts_out.rows = ts_inout->rows;
        ts_in.cols = NN_INPUT(nn).cols;
        ts_out.cols = NN_OUTPUT(nn).cols;
        ts_in.stride = ts_inout->cols;
        ts_out.stride = ts_inout->cols;
        ts_in.data = &NN_MATRIX_AT(*ts_inout, 0, 0);
        ts_out.data = &NN_MATRIX_AT(*ts_inout, 0, NN_INPUT(nn).cols);
        cfg.optional_render_results_fn(NN_RENDER_RESULTS_FUNC_ARGS);
    }

    EndDrawing();
}

void
nn_render_perform_interactive_gui_menu_actions(
    int key,
    uint8_t *gui_flags,
    nn_render_cfg_t *cfg,
    nn_t *nn,
    nn_matrix_t *ts_inout,
    nn_render_cost_info *cost_info)
{
    switch (key) {
    case KEY_ESCAPE:
        break;

    case KEY_SPACE:
        (*gui_flags) ^= NN_RENDER_PAUSE_FLAG;
        break;

    case KEY_R:
        (*gui_flags) |= NN_RENDER_RESTART_FLAG;
        break;

    case KEY_P:
        if (cfg->optional_print_results_fn) {
            nn_matrix_t ts_in, ts_out;

            ts_in.rows = ts_inout->rows;
            ts_out.rows = ts_inout->rows;
            ts_in.cols = NN_INPUT(*nn).cols;
            ts_out.cols = NN_OUTPUT(*nn).cols;
            ts_in.stride = ts_inout->cols;
            ts_out.stride = ts_inout->cols;
            ts_in.data = &NN_MATRIX_AT(*ts_inout, 0, 0);
            ts_out.data = &NN_MATRIX_AT(*ts_inout, 0, NN_INPUT(*nn).cols);
            cfg->optional_print_results_fn(NN_PRINT_RESULTS_FUNC_ARGS);
        }
        break;

    case KEY_D:
        nn_render_gui_menu_print_training_data(*nn, *ts_inout);
        break;

    case KEY_M:
        nn_render_gui_menu_print_model(*nn);
        break;

    case KEY_L:
        (*gui_flags) |= NN_RENDER_PAUSE_FLAG;
        nn_render_gui_menu_load_training_data(cfg, ts_inout);
        break;

    case KEY_H:
        nn_render_gui_menu_print_help_menu();
        break;

    case KEY_F1: {
        nn_render_gui_menu_save_nn_model(nn);
        break;
    }

    case KEY_F2:
        (*gui_flags) |= NN_RENDER_PAUSE_FLAG;
        nn_render_gui_menu_load_nn_model(nn, cost_info, ts_inout);
        break;

    case KEY_DOWN:
    case KEY_UP:
    case KEY_KP_0:
    case KEY_KP_1:
    case KEY_KP_2:
    case KEY_KP_3:
    case KEY_KP_4:
    case KEY_KP_5:
    case KEY_KP_6:
    case KEY_KP_7:
    case KEY_KP_8:
    case KEY_KP_9:
        nn_render_gui_menu_adjust_rate(key, gui_flags, cfg);
        break;
    }
}

void
nn_render_gui_menu_adjust_rate(int key, uint8_t *gui_flags, nn_render_cfg_t *cfg)
{
    switch (key) {
    case KEY_DOWN:
        if (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT))
            for (size_t i = 0; i < 69420; i++)
                fprintf(stdout, "69420\n");

        else if (IsKeyDown(KEY_LEFT_SHIFT))
            cfg->nn_rate -= 1e-3;

        else if (IsKeyDown(KEY_LEFT_CONTROL))
            cfg->nn_rate -= 1e-2;

        else
            cfg->nn_rate -= 1e-1;

        break;

    case KEY_UP:
        if (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT))
            for (size_t i = 0; i < 69420; i++)
                fprintf(stdout, "69420\n");

        else if (IsKeyDown(KEY_LEFT_SHIFT))
            cfg->nn_rate += 1e-3;

        else if (IsKeyDown(KEY_LEFT_CONTROL))
            cfg->nn_rate += 1e-2;

        else
            cfg->nn_rate += 1e-1;

        break;

    case KEY_KP_0:
    case KEY_KP_1:
    case KEY_KP_2:
    case KEY_KP_3:
    case KEY_KP_4:
    case KEY_KP_5:
    case KEY_KP_6:
    case KEY_KP_7:
    case KEY_KP_8:
    case KEY_KP_9:
        cfg->nn_rate = (key - KEY_KP_0);
        break;
    }
}

void
nn_render_gui_menu_print_training_data(nn_t nn, nn_matrix_t ts_inout)
{
    nn_matrix_t ts_in, ts_out;

    ts_in.rows = ts_inout.rows;
    ts_out.rows = ts_inout.rows;
    ts_in.cols = NN_INPUT(nn).cols;
    ts_out.cols = NN_OUTPUT(nn).cols;
    ts_in.stride = ts_inout.cols;
    ts_out.stride = ts_inout.cols;
    ts_in.data = &NN_MATRIX_AT(ts_inout, 0, 0);
    ts_out.data = &NN_MATRIX_AT(ts_inout, 0, NN_INPUT(nn).cols);

    fprintf(stdout, "\n\n----------------------------------\n\n");
    NN_MATRIX_PRINT(ts_in);
    NN_MATRIX_PRINT(ts_out);
    fprintf(stdout, "ts_in.rows: %zu\n", ts_in.rows);
    fprintf(stdout, "ts_in.cols: %zu\n", ts_in.cols);
    fprintf(stdout, "ts_out.rows: %zu\n", ts_out.rows);
    fprintf(stdout, "ts_out.cols: %zu\n", ts_out.cols);
    fprintf(stdout, "\n\n----------------------------------\n\n");
}

void
nn_render_gui_menu_print_model(nn_t nn)
{
    fprintf(stdout, "\n\n----------------------------------\n\n");
    NN_PRINT(nn);
    fprintf(stdout, "\n\n----------------------------------\n\n");
}

void
nn_render_gui_menu_load_training_data(
    nn_render_cfg_t *cfg, nn_matrix_t *ts_inout)
{
    nn_matrix_free(*ts_inout);
    ts_inout->data = NULL;
    cfg->load_data_fn(NN_LOAD_DATA_FUNC_ARGS);
    NN_ASSERT(ts_inout->data);
}

void
nn_render_gui_menu_save_nn_model(nn_t *nn)
{
    FILE *mfp = NULL;
    char buf[FILENAME_MAX + 1] = {0};

    fprintf(stdout, "Please provide a path to save the model to: ");

    if (nn_io_get_line(buf, FILENAME_MAX, stdin) <= 0)
        return;

    if (!(mfp = nn_io_fopen(buf, ".nn_model", "wb")))
        return;

    nn_save_model(mfp, *nn);
    fclose(mfp);
    fprintf(stdout, "Model was successfully saved to: %s.nn_model\n", buf);
}

void
nn_render_gui_menu_load_nn_model(
    nn_t *nn, nn_render_cost_info *cost_info, nn_matrix_t *ts_inout)
{
    FILE *mfp = NULL;
    char buf[FILENAME_MAX + 1] = {0};
    nn_matrix_t ts_in, ts_out;

    ts_in.rows = ts_inout->rows;
    ts_out.rows = ts_inout->rows;
    ts_in.cols = NN_INPUT(*nn).cols;
    ts_out.cols = NN_OUTPUT(*nn).cols;
    ts_in.stride = ts_inout->cols;
    ts_out.stride = ts_inout->cols;
    ts_in.data = &NN_MATRIX_AT(*ts_inout, 0, 0);
    ts_out.data = &NN_MATRIX_AT(*ts_inout, 0, NN_INPUT(*nn).cols);

    fprintf(stdout, "Please provide a path to load the model from: ");

    if (nn_io_get_line(buf, FILENAME_MAX, stdin) <= 0)
        return;

    if (!(mfp = nn_io_fopen(buf, ".nn_model", "rb")))
        return;

    nn_free(*nn);

    (*nn) = nn_load_model(mfp);
    cost_info->count = 0;
    cost_info_append(cost_info, nn_cost(*nn, ts_in, ts_out));

    fclose(mfp);
}

void
nn_render_gui_menu_print_help_menu(void)
{
    fprintf(
        stdout,
        "\n\n--------------------------------------------------------------------------------------\n"
        "Help Menu:\n"
        "--------------------------------------------------------------------------------------\n"
        "Spacebar       : Pause/Continue\n"
        "R              : Restart\n"
        "P              : Call optional user defined print results function (if provided)\n"
        "D              : Print training data matrices\n"
        "M              : Print nn model\n"
        "L              : Call user defined function to load training data\n"
        "H              : Print this menu\n"
        "F1             : Save current nn model to binary file\n"
        "F2             : Load a model from an existing binary file\n"
        "Down/Up        : Decrement/Increment learning rate by 0.1f\n"
        "Ctrl+Down/Up   : Decrement/Increment learning rate by 0.01f\n"
        "Shift+Down/Up  : Decrement/Increment learning rate by 0.001f\n"
        "Alt+Down/Up    : Prints \"69420\" 69420 times (Recommended)\n"
        "\n--------------------------------------------------------------------------------------\n\n");
}

void
nn_render_model_information_text(
    Rectangle frame,
    nn_render_cfg_t cfg,
    nn_render_cost_info cost_info,
    float rate)
{
    char buf[256];

    if (!cost_info.count)
        return;

    snprintf(
        buf,
        NN_SIZEOF_ARR(buf) - 1,
        "Rate: %2.8f, Iteration: %zu, Cost: %.12f",
        rate,
        cost_info.count - 1,
        cost_info.items[cost_info.count - 1]);
    DrawText(
        buf, get_frame_hpad(frame) * 3, get_frame_vpad(frame) * 3, 20, RAYWHITE);
}

void
nn_render_network(nn_t nn, Rectangle nn_frame)
{
    for (size_t l = 0; l <= nn.nof_layers; l++) {
        nn_render_neuron_layer_circles(nn, l, nn_frame);
        nn_render_neuron_layer_connections(nn, l, nn_frame);
    }
}

void
nn_render_your_mom(nn_t nn, Rectangle frame)
{
    nn_render_network(nn, frame);
    DrawText("<3 Your Mom <3", GetRenderWidth() / 4, 20, 80, PINK);
}

void
nn_render_cost_graph(Rectangle frame, nn_render_cost_info cost)
{
    const size_t min_count_to_render = 1000;

    size_t n;
    float max, min;
    float frame_hspace, frame_vspace, frame_hpad, frame_vpad;
    Vector2 start_pos, end_pos;

    NN_ASSERT(cost.items);
    NN_ASSERT(cost.count <= cost.capacity);

    if (cost.count <= 1)
        return;

    frame_hspace = get_frame_hspace(frame);
    frame_vspace = get_frame_vspace(frame);
    frame_hpad = get_frame_hpad(frame);
    frame_vpad = get_frame_vpad(frame);
    n = (cost.count > min_count_to_render ? cost.count : min_count_to_render);
    nn_render_find_max_min_costs(cost, &max, &min);

    for (size_t i = 0; i + 1 < cost.count; i++) {
        start_pos.x = NN_RENDER_GET_COST_GRAPH_POS_X(
            frame.x + frame_hpad, i, n, frame_hspace);
        start_pos.y = NN_RENDER_GET_COST_GRAPH_POS_Y(
            frame.y + frame_vpad, cost.items[i], min, max, frame_vspace);
        end_pos.x = NN_RENDER_GET_COST_GRAPH_POS_X(
            frame.x + frame_hpad, i + 1, n, frame_hspace);
        end_pos.y = NN_RENDER_GET_COST_GRAPH_POS_Y(
            frame.y + frame_vpad, cost.items[i + 1], min, max, frame_vspace);

        DrawLineEx(start_pos, end_pos, 1, RED);
    }

    nn_render_x_y_axis_internal(frame);
}

void
nn_render_x_y_axis_internal(Rectangle frame)
{
    Vector2 start_pos, end_pos;
    float frame_hpad, frame_vpad, frame_hspace, frame_vspace;

    frame_hspace = get_frame_hspace(frame);
    frame_vspace = get_frame_vspace(frame);
    frame_hpad = get_frame_hpad(frame);
    frame_vpad = get_frame_vpad(frame);

    start_pos.x = frame.x + frame_hpad;
    start_pos.y = frame.y + frame_vpad + frame_vspace;
    end_pos.x = frame.x + frame_hpad + frame_hspace;
    end_pos.y = start_pos.y;
    DrawLineEx(start_pos, end_pos, 1, WHITE);

    start_pos.x = frame.x + frame_hpad;
    start_pos.y = frame.y + frame_vpad;
    end_pos.x = start_pos.x;
    end_pos.y = frame.y + frame_vpad + frame_vspace;
    DrawLineEx(start_pos, end_pos, 1, WHITE);
}

void
nn_render_neuron_layer_connections(nn_t nn, size_t layer, Rectangle frame)
{
    Color line_color;
    Vector2 line_start_pos, line_end_pos;
    nn_r_layer_circ_info curr_circ_info, next_circ_info;

    NN_ASSERT(layer <= nn.nof_layers);

    if (layer == nn.nof_layers)
        return;

    curr_circ_info = get_layer_circle_info(nn, layer, frame);
    next_circ_info = get_layer_circle_info(nn, layer + 1, frame);
    line_start_pos.x = curr_circ_info.start_pos.x + curr_circ_info.radius;
    line_start_pos.y = curr_circ_info.start_pos.y;
    line_end_pos.x = next_circ_info.start_pos.x - next_circ_info.radius;
    line_end_pos.y = next_circ_info.start_pos.y;

    for (size_t i = 0; i < curr_circ_info.nof_vcircles; i++) {
        Color line_color_high = NN_RENDER_LINE_COLOR_HIGH;
        Color line_color_low = NN_RENDER_LINE_COLOR_LOW;

        for (size_t j = 0; j < next_circ_info.nof_vcircles; j++) {
            float line_weight_val
                = sigmoidf(NN_MATRIX_AT(nn.weights[layer], i, j));

            line_color_high.a = floorf(255.f * line_weight_val);
            line_color
                = ColorAlphaBlend(line_color_low, line_color_high, WHITE);

            DrawLineEx(line_start_pos, line_end_pos, 1, line_color);
            line_end_pos.y -= next_circ_info.offset.y;
        }

        line_start_pos.y -= curr_circ_info.offset.y;
        line_end_pos.y = next_circ_info.start_pos.y;
    }
}

void
nn_render_neuron_layer_circles(nn_t nn, size_t layer, Rectangle frame)
{
    nn_r_layer_circ_info circ_info;
    NN_ASSERT(layer <= nn.nof_layers);

    circ_info = get_layer_circle_info(nn, layer, frame);

    for (size_t i = 0; i < circ_info.nof_vcircles; i++) {
        if (layer) {
            Color circ_color_high = NN_RENDER_CIRCLE_COLOR_HIGH;
            Color circ_color_low = NN_RENDER_CIRCLE_COLOR_LOW;

            circ_color_high.a = floorf(
                255.f * sigmoidf(NN_MATRIX_AT(nn.biases[layer - 1], 0, i)));
            circ_info.init_color
                = ColorAlphaBlend(circ_color_low, circ_color_high, WHITE);
        }

        DrawCircleV(
            circ_info.start_pos, circ_info.radius, circ_info.init_color);
        circ_info.start_pos.y -= circ_info.offset.y;
    }
}

nn_r_layer_circ_info
get_layer_circle_info(nn_t nn, size_t layer, Rectangle frame)
{
    nn_r_layer_circ_info info;

    NN_ASSERT(layer <= nn.nof_layers);

    info.nof_vcircles = (layer ? nn.biases[layer - 1].cols : NN_INPUT(nn).cols);
    info.nof_hcircles = nn.nof_layers + 1;
    info.vspace = get_relative_object_vspace(frame, info.nof_vcircles);
    info.hspace = get_relative_object_hspace(frame, info.nof_hcircles);
    info.radius = nn_render_calc_neuron_circle_radius(nn, frame);
    info.hpad = (info.hspace - info.radius * 2) / 2;
    info.vpad = (info.vspace - info.radius * 2) / 2;
    info.offset.x = info.hpad * 2 + info.radius * 2;
    info.offset.y = info.vpad * 2 + info.radius * 2;
    info.start_pos.x
        = (frame.x + get_frame_hpad(frame) + info.hpad + info.radius
           + layer * info.offset.x);
    info.start_pos.y
        = ((frame.y + frame.height) - get_frame_vpad(frame) - info.vpad
           - info.radius);
    info.init_color
        = (layer ? NN_RENDER_CIRCLE_COLOR_HIGH : NN_RENDER_CIRCLE_COLOR_OFF);

    return info;
}

float
get_frame_hpad(Rectangle frame)
{
    return (frame.width * frame_hpad_multiplier);
}

float
get_frame_vpad(Rectangle frame)
{
    return (frame.height * frame_vpad_multiplier);
}

float
get_frame_hspace(Rectangle frame)
{
    return (frame.width - get_frame_hpad(frame) * 2);
}

float
get_frame_vspace(Rectangle frame)
{
    return (frame.height - get_frame_vpad(frame) * 2);
}

float
get_relative_object_vspace(Rectangle frame, int nof_vertical_objects)
{
    NN_ASSERT(nof_vertical_objects);

    return (get_frame_vspace(frame) / nof_vertical_objects);
}

float
get_relative_object_hspace(Rectangle frame, int nof_horizontal_objects)
{
    NN_ASSERT(nof_horizontal_objects);

    return (get_frame_hspace(frame) / nof_horizontal_objects);
}

float
get_object_vpad(float relative_object_vspace)
{
    return (relative_object_vspace * default_obj_vpad_multiplier);
}

float
get_object_hpad(float relative_object_hspace)
{
    return (relative_object_hspace * default_obj_hpad_multiplier);
}

/* ----------------------
 * Static Function Impls:
 * ---------------------- */
static int
nn_render_find_max_nof_neuron_circles(nn_t nn)
{
    int max
        = (NN_INPUT(nn).cols > NN_OUTPUT(nn).cols ? NN_INPUT(nn).cols
                                                  : NN_OUTPUT(nn).cols);

    for (size_t l = 0; l < nn.nof_layers; l++) {
        if (max < nn.biases[l].cols)
            max = nn.biases[l].cols;
    }

    return max;
}

static void
nn_render_find_max_min_costs(nn_render_cost_info c_info, float *max, float *min)
{
    *max = FLT_MIN;
    *min = FLT_MAX;

    for (size_t i = 0; i < c_info.count; i++) {
        if (c_info.items[i] > *max)
            *max = c_info.items[i];
        if (c_info.items[i] < *min)
            *min = c_info.items[i];
    }

    if (*min > 0)
        *min = 0;
}

static void
nn_render_update_frames_internal_(
    Rectangle *main_frame,
    Rectangle *network_frame,
    Rectangle *graph_frame,
    Rectangle *results_frame)
{
    *main_frame = (Rectangle){
        .x = 0,
        .y = 0,
        .width = GetRenderWidth(),
        .height = GetRenderHeight(),
    };

    *network_frame = (Rectangle){
        .width = main_frame->width * default_nn_network_frame_w_multiplier,
        .height = main_frame->height * default_nn_network_frame_h_multiplier,
        .x
        = (main_frame->width - get_frame_hpad(*main_frame)
           - main_frame->width * default_nn_network_frame_w_multiplier),
        .y
        = (main_frame->height - get_frame_vpad(*main_frame)
           - main_frame->height * default_nn_network_frame_h_multiplier)};

    *graph_frame = (Rectangle){
        .width = main_frame->width * default_nn_graph_frame_w_multiplier,
        .height = main_frame->height * default_nn_graph_frame_h_multiplier,
        .x = get_frame_hpad(*main_frame),
        .y
        = (main_frame->height - get_frame_vpad(*main_frame)
           - main_frame->height * default_nn_graph_frame_h_multiplier),
    };

    *results_frame = (Rectangle){
        .width = network_frame->width,
        .height = main_frame->height * default_nn_results_frame_h_multiplier,
        .x = network_frame->x,
        .y = graph_frame->y,
    };
}

static float
nn_render_calc_neuron_circle_radius(nn_t nn, Rectangle frame)
{
    float circ_hspace = get_relative_object_hspace(frame, nn.nof_layers + 1);
    float circ_vspace = get_relative_object_vspace(
        frame, nn_render_find_max_nof_neuron_circles(nn));
    float circ_vpad = get_object_vpad(circ_vspace);
    float circ_hpad = get_object_hpad(circ_hspace);

    return (
        circ_vspace < circ_hspace ? ((circ_vspace - circ_vpad * 2) / 2)
                                  : ((circ_hspace - circ_hpad * 2) / 2));
}

#endif /* NN_RENDER_IMPLEMENTATION */
