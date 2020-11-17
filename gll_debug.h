//
// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "glad/glad.h" // glad must be included before anything else opengl related.


namespace gll
{
///////////////////

// Checks if a debug context is active. Debug output callbacks only work for
// debug contexts.
// If using GLFW, a debug contex can be created by setting the GLFW_OPENGL_DEBUG_CONTEXT
// hint.
// If using the glfw utilities lib (glfwl), a debug contex can be created by passing
// DebugOutput::On to Lib::init().
bool haveDebugContext();

// Sets a callback for debug output. When null is passed the default callback below
// is used.
using DebugCallback = void(APIENTRY*)(GLenum source, GLenum type, unsigned int id,
                                      GLenum severity, GLsizei length,
                                      const char* message, const void* userParam);
bool setDebugOutputCallback(DebugCallback callback = nullptr);

// Default callback for debug output.
void APIENTRY logDebugInfo(GLenum source, GLenum type, unsigned int id, GLenum severity,
                           GLsizei length, const char* message, const void* userParam);

} // namespace gll
