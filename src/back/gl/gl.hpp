#pragma once

#if SEAL_WEBGL
#include <glad/gles2.h>
#include <GLFW/glfw3.h>

#define SEAL_GLES_2

#else
#include <glad/glad.h>

#define SEAL_GL_4
#endif

namespace seal::gles3 {}
