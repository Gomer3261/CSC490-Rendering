uniform sampler2D fbo_depth;
uniform sampler2D fbo_render;
uniform int fbo_width;
uniform int fbo_height;
varying vec2 f_texcoord;

#define PI    3.14159265

vec2 texture_pixel = vec2(1.0/fbo_width,1.0/fbo_height); // Texel

int samples = 4; //samples grow exponentially per ring (3*5^n)
int rings = 10; //ring count

float threshold = 0.4; //highlight threshold;
float gain = 8.0; //highlight gain;

float bias = 0.7; //bokeh edge bias
float fringe = 0.7; //bokeh chromatic aberration/fringing

float blur = 3.0;
vec3 luminocity_coeff = vec3(0.299,0.587,0.114);

float noise_multiplier = 0.00001;

// Generating noise/pattern texture for dithering
vec2 rand(vec2 coord)
{
    float noise_x = clamp(fract(sin(dot(coord ,vec2(12.9898,78.233))) * 43758.5453),0.0,1.0)*2.0-1.0;
    float noise_y = clamp(fract(sin(dot(coord ,vec2(12.9898,78.233)*2.0)) * 43758.5453),0.0,1.0)*2.0-1.0;
    return vec2(noise_x,noise_y)*noise_multiplier;
}

vec3 color(vec2 coords, float blur) //processing the sample
{
    vec3 ret_color = vec3(0.0);

    // Split the color smaples to simulate chromatic aberration.
    ret_color.r = texture2D(fbo_render, coords + vec2(0.0,1.0)*texture_pixel*fringe*blur).r;
    ret_color.g = texture2D(fbo_render, coords + vec2(-0.866,-0.5)*texture_pixel*fringe*blur).g;
    ret_color.b = texture2D(fbo_render, coords + vec2(0.866,-0.5)*texture_pixel*fringe*blur).b;

    // Adjusting color for luminance (Brightness adjusted to the way human eyes percieve it)
    float luminocity = dot(ret_color.rgb, luminocity_coeff);
    float thresh = max((luminocity - threshold)*gain, 0.0);
    return ret_color + mix(vec3(0.0), ret_color, thresh*blur);
}

void main()
{
  vec2 noise = rand(f_texcoord.xy);

  // Add a little noise to our bluring.
  float blur_width = (texture_pixel.x+noise.x)*blur;
  float blur_height = (texture_pixel.y+noise.y)*blur;

  // Use a full strength sample from the fbo.
  vec3 result = texture2D(fbo_render, f_texcoord.xy).rgb;
  float strength = 1.0;

  int ring_samples;

  for (int i = 1; i <= rings; i += 1)
  {
    ring_samples = i * samples;

    for (int j = 0 ; j < ring_samples ; j += 1)
    {
        // Circular sampling, number of samples increases with each ring.
        float circumference_step = PI*2.0 / float(ring_samples);
        float offset_x = (cos(float(j)*circumference_step)*float(i));
        float offset_y = (sin(float(j)*circumference_step)*float(i));

        // outer rings affect the resulting pixel more.
        float sample_strength = mix(1.0, i/(float(rings)), bias);

        // strength adjusted sample results, and weight. (For normalizing later)
        result += color(f_texcoord.xy + vec2(offset_x*blur_width,offset_y*blur_height), blur) * sample_strength;
        strength += sample_strength;
    }
  }

  // Normalize Result
  result /= strength;

  // Draw
  gl_FragColor = vec4(result, 1.0);
}
