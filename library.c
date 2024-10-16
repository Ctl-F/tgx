#include "library.h"

#include <stdio.h>

int update_proc(double time, tgxContext* context) {
    tgx_clear_framebuffer(context, 255, 0, 0);

    uint32_t length;
    uint8_t* framebuffer = tgx_get_framebuffer(context, &length);

    framebuffer[0] = 0;
    framebuffer[1] = 0;
    framebuffer[2] = 0;
    framebuffer[3] = 0;
    framebuffer[4] = 0;
    framebuffer[5] = 0;

    return true;
}

int main(int argc, char** argv) {
    tgxConfigInfo info = {0};
    tgxContext context = {0};

    info.flags = TGX_FLAG_NONE;
    info.resolution = TGX_CONFIG_SMALL;
    info.graphics_mode = TGX_MODE_PIXEL;
    info.title = "Pixel Mode Test";

    if(tgx_initialize(&info, &context) != TGX_OK) {
        tgx_panic();
        return 1;
    }

    tgx_run(&context, update_proc);

    tgx_shutdown(&context);
    return 0;
}

/*

tgxVertexInstanceID helloTriangle;
tgxTexture helloTexture;

//const uint8_t texData[] = { 0xFF, 0xFF, 0xFF,   0xFF, 0xFF, 0xFF,  0xFF, 0xFF, 0xFF,
//                            0xFF, 0xFF, 0xFF,   0x10, 0x10, 0x10,  0xFF, 0xFF, 0xFF,
//                            0xFF, 0xFF, 0xFF,   0xFF, 0xFF, 0xFF,  0xFF, 0xFF, 0xFF };
//const uint32_t texWidth = 3;
//const uint32_t texHeight = 3;

uint8_t* texData;
uint32_t texWidth;
uint32_t texHeight;

void init_tex() {
    texWidth = 32;
    texHeight = 32;

    int length = texWidth * texHeight * 3;
    texData = (uint8_t*)malloc(length);

    for(int i=0; i<length; i++) {
        texData[i] = 0xFF;
    }
    texData[10] = 0x00;
    texData[48] = 0xAA;
    texData[64] = 0xCC;
}
void destroy_tex() {
    free(texData);
}

int update_proc(double time, tgxContext* context) {

    glClearColor(0.2, 0.3, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    tgxUniformData uniforms[1] = {0};
    uniforms[0].name = "_texture";
    uniforms[0].type = TGX_VTYPE_INT1;
    uniforms[0].i1 = 0;

    tgx_assign_texture_slot(0, helloTexture);
    tgx_vmode_present_instance( &((tgxVectorContext*)context->graphics_context)->instances, helloTriangle, TGX_TRIANGLE_LIST, uniforms, 1);

    return true;
}

static const char * VERTEX_PASSTHROUGH = "#version 330 core\n"
    "layout (location = 0) in vec3 tgxPosition;\n"
    "layout (location = 1) in vec3 tgxNormal;\n"
    "layout (location = 2) in vec2 tgxUv;\n"
    "layout (location = 3) in vec4 tgxColor;\n"
    "out vec2 fUv;\n"
    "out vec4 fColor;\n"
    "void main(){\n"
    "  gl_Position = vec4(tgxPosition, 1.0);\n"
    "  fUv = tgxUv;\n"
    "  fColor = tgxColor;\n"
    "}\n";

static const char * FRAGMENT_PASSTHROUGH =
    "#version 330 core\n"
    "in vec2 fUv;\n"
    "in vec4 fColor;\n"
    "out vec4 fragColor;\n"

    "uniform sampler2D _texture;\n"

    "void main(){\n"
    "  fragColor = texture(_texture, fUv) * fColor;\n"
    "}\n";

// TODO: Why is triangle red?
int main(int argc, char** argv) {
    tgxConfigInfo config;
    tgxContext context;

    config.flags = TGX_FLAG_NONE;
    config.graphics_mode = TGX_MODE_VECTOR;
    config.resolution = TGX_CONFIG_SMALL;
    config.title = "TGX Hello";
    config.user_data = NULL;

    if(tgx_initialize(&config, &context) != TGX_OK) {
        fprintf(stderr, "Error initializing tgx system.\n");
        return 1;
    }

    tgxVertex buffer[3] = {
        {.position = { -0.5, -0.5, 0.0 }, .normal = { 0 }, .uv = { 0, 0 }, .color = { 1.0, 0.0, 0.0, 1.0 } },
        {.position = {  0.5, -0.5, 0.0 }, .normal = { 0 }, .uv = { 1, 0 }, .color = { 0.0, 1.0, 0.0, 1.0 } },
        {.position = { -0.5,  0.5, 0.0 }, .normal = { 0 }, .uv = { 0, 1 }, .color = { 0.0, 0.0, 1.0, 1.0 } },
    };

    for(int i=0; i<3; i++) {
        buffer[i].color[0] = 1.0;
        buffer[i].color[1] = 1.0;
        buffer[i].color[2] = 1.0;
    }

    tgxProgram passthrough_shader;
    if(tgx_vmode_compile_shader(VERTEX_PASSTHROUGH, FRAGMENT_PASSTHROUGH, &passthrough_shader) != TGX_OK) {
        tgx_panic();
    }

    init_tex();

    tgxTextureSubmitInfo info = { 0 };
    info.data = texData;
    info.width = texWidth;
    info.height = texHeight;
    info.format = TGX_RGB;
    info.mag_filter = TGX_FILTER_NEAREST;
    info.min_filter = TGX_FILTER_NEAREST;
    info.border_policy = TGX_TEXTURE_REPEAT;
    info.generate_mipmaps = true;
    info.desired_format = TGX_RGB;
    info.existing_handle = TGX_TEXTURE_NULL;



    helloTexture = tgx_submit_texture(&info);

    helloTriangle = tgx_vmode_create_instance(context.graphics_context, *tgx_vmode_get_default_format(), buffer, sizeof(buffer), TGX_STATIC_MEMORY_HINT);
    tgx_vmode_set_instance_shader(&((tgxVectorContext*)context.graphics_context)->instances, helloTriangle, passthrough_shader);

    if(helloTriangle == TGX_VERTEX_INSTANCE_NULL) {
        tgx_shutdown(&context);
        tgx_panic();
    }

    tgx_run(&context, update_proc);

    destroy_tex();

    tgx_delete_texture(helloTexture);
    tgx_vmode_destroy_instance(&((tgxVectorContext*)context.graphics_context)->instances, helloTriangle);
    tgx_vmode_destroy_shader(passthrough_shader);
    tgx_shutdown(&context);
    return 0;
}
*/