#include <raylib.h>
#include <nn_render.h>

#define BITS    (4)
#define MAX_N   (1 << BITS)


int find_max_nof_neurons(nn_t nn);
float calc_neuron_circle_radius(nn_t nn, Rectangle frame);
void nn_render_neuron_circles(nn_t nn, Rectangle nn_frame);

int main(int argc, char **argv)
{
    srand(time(0));
    /* General Window Constants */
    const int       default_window_width    = 1200;
    const int       default_window_height   = 800;
    const int       default_target_fps      = 60;
    const char      default_window_title[]  = "nn_visualization";
    const float     nn_rec_thickness        = 4.f;
    const float     border_hpad_mult        = 0.02;
    const float     border_vpad_mult        = 0.02;

    /* NN Constants */

    Rectangle   nn_frame;
    size_t      arch[]      = { 2 * BITS, 4 * BITS, 4*BITS, BITS + 1};
    size_t      arch_len    = NN_SIZEOF_ARR(arch);
    nn_t        nn          = nn_alloc(arch, arch_len);

    nn_rand(nn, -10, 20);

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(default_window_width, default_window_height, default_window_title);
    SetTargetFPS(default_target_fps);

    while (!WindowShouldClose()) {
        int     w                       = GetRenderWidth();
        int     h                       = GetRenderHeight();
#if 0
        nn_frame = (Rectangle) {
            .x      = w - w * border_hpad_mult - nn_frame.width, 
            .y      = h - h * border_vpad_mult - nn_frame.height, 
            .height = GetRenderHeight()/3.f, 
            .width  = GetRenderWidth()/2.5f,
        };
#else
        Rectangle   nn_frame      = {
            .x      = w/2 - nn_frame.width/2, 
            .y      = h/2 - nn_frame.height/2, 
            .height = h/1.4, 
            .width  = w/1.4,
        };
#endif


        BeginDrawing();
        ClearBackground(BLACK);
        nn_render_your_mom(nn, nn_frame);
        EndDrawing();
    }

    CloseWindow();
}




