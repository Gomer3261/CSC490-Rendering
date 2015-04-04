uniform sampler2D fbo_depth;
uniform sampler2D fbo_render;
uniform sampler2D fbo_backface;
uniform sampler2D fbo_backface_depth;
uniform int fbo_width;
uniform int fbo_height;
varying vec2 f_texcoord;

float nearPlane = 0.1f;
float farPlane = 25.0f;

void main() {
  float depth = (2.0 * nearPlane) / (farPlane + nearPlane - texture2D(fbo_backface_depth, f_texcoord).x * (farPlane - nearPlane));

  gl_FragColor = vec4(depth, depth, depth, 1.0);//texture2D(fbo_backface, f_texcoord);
}
