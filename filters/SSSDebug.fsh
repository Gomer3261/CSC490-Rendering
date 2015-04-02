uniform sampler2D fbo_depth;
uniform sampler2D fbo_render;
uniform sampler2D fbo_backface;
uniform int fbo_width;
uniform int fbo_height;
varying vec2 f_texcoord;

void main() {
    gl_FragColor = texture2D(fbo_render, f_texcoord) + texture2D(fbo_backface, f_texcoord);
}
