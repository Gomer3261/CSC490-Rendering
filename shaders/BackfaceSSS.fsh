varying vec3 vNormal;
varying vec3 vPosition;
varying vec3 vView;

uniform int lCount;
uniform int ambient_on;
uniform int diffuse_on;
uniform int specular_on;
uniform int emission_on;

uniform int backface_width;
uniform int backface_height;
uniform sampler2D backface_texture;
uniform sampler2D backface_depth;

uniform float rim_multiplier;

float thickness = 0;
vec3 back_normal;

float radius = 50;

float nearPlane = 0.1f;
float farPlane = 25.0f;

vec4 lightSource( vec3 normal, vec3 back_normal, vec3 position, gl_LightSourceParameters light )
{
    float attenuation = 1.0 - distance(light.position.xyz, position)/farPlane;
    vec3 direction = normalize(light.position.xyz - position);
    vec3 eye = normalize(-position);
    vec3 reflection = normalize(-reflect(direction, normal));

    float NdotL = max(0.0, dot(normal, direction));
    float HLINdotL = 0.5 + dot(back_normal,direction)*0.5;
    float NdotH = max(0.0, dot(reflection, eye));

    float Idiff = NdotL;
    float Ispec = pow( NdotH, (1-gl_FrontMaterial.shininess)*1000 );

    // Backlight coefficient multiplied by thickness of object.
    float Iback = (1.0 - thickness)*(1.0 - thickness) * HLINdotL;

    // Hightlight rims to simulate light reflecting under surface.
    float Iedge = 1.0 - max(0.0,dot(normal,eye));
    Iedge *= Iedge * NdotL;

    return
            (gl_FrontMaterial.ambient * light.ambient * ambient_on) +
            (gl_FrontMaterial.diffuse * light.diffuse * Idiff * diffuse_on) +
            (gl_FrontMaterial.diffuse * Iback * light.diffuse * attenuation * gl_FrontMaterial.emission) +
            (gl_FrontMaterial.diffuse * Iedge * rim_multiplier * light.specular * gl_FrontMaterial.emission) +
            (gl_FrontMaterial.specular * light.specular * Ispec * specular_on);
}

vec4 lighting( void )
{
    // normal might be damaged by linear interpolation.
    vec3 normal = normalize( vNormal );

    vec4 lighting = vec4(0.0, 0.0, 0.0, 0.0);

    for(int i = 0; i<lCount; i++)
    {
        lighting += lightSource( normal, back_normal, vPosition, gl_LightSource[i] );
    }

    return lighting;
}

void main() {
    vec2 f_texcoord = vec2((vView.x+1.0)/2.0, (vView.y+1.0)/2.0);

    //this will be our RGBA sum
    vec4 texture = vec4(0.0);
    float backface_raw_depth = 0.0;
    float blur = (radius/backface_width)*(1-texture2D(backface_depth, f_texcoord).x);

    //apply blurring, using a 9 sample filter with predefined gaussian weights
    texture += texture2D(backface_texture, vec2(f_texcoord.x, f_texcoord.y - 4.0*blur)) * 0.0162162162;
    texture += texture2D(backface_texture, vec2(f_texcoord.x, f_texcoord.y - 3.0*blur)) * 0.0540540541;
    texture += texture2D(backface_texture, vec2(f_texcoord.x, f_texcoord.y - 2.0*blur)) * 0.1216216216;
    texture += texture2D(backface_texture, vec2(f_texcoord.x, f_texcoord.y - 1.0*blur)) * 0.1945945946;

    texture += texture2D(backface_texture, vec2(f_texcoord.x, f_texcoord.y)) * 0.2270270270;

    texture += texture2D(backface_texture, vec2(f_texcoord.x, f_texcoord.y + 1.0*blur)) * 0.1945945946;
    texture += texture2D(backface_texture, vec2(f_texcoord.x, f_texcoord.y + 2.0*blur)) * 0.1216216216;
    texture += texture2D(backface_texture, vec2(f_texcoord.x, f_texcoord.y + 3.0*blur)) * 0.0540540541;
    texture += texture2D(backface_texture, vec2(f_texcoord.x, f_texcoord.y + 4.0*blur)) * 0.0162162162;

    //texture = texture2D(backface_texture, vec2(f_texcoord.x, f_texcoord.y));

    float frag_depth = texture2D(backface_depth, vec2(f_texcoord.x, f_texcoord.y)).x;
    if(frag_depth != 1.0) {
        float sample_depth;

        sample_depth = texture2D(backface_depth, vec2(f_texcoord.x, f_texcoord.y - 4.0*blur)).x;
        backface_raw_depth += ((sample_depth >= 0.95) ? frag_depth : sample_depth) * 0.0162162162;
        sample_depth = texture2D(backface_depth, vec2(f_texcoord.x, f_texcoord.y - 3.0*blur)).x;
        backface_raw_depth += ((sample_depth >= 0.95) ? frag_depth : sample_depth) * 0.0540540541;
        sample_depth = texture2D(backface_depth, vec2(f_texcoord.x, f_texcoord.y - 2.0*blur)).x;
        backface_raw_depth += ((sample_depth >= 0.95) ? frag_depth : sample_depth) * 0.1216216216;
        sample_depth = texture2D(backface_depth, vec2(f_texcoord.x, f_texcoord.y - 1.0*blur)).x;
        backface_raw_depth += ((sample_depth >= 0.95) ? frag_depth : sample_depth) * 0.1945945946;

        backface_raw_depth += frag_depth * 0.2270270270;

        sample_depth = texture2D(backface_depth, vec2(f_texcoord.x, f_texcoord.y + 1.0*blur)).x;
        backface_raw_depth += ((sample_depth >= 0.95) ? frag_depth : sample_depth) * 0.1945945946;
        sample_depth = texture2D(backface_depth, vec2(f_texcoord.x, f_texcoord.y + 2.0*blur)).x;
        backface_raw_depth += ((sample_depth >= 0.95) ? frag_depth : sample_depth) * 0.1216216216;
        sample_depth = texture2D(backface_depth, vec2(f_texcoord.x, f_texcoord.y + 3.0*blur)).x;
        backface_raw_depth += ((sample_depth >= 0.95) ? frag_depth : sample_depth) * 0.0540540541;
        sample_depth = texture2D(backface_depth, vec2(f_texcoord.x, f_texcoord.y + 4.0*blur)).x;
        backface_raw_depth += ((sample_depth >= 0.95) ? frag_depth : sample_depth) * 0.0162162162;
    } else {
        backface_raw_depth = frag_depth;
    }

    //backface_raw_depth = frag_depth;

    back_normal = normalize(texture.rgb);

    float backface_depth_value = 1.0 - (2.0 * nearPlane) / (farPlane + nearPlane - backface_raw_depth * (farPlane - nearPlane));
    float frontface_depth_value = 1.0 - (2.0 * nearPlane) / (farPlane + nearPlane - ((vView.z+1.0)/2.0) * (farPlane - nearPlane));
    thickness = clamp(0.0, 1.0, (frontface_depth_value-backface_depth_value)*25);

    //gl_FragData[0] = vec4(depth_difference, depth_difference, depth_difference, 1.0);//texture2D(backface_texture, vec2((vView.x+1.0)/2.0, (vView.y+1.0)/2.0));
    gl_FragData[0] = lighting();
    gl_FragData[1] = (gl_FrontMaterial.emission + gl_FrontMaterial.diffuse) * emission_on;
}
