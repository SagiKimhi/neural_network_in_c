#define NN_BACKPROP_TRADITIONAL
#define NN_ACT_FUNC ACT_SIGMOID
#define NN_RELU_PARAM 0.01f
#define NN_RENDER_IMPLEMENTATION
#include "nn_render.h"

void load_training_data(nn_matrix_t *ts_inout);
void render_results(Rectangle frame, nn_t nn);
float render_scrollbar(Rectangle frame);
void print_results(nn_t nn, nn_matrix_t ts_in, nn_matrix_t ts_out);

const size_t    out_img_width           = 512;
const size_t    out_img_height          = 512;
const size_t    input_size              = 3;
const size_t    output_size             = 1;
uint8_t         textures_were_loaded    = 0;
Vector2         img_selector_pos        = {0, 0};
size_t          nof_input_imgs          = 0;
Image           *input_imgs             = NULL;
Image           actual_img;
Texture2D       *input_textures;
Texture2D       actual_texture;

int main(int argc, char **argv)
{
    srand(time(0));

    size_t          arch[]      = {input_size, 28, 28, 16, 13, 9, output_size};
    size_t          arch_len    = NN_SIZEOF_ARR(arch);
    nn_render_cfg_t render_cfg  = {
        .nn_arch                    = {.arch = arch, .arch_len = arch_len},
        .nn_rate                    = 1e-1,
        .rand_low                   = -1,
        .rand_high                  = 1,
        .batch_size                 = 75,
        .batches_per_frame          = 0x80,
        .shuffle_rows               = true,
        .load_data_fn               = &load_training_data,
        .optional_render_results_fn = &render_results,
        .optional_print_results_fn  = &print_results,
    };

    nn_render_gui(render_cfg);
    free(input_imgs);
    free(input_textures);

    return 0;
}

void load_training_data(nn_matrix_t *ts_inout)
{
    Image           img;
    char            path[FILENAME_MAX + 1];
    uint8_t         *pixels;
    FilePathList    files;

    fprintf(stdout, "Please enter the path to your desired training images directory: ");
    nn_io_get_line(path, FILENAME_MAX, stdin);

    files           = LoadDirectoryFilesEx(path, ".png", false);
    nof_input_imgs  = files.count;
    input_imgs      = NN_MALLOC(sizeof(Image) * files.count);
    NN_ASSERT(files.count);
    NN_ASSERT(input_imgs);

    img         = LoadImage(files.paths[0]);
    actual_img  = GenImageColor(img.width, img.height, BLACK);

    for (size_t y = 0; y < actual_img.height; y++) {
        for (size_t x = 0; x < actual_img.width; x++) {
            ImageDrawPixel(&actual_img, x, y, BLACK);
        }
    }

    if (ts_inout->data) {
        nn_matrix_memset(*ts_inout, 0);
    }
    else {
        size_t num_rows = img.width * img.height * nof_input_imgs;
        *ts_inout = nn_matrix_alloc(num_rows, input_size + output_size);
    }

    for (size_t i = 0; i < nof_input_imgs; i++) {
        if (i) {
            img = LoadImage(files.paths[i]);
            NN_ASSERT(input_imgs[i-1].width == img.width);
            NN_ASSERT(input_imgs[i-1].height == img.height);
        }

        ImageColorGrayscale(&img);

        pixels          = img.data;
        input_imgs[i]   = GenImageColor(img.width, img.height, BLACK);

        for (size_t y = 0; y < img.height; y++) {
            for (size_t x = 0; x < img.width; x++) {
                size_t  pixel_idx   = (y * img.width + x);
                size_t  row_offset  = (i * img.width * img.height);
                float   pixel       = pixels[pixel_idx];

                NN_MATRIX_AT(*ts_inout, (row_offset + pixel_idx), 0) = (
                    (float) x / (img.width - 1)
                );
                NN_MATRIX_AT(*ts_inout, (row_offset + pixel_idx), 1) = (
                    (float) y / (img.width - 1)
                );
                NN_MATRIX_AT(*ts_inout, (row_offset + pixel_idx), 2) = i;
                NN_MATRIX_AT(*ts_inout, (row_offset + pixel_idx), input_size) = (
                    pixel / 255.f
                );
                ImageDrawPixel(&input_imgs[i], x, y, (Color) {pixel, pixel, pixel, 255});
            }
        }

        UnloadImage(img);
    }

    UnloadDirectoryFiles(files);
}

void render_results(Rectangle frame, nn_t nn)
{
    float       img_selector_val;
    float       frame_hpad, frame_vpad;
    float       input_textures_vspace, input_textures_hspace;
    float       input_texture_scale, actual_texture_scale;
    Vector2     input_texture_pos, actual_texture_pos;
    uint32_t    *actual_pixels;
    uint8_t     actual_pixel;

    actual_pixels       = actual_img.data;
    img_selector_val    = render_scrollbar(frame);

    for (size_t y = 0; y < input_imgs[0].height; y++) {
        for (size_t x = 0; x < input_imgs[0].width; x++) {
            size_t i = y * input_imgs[0].width + x;
            NN_MATRIX_AT(NN_INPUT(nn), 0, 0) = (float) x / (input_imgs[0].width - 1);
            NN_MATRIX_AT(NN_INPUT(nn), 0, 1) = (float) y / (input_imgs[0].height - 1);
            NN_MATRIX_AT(NN_INPUT(nn), 0, 2) = img_selector_val;

            nn_forward(nn);
            actual_pixel = NN_MATRIX_AT(NN_OUTPUT(nn), 0, 0) * 255;

            SetPixelColor(
                &actual_pixels[i], 
                (Color) {actual_pixel, actual_pixel, actual_pixel, 255},
                actual_img.format
            );
        }
    }

    if (!textures_were_loaded) {
        NN_ASSERT((input_textures = NN_MALLOC(sizeof(Texture2D) * nof_input_imgs)));

        for (size_t i = 0; i < nof_input_imgs; i++)
            input_textures[i] = LoadTextureFromImage(input_imgs[i]);

        actual_texture          = LoadTextureFromImage(actual_img);
        textures_were_loaded    = 1;
    }
    
    frame_hpad              = get_frame_hpad(frame);
    frame_vpad              = get_frame_vpad(frame);
    input_textures_vspace   = get_relative_object_vspace(frame, 2);
    input_textures_hspace   = get_relative_object_hspace(frame, nof_input_imgs);
    input_texture_scale     = (
        input_textures_hspace > input_textures_vspace       ? 
        (input_textures_vspace / input_textures[0].height)  :
        (input_textures_hspace / input_textures[0].width )
    );

    for (size_t i = 0; i < nof_input_imgs; i++) {
        input_texture_pos.x = frame.x + frame_hpad + i * input_textures_hspace;
        input_texture_pos.y = frame.y + frame_vpad;
        UpdateTexture(input_textures[i], input_imgs[i].data);
        DrawTextureEx(input_textures[i], input_texture_pos, 0, input_texture_scale, WHITE);
    }

    actual_texture_scale    = input_textures_vspace / actual_texture.height;
    actual_texture_pos.x    = frame.x + frame.width / 2;
    actual_texture_pos.x   -= actual_texture.width * actual_texture_scale / 2;
    actual_texture_pos.y    = frame.y + frame_vpad + input_textures_vspace;
    UpdateTexture(actual_texture, actual_img.data);
    DrawTextureEx(actual_texture, actual_texture_pos, 0, actual_texture_scale, WHITE);
    render_scrollbar(frame);
}

float render_scrollbar(Rectangle frame)
{
    Vector2     mouse_pos;
    Rectangle   bar, hitbox;
    float       selector_radius, bar_right_edge;
    
    bar.x               = frame.x + get_frame_hpad(frame) * 4;
    bar.y               = frame.y + get_frame_vpad(frame) * 3;
    bar.height          = frame.height * 0.01f;
    bar.width           = get_frame_hspace(frame) * 0.98;
    bar_right_edge      = bar.x + bar.width;

    hitbox.x            = bar.x - bar.width * 0.025;
    hitbox.y            = bar.y - bar.height * 5;
    hitbox.width        = bar.width * 1.05;
    hitbox.height       = bar.height * 10;

    selector_radius     = bar.height * 1.5f;
    img_selector_pos.y  = bar.y + bar.height / 2;

    mouse_pos           = GetMousePosition();

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse_pos, hitbox)) {
        img_selector_pos.x = mouse_pos.x;
    }

    img_selector_pos.x = (
        img_selector_pos.x < bar.x          ? bar.x :
        img_selector_pos.x > bar_right_edge ? bar_right_edge :
        img_selector_pos.x
    );

    DrawRectangleRec(bar, WHITE);
    DrawCircleV(img_selector_pos, selector_radius, RED);

    return (
        (nof_input_imgs - 1) * (
            1 - (img_selector_pos.x - bar_right_edge) / (bar.x - bar_right_edge) 
        )
    );
}

void print_results(nn_t nn, nn_matrix_t ts_in, nn_matrix_t ts_out)
{
    Image       img;
    char        filename[FILENAME_MAX + 1] = {0};
    uint32_t    *pixels_u32;

    img = GenImageColor(out_img_width, out_img_height, BLACK);

    pixels_u32 = img.data;

    for (size_t i = 0; i < out_img_height * out_img_width; i++)
        pixels_u32[i] = 0xFF000000;

    fprintf(
        stdout, 
        "Please provide a path to save the image to: "
    );
    nn_io_get_line(filename, FILENAME_MAX, stdin);

    size_t len = strlen(filename);
    for (size_t img_idx = 0; img_idx < nof_input_imgs; img_idx++) {
        for (size_t i = 0; i <= 200; i++) {
            snprintf(
                &filename[len], FILENAME_MAX-len, "_%05zu.png", 
                img_idx * 200 + i );

            for (size_t y = 0; y < out_img_height; ++y) {
                for (size_t x = 0; x < out_img_width; ++x) {
                    NN_MATRIX_AT(NN_INPUT(nn), 0, 0) = (float)x/(out_img_width-1);
                    NN_MATRIX_AT(NN_INPUT(nn), 0, 1) = (float)y/(out_img_height-1);
                    NN_MATRIX_AT(NN_INPUT(nn), 0, 2) = img_idx + i * 0.005f;
                    nn_forward(nn);
                    float a = NN_MATRIX_AT(NN_OUTPUT(nn), 0, 0);
                    if (a < 0) a = 0;
                    if (a > 1) a = 1;
                    uint32_t pixel = a * 255.f;
                    pixels_u32[y*out_img_width + x] = (0xFF<<(8*3))|(pixel<<(8*2))|(pixel<<(8*1))|(pixel<<(8*0));
                }
            }

            if (!ExportImage(img, filename)) {
                fprintf(
                        stderr, 
                        "\n\nFailed to save image, please make sure the provided "\
                        "path is valid:\n%s",
                        filename
                       );

                return;
            }
        }
    }
}

