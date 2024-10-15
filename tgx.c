#include "library.h"
#include "string.h"
#include <math.h>
#include <time.h>

static char sConsoleRam[TGX_TOTAL_CONSOLE_RAM] = { 0 };

//static char* sConsoleStart = sConsoleRam;
//static char* sConsoleEnd = sConsoleRam + TGX_TOTAL_CONSOLE_RAM;
//static char* sSystemSpaceStart = sConsoleRam;
//static char* sSystemSpaceEnd = sConsoleRam + TGX_SYSTEM_MEMORY_SIZE;
//static char* sUserSpaceStart = sSystemSpaceEnd;
//static char* sUserSpaceEnd = sUserSpaceStart + TGX_USER_MEMORY_SIZE;
//static char* sMemoryCardStart = sUserSpaceEnd;
//static char* sMemoryCardEnd = sMemoryCardStart + TGX_MEMORY_CARD_PAGE_SIZE;

static char* sConsoleStart;
static char* sConsoleEnd;
static char* sSystemSpaceStart;
static char* sSystemSpaceEnd;
static char* sUserSpaceStart;
static char* sUserSpaceEnd;
static char* sMemoryCardStart;
static char* sMemoryCardEnd;

const uint8_t* tgx_get_system_version_major(void) {
    //(void*)sConsoleStart; (void*)sConsoleEnd; // REMOVE ME
    return (uint8_t*)(sSystemSpaceStart + TGXP_SYSTEM_VERSION_MAJOR);
}
const uint8_t* tgx_get_system_version_minor(void) {
    return (uint8_t*)(sSystemSpaceStart + TGXP_SYSTEM_VERSION_MINOR);
}
const uint16_t* tgx_get_system_version_patch(void) {
    return (uint16_t*)(sSystemSpaceStart + TGXP_SYSTEM_VERSION_PATCH);
}
const char* tgx_get_system_name(void) {
    return (const char*)(sSystemSpaceStart + TGXP_SYSTEM_NAME);
}
const uint64_t* tgx_get_system_boot_time(void) {
    return (uint64_t*)(sSystemSpaceStart + TGXP_SYSTEM_BOOT_TIME);
}
const uint64_t* tgx_get_system_up_time(void) {
    return (uint64_t*)(sSystemSpaceStart + TGXP_SYSTEM_UP_TIME);
}
const uint64_t* tgx_get_random_buffer(uint32_t* count) {
    *count = TGX_SYSTEM_RANDOM_BUFFER_COUNT;
    return (uint64_t*)(sSystemSpaceStart + TGXP_SYSTEM_RANDOM_BUFFER);
}

const uint32_t* tgx_get_console_mode(void) {
    return (uint32_t*)(sSystemSpaceStart + TGXP_SYSTEM_MODE);
}
const uint32_t* tgx_get_console_resolution_buffer(void) {
    return (uint32_t*)(sSystemSpaceStart + TGXP_SYSTEM_RESOLUTION_BUFFER);
}
uint32_t* tgx_get_console_system_flags(void) {
    return (uint32_t*)(sSystemSpaceStart + TGXP_SYSTEM_FLAGS);
}

const uint32_t* tgx_get_input_framebuffer_ptr(void) {
    return (uint32_t*)(sSystemSpaceStart + TGXP_SYSTEM_INPUT_BUFFER);
}
const uint32_t* tgx_get_input_last_framebuffer_ptr(void) {
    return (uint32_t*)(sSystemSpaceStart + TGXP_SYSTEM_LAST_INPUT_BUFFER);
}

void* tgx_get_user_ram_begin(void) {
    return sUserSpaceStart;
}
void* tgx_get_user_ram_end(void) {
    return sUserSpaceEnd;
}
void* tgx_get_memory_card_begin(void) {
    return sMemoryCardStart;
}
void* tgx_get_memory_card_end(void) {
    return sMemoryCardEnd;
}
void* tgx_get_system_ram_begin(void) {
    return sSystemSpaceStart;
}
void* tgx_get_system_ram_end(void) {
    return sSystemSpaceEnd;
}

static void tgx_init_system_memory(void) {
    sConsoleStart = sConsoleRam;
    sConsoleEnd = sConsoleRam + TGX_TOTAL_CONSOLE_RAM;
    sSystemSpaceStart = sConsoleRam;
    sSystemSpaceEnd = sSystemSpaceStart + TGX_SYSTEM_MEMORY_SIZE;
    sUserSpaceStart = sSystemSpaceEnd;
    sUserSpaceEnd = sUserSpaceStart + TGX_USER_MEMORY_SIZE;
    sMemoryCardStart = sUserSpaceEnd;
    sMemoryCardEnd = sMemoryCardStart + TGX_MEMORY_CARD_PAGE_SIZE;

    TGXVAL(uint8_t, sSystemSpaceStart + TGXP_SYSTEM_VERSION_MAJOR) = TGX_VERSION_MAJOR;
    TGXVAL(uint8_t, sSystemSpaceStart + TGXP_SYSTEM_VERSION_MINOR) = TGX_VERSION_MINOR;
    TGXVAL(uint16_t, sSystemSpaceStart + TGXP_SYSTEM_VERSION_PATCH) = TGX_VERSION_PATCH;

    char* name = TGXREF(char, sSystemSpaceStart + TGXP_SYSTEM_NAME);
    *name = 'T'; name++;
    *name = 'G'; name++;
    *name = 'X'; name++;
    *name = '-'; name++;
    *name = '6'; name++;
    *name = '4'; name++;
    *name = '\0';

    TGXVAL(uint64_t, sSystemSpaceStart + TGXP_SYSTEM_BOOT_TIME) = time(NULL);
    TGXVAL(uint64_t, sSystemSpaceStart + TGXP_SYSTEM_UP_TIME) = 0;

    srand((unsigned int)time(NULL));
    TGXVAL(uint32_t, sSystemSpaceStart + TGXP_SYSTEM_RANDOM_BUFFER +  0) = rand();
    TGXVAL(uint32_t, sSystemSpaceStart + TGXP_SYSTEM_RANDOM_BUFFER +  4) = rand();
    TGXVAL(uint32_t, sSystemSpaceStart + TGXP_SYSTEM_RANDOM_BUFFER +  8) = rand();
    TGXVAL(uint32_t, sSystemSpaceStart + TGXP_SYSTEM_RANDOM_BUFFER + 12) = rand();

    TGXVAL(uint32_t, sSystemSpaceStart + TGXP_SYSTEM_FLAGS) = TGX_FLAG_NONE;

    float* uni = TGXREF(float, sSystemSpaceStart + TGXP_SYSTEM_UNIFORM_PROJ_MATRIX);
    tgx_mat4_build_identity(uni);
    uni = TGXREF(float, sSystemSpaceStart + TGXP_SYSTEM_UNIFORM_VIEW_MATRIX);
    tgx_mat4_build_identity(uni);
    uni = TGXREF(float, sSystemSpaceStart + TGXP_SYSTEM_UNIFORM_MODEL_MATRIX);
    tgx_mat4_build_identity(uni);

    uni = TGXREF(float, sSystemSpaceStart + TGXP_SYSTEM_UNIFORM_COLOR_TINT);
    *uni = 1.0f; ++uni;
    *uni = 1.0f; ++uni;
    *uni = 1.0f; ++uni;
    *uni = 1.0f; ++uni;

    uni = TGXREF(float, sSystemSpaceStart + TGXP_SYSTEM_UNIFORM_UV_WAVE_COEF);
    *uni = 0.0f; ++uni; *uni = 0.0f;
}
static void tgx_update_system_memory(void) {
    time_t _time = time(NULL);
    TGXVAL(uint64_t, sSystemSpaceStart + TGXP_SYSTEM_UP_TIME) =
        _time - TGXVAL(uint64_t, sSystemSpaceStart + TGXP_SYSTEM_BOOT_TIME);

    TGXVAL(uint32_t, sSystemSpaceStart + TGXP_SYSTEM_RANDOM_BUFFER +  0) = rand();
    TGXVAL(uint32_t, sSystemSpaceStart + TGXP_SYSTEM_RANDOM_BUFFER +  4) = rand();
    TGXVAL(uint32_t, sSystemSpaceStart + TGXP_SYSTEM_RANDOM_BUFFER +  8) = rand();
    TGXVAL(uint32_t, sSystemSpaceStart + TGXP_SYSTEM_RANDOM_BUFFER + 12) = rand();
}

// PIXEL SHADER
static const char * const TGX_PIXEL_VERTEX_SHADER =
    "#version 330 core\n"
    "layout (location = 0) in vec3 tgxPosition;\n"
    "layout (location = 1) in vec3 tgxNormal;\n"
    "layout (location = 2) in vec2 tgxUv;\n"
    "layout (location = 3) in vec4 tgxColor;\n"
    "uniform mat4 tgxMatProjection;\n"
    "uniform mat4 tgxMatView;\n"
    "uniform mat4 tgxMatModel;\n"
    "out vec2 fUv;\n"
    "out vec4 fColor;\n"
    "void main(){\n"
    "  gl_Position = tgxMatProjection * tgxMatView * tgxMatModel * vec4(tgxPosition, 1.0);\n"
    "  fUv = tgxUv;\n"
    "  fColor = tgxColor;\n"
    "}\n";

static const char * const TGX_PIXEL_FRAGMENT_SHADER =
    "#version 330 core\n"
    "in vec2 fUv;\n"
    "in vec4 fColor;\n"
    "out vec4 fragColor;\n"
    "uniform sampler2D framebuffer;\n"
    "void main(){\n"
    "  fragColor = texture(framebuffer, fUv) * fColor;\n"
    "}\n";

static tgxVertexFormat s_DefaultVertexFormat = {
    .attributes = {
        { .type = TGX_VTYPE_FLOAT3, .offset = offsetof(tgxVertex, position) },
        { .type = TGX_VTYPE_FLOAT3, .offset = offsetof(tgxVertex, normal) },
        { .type = TGX_VTYPE_FLOAT2, .offset = offsetof(tgxVertex, uv) },
        { .type = TGX_VTYPE_FLOAT4, .offset = offsetof(tgxVertex, color) },
    },
    .attribute_count = 4,
    .stride = sizeof(tgxVertex),
};

tgxVertexFormat* tgx_vmode_get_default_format(void) {
    (void*)TGX_PIXEL_FRAGMENT_SHADER; (void*)TGX_PIXEL_VERTEX_SHADER; // REMOVE ME
    return &s_DefaultVertexFormat;
}

// SECTION ERROR Handling
void tgx_panic(void) {
    fprintf(stderr, "Panic has been invoked!\n");

    tgx_dump_errors();

    exit(1);
}

static tgxErrorInfo s_ErrorStack[TGX_ERROR_STACK_LIMIT] = { 0 };
static uint32_t s_ErrorStackPtr = 0;

void tgx_print_error(FILE* out, const tgxErrorInfo* info) {
    fprintf(out, "Error(%u): %s\n| On Line: %u\n File: %s", info->errcode, info->message, info->line, info->file);
}

void tgx_dump_errors(void) {
    tgxErrorInfo info;
    while(tgx_has_error()) {
        tgx_pop_error(&info);
        tgx_print_error(stderr, &info);
    }
}

int tgx_error_count(void) {
    return s_ErrorStackPtr;
}
int tgx_has_error(void) {
    return tgx_error_count() > 0;
}
int tgx_is_error(int errcode) {
    return s_ErrorStackPtr > 0 && s_ErrorStack[s_ErrorStackPtr - 1].errcode == errcode;
}
int tgx_catch_error(int errcode, tgxErrorInfo* info) {
    if(tgx_is_error(errcode)) {
        tgx_pop_error(info);
        return true;
    }
    return false;
}
void tgx_pop_error(tgxErrorInfo* info) {
    if(s_ErrorStackPtr == 0) {
        fprintf(stderr, "Error stack underflow!\n");
        tgx_panic();
    }

    *info = s_ErrorStack[--s_ErrorStackPtr];
}
void tgx_push_error(tgxErrorInfo* info) {
    if(s_ErrorStackPtr >= TGX_ERROR_STACK_LIMIT) {
        fprintf(stderr, "Error stack overflow!\nCompile with a greater stack size or fix your bad code!\n");
        tgx_panic();
    }
    s_ErrorStack[s_ErrorStackPtr++] = *info;
}
void _tgx_push_error_(tgxErrorInfo info) {
    tgx_push_error(&info);
}

void tgx_clear_errors(void) {
    s_ErrorStackPtr = 0;
}

#ifndef TGX_CUSTOM_BACKEND

//static int s_KeyFrameInputs[TGX_BTN_COUNT] = { 0 };
//static int s_KeyLastFrameInputs[TGX_BTN_COUNT] = { 0 };

//static uint32_t s_PixelFrameBufferLength = 0;
//static uint32_t s_PixelFrameBufferVerticesLength = 0;
//static uint32_t *s_PixelFrameBuffer = NULL;
//static tgxVertex *s_PixelFrameBufferVertices = NULL;

typedef struct {
    void* address;
    uint32_t size;
} Allocation;

static uint32_t s_AllocationCount = 0;
static Allocation s_Allocations[TGX_ALLOCATION_LIMIT] = { 0 };
static Allocation s_ResizeAllocation = { 0 };

void tgx_init_allocations(void) {
    for(int i=0; i<TGX_ALLOCATION_LIMIT; i++) {
        s_Allocations[i].address = NULL;
        s_Allocations[i].size = 0;
    }
}

void tgx_deinit_allocations(void) {
    for(int i=0; i<TGX_ALLOCATION_LIMIT; i++) {
        if(s_Allocations[i].address != NULL) {
            free(s_Allocations[i].address);
        }
        s_Allocations[i].address = NULL;
        s_Allocations[i].size = 0;
    }
}

int tgx_find_free_allocation(void) {
    for(int i=0; i<TGX_ALLOCATION_LIMIT; i++) {
        if(s_Allocations[i].address == NULL) {
            return i;
        }
    }
    return -1;
}

int tgx_find_allocation(void* address) {
    for(int i=0; i<TGX_ALLOCATION_LIMIT; i++) {
        if(s_Allocations[i].address == address) {
            return i;
        }
    }
    return -1;
}

void* tgx_alloc(uint32_t bytes) {
    if(s_AllocationCount >= TGX_ALLOCATION_LIMIT) {
        return NULL;
    }

    void* allocation = malloc(bytes);

    if(allocation == NULL) {
        TGX_THROW("Malloc returned null", TGX_ERROR_NO_MEMORY);
        return NULL;
    }

    int slot = tgx_find_free_allocation();
    s_AllocationCount++;

    s_Allocations[slot].address = allocation;
    s_Allocations[slot].size = bytes;

    return allocation;
}

void tgx_free(void* ptr) {
    int slot = tgx_find_allocation(ptr);
    if(slot == -1) {
        free(ptr);
        return;
    }

    free(s_Allocations[slot].address);
    s_Allocations[slot].address = NULL;
    s_Allocations[slot].size = 0;
    s_AllocationCount--;
}

void* tgx_realloc(void* addr, uint32_t bytes) {
    int slot = tgx_find_allocation(addr);
    if(slot == -1) {
        TGX_THROW("Allocation not found", TGX_ERROR_NOT_FOUND);
        return NULL;
    }

    s_ResizeAllocation = s_Allocations[slot];
    s_Allocations[slot].address = malloc(bytes);

    if(s_Allocations[slot].address == NULL) {
        s_Allocations[slot].address = s_ResizeAllocation.address;
        TGX_THROW("Could not resize allocation", TGX_ERROR_NO_MEMORY);
        return s_Allocations[slot].address;
    }

    uint32_t size = s_Allocations[slot].size;
    if(s_ResizeAllocation.size < size) {
        size = s_ResizeAllocation.size;
    }

    s_Allocations[slot].size = bytes;
    memcpy(s_Allocations[slot].address, s_ResizeAllocation.address, size);
    free(s_ResizeAllocation.address);

    return s_Allocations[slot].address;
}

static void tgx_update_frame_inputs(tgxWindow* window) {
    uint32_t* input_buffer = TGXREF(uint32_t, sSystemSpaceStart + TGXP_SYSTEM_INPUT_BUFFER);
    TGXVAL(uint32_t, sSystemSpaceStart + TGXP_SYSTEM_LAST_INPUT_BUFFER) = *input_buffer;

    static uint32_t mask[2] = { 0x00000000, 0xFFFFFFFF };

    *input_buffer =
        (TGX_PAD_UP    & mask[glfwGetKey(window, _TGX_MAP_PAD_UP)    != GLFW_RELEASE]) |
        (TGX_PAD_LEFT  & mask[glfwGetKey(window, _TGX_MAP_PAD_LEFT)  != GLFW_RELEASE]) |
        (TGX_PAD_DOWN  & mask[glfwGetKey(window, _TGX_MAP_PAD_DOWN)  != GLFW_RELEASE]) |
        (TGX_PAD_RIGHT & mask[glfwGetKey(window, _TGX_MAP_PAD_RIGHT) != GLFW_RELEASE]) |
        (TGX_BTN_0     & mask[glfwGetKey(window, _TGX_MAP_BTN_0)     != GLFW_RELEASE]) |
        (TGX_BTN_1     & mask[glfwGetKey(window, _TGX_MAP_BTN_1)     != GLFW_RELEASE]) |
        (TGX_BTN_2     & mask[glfwGetKey(window, _TGX_MAP_BTN_2)     != GLFW_RELEASE]) |
        (TGX_BTN_3     & mask[glfwGetKey(window, _TGX_MAP_BTN_3)     != GLFW_RELEASE]) |
        (TGX_BTN_4     & mask[glfwGetKey(window, _TGX_MAP_BTN_4)     != GLFW_RELEASE]) |
        (TGX_BTN_5     & mask[glfwGetKey(window, _TGX_MAP_BTN_5)     != GLFW_RELEASE]) |
        (TGX_BTN_6     & mask[glfwGetKey(window, _TGX_MAP_BTN_6)     != GLFW_RELEASE]) |
        (TGX_BTN_7     & mask[glfwGetKey(window, _TGX_MAP_BTN_7)     != GLFW_RELEASE]);


    /*for(int i=0; i<TGX_BTN_COUNT; i++) {
        s_KeyLastFrameInputs[i] = s_KeyFrameInputs[i];
    }

    s_KeyFrameInputs[TGX_PAD_UP]    = glfwGetKey(window, _TGX_MAP_PAD_UP)    == GLFW_PRESS;
    s_KeyFrameInputs[TGX_PAD_LEFT]  = glfwGetKey(window, _TGX_MAP_PAD_LEFT)  == GLFW_PRESS;
    s_KeyFrameInputs[TGX_PAD_DOWN]  = glfwGetKey(window, _TGX_MAP_PAD_DOWN)  == GLFW_PRESS;
    s_KeyFrameInputs[TGX_PAD_RIGHT] = glfwGetKey(window, _TGX_MAP_PAD_RIGHT) == GLFW_PRESS;

    s_KeyFrameInputs[TGX_BTN_0] = glfwGetKey(window, _TGX_MAP_BTN_0) == GLFW_PRESS;
    s_KeyFrameInputs[TGX_BTN_1] = glfwGetKey(window, _TGX_MAP_BTN_1) == GLFW_PRESS;
    s_KeyFrameInputs[TGX_BTN_2] = glfwGetKey(window, _TGX_MAP_BTN_2) == GLFW_PRESS;
    s_KeyFrameInputs[TGX_BTN_3] = glfwGetKey(window, _TGX_MAP_BTN_3) == GLFW_PRESS;
    s_KeyFrameInputs[TGX_BTN_4] = glfwGetKey(window, _TGX_MAP_BTN_4) == GLFW_PRESS;
    s_KeyFrameInputs[TGX_BTN_5] = glfwGetKey(window, _TGX_MAP_BTN_5) == GLFW_PRESS;
    s_KeyFrameInputs[TGX_BTN_6] = glfwGetKey(window, _TGX_MAP_BTN_6) == GLFW_PRESS;
    s_KeyFrameInputs[TGX_BTN_7] = glfwGetKey(window, _TGX_MAP_BTN_7) == GLFW_PRESS;*/
}

void tgx_context_hint(int major, int minor, int profile) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, profile);
}

tgxWindow* tgx_create_window(int width, int height, const char* title) {
    tgxWindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    return window;
}

void tgx_destroy_window(tgxWindow* window) {
    glfwDestroyWindow(window);
}

void tgx_set_window_monitor(tgxContext* window, tgxEnum monitor) {
    int width = tgx_get_width(window->resolution);
    int height = tgx_get_height(window->resolution);

    if(monitor == TGX_MONITOR_NONE) {
        glfwSetWindowMonitor(window->display_handle, NULL, 0, 0, width, height, 60);
    }
    else {
        int monitorCount;
        GLFWmonitor **monitors = glfwGetMonitors(&monitorCount);

        if(monitor < 0) monitor = 0;
        if(monitor >= monitorCount) monitor = 0;

        glfwSetWindowMonitor(window->display_handle, monitors[monitor], 0, 0, width, height, 60);
    }
}

int tgx_get_window_width(tgxContext *context) {
    return tgx_get_width(context->resolution);
}
int tgx_get_window_height(tgxContext *context) {
    return tgx_get_height(context->resolution);
}

void tgx_poll_events(tgxContext* ctx) {
    glfwPollEvents();
    tgx_update_frame_inputs(ctx->display_handle);

    if(glfwWindowShouldClose(ctx->display_handle)) {
        tgx_clear_flag(TGX_FLAG_POWER);
    }
}

void tgx_swap_buffers(tgxWindow* window) {
    glfwSwapBuffers(window);
}

void tgx_context_initialize(void) {
    glfwInit();
}

void tgx_context_terminate(void) {
    // TODO: deinit vertex/pixel context
    glfwTerminate();
}

tgxResult tgx_context_prepare(tgxContext *context) {
    glfwMakeContextCurrent(context->display_handle);

    if(!gladLoadGLLoader(glfwGetProcAddress)) {
        TGX_THROW("Error initializing glad", TGX_ERROR_GLAD_LOADER);
        return TGX_ERR;
    }
    return TGX_OK;
}

int tgx_get_window_is_open(tgxWindow* window) {
    return tgx_get_flag(TGX_FLAG_POWER);
}
void tgx_set_window_should_close(tgxWindow* window, int close) {
    if(close) {
        tgx_clear_flag(TGX_FLAG_POWER);
        return;
    }
    tgx_set_flag(TGX_FLAG_POWER);
}

double tgx_get_time(void) {
    return glfwGetTime();
}

int tgx_key_check(tgxContext* context, tgxEnum key) {
    return TGXVAL(uint32_t, sSystemSpaceStart + TGXP_SYSTEM_INPUT_BUFFER) & key;
}
int tgx_key_check_pressed(tgxContext* context, tgxEnum key) {
    return (TGXVAL(uint32_t, sSystemSpaceStart + TGXP_SYSTEM_INPUT_BUFFER) & key) &&
            !(TGXVAL(uint32_t, sSystemSpaceStart + TGXP_SYSTEM_LAST_INPUT_BUFFER) & key);
}
int tgx_key_check_released(tgxContext* context, tgxEnum key) {
    return !(TGXVAL(uint32_t, sSystemSpaceStart + TGXP_SYSTEM_INPUT_BUFFER) & key) &&
            (TGXVAL(uint32_t, sSystemSpaceStart + TGXP_SYSTEM_LAST_INPUT_BUFFER) & key);
}


// this could cause a memory leak if used on a pre-existing tgxVertexInstances
tgxResult tgx_vmode_init_vector_instances(tgxVertexInstances *instances) {
    instances->instances = malloc(TGX_INITIAL_CAPACITY * sizeof(tgxVertexPacket));

    if(instances->instances == NULL) {
        TGX_THROW("Allocation returned null", TGX_ERROR_NO_MEMORY);
        return TGX_ERR;
    }

    instances->instance_capacity = TGX_INITIAL_CAPACITY;

    return TGX_OK;
}
void tgx_vmode_deinit_vector_instances(tgxVertexInstances *instances) {

    for(uint32_t i=0; i<instances->instance_count; i++) {
        if(tgx_vmode_instance_valid(instances, i)) {
            tgx_vmode_destroy_instance(instances, i);
        }
    }

    tgx_free(instances->instances);

    instances->instances = NULL;
    instances->instance_count = 0;
    instances->instance_capacity = 0;
}

tgxVertexInstanceID tgx_vmode_find_invalid(tgxVertexInstances *instances) {
    for(uint32_t i=0; i<instances->instance_count; i++) {
        if(!tgx_vmode_instance_valid(instances, i)) {
            return i;
        }
    }
    return TGX_VERTEX_INSTANCE_NULL;
}

tgxResult tgx_vector_instances_pushback(tgxVertexInstances *instances, uint32_t *added_index) {
    if(instances->instance_count >= instances->instance_capacity) {
        instances->instance_capacity *= TGX_GROWTH_FACTOR;
        instances->instances = tgx_realloc(instances->instances, instances->instance_capacity);

        if(tgx_has_error()) {
            tgxErrorInfo info;
            tgx_pop_error(&info);
            tgx_print_error(stderr, &info);

            TGX_THROW("Out of memory", TGX_ERROR_NO_MEMORY);
            return TGX_ERR;
        }
    }
    *added_index = instances->instance_count;
    instances->instance_count++;
    return TGX_OK;
}

tgxResult tgx_vmode_setup_attribs(tgxVertexFormat format) {
    for(uint32_t i=0; i<format.attribute_count; i++) {
        tgxVertexAttribute attr = format.attributes[i];

        GLenum type;
        GLint size;
        switch(attr.type) {
            case TGX_VTYPE_FLOAT1:
                type = GL_FLOAT;
                size = 1;
                break;
            case TGX_VTYPE_FLOAT2:
                type = GL_FLOAT;
                size = 2;
                break;
            case TGX_VTYPE_FLOAT3:
                type = GL_FLOAT;
                size = 3;
                break;
            case TGX_VTYPE_FLOAT4:
                type = GL_FLOAT;
                size = 4;
                break;
            case TGX_VTYPE_INT1:
                type = GL_INT;
                size = 1;
                break;
            case TGX_VTYPE_INT2:
                type = GL_INT;
                size = 2;
                break;
            case TGX_VTYPE_INT3:
                type = GL_INT;
                size = 3;
                break;
            case TGX_VTYPE_INT4:
                type = GL_INT;
                size = 4;
                break;
            default:
                TGX_THROW("Invalid type specifier for vertex attribute", TGX_ERROR_INVALID_VERTEX_ATTRIBUTE);
                return TGX_ERR;
        }

        glVertexAttribPointer(i, size, type, false, format.stride, (void*)attr.offset);
        glEnableVertexAttribArray(i);
    }

    return TGX_OK;
}

tgxVertexInstanceID tgx_vmode_create_instance(tgxVertexInstances *instances, tgxVertexFormat format, void* buffer, uint32_t buffer_length, tgxEnum usage_hint) {
    uint32_t _id = tgx_vmode_find_invalid(instances);

    if(_id == TGX_VERTEX_INSTANCE_NULL) {
        if(tgx_vector_instances_pushback(instances, &_id) != TGX_OK) {
            return TGX_VERTEX_INSTANCE_NULL;
        }
    }

    tgxVertexPacket* instance = &instances->instances[_id];
    instance->format = format;
    instance->program_id = 0;
    instance->vertex_count = buffer_length / format.stride;

    glGenVertexArrays(1, &instance->vao);
    glBindVertexArray(instance->vao);

    glGenBuffers(1, &instance->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, instance->vbo);

    GLenum usage;
    if(usage_hint == TGX_DYNAMIC_MEMORY_HINT) {
        usage = GL_DYNAMIC_DRAW;
    }
    else {
        usage = GL_STATIC_DRAW;
    }

    glBufferData(GL_ARRAY_BUFFER, buffer_length, buffer, usage);

    tgx_vmode_setup_attribs(format);
    tgxErrorInfo errinfo;
    if(tgx_catch_error(TGX_ERROR_INVALID_VERTEX_ATTRIBUTE, &errinfo)) {
        tgx_vmode_destroy_instance(instances, _id);
        return TGX_VERTEX_INSTANCE_NULL;
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return _id;
}

tgxResult tgx_vmode_update_instance(tgxVertexInstances* instances, tgxVertexInstanceID instance, void* buffer, uint32_t buffer_len, tgxEnum usage_hint) {
    if(!tgx_vmode_instance_valid(instances, instance)) {
        TGX_THROW("Instance not found", TGX_ERROR_INSTANCE_NOT_FOUND);
        return TGX_ERR;
    }

    tgxVertexPacket *packet = &instances->instances[instance];
    glBindVertexArray(packet->vao);
    glBindBuffer(GL_ARRAY_BUFFER, packet->vbo);

    packet->vertex_count = buffer_len / packet->format.stride;

    GLenum usage;
    if(usage_hint == TGX_DYNAMIC_MEMORY_HINT) {
        usage = GL_DYNAMIC_DRAW;
    }
    else {
        usage = GL_STATIC_DRAW;
    }

    glBufferData(GL_ARRAY_BUFFER, buffer_len, buffer, usage);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return TGX_OK;
}

void tgx_vmode_destroy_instance(tgxVertexInstances* instances, tgxVertexInstanceID instance) {
    if(!tgx_vmode_instance_valid(instances, instance)) {
        return;
    }

    tgxVertexPacket *packet = &instances->instances[instance];

    glDeleteBuffers(1, &packet->vbo);
    glDeleteVertexArrays(1, &packet->vao);

    packet->vao = 0; // this should be enough to set the instance as "invalid" and allow for it to be reused
    packet->vbo = 0;
    packet->program_id = 0; // program is not owned by packet as shaders can (and should) be shared
}

tgxResult tgx_vmode_set_instance_shader(tgxVertexInstances *instances, tgxVertexInstanceID instance, tgxProgram program) {
    if(!tgx_vmode_instance_valid(instances, instance)) {
        TGX_THROW("Instance provided for shader set is not valid.", TGX_ERROR_INSTANCE_NOT_FOUND);
        return TGX_ERR;
    }

    instances->instances[instance].program_id = program;

    return TGX_OK;
}

tgxResult tgx_vmode_present_instance(tgxVertexInstances *instances, tgxVertexInstanceID instance, tgxEnum primitive_type, tgxUniformData* uniforms, uint32_t uniform_count) {
    static float s_mat_identity[16] = { 1.0f, 0.0f, 0.0f, 0.0f,
                                     0.0f, 1.0f, 0.0f, 0.0f,
                                     0.0f, 0.0f, 1.0f, 0.0f,
                                     0.0f, 0.0f, 0.0f, 1.0f };

    if(!tgx_vmode_instance_valid(instances, instance)) {
        TGX_THROW("Instance provided for presentation is not valid.", TGX_ERROR_INSTANCE_NOT_FOUND);
        return TGX_ERR;
    }

    tgxVertexPacket *packet = &instances->instances[instance];

    glBindVertexArray(packet->vao);
    glBindBuffer(GL_ARRAY_BUFFER, packet->vbo);
    glUseProgram(packet->program_id);



    if(TGXVAL(uint32_t, TGXP_SYSTEM_FLAGS + sSystemSpaceStart) & TGX_FLAG_UPLOAD_DEFAULT_UNIS) {
        GLuint proj = glGetUniformLocation(packet->program_id, "tgxMatUniform");
        GLuint view = glGetUniformLocation(packet->program_id, "tgxMatView");
        GLuint model = glGetUniformLocation(packet->program_id, "tgxMatModel");

        glUniform1fv(packet->program_id, proj, TGXREF(float, sSystemSpaceStart + TGXP_SYSTEM_UNIFORM_PROJ_MATRIX));
        glUniform1fv(packet->program_id, view, TGXREF(float, sSystemSpaceStart + TGXP_SYSTEM_UNIFORM_VIEW_MATRIX));
        glUniform1fv(packet->program_id, model, TGXREF(float, sSystemSpaceStart + TGXP_SYSTEM_UNIFORM_MODEL_MATRIX));
    }

    for(uint32_t i=0; i<uniform_count; i++) {
        GLuint loc = glGetUniformLocation(packet->program_id, uniforms->name);
        switch(uniforms->type) {
            case TGX_VTYPE_FLOAT1:
                glUniform1f(loc, uniforms->f1);
                break;
            case TGX_VTYPE_FLOAT2:
                glUniform2f(loc, uniforms->f2[0], uniforms->f2[1]);
                break;
            case TGX_VTYPE_FLOAT3:
                glUniform3f(loc, uniforms->f3[0], uniforms->f3[1], uniforms->f3[2]);
                break;
            case TGX_VTYPE_FLOAT4:
                glUniform4f(loc, uniforms->f4[0], uniforms->f4[1], uniforms->f4[2], uniforms->f4[3]);
                break;
            case TGX_VTYPE_INT1:
                glUniform1i(loc, uniforms->i1);
                break;
            case TGX_VTYPE_INT2:
                glUniform2i(loc, uniforms->i2[0], uniforms->i2[1]);
                break;
            case TGX_VTYPE_INT3:
                glUniform3i(loc, uniforms->i3[0], uniforms->i3[1], uniforms->i3[2]);
                break;
            case TGX_VTYPE_INT4:
                glUniform4i(loc, uniforms->i4[0], uniforms->i4[1], uniforms->i4[2], uniforms->i4[3]);
                break;
            case TGX_VTYPE_MAT4:
                glUniformMatrix4fv(loc, 1, GL_FALSE, uniforms->mat4);
                break;
            case TGX_VTYPE_SAMPLER2D:
                // TODO: finish
                break;
            default:
                TGX_THROW("Invalid uniform type", TGX_ERROR_INVALID_UNIFORM_TYPE);
                return TGX_ERR;
        }
        uniforms++;
    }

    GLenum pmode;
    switch(primitive_type) {
        default:
        case TGX_TRIANGLE_LIST:
            pmode = GL_TRIANGLES;
            break;
        case TGX_TRIANGLE_FAN:
            pmode = GL_TRIANGLE_FAN;
            break;
        case TGX_TRIANGLE_STRIP:
            pmode = GL_TRIANGLE_STRIP;
            break;
        case TGX_LINE_LIST:
            pmode = GL_LINES;
            break;
        case TGX_LINE_STRIP:
            pmode = GL_LINE_STRIP;
            break;
        case TGX_POINT_LIST:
            pmode = GL_POINTS;
            break;
    }

    glDrawArrays(pmode, 0, packet->vertex_count);

    return TGX_OK;
}

// Function to compile a shader
static GLuint _compile_shader(GLenum shaderType, const char* shaderSource) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);

    // Check compilation status
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        fprintf(stderr, "Error: Shader compilation failed:\n%s\n", infoLog);
        glDeleteShader(shader);

        TGX_THROW("Error compiling shader. See stderr for details.", TGX_ERROR_COMPILING_SHADER);
        return 0;
    }

    return shader;
}

// Function to link shaders into a program
static GLuint _compile_shader_program(const char* vertexData, const char* fragmentData) {
    // Compile the vertex shader
    GLuint vertexShader = _compile_shader(GL_VERTEX_SHADER, vertexData);
    if(tgx_has_error()) {
        return 0;
    }

    // Compile the fragment shader
    GLuint fragmentShader = _compile_shader(GL_FRAGMENT_SHADER, fragmentData);
    if(tgx_has_error()) {
        return 0;
    }

    // Create and link the shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check linking status
    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        fprintf(stderr, "Error: Shader program linking failed:\n%s\n", infoLog);
        glDeleteProgram(shaderProgram);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        TGX_THROW("Error linking shader, see stderr for details.", TGX_ERROR_LINKING_SHADER);
        return 0;
    }

    // Clean up shaders (they are no longer needed after linking)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

tgxResult tgx_vmode_compile_shader(const char* vdata, const char* fdata, tgxProgram* program) {
    GLuint gl_program = _compile_shader_program(vdata, fdata);
    tgxErrorInfo info;
    if(tgx_catch_error(TGX_ERROR_COMPILING_SHADER, &info)) {
        tgx_print_error(stderr, &info);
        tgx_push_error(&info);
        return TGX_ERR;
    }

    if(tgx_catch_error(TGX_ERROR_LINKING_SHADER, &info)) {
        tgx_print_error(stderr, &info);
        tgx_push_error(&info);
        return TGX_ERR;
    }

    *program = gl_program;
    return TGX_OK;
}

void tgx_vmode_destroy_shader(tgxProgram program) {
    glDeleteProgram(program);
}

tgxResult tgx_initialize_vector_mode(tgxContext* context) {
    context->graphics_context = malloc(sizeof(tgxVectorContext));
    if(context->graphics_context == NULL) {
        TGX_THROW("Out of memory", TGX_ERROR_NO_MEMORY);
        return TGX_ERR;
    }

    if(tgx_vmode_init_vector_instances(&((tgxVectorContext*)context->graphics_context)->instances) != TGX_OK) {
        return TGX_ERR;
    }

    return TGX_OK;
}


int tgx_vmode_instance_valid(tgxVertexInstances* instances, tgxVertexInstanceID id) {
    return (instances != NULL && instances->instances != NULL) && id < instances->instance_count && instances->instances[id].vao != 0;
}


tgxResult tgx_initialize_pixel_mode(tgxContext* context) {
    context->graphics_context = malloc(sizeof(tgxPixelContext));
    if(context->graphics_context == NULL) {
        TGX_THROW("Out of Memory", TGX_ERROR_NO_MEMORY);
        return TGX_ERR;
    }

    tgxPixelContext* pContext = (tgxPixelContext*)context->graphics_context;

    if(tgx_vmode_init_vector_instances(&(pContext->vector_context.instances)) != TGX_OK) {
        return TGX_ERR;
    }

    tgxVertex buffer[6] = {
        (tgxVertex){ .position = { 0.0f, 0.0f, 0.0f}, .normal = {0.0f, 0.0f, -1.0f}, .uv = {0.0f, 0.0f}, .color = {1.0f, 1.0f, 1.0f, 1.0f} },
        (tgxVertex){ .position = { 1.0f, 0.0f, 0.0f}, .normal = {0.0f, 0.0f, -1.0f}, .uv = {1.0f, 0.0f}, .color = {1.0f, 1.0f, 1.0f, 1.0f} },
        (tgxVertex){ .position = { 0.0f, 1.0f, 0.0f}, .normal = {0.0f, 0.0f, -1.0f}, .uv = {0.0f, 1.0f}, .color = {1.0f, 1.0f, 1.0f, 1.0f} },
        (tgxVertex){ .position = { 0.0f, 1.0f, 0.0f}, .normal = {0.0f, 0.0f, -1.0f}, .uv = {0.0f, 1.0f}, .color = {1.0f, 1.0f, 1.0f, 1.0f} },
        (tgxVertex){ .position = { 1.0f, 0.0f, 0.0f}, .normal = {0.0f, 0.0f, -1.0f}, .uv = {1.0f, 0.0f}, .color = {1.0f, 1.0f, 1.0f, 1.0f} },
        (tgxVertex){ .position = { 1.0f, 1.0f, 0.0f}, .normal = {0.0f, 0.0f, -1.0f}, .uv = {1.0f, 1.0f}, .color = {1.0f, 1.0f, 1.0f, 1.0f} },
    };

    pContext->framebuffer_instance = tgx_vmode_create_instance(&(pContext->vector_context.instances), s_DefaultVertexFormat, buffer, sizeof(buffer), TGX_STATIC_MEMORY_HINT);

    if(tgx_vmode_compile_shader(TGX_PIXEL_VERTEX_SHADER, TGX_PIXEL_FRAGMENT_SHADER,&pContext->shader) != TGX_OK){
        tgx_vmode_deinit_vector_instances(&(pContext->vector_context.instances));
        free(context->graphics_context);
        context->graphics_context = NULL;
        return TGX_ERR;
    }
    tgx_vmode_set_instance_shader(&pContext->vector_context.instances, pContext->framebuffer_instance, pContext->shader);

    tgx_get_size(context->resolution, &pContext->width, &pContext->height);

    uint32_t texture_length;
    uint8_t *data = tgx_get_framebuffer(context, &texture_length);

    tgxTextureSubmitInfo info = {0};
    info.width = pContext->width;
    info.height = pContext->height;
    info.data = data;
    info.border_policy = TGX_TEXTURE_REPEAT;
    info.format = TGX_RGB;
    info.desired_format = TGX_RGBA;
    info.min_filter = TGX_FILTER_NEAREST;
    info.mag_filter = TGX_FILTER_NEAREST;
    info.existing_handle = TGX_TEXTURE_NULL;

    pContext->framebuffer_handle = tgx_submit_texture(&info);

    tgx_set_flag(TGX_FLAG_UPLOAD_DEFAULT_UNIS);
    //tgx_mat4_build_ortho(TGXREF(float, sSystemSpaceStart + TGXP_SYSTEM_UNIFORM_PROJ_MATRIX), 0.0f, 0.0f, 1.0f, 1.0f, 0.1f, 1.0f);

    return TGX_OK;
}


void tgx_mat4_build_identity(float mat[16]) {
    mat[ 0] = 1.0f; mat[ 1] = 0.0f; mat[ 2] = 0.0f; mat[ 3] = 0.0f;
    mat[ 4] = 0.0f; mat[ 5] = 1.0f; mat[ 6] = 0.0f; mat[ 7] = 0.0f;
    mat[ 8] = 0.0f; mat[ 9] = 0.0f; mat[10] = 1.0f; mat[11] = 0.0f;
    mat[12] = 0.0f; mat[13] = 0.0f; mat[14] = 0.0f; mat[15] = 1.0f;
}
void tgx_mat4_build_value(float mat[16], float value) {
    mat[ 0] = value; mat[ 1] =  0.0f; mat[ 2] =  0.0f; mat[ 3] =  0.0f;
    mat[ 4] =  0.0f; mat[ 5] = value; mat[ 6] =  0.0f; mat[ 7] =  0.0f;
    mat[ 8] =  0.0f; mat[ 9] =  0.0f; mat[10] = value; mat[11] =  0.0f;
    mat[12] =  0.0f; mat[13] =  0.0f; mat[14] =  0.0f; mat[15] = value;
}

void tgx_mat4_build(float mat[16],
    float translation_x, float translation_y, float translation_z,
    float rotation_x, float rotation_y, float rotation_z,
    float scale_x, float scale_y, float scale_z) {

    float cos_x = cosf(rotation_x); float sin_x = sinf(rotation_x);
    float cos_y = cosf(rotation_y); float sin_y = sinf(rotation_y);
    float cos_z = cosf(rotation_z); float sin_z = sinf(rotation_z);

    float scale[16] = {
        scale_x, 0.0f, 0.0f, 0.0f,
        0.0f, scale_y, 0.0f, 0.0f,
        0.0f, 0.0f, scale_z, 0.0f,
        0.0f, 0.0f, 0.0f,    1.0f,
    };
    float rotation_x_mat[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, cos_x, -sin_x, 0.0f,
        0.0f, sin_x, cos_x, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };
    float rotation_y_mat[16] = {
        cos_y, 0.0f, sin_y, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        -sin_y, 0.0f, cos_y, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };
    float rotation_z_mat[16] = {
        cos_z, -sin_z, 0.0f, 0.0f,
        sin_z, cos_z, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };

    float translation_mat[16] = {
        1.0f, 0.0f, 0.0f, translation_x,
        0.0f, 1.0f, 0.0f, translation_y,
        0.0f, 0.0f, 1.0f, translation_z,
        0.0f, 0.0f, 0.0f, 1.0f,
    };

    float t0[16], t1[16];
    tgx_mat4_mul(scale, rotation_x_mat, t0);
    tgx_mat4_mul(t0, rotation_y_mat, t1);
    tgx_mat4_mul(t1, rotation_z_mat, t0);
    tgx_mat4_mul(translation_mat, t0, mat);
}

void tgx_mat4_build_perspective(float mat[16], float fov, float aspect, float near, float far) {
    // convert to radians
    float fov_rad = fov * (M_PI / 180.0f);

    float f = 1.0f / tanf(fov_rad / 2.0f);

    tgx_mat4_build_value(mat, 0.0f);

    mat[0] = f / aspect;
    mat[5] = f;
    mat[10] = -(far + near) / (far - near);
    mat[11] = -1.0f;
    mat[14] = -(2.0f * far * near) / (far - near);
}

void tgx_mat4_build_ortho(float mat[16], float left, float top, float right, float bottom, float near, float far) {
    tgx_mat4_build_value(mat, 0.0f);

    mat[0] = 2.0f / (right - left);
    mat[5] = 2.0f / (top - bottom);
    mat[10] = -2.0f / (far - near);

    mat[12] = -(right + left) / (right - left);
    mat[13] = -(top + bottom) / (top - bottom);
    mat[14] = -(far + near) / (far - near);
    mat[15] = 1.0f;
}

void tgx_mat4_mul(float mat0[16], float mat1[16], float result[16]) {
    for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++) {
            result[i * 4 + j] =
                mat0[i * 4 + 0] * mat1[0 * 4 + j] +
                mat0[i * 4 + 1] * mat1[1 * 4 + j] +
                mat0[i * 4 + 2] * mat1[2 * 4 + j] +
                mat0[i * 4 + 3] * mat1[3 * 4 + j];
        }
    }
}

float tgx_vec3_length(float v[3]) {
    return sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}
void tgx_vec3_normalize(float v[3]) {
    float len = tgx_vec3_length(v);
    if(len > 0.0f) {
        v[0] /= len;
        v[1] /= len;
        v[2] /= len;
    }
}
void tgx_vec3_cross(float result[3], float a[3], float b[3]) {
    result[0] = a[1] * b[2] - a[2] * b[1];
    result[1] = a[2] * b[0] - a[0] * b[2];
    result[2] = a[0] * b[1] - a[1] * b[0];
}

void tgx_mat4_build_lookat(float mat[16],
    float xfrom, float yfrom, float zfrom,
    float xto, float yto, float zto,
    float xup, float yup, float zup) {

    float forward[3] = {xfrom - xto, yfrom - yto, zfrom - zto};
    tgx_vec3_normalize(forward);

    float up[3] = {xup, yup, zup};
    float right[3];
    tgx_vec3_cross(right, up, forward);

    float up_recalc[3];
    tgx_vec3_cross(up_recalc, forward, right);

    mat[0] = right[0];
    mat[1] = up_recalc[0];
    mat[2] = -forward[0];
    mat[3] = 0.0f;

    mat[4] = right[1];
    mat[5] = up_recalc[1];
    mat[6] = -forward[1];
    mat[7] = 0.0f;

    mat[8] = right[2];
    mat[9] = up_recalc[2];
    mat[10] = -forward[2];
    mat[11] = 0.0f;

    mat[12] = -(right[0] * xfrom + right[1] * yfrom + right[2] * zfrom);
    mat[13] = -(up_recalc[0] * xfrom + up_recalc[1] * yfrom + up_recalc[2] * zfrom);
    mat[14] = forward[0] * xfrom + forward[1] * zfrom + forward[2] * zfrom;
    mat[15] = 1.0f;
}

void tgx_assign_texture_slot(int slot, tgxTexture texture) {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, texture);
}

tgxTexture tgx_submit_texture(const tgxTextureSubmitInfo* info) {
    GLuint tex;
    if(info->existing_handle == TGX_TEXTURE_NULL) {
        glGenTextures(1, &tex);
    }
    else {
        tex = info->existing_handle;
    }

    glBindTexture(GL_TEXTURE_2D, tex);

    GLenum wrap;
    GLenum min_filter;
    GLenum mag_filter;

    switch(info->border_policy) {
        default:
        case TGX_TEXTURE_REPEAT: wrap = GL_REPEAT; break;
        case TGX_TEXTURE_MIRROR: wrap = GL_MIRRORED_REPEAT; break;
        case TGX_TEXTURE_CLAMP: wrap = GL_CLAMP_TO_EDGE; break;
    }

    switch(info->min_filter) {
        default:
        case TGX_FILTER_NEAREST:
            min_filter = GL_NEAREST; break;
        case TGX_FILTER_LINEAR:
            min_filter = GL_LINEAR;
    }

    switch(info->mag_filter) {
        default:
        case TGX_FILTER_NEAREST:
            mag_filter = GL_NEAREST; break;
        case TGX_FILTER_LINEAR:
            mag_filter = GL_LINEAR;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);

    GLenum sourcefmt, destfmt;

    switch(info->format) {
        default:
        case TGX_RGB:
            sourcefmt = GL_RGB; break;
        case TGX_RGBA:
            sourcefmt = GL_RGBA; break;
    }


    switch(info->desired_format) {
        default:
        case TGX_RGB:
            destfmt = GL_RGB; break;
        case TGX_RGBA:
            destfmt = GL_RGBA; break;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, destfmt, info->width, info->height, 0, sourcefmt, GL_UNSIGNED_BYTE, info->data);

    if(info->generate_mipmaps) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    return tex;
}

void tgx_delete_texture(tgxTexture texture) {
    glDeleteTextures(1, &texture);
}

#endif

void tgx_get_size(tgxEnum resolution, int* width, int* height) {
    int w;
    int h;

    switch(resolution) {
        case TGX_CONFIG_TINY:
            w = _TGX_RES_TINY_W;
            h = _TGX_RES_TINY_H;
            break;
        default:
        case TGX_CONFIG_SMALL:
            w = _TGX_RES_SMALL_W;
            h = _TGX_RES_SMALL_H;
            break;
        case TGX_CONFIG_MEDIUM:
            w = _TGX_RES_MEDIUM_W;
            h = _TGX_RES_MEDIUM_H;
            break;
        case TGX_CONFIG_LARGE:
            w = _TGX_RES_LARGE_W;
            h = _TGX_RES_LARGE_H;
            break;
    }

    if(width != NULL) *width = w;
    if(height != NULL) *height = h;
}

int tgx_get_width(tgxEnum resolution) {
    int res;
    tgx_get_size(resolution, &res, NULL);
    return res;
}

int tgx_get_height(tgxEnum resolution) {
    int res;
    tgx_get_size(resolution, NULL, &res);
    return res;
}

int tgx_get_flag(tgxEnum flag) {
    return TGXVAL(uint32_t, sSystemSpaceStart + TGXP_SYSTEM_FLAGS) & flag;
}
void tgx_set_flag(tgxEnum flag) {
    TGXVAL(uint32_t, sSystemSpaceStart + TGXP_SYSTEM_FLAGS) |= flag;
}
void tgx_clear_flag(tgxEnum flag) {
    TGXVAL(uint32_t, sSystemSpaceStart + TGXP_SYSTEM_FLAGS) &= (~flag);
}

tgxResult tgx_initialize(const tgxConfigInfo* info, tgxContext *context) {
    tgx_init_allocations();
    tgx_init_system_memory();
    tgx_context_initialize();
    tgx_context_hint(TGX_PROFILE_MAJOR, TGX_PROFILE_MINOR, TGX_PROFILE);

    int width = tgx_get_width(info->resolution);
    int height = tgx_get_height(info->resolution);

    context->display_handle = tgx_create_window(width, height, info->title);

    if(context->display_handle == NULL) {
        tgx_context_terminate();

        TGX_THROW("Failed to create window", TGX_ERROR_GLFW_CREATE_WINDOW);

        return TGX_ERR;
    }

    if(tgx_context_prepare(context) != TGX_OK) {
        tgx_shutdown(context);
        return TGX_ERR;
    }

    context->resolution = info->resolution;
    context->mode = info->graphics_mode;

    if(info->graphics_mode == TGX_MODE_VECTOR) {
        if(tgx_initialize_vector_mode(context) != TGX_OK) {
            tgx_shutdown(context);
            return TGX_ERR;
        }
    }
    else {
        if(tgx_initialize_pixel_mode(context) != TGX_OK) {
            tgx_shutdown(context);
            return TGX_ERR;
        }
    }

    tgx_set_flag(TGX_FLAG_POWER);

    return TGX_OK;
}

tgxResult tgx_shutdown(tgxContext* context) {
    tgx_destroy_window(context->display_handle);
    context->display_handle = NULL;
    tgx_context_terminate();
    tgx_deinit_allocations();
    return TGX_OK;
}

void tgx_run(tgxContext* context, TGXUpdateFunc func) {
    double time = tgx_get_time();
    while(tgx_get_flag(TGX_FLAG_POWER)) {
        double delta = tgx_get_time() - time;
        time = tgx_get_time();

        tgx_update_system_memory();
        if(context->mode == TGX_MODE_PIXEL) {
            tgx_begin_frame(context);
        }

        if(!func(delta, context)) {
            tgx_set_window_should_close(context->display_handle, true);
        }

        if(context->mode == TGX_MODE_PIXEL) {
            tgx_end_frame(context);
        }

        tgx_poll_events(context);
        tgx_swap_buffers(context->display_handle);
    }
}

void tgx_begin_frame(tgxContext*  context) {
    (void*)context;
}

void tgx_end_frame(tgxContext* context) {
    tgxPixelContext *pContext = (tgxPixelContext*)context->graphics_context;

    tgxTextureSubmitInfo info = {0};
    tgx_get_size(context->resolution, &info.width, &info.height);
    info.data = tgx_get_framebuffer(context, NULL);
    info.border_policy = TGX_TEXTURE_REPEAT;
    info.min_filter = TGX_FILTER_NEAREST;
    info.mag_filter = TGX_FILTER_NEAREST;
    info.format = TGX_RGB;
    info.desired_format = TGX_RGBA;
    info.generate_mipmaps = false;
    info.existing_handle = pContext->framebuffer_handle;

    pContext->framebuffer_handle = tgx_submit_texture(&info);

    tgxUniformData uniforms[1];
    uniforms[0].name = "framebuffer";
    uniforms[0].type = TGX_VTYPE_INT1;
    uniforms[0].i1 = 0;

    tgx_assign_texture_slot(0, pContext->framebuffer_handle);
    tgx_vmode_present_instance(&pContext->vector_context.instances, pContext->framebuffer_instance, TGX_TRIANGLE_LIST, uniforms, 0);
}

void tgx_clear_framebuffer(tgxContext* context, uint8_t r, uint8_t g, uint8_t b) {
    uint32_t length;
    uint8_t* fb = tgx_get_framebuffer(context, &length);

    int64_t _length = length;

    while(_length > 0) {
        fb[0] = r;
        fb[1] = g;
        fb[2] = b;
        fb += 3;
        _length -= 3;
    }
}

uint8_t* tgx_get_framebuffer(tgxContext* context, uint32_t* length) {
    int width, height, len;
    tgx_get_size(context->resolution, &width, &height);
    len = width * height * 3;

    if(length != NULL) {
        *length = len;
    }

    return TGXREF(uint8_t, sSystemSpaceEnd - len);
}
