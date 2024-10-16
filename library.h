#ifndef TGX_LIBRARY_H
#define TGX_LIBRARY_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef int32_t tgxEnum;
typedef uint32_t tgxFlag;
typedef int32_t tgxResult;

#define TGX_VERSION_MAJOR 0
#define TGX_VERSION_MINOR 1
#define TGX_VERSION_PATCH 0

#define KILOBYTES(bytes) (bytes * 1024)
#define MEGABYTES(bytes) (KILOBYTES(bytes) * 1024)
#define GIGABYTES(bytes) (MEGABYTES(bytes) * 1024)

#ifndef NDEBUG
#define TGX_DEBUG
#endif



#ifndef TGX_USER_MEMORY_SIZE
#define TGX_USER_MEMORY_SIZE (MEGABYTES(4))
#endif
#ifndef TGX_SYSTEM_MEMORY_SIZE
#define TGX_SYSTEM_MEMORY_SIZE (MEGABYTES(4))
#endif
#ifndef TGX_MEMORY_CARD_COUNT
#define TGX_MEMORY_CARD_COUNT 4
#endif
#ifndef TGX_MEMORY_CARD_PAGE_SIZE
#define TGX_MEMORY_CARD_PAGE_SIZE (MEGABYTES(2))
#endif

#define TGX_TOTAL_CONSOLE_RAM (TGX_USER_MEMORY_SIZE + TGX_SYSTEM_MEMORY_SIZE + TGX_MEMORY_CARD_PAGE_SIZE)

#ifndef TGX_ERROR_STACK_LIMIT
#define TGX_ERROR_STACK_LIMIT 32
#endif

#define TGXP_SYSTEM_VERSION_MAJOR 0x08
#define TGXP_SYSTEM_VERSION_MINOR 0x09
#define TGXP_SYSTEM_VERSION_PATCH 0x0A
#define TGXP_SYSTEM_NAME 0x0C
#define TGXP_SYSTEM_BOOT_TIME 0x18
#define TGXP_SYSTEM_UP_TIME 0x20
#define TGX_SYSTEM_RANDOM_BUFFER_COUNT 2
#define TGXP_SYSTEM_RANDOM_BUFFER 0x28
#define TGXP_SYSTEM_MODE 0x38
#define TGXP_SYSTEM_RESOLUTION_BUFFER 0x3C
#define TGXP_SYSTEM_FLAGS 0x44
#define TGXP_SYSTEM_INPUT_BUFFER 0x1000
#define TGXP_SYSTEM_LAST_INPUT_BUFFER 0x1004

#define TGXP_SYSTEM_UNIFORM_PROJ_MATRIX  0x00001100
#define TGXP_SYSTEM_UNIFORM_VIEW_MATRIX  0x00001140
#define TGXP_SYSTEM_UNIFORM_MODEL_MATRIX 0x00001180
#define TGXP_SYSTEM_UNIFORM_COLOR_TINT   0x000011C0
#define TGXP_SYSTEM_UNIFORM_UV_WAVE_COEF 0x000011D0

typedef struct {
    const char *message;
    uint32_t errcode;
    uint32_t line;
    const char* file;
} tgxErrorInfo;

int tgx_error_count(void);
int tgx_has_error(void); // alias of tgx_error_count() > 0
int tgx_is_error(int errcode);
int tgx_catch_error(int errcode, tgxErrorInfo* info);
void tgx_pop_error(tgxErrorInfo* info);
void tgx_push_error(tgxErrorInfo* info);
void _tgx_push_error_(tgxErrorInfo info);
void tgx_clear_errors(void);
void tgx_dump_errors(void);
void tgx_panic(void);
void tgx_print_error(FILE* out, const tgxErrorInfo* info);

// enums
#define TGX_OK 0
#define TGX_ERR 1

#define TGXVAL(type, addr) (*(type*)(addr))
#define TGXREF(type, addr) (type*)(addr)

#define _TGX_LINE() __LINE__
#define _TGX_FILE() __FILE__

#define TGX_THROW(what, code) _tgx_push_error_( (tgxErrorInfo){ .message = what, .errcode = code, .line = _TGX_LINE(), .file = _TGX_FILE() } )

#define TGX_ERROR_NO_MEMORY 1
#define TGX_BAD_CONFIG 2
#define TGX_ERROR_NOT_FOUND 3
#define TGX_ERROR_INSTANCE_NOT_FOUND 4

#define TGX_STATIC_MEMORY_HINT 0
#define TGX_DYNAMIC_MEMORY_HINT 1

#define TGX_TRIANGLE_LIST 0
#define TGX_TRIANGLE_STRIP 1
#define TGX_TRIANGLE_FAN 2
#define TGX_LINE_LIST 3
#define TGX_LINE_STRIP 4
#define TGX_POINT_LIST 5

#define TGX_CONFIG_TINY 0
#define TGX_CONFIG_SMALL 1
#define TGX_CONFIG_MEDIUM 2
#define TGX_CONFIG_LARGE 3

#ifndef TGX_ALLOCATION_LIMIT
#define TGX_ALLOCATION_LIMIT 1024
#endif

#ifndef _TGX_CUSTOM_RESOLUTION_SPECS_
// gameboy
#define _TGX_RES_TINY_W 160
#define _TGX_RES_TINY_H 144
// snes
#define _TGX_RES_SMALL_W 256
#define _TGX_RES_SMALL_H 224
// ps1
#define _TGX_RES_MEDIUM_W 320
#define _TGX_RES_MEDIUM_H 240
// gamecube
#define _TGX_RES_LARGE_W 640
#define _TGX_RES_LARGE_H 480
#endif

#define TGX_MODE_DEFAULT 0
#define TGX_MODE_PIXEL 0
#define TGX_MODE_VECTOR 1

/// INPUT pad
#define TGX_PAD_UP     0b000000000000000000000001
#define TGX_PAD_RIGHT  0b000000000000000000000010
#define TGX_PAD_DOWN   0b000000000000000000000100
#define TGX_PAD_LEFT   0b000000000000000000001000

/// INPUT Buttons
//    X  Y
//  A  B
#define TGX_BTN_0      0b000000000000000000010000
#define TGX_BTN_1      0b000000000000000000100000
#define TGX_BTN_2      0b000000000000000001000000
#define TGX_BTN_3      0b000000000000000010000000
// RTrigger     LTrigger
//     Start  Pause
#define TGX_BTN_4      0b000000000000000100000000
#define TGX_BTN_5      0b000000000000001000000000
#define TGX_BTN_6      0b000000000000010000000000
#define TGX_BTN_7      0b000000000000100000000000
#define TGX_BTN_A TGX_BTN_0
#define TGX_BTN_B TGX_BTN_1
#define TGX_BTN_X TGX_BTN_2
#define TGX_BTN_Y TGX_BTN_3
#define TGX_BTN_R TGX_BTN_4
#define TGX_BTN_L TGX_BTN_5
#define TGX_BTN_START TGX_BTN_6
#define TGX_BTN_PAUSE TGX_BTN_7
#define TGX_BTN_COUNT 12

#define TGX_FLAG_NONE         0b00000000000000000000000000000000
#define TGX_FLAG_POWER        0b00000000000000000000000000000001
#define TGX_FLAG_UPLOAD_DEFAULT_UNIS  0b00000000000000000000000000000010


int tgx_get_flag(tgxEnum flag);
void tgx_set_flag(tgxEnum flag);
void tgx_clear_flag(tgxEnum flag);

#define TGX_BUFFER_FLAG_CLEAR 0x00000001

#define TGX_MONITOR_NONE -1
#define TGX_MONITOR_0 0
#define TGX_MONITOR_1 1
#define TGX_MONITOR_2 2
#define TGX_MONITOR_3 3
#define TGX_MONITOR_4 4
#define TGX_MONITOR_5 5
#define TGX_MONITOR_6 6

const uint8_t*  tgx_get_system_version_major(void);
const uint8_t*  tgx_get_system_version_minor(void);
const uint16_t* tgx_get_system_version_patch(void);
const char*     tgx_get_system_name(void);
const uint64_t* tgx_get_system_boot_time(void);
const uint64_t* tgx_get_system_up_time(void);
const uint64_t* tgx_get_random_buffer(uint32_t* count);

const uint32_t* tgx_get_console_mode(void);
const uint32_t* tgx_get_console_resolution_buffer(void);
      uint32_t* tgx_get_console_system_flags(void);
const uint32_t* tgx_get_input_framebuffer_ptr(void);
const uint32_t* tgx_get_input_last_framebuffer_ptr(void);
      void*     tgx_get_user_ram_begin(void);
      void*     tgx_get_user_ram_end(void);
      void*     tgx_get_memory_card_begin(void);
      void*     tgx_get_memory_card_end(void);
      void*     tgx_get_system_ram_begin(void);
      void*     tgx_get_system_ram_end(void);

typedef struct {
    tgxEnum type;
    uint32_t offset;
} tgxVertexAttribute;

typedef struct {
    tgxVertexAttribute attributes[16];
    uint32_t attribute_count;
    uint32_t stride;
} tgxVertexFormat;

#ifndef TGX_CUSTOM_BACKEND

#include "glad/glad.h"
#include "GLFW/glfw3.h"

typedef GLFWwindow tgxWindow;

#define TGX_ERROR_GLAD_LOADER 0x1001
#define TGX_ERROR_GLFW_CREATE_WINDOW 0x1002
#define TGX_ERROR_INVALID_VERTEX_ATTRIBUTE 0x1003
#define TGX_ERROR_INVALID_UNIFORM_TYPE 0x1004
#define TGX_ERROR_COMPILING_SHADER 0x1005
#define TGX_ERROR_LINKING_SHADER 0x1006

#define _TGX_MAP_PAD_UP GLFW_KEY_W
#define _TGX_MAP_PAD_RIGHT GLFW_KEY_D
#define _TGX_MAP_PAD_DOWN GLFW_KEY_S
#define _TGX_MAP_PAD_LEFT GLFW_KEY_A
#define _TGX_MAP_BTN_0 GLFW_KEY_J
#define _TGX_MAP_BTN_1 GLFW_KEY_K
#define _TGX_MAP_BTN_2 GLFW_KEY_I
#define _TGX_MAP_BTN_3 GLFW_KEY_O
#define _TGX_MAP_BTN_4 GLFW_KEY_E
#define _TGX_MAP_BTN_5 GLFW_KEY_Q
#define _TGX_MAP_BTN_6 GLFW_KEY_ESCAPE
#define _TGX_MAP_BTN_7 GLFW_KEY_F

#define TGX_OPENGL_CORE_PROFILE GLFW_OPENGL_CORE_PROFILE
#define TGX_OPENGL_COMPAT_PROFILE GLFW_OPENGL_COMPAT_PROFILE

#define TGX_VTYPE_FLOAT1 0
#define TGX_VTYPE_FLOAT2 1
#define TGX_VTYPE_FLOAT3 2
#define TGX_VTYPE_FLOAT4 3
#define TGX_VTYPE_INT1 4
#define TGX_VTYPE_INT2 5
#define TGX_VTYPE_INT3 6
#define TGX_VTYPE_INT4 7
#define TGX_VTYPE_SAMPLER2D 8
#define TGX_VTYPE_MAT4 9
#define TGX_VTYPE_REFMAT4 10

typedef struct {
    uint32_t vao;
    uint32_t vbo;
    uint32_t program_id;
    tgxVertexFormat format;
    uint32_t vertex_count;
} tgxVertexPacket;

typedef uint32_t tgxProgram;
typedef uint32_t tgxTexture;

#define TGX_RGB 0
#define TGX_RGBA 1

#define TGX_TEXTURE_REPEAT 0
#define TGX_TEXTURE_MIRROR 1
#define TGX_TEXTURE_CLAMP 2

#define TGX_FILTER_NEAREST 0
#define TGX_FILTER_LINEAR 1

#define TGX_TEXTURE_NULL 0

typedef struct {
    uint32_t width;
    uint32_t height;
    void *data;
    tgxEnum format;
    tgxEnum desired_format;
    tgxEnum border_policy;
    tgxEnum min_filter;
    tgxEnum mag_filter;
    int generate_mipmaps;
    tgxTexture existing_handle;
} tgxTextureSubmitInfo;

void tgx_opengl_assert(const char* call, int line, const char* file);

#ifdef TGX_DEBUG
#define TGX_GLCALL(a) do { (a); tgx_opengl_assert(#a, __LINE__, __FILE__); } while(false)
#else
#define TGX_GLCALL(a) (a)
#endif

#endif

typedef struct {
    tgxEnum resolution;
    tgxFlag flags;
    tgxEnum graphics_mode;
    const char* title;
    void* user_data;
} tgxConfigInfo;

typedef uint32_t tgxVertexInstanceID;
#define TGX_VERTEX_INSTANCE_NULL ((uint32_t)-1)

#ifndef TGX_INITIAL_CAPACITY
#define TGX_INITIAL_CAPACITY 32
#endif

#ifndef TGX_GROWTH_FACTOR
#define TGX_GROWTH_FACTOR 2
#endif

typedef struct {
    const char* name;
    tgxEnum type;
    union {
        float f1;
        float f2[2];
        float f3[3];
        float f4[4];
        float mat4[16];
        int i1;
        int i2[2];
        int i3[3];
        int i4[4];
        void* ptr;
    };
} tgxUniformData;

typedef struct {
    uint32_t instance_count;
    uint32_t instance_capacity;
    tgxVertexPacket *instances;
} tgxVertexInstances;

typedef struct {
    tgxVertexInstances instances;
} tgxVectorContext;

typedef struct {
    tgxVectorContext vector_context;
    tgxVertexInstanceID framebuffer_instance;
    tgxProgram shader;
    tgxTexture framebuffer_handle;
    int width;
    int height;
} tgxPixelContext;

typedef struct {
    tgxWindow* display_handle;
    tgxEnum resolution;
    void* graphics_context;
    tgxEnum mode;
} tgxContext;

typedef int(*TGXUpdateFunc)(double, tgxContext* context);

typedef struct {
    float position[3];
    float normal[3];
    float uv[2];
    float color[4];
} tgxVertex;


#ifndef _TGX_CUSTOM_PROFILE
#define TGX_PROFILE_MAJOR 3
#define TGX_PROFILE_MINOR 3
#define TGX_PROFILE TGX_OPENGL_CORE_PROFILE
#endif


// MAIN API Methods
tgxResult tgx_initialize(const tgxConfigInfo *info, tgxContext* context);
tgxResult tgx_shutdown(tgxContext* context);

double tgx_get_time(void);

int tgx_get_width(tgxEnum resolution);
int tgx_get_height(tgxEnum resolution);
void tgx_get_size(tgxEnum resolution, int* width, int* height);

void tgx_context_hint(int major, int minor, int profile);
tgxWindow* tgx_create_window(int width, int height, const char* title);
void tgx_destroy_window(tgxWindow* window);
void tgx_set_window_monitor(tgxContext* window, tgxEnum monitor);
int tgx_get_window_width(tgxContext* context);
int tgx_get_window_height(tgxContext* context);
void tgx_poll_events(tgxContext* context);
void tgx_swap_buffers(tgxWindow* context);

int tgx_get_window_is_open(tgxWindow* window);
void tgx_set_window_should_close(tgxWindow* window, int close);

void tgx_context_initialize(void);
void tgx_context_terminate(void);
tgxResult tgx_context_prepare(tgxContext* context);

int tgx_key_check(tgxContext* context, tgxEnum key);
int tgx_key_check_pressed(tgxContext* context, tgxEnum key);
int tgx_key_check_released(tgxContext* context, tgxEnum key);

void *tgx_alloc(uint32_t bytes);
void tgx_free(void* ptr);
void *tgx_realloc(void* ptr, uint32_t bytes);

tgxResult tgx_vmode_init_vector_instances(tgxVertexInstances* instances);
void tgx_vmode_deinit_vector_instances(tgxVertexInstances* instances);

int tgx_vmode_instance_valid(tgxVertexInstances* instances, tgxVertexInstanceID instance);

tgxVertexInstanceID tgx_vmode_create_instance(tgxVertexInstances *instances, tgxVertexFormat format, void* buffer, uint32_t buffer_len, tgxEnum usage_hint);
tgxResult tgx_vmode_update_instance(tgxVertexInstances *instances, tgxVertexInstanceID instance, void* buffer, uint32_t buffer_len, tgxEnum usage_hint);
void tgx_vmode_destroy_instance(tgxVertexInstances *instances, tgxVertexInstanceID instance);
tgxResult tgx_vmode_present_instance(tgxVertexInstances *instances, tgxVertexInstanceID instance, tgxEnum primitive_type, tgxUniformData* uniforms, uint32_t uniform_count);
tgxResult tgx_vmode_set_instance_shader(tgxVertexInstances *instances, tgxVertexInstanceID instance, tgxProgram program_id);

tgxVertexFormat* tgx_vmode_get_default_format(void);

tgxResult tgx_vmode_compile_shader(const char* vdata, const char* fdata, tgxProgram* program);
void tgx_vmode_destroy_shader(tgxProgram program);

void tgx_begin_frame(tgxContext* context);
void tgx_end_frame(tgxContext* context);
uint8_t* tgx_get_framebuffer(tgxContext* context, uint32_t* length);

void tgx_clear_framebuffer(tgxContext* context, uint8_t red, uint8_t green, uint8_t blue);

tgxTexture tgx_submit_texture(const tgxTextureSubmitInfo* info);
void tgx_assign_texture_slot(int slot, tgxTexture texture);
void tgx_delete_texture(tgxTexture texture);

// OPTIONAL
// this can be coded in manually with the other
// parts of the API if more control is needed
void tgx_run(tgxContext* context, TGXUpdateFunc func);



void tgx_mat4_build_identity(float mat[16]);
void tgx_mat4_build_value(float mat[16], float value);
void tgx_mat4_build(float mat[16],
        float translation_x, float translation_y, float translation_z,
        float rotation_x, float rotation_y, float rotation_z,
        float scale_x, float scale_y, float scale_z);
void tgx_mat4_build_lookat(float mat4[16],
        float xfrom, float yfrom, float zfrom,
        float xto, float yto, float zto,
        float xup, float yup, float zup);
void tgx_mat4_build_perspective(float mat[16], float fov, float aspect, float near, float far);
void tgx_mat4_build_ortho(float mat[16], float left, float top, float right, float bottom, float near, float far);

void tgx_mat4_mul(float mat0[16], float mat1[16], float result[16]);

float tgx_vec3_length(float v[3]);
void tgx_vec3_normalize(float v[3]);
void tgx_vec3_cross(float result[3], float a[3], float b[3]);
/*
#ifndef TGX_CUSTOM_BACKEND
// PIXEL SHADER
const char * const TGX_PIXEL_VERTEX_SHADER =
    "#version 330 core\n"
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

const char * const TGX_PIXEL_FRAGMENT_SHADER =
    "#version 330 core\n"
    "in vec2 fUv;\n"
    "in vec4 fColor;\n"
    "out vec4 fragColor;\n"
    "uniform sampler2D framebuffer;\n"
    "void main(){\n"
    "  fragColor = texture(framebuffer, fUv) * fColor;\n"
    "}\n";

#endif
*/
#endif //TGX_LIBRARY_H
