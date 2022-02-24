#ifndef ITFPLAYER_HEADER_OPENGL_H
#define ITFPLAYER_HEADER_OPENGL_H

#define PLATFORM_ANDROID
#undef PLATFORM_IOS

#ifdef PLATFORM_ANDROID

#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

#elif PLATFORM_IOS

//???????

#endif

#endif //ITFPLAYER_HEADER_OPENGL_H
