uniform sampler2D fbo_render;
uniform sampler2D fbo_depth;
uniform float offset;
varying vec2 f_texcoord;

void main(void) {
  vec2 texcoord = f_texcoord;
  texcoord.x += sin(texcoord.y * 4*2*3.14159 + offset) / 100.0;
  gl_FragColor = texture2D(fbo_render, texcoord);
  gl_FragDepth = texture2D(fbo_depth, texcoord).x;
  //gl_FragColor = vec4(offset/100, offset/100, offset/100, 1);
}
