#ifndef _UTILS_GL_
# define _UTILS_GL_
# ifdef OS_ANDROID
#  include <GLES3/gl3.h>
#  include <GLES/glext.h>
# else
#  include <stdlib.h>
#  include <GL/glew.h>
#  include <GL/gl.h>
# endif
#endif // _UTILS_GL_