uniform sampler2D backface_depth;
uniform sampler2D backface_texture;
uniform int backface_width;
uniform int backface_height;
varying vec2 f_texcoord;

float radius = 50;

void main() {
    //this will be our RGBA sum
    vec4 texture = vec4(0.0);
    float depth = 0.0;
    float blur = (radius/backface_width)*(1-texture2D(backface_depth, f_texcoord).x);

    //apply blurring, using a 9 sample filter with predefined gaussian weights
    texture += texture2D(backface_texture, vec2(f_texcoord.x - 4.0*blur, f_texcoord.y)) * 0.0162162162;
    texture += texture2D(backface_texture, vec2(f_texcoord.x - 3.0*blur, f_texcoord.y)) * 0.0540540541;
    texture += texture2D(backface_texture, vec2(f_texcoord.x - 2.0*blur, f_texcoord.y)) * 0.1216216216;
    texture += texture2D(backface_texture, vec2(f_texcoord.x - 1.0*blur, f_texcoord.y)) * 0.1945945946;

    texture += texture2D(backface_texture, vec2(f_texcoord.x, f_texcoord.y)) * 0.2270270270;

    texture += texture2D(backface_texture, vec2(f_texcoord.x + 1.0*blur, f_texcoord.y)) * 0.1945945946;
    texture += texture2D(backface_texture, vec2(f_texcoord.x + 2.0*blur, f_texcoord.y)) * 0.1216216216;
    texture += texture2D(backface_texture, vec2(f_texcoord.x + 3.0*blur, f_texcoord.y)) * 0.0540540541;
    texture += texture2D(backface_texture, vec2(f_texcoord.x + 4.0*blur, f_texcoord.y)) * 0.0162162162;

    float frag_depth = texture2D(backface_depth, vec2(f_texcoord.x, f_texcoord.y)).x;
    float sample_depth;

    if(frag_depth != 1.0) {
        sample_depth = texture2D(backface_depth, vec2(f_texcoord.x - 4.0*blur, f_texcoord.y)).x;
        depth += ((sample_depth >= 0.95) ? frag_depth : sample_depth) * 0.0162162162;
        sample_depth = texture2D(backface_depth, vec2(f_texcoord.x - 3.0*blur, f_texcoord.y)).x;
        depth += ((sample_depth >= 0.95) ? frag_depth : sample_depth) * 0.0540540541;
        sample_depth = texture2D(backface_depth, vec2(f_texcoord.x - 2.0*blur, f_texcoord.y)).x;
        depth += ((sample_depth >= 0.95) ? frag_depth : sample_depth) * 0.1216216216;
        sample_depth = texture2D(backface_depth, vec2(f_texcoord.x - 1.0*blur, f_texcoord.y)).x;
        depth += ((sample_depth >= 0.95) ? frag_depth : sample_depth) * 0.1945945946;

        depth += frag_depth * 0.2270270270;

        sample_depth = texture2D(backface_depth, vec2(f_texcoord.x + 1.0*blur, f_texcoord.y)).x;
        depth += ((sample_depth >= 0.95) ? frag_depth : sample_depth) * 0.1945945946;
        sample_depth = texture2D(backface_depth, vec2(f_texcoord.x + 2.0*blur, f_texcoord.y)).x;
        depth += ((sample_depth >= 0.95) ? frag_depth : sample_depth) * 0.1216216216;
        sample_depth = texture2D(backface_depth, vec2(f_texcoord.x + 3.0*blur, f_texcoord.y)).x;
        depth += ((sample_depth >= 0.95) ? frag_depth : sample_depth) * 0.0540540541;
        sample_depth = texture2D(backface_depth, vec2(f_texcoord.x + 4.0*blur, f_texcoord.y)).x;
        depth += ((sample_depth >= 0.95) ? frag_depth : sample_depth) * 0.0162162162;
    } else {
        depth = frag_depth;
    }

    //texture2D(backface_texture, vec2(f_texcoord.x, f_texcoord.y));
    gl_FragColor = vec4(normalize(texture.rgb), 1.0);
    //frag_depth;
    gl_FragDepth = depth;
}
