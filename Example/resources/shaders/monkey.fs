
varying vec3 v_norm;

void main()
{
    float r = v_norm.x;
    float g = v_norm.y;
    float b = v_norm.z;
    if(r < 0.0) r  = -r;
    if(g < 0.0) g  = -g;
    if(b < 0.0) b  = -b;
    gl_FragColor = vec4(r,g,b,1.0);
}
