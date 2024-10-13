#include "library.h"

#include <stdio.h>


tgxVertexInstanceID helloTriangle;

int update_proc(double time, tgxContext* context) {

    glClearColor(0.2, 0.3, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    tgx_vmode_present_instance( &((tgxVectorContext*)context->graphics_context)->instances, helloTriangle, TGX_TRIANGLE_LIST, NULL, 0);

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
    "void main(){\n"
    "  fragColor = fColor;\n"
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
        {.position = { -0.5, -0.5, 0.0 }, .normal = { 0 }, .uv = { 0 }, .color = { 1.0, 0.0, 0.0, 1.0 } },
        {.position = {  0.5, -0.5, 0.0 }, .normal = { 0 }, .uv = { 0 }, .color = { 0.0, 1.0, 0.0, 1.0 } },
        {.position = {  0.0,  0.5, 0.0 }, .normal = { 0 }, .uv = { 0 }, .color = { 0.0, 0.0, 1.0, 1.0 } },
    };

    tgxProgram passthrough_shader;
    if(tgx_vmode_compile_shader(VERTEX_PASSTHROUGH, FRAGMENT_PASSTHROUGH, &passthrough_shader) != TGX_OK) {
        tgx_panic();
    }

    helloTriangle = tgx_vmode_create_instance(context.graphics_context, *tgx_vmode_get_default_format(), buffer, sizeof(buffer), TGX_STATIC_MEMORY_HINT);
    tgx_vmode_set_instance_shader(&((tgxVectorContext*)context.graphics_context)->instances, helloTriangle, passthrough_shader);

    if(helloTriangle == TGX_VERTEX_INSTANCE_NULL) {
        tgx_shutdown(&context);
        tgx_panic();
    }

    tgx_run(&context, update_proc);

    tgx_vmode_destroy_shader(passthrough_shader);
    tgx_shutdown(&context);
    return 0;
}
