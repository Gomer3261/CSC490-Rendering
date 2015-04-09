uniform sampler2D fbo_depth;
uniform sampler2D fbo_render;
uniform int fbo_width;
uniform int fbo_height;
varying vec2 f_texcoord;

uniform float focal_point;

vec2 texture_pixel = vec2(1.0/fbo_width,1.0/fbo_height); // Texel

// DOF
float range = 5.0; // Focal Range
float radius_max = 4; // Prevent obscene blur.

// Depth sample bluring for smoother edges.
float depth_blur_size = 4.0;

float blur_depth(vec2 coords) //blurring depth
{
    float depth = 0.0;
    float sample_strength[9];
    vec2  sample_offset[9];

    vec2 blur_area = vec2(texture_pixel.x, texture_pixel.y) * depth_blur_size;

    // simple 9 sample blur
    sample_offset[0] = vec2( -blur_area.x, -blur_area.y);
    sample_offset[1] = vec2( 0.0,          -blur_area.y);
    sample_offset[2] = vec2( blur_area.x   -blur_area.y);

    sample_offset[3] = vec2( -blur_area.x, 0.0);
    sample_offset[4] = vec2( 0.0,          0.0);
    sample_offset[5] = vec2( blur_area.x,  0.0);

    sample_offset[6] = vec2( -blur_area.x, blur_area.y);
    sample_offset[7] = vec2( 0.0,          blur_area.y);
    sample_offset[8] = vec2( blur_area.x,  blur_area.y);

    // different strengths for each sample.
    // Total is 16 strength, allows for hard coding.
    sample_strength[0] = 1.0/16.0;   sample_strength[1] = 2.0/16.0; sample_strength[2] = 1.0/16.0;
    sample_strength[3] = 2.0/16.0;   sample_strength[4] = 4.0/16.0; sample_strength[5] = 2.0/16.0;
    sample_strength[6] = 1.0/16.0;   sample_strength[7] = 2.0/16.0; sample_strength[8] = 1.0/16.0;

    // Add samples and strengths together and merge into depth.
    for( int i = 0; i < 9; i++ )
    {
          depth += texture2D(fbo_depth, coords + sample_offset[i]).x * sample_strength[i];
    }
    return depth;
}

void main() {
    float depth = blur_depth(f_texcoord.xy);
    float radius = clamp((abs(depth - focal_point)/range)*30,-radius_max,radius_max);

    //this will be our RGBA sum
    vec4 result = vec4(0.0);
    float blur = radius/fbo_width;

    //apply blurring, using a 9-tap filter with predefined gaussian weights
    result += texture2D(fbo_render, vec2(f_texcoord.x - 4.0*blur, f_texcoord.y)) * 0.0162162162;
    result += texture2D(fbo_render, vec2(f_texcoord.x - 3.0*blur, f_texcoord.y)) * 0.0540540541;
    result += texture2D(fbo_render, vec2(f_texcoord.x - 2.0*blur, f_texcoord.y)) * 0.1216216216;
    result += texture2D(fbo_render, vec2(f_texcoord.x - 1.0*blur, f_texcoord.y)) * 0.1945945946;

    result += texture2D(fbo_render, vec2(f_texcoord.x, f_texcoord.y)) * 0.2270270270;

    result += texture2D(fbo_render, vec2(f_texcoord.x + 1.0*blur, f_texcoord.y)) * 0.1945945946;
    result += texture2D(fbo_render, vec2(f_texcoord.x + 2.0*blur, f_texcoord.y)) * 0.1216216216;
    result += texture2D(fbo_render, vec2(f_texcoord.x + 3.0*blur, f_texcoord.y)) * 0.0540540541;
    result += texture2D(fbo_render, vec2(f_texcoord.x + 4.0*blur, f_texcoord.y)) * 0.0162162162;

    //discard alpha for our simple demo, multiply by vertex color and return
    gl_FragColor = vec4(result.rgb, 1.0);
}
