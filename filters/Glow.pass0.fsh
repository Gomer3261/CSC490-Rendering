uniform sampler2D fbo_depth;
uniform sampler2D fbo_render;
uniform sampler2D fbo_glow;
uniform int fbo_width;
uniform int fbo_height;
varying vec2 f_texcoord;

float radius = 120;

float near_plane = 0.1; //Z-near
float far_plane = 25.0; //Z-far

void main() {
    //this will be our RGBA sum
    vec4 result = vec4(0.0);
    float blur = (radius/fbo_width)*(1-texture2D(fbo_depth, f_texcoord).x);

    //apply blurring, using a 9-tap filter with predefined gaussian weights
    result += texture2D(fbo_glow, vec2(f_texcoord.x - 7.0*blur, f_texcoord.y)) * 0.004429912106;
    result += texture2D(fbo_glow, vec2(f_texcoord.x - 6.0*blur, f_texcoord.y)) * 0.008957812118;
    result += texture2D(fbo_glow, vec2(f_texcoord.x - 5.0*blur, f_texcoord.y)) * 0.021596386605;
    result += texture2D(fbo_glow, vec2(f_texcoord.x - 4.0*blur, f_texcoord.y)) * 0.044368333872;
    result += texture2D(fbo_glow, vec2(f_texcoord.x - 3.0*blur, f_texcoord.y)) * 0.077674421993;
    result += texture2D(fbo_glow, vec2(f_texcoord.x - 2.0*blur, f_texcoord.y)) * 0.115876621105;
    result += texture2D(fbo_glow, vec2(f_texcoord.x - 1.0*blur, f_texcoord.y)) * 0.147308056121;

    result += texture2D(fbo_glow, vec2(f_texcoord.x, f_texcoord.y)) * 0.159576912161;

    result += texture2D(fbo_glow, vec2(f_texcoord.x + 1.0*blur, f_texcoord.y)) * 0.147308056121;
    result += texture2D(fbo_glow, vec2(f_texcoord.x + 2.0*blur, f_texcoord.y)) * 0.115876621105;
    result += texture2D(fbo_glow, vec2(f_texcoord.x + 3.0*blur, f_texcoord.y)) * 0.077674421993;
    result += texture2D(fbo_glow, vec2(f_texcoord.x + 4.0*blur, f_texcoord.y)) * 0.044368333872;
    result += texture2D(fbo_glow, vec2(f_texcoord.x + 5.0*blur, f_texcoord.y)) * 0.021596386605;
    result += texture2D(fbo_glow, vec2(f_texcoord.x + 6.0*blur, f_texcoord.y)) * 0.008957812118;
    result += texture2D(fbo_glow, vec2(f_texcoord.x + 7.0*blur, f_texcoord.y)) * 0.004429912106;

    //discard alpha for our simple demo, multiply by vertex color and return
    gl_FragColor = vec4(result.rgb, 1.0);
}
