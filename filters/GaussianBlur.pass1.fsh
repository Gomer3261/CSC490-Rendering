uniform sampler2D fbo_depth;
uniform sampler2D fbo_render;
uniform int fbo_width;
uniform int fbo_height;
varying vec2 f_texcoord;

float radius = 4;

void main() {
    //this will be our RGBA sum
    vec4 result = vec4(0.0);
    float blur = radius/fbo_height;

    //apply blurring, using a 9-tap filter with predefined gaussian weights
    result += texture2D(fbo_render, vec2(f_texcoord.x, f_texcoord.y - 4.0*blur)) * 0.0162162162;
    result += texture2D(fbo_render, vec2(f_texcoord.x, f_texcoord.y - 3.0*blur)) * 0.0540540541;
    result += texture2D(fbo_render, vec2(f_texcoord.x, f_texcoord.y - 2.0*blur)) * 0.1216216216;
    result += texture2D(fbo_render, vec2(f_texcoord.x, f_texcoord.y - 1.0*blur)) * 0.1945945946;

    result += texture2D(fbo_render, vec2(f_texcoord.x, f_texcoord.y)) * 0.2270270270;

    result += texture2D(fbo_render, vec2(f_texcoord.x, f_texcoord.y + 1.0*blur)) * 0.1945945946;
    result += texture2D(fbo_render, vec2(f_texcoord.x, f_texcoord.y + 2.0*blur)) * 0.1216216216;
    result += texture2D(fbo_render, vec2(f_texcoord.x, f_texcoord.y + 3.0*blur)) * 0.0540540541;
    result += texture2D(fbo_render, vec2(f_texcoord.x, f_texcoord.y + 4.0*blur)) * 0.0162162162;

    //discard alpha for our simple demo, multiply by vertex color and return
    gl_FragColor = vec4(result.rgb, 1.0);
}
