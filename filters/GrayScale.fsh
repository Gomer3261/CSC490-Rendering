uniform sampler2D fbo_render;
uniform sampler2D fbo_depth;
varying vec2 f_texcoord;

vec3 luminocity_coeff = vec3(0.299,0.587,0.114);

void main(void) {
  gl_FragColor = vec4(vec3(dot(texture2D(fbo_render, f_texcoord).rgb, luminocity_coeff)), 1.0);
}
