
uniform sampler2D fbo_depth;
uniform sampler2D fbo_texture;
uniform int fbo_width;
uniform int fbo_height;
varying vec2 f_texcoord;

float radius = 150;

void main() {
    //this will be our RGBA sum
    vec4 colour = vec4(0.0);
    float blur = (radius/fbo_width)*(1-texture2D(fbo_depth, f_texcoord).x);

    //apply blurring, using a 9 sample filter with predefined gaussian weights
    colour += texture2D(fbo_texture, vec2(f_texcoord.x - 4.0*blur, f_texcoord.y)) * 0.0162162162;
    colour += texture2D(fbo_texture, vec2(f_texcoord.x - 3.0*blur, f_texcoord.y)) * 0.0540540541;
    colour += texture2D(fbo_texture, vec2(f_texcoord.x - 2.0*blur, f_texcoord.y)) * 0.1216216216;
    colour += texture2D(fbo_texture, vec2(f_texcoord.x - 1.0*blur, f_texcoord.y)) * 0.1945945946;

    colour += texture2D(fbo_texture, f_texcoord) * 0.2270270270;

    colour += texture2D(fbo_texture, vec2(f_texcoord.x + 1.0*blur, f_texcoord.y)) * 0.1945945946;
    colour += texture2D(fbo_texture, vec2(f_texcoord.x + 2.0*blur, f_texcoord.y)) * 0.1216216216;
    colour += texture2D(fbo_texture, vec2(f_texcoord.x + 3.0*blur, f_texcoord.y)) * 0.0540540541;
    colour += texture2D(fbo_texture, vec2(f_texcoord.x + 4.0*blur, f_texcoord.y)) * 0.0162162162;

    gl_FragColor = colour;
    gl_FragDepth = texture2D(fbo_depth, f_texcoord).x;
}
