uniform sampler2D fbo_render;
uniform sampler2D fbo_depth;
varying vec2 f_texcoord;

float nearPlane = 0.1f;
float farPlane = 25.0f;

void main(void) {
  float depth = (2.0 * nearPlane) / (farPlane + nearPlane - texture2D(fbo_depth, f_texcoord).x * (farPlane - nearPlane));

  float vis;
  if(f_texcoord.x > 0.5)
    vis = depth;
  else
    vis = texture2D(fbo_depth, f_texcoord).x;
  gl_FragColor = vec4(vis, vis, vis, 1);
}
