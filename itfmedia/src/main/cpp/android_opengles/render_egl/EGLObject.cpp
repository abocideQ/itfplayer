#include "EGLObject.h"

void EGLObject::CreatePBufferSurface(EGLContext *shareContext) {
    //初始化EGL
    m_eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (m_eglDisplay == EGL_NO_DISPLAY) {
        return;
    }
    if (!eglInitialize(m_eglDisplay, m_majorVersion, m_minorVersion)) {
        return;
    }
    //获取 渲染表面(Surface)配置
    /* eglGetConfigs 获取可用配置列表 -> eglGetConfigAttrib 获取配置信息
     if (!eglGetConfigs(m_eglDisplay, m_configs, m_configsMax, m_configsNum)) {
        return;
    }
    for (int i = 0; i < *m_configsNum; ++i) {
        EGLint *value;
        eglGetConfigAttrib(m_eglDisplay, m_configs[i], nullptr, value);
    }*/
    EGLint config_attrs[] = {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_ALPHA_SIZE, 8,
            EGL_DEPTH_SIZE, 8,//深度测试(防止越位)
            EGL_STENCIL_SIZE, 8,//模板测试(边缘效果)
            EGL_NONE
    };
    if (!eglChooseConfig(m_eglDisplay, config_attrs, m_configs, m_configsMax, m_configsNum)) {
        return;
    }
    EGLConfig config = m_configs[0];
    //创建 渲染表面(Surface)
    int surface_attrs[] = {
            EGL_RENDER_BUFFER,
            EGL_BACK_BUFFER,
            EGL_NONE
    };
    m_eglSurface = eglCreatePbufferSurface(m_eglDisplay, config, surface_attrs);
    if (m_eglSurface == EGL_NO_SURFACE) {
        switch (eglGetError()) {
            case EGL_BAD_ALLOC:
                // Not enough resources available. Handle and recover
                break;
            case EGL_BAD_CONFIG:
                // Verify that provided EGLConfig is valid
                break;
            case EGL_BAD_PARAMETER:
                // Verify that the EGL_WIDTH and EGL_HEIGHT are
                // non-negative values
                break;
            case EGL_BAD_MATCH:
                // Check window and EGLConfig attributes to determine
                // compatibility and pbuffer-texture parameters
                break;
        }
        return;
    }
    //创建 上下文
    const EGLint context_attrs = {
            EGL_CONTEXT_CLIENT_VERSION, 2
            EGL_NONE
    };//EGL_NO_CONTEXT 表示不与其他上下文 共享数据
    m_eglContext = eglCreateContext(m_eglDisplay, config, EGL_NO_CONTEXT, context_attrs);
    if (m_context == EGL_NO_CONTEXT) {
        return;
    }
    //关联 指定上下文
    if (!eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext)) {
        return;
    }
}

void EGLObject::CreateWindowSurface(ANativeWindow *window) {
    //初始化EGL
    m_eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (m_eglDisplay == EGL_NO_DISPLAY) {
        return;
    }
    if (!eglInitialize(m_eglDisplay, m_majorVersion, m_minorVersion)) {
        return;
    }
    //获取 渲染表面(Surface)配置
    /* eglGetConfigs 获取可用配置列表 -> eglGetConfigAttrib 获取配置信息
     if (!eglGetConfigs(m_eglDisplay, m_configs, m_configsMax, m_configsNum)) {
        return;
    }
    for (int i = 0; i < *m_configsNum; ++i) {
        EGLint *value;
        eglGetConfigAttrib(m_eglDisplay, m_configs[i], nullptr, value);
    }*/
    EGLint config_attrs[] = {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_ALPHA_SIZE, 8,
            EGL_DEPTH_SIZE, 8,//深度测试(防止越位)
            EGL_STENCIL_SIZE, 8,//模板测试(边缘效果)
            EGL_NONE
    };
    if (!eglChooseConfig(m_eglDisplay, config_attrs, m_configs, m_configsMax, m_configsNum)) {
        return;
    }
    EGLConfig config = m_configs[0];
    //创建 渲染表面(Surface)
    int surface_attrs[] = {
            EGL_RENDER_BUFFER,
            EGL_BACK_BUFFER,
            EGL_NONE
    };
    m_eglSurface = eglCreateWindowSurface(m_eglDisplay, config, window, surface_attrs);
    if (m_eglSurface == EGL_NO_SURFACE) {
        switch (eglGetError()) {
            case EGL_BAD_ALLOC:
                // Not enough resources available. Handle and recover
                break;
            case EGL_BAD_CONFIG:
                // Verify that provided EGLConfig is valid
                break;
            case EGL_BAD_PARAMETER:
                // Verify that the EGL_WIDTH and EGL_HEIGHT are
                // non-negative values
                break;
            case EGL_BAD_MATCH:
                // Check window and EGLConfig attributes to determine
                // compatibility and pbuffer-texture parameters
                break;
        }
        return;
    }
    //创建 上下文
    const EGLint context_attrs = {
            EGL_CONTEXT_CLIENT_VERSION, 2
            EGL_NONE
    };//EGL_NO_CONTEXT 表示不与其他上下文 共享数据
    m_eglContext = eglCreateContext(m_eglDisplay, config, EGL_NO_CONTEXT, context_attrs);
    if (m_context == EGL_NO_CONTEXT) {
        return;
    }
    //关联 指定上下文
    if (!eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext)) {
        return;
    }
}

void EGLObject::SwapWindowBuffer() {
    if (!eglSwapBuffers(m_eglDisplay, m_eglSurface)) {
        return;
    }
}

void EGLObject::ReleaseEgl() {
    if (m_eglDisplay == EGL_NO_DISPLAY) {
        return;
    }
    eglMakeCurrent(m_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroySurface(m_eglDisplay, m_eglSurface);
    m_eglSurface = EGL_NO_SURFACE;
    eglDestroyContext(m_eglDisplay, m_eglContext);
    m_eglContext = EGL_NO_CONTEXT;
    eglReleaseThread();
    eglTerminate(m_eglDisplay);
    m_eglDisplay = EGL_NO_DISPLAY;
}