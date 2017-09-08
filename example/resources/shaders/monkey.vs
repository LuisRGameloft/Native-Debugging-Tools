
uniform mat4 mvp;
attribute vec3 pos;
attribute vec3 norm;

varying vec3 v_norm;

void main()
{
    v_norm = norm;
    gl_Position = mvp * vec4(pos,1.0);
}
