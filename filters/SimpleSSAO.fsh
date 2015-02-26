uniform sampler2D fbo_depth;
uniform sampler2D fbo_render;
uniform int fbo_width;
uniform int fbo_height;
varying vec2 f_texcoord;

#define PI    3.14159265

float nearPlane = 0.1; //Z-near
float farPlane = 25.0; //Z-far

bool use_noise = true;
float noise_multiplier = 0.0005;

float ao_radius = 3.0;
float ao_clamp = 0.25; //depth clamp - reduces haloing at screen edges

float gauss_center = 0.4; //gauss bell center
float gauss_width = 2.0; //gauss bell width
float gauss_width_reduced = 0.4; // Reduce self shadowing

int samples = 16;

vec2 rand(vec2 coord) //generating noise/pattern texture for dithering
{
  float noise_x = ((fract(1.0-coord.x*(fbo_width/2.0))*0.25)+(fract(coord.y*(fbo_height/2.0))*0.75))*2.0-1.0;
  float noise_y = ((fract(1.0-coord.x*(fbo_width/2.0))*0.75)+(fract(coord.y*(fbo_height/2.0))*0.25))*2.0-1.0;

  if (use_noise)
  {
    noise_x = clamp(fract(sin(dot(coord ,vec2(12.9898,78.233))) * 43758.5453),0.0,1.0)*2.0-1.0;
    noise_y = clamp(fract(sin(dot(coord ,vec2(12.9898,78.233)*2.0)) * 43758.5453),0.0,1.0)*2.0-1.0;
  }
  return vec2(noise_x,noise_y)*noise_multiplier;
}

float readDepth(in vec2 coord)
{
    if (coord.x<0||coord.y<0||coord.x>1||coord.y>1) return 1.0;
    return (2.0 * nearPlane) / (nearPlane + farPlane - texture2D(fbo_depth, coord).x * (farPlane - nearPlane));
}

float compareDepths(float depth1, float depth2, inout int far)
{
    float gauss = 0.0;
    float depth_difference = (depth1 - depth2)*100.0; //depth difference (0-100) ?? +/-?

    if (depth_difference<gauss_center) // Sampled depth is either very close or behind us.
    {
        gauss_width = gauss_width_reduced;
    } else {
        far = 1;
    }

    gauss = pow(2.7182,-2*(depth_difference-gauss_center)*(depth_difference-gauss_center)/(gauss_width*gauss_width));

    return gauss;
}

float calAO(float depth, float offset_x, float offset_y)
{
    float depth_multiplier = (1.0-depth)*ao_radius;

    vec2 coord = f_texcoord + (vec2(offset_x, offset_y) * depth_multiplier);
    vec2 coord2 = f_texcoord - (vec2(offset_x, offset_y) * depth_multiplier);

    int far = 0;
    float ao = compareDepths(depth, readDepth(coord), far);
    //DEPTH EXTRAPOLATION:
    if (far != 0)
    {
        ao += (1.0-ao)*compareDepths(readDepth(coord2), depth, far);
    }
    return ao;
}

void main(void)
{
    // noise generation:
    vec2 random = rand(f_texcoord);

    // initialize screen data:
    float depth = readDepth(f_texcoord);
    float ao_width = (1.0 / fbo_width)/clamp(depth,ao_clamp,1.0)+(random.x*(1.0-random.x)); // Noisily reduce scale of ao sampling
    float ao_height = (1.0 / fbo_height)/clamp(depth,ao_clamp,1.0)+(random.y*(1.0-random.y)); // Noisily reduce scale of ao sampling

    // initialize sampling variables:
    float ao = 0.0;

    float change_angle = PI*(3.0-sqrt(5.0)); // approx .7? Assuming radians.
    float change_radius_sq = 1.0/float(samples+1);
    float angle = 0.0;
    float radius_sq = 0.0 + change_radius_sq/2.0; // Changes from ~0 to ~1

    for(int i=0; i<samples; ++i)
    {
        // Leans towards lower values
        float radius = sqrt(radius_sq);

        float offset_x = cos(angle)*radius;
        float offset_y = sin(angle)*radius;

        ao += calAO(depth, offset_x*ao_width, offset_y*ao_height);

        radius_sq = radius_sq + change_radius_sq;
        angle = angle + change_angle;
    }

    //final values, some adjusting:
    vec3 final_ao = vec3(1.0-(ao/samples));

    //gl_FragColor = vec4(texture2D(fbo_render, f_texcoord).rgb*final_ao+final_gi, 1.0);

    gl_FragColor = vec4(texture2D(fbo_render, f_texcoord).rgb*vec3(final_ao), 1.0);
}
