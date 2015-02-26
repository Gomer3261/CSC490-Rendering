uniform sampler2D fbo_render;
uniform sampler2D fbo_depth;
varying vec2 f_texcoord;

void main(void) {
  gl_FragColor = texture2D(fbo_render, f_texcoord);
}
