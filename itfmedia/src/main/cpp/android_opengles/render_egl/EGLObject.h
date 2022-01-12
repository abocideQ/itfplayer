#ifndef ITFPLAYER_EGLOBJECT_H
#define ITFPLAYER_EGLOBJECT_H

#include <EGL/egl.h>
#include <GLES3/gl3.h>

class EGLObject {
public:
    //屏幕外Surface(常用于 共享上下文的多线程egl 处理),shareContext = eglGetCurrentContext()
    void CreatePBufferSurface(EGLContext *shareContext);

    //屏幕内Surface
    void CreateWindowSurface(ANativeWindow *window);

    //交换缓冲 OpenGLES绘制后,使用eglSwapBuffers交换缓冲,将绘制内容显示到屏幕上,屏幕外Surface不需要交换
    void SwapWindowBuffer();

    //释放Egl
    void ReleaseEgl();

private:
    //display
    EGLDisplay m_eglDisplay;
    EGLint *m_majorVersion;
    EGLint *m_minorVersion;
    //surface 配置
    const EGLint m_configsMax = 10;
    EGLConfig m_configs[m_configsMax]
    EGLint *m_configsNum;
    //surface
    EGLSurface *m_eglSurface;
    //context
    EGLContext *m_eglContext;
};

/*
 * EGLDisplay(Display) 实际显示设备的抽象
 * EGLSurface（Surface）来存储图像的内存区域
 * EGLContext (Context) OpenGLES绘图的一些状态信息
 * FrameBuffer 包含Color Buffer,Stencil Buffer,Depth Buffer
 * 1.获取 EGL Display 对象：eglGetDisplay()
 * 2.初始化与 EGLDisplay 之间的连接：eglInitialize()
 * 3.获取 EGLConfig 对象：eglChooseConfig()
 * 4.创建 EGLContext 实例：eglCreateContext()
 * 5.创建 EGLSurface 实例：eglCreateWindowSurface()
 * 6.连接 EGLContext 和 EGLSurface：eglMakeCurrent()
 * 7.使用 OpenGL ES API 绘制图形：gl_*()
 * 8.切换 front buffer 和 back buffer 送显：eglSwapBuffer()
 * 1.断开并释放与 EGLSurface 关联的 EGLContext 对象：eglRelease()
 * 2.删除 EGLSurface 对象
 * 3.删除 EGLContext 对象
 * 4.终止与 EGLDisplay 之间的连接*/

#endif //ITFPLAYER_EGLOBJECT_H