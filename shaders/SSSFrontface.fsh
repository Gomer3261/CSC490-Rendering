varying vec3 vNormal;
varying vec3 vPosition;
varying vec3 vView;

uniform int lCount;
uniform int ambient_on;
uniform int diffuse_on;
uniform int specular_on;
uniform int emission_on;

uniform int fbo_width;
uniform int fbo_height;
uniform sampler2D fbo_texture;
uniform sampler2D fbo_depth;

float nearPlane = 0.1f;
float farPlane = 25.0f;

vec4 lightSource( vec3 back_normal, float thickness, vec3 position, gl_LightSourceParameters light )
{
    float attenuation = 1.0 - distance(light.position.xyz, position)/farPlane;
    vec3 direction = normalize(light.position.xyz - position);

    float HLINdotL = 0.5 + dot(back_normal,direction)*0.5;

    // Backlight coefficient multiplied by thickness of object.
    float Iback = (1.0 - thickness)*(1.0 - thickness) * HLINdotL;

    return gl_FrontMaterial.diffuse * Iback * light.diffuse * attenuation * gl_FrontMaterial.emission;
}

vec4 lighting( vec3 back_normal, float thickness )
{
    vec4 lighting = vec4(0.0, 0.0, 0.0, 1.0);

    for(int i = 0; i<lCount; i++)
    {
        lighting += lightSource( back_normal, thickness, vPosition, gl_LightSource[i] );
    }

    return lighting;
}

void main() {
    vec2 f_texcoord = vec2((vView.x+1.0)/2.0, (vView.y+1.0)/2.0);

    vec4 backface_texture = texture2D(fbo_texture, f_texcoord);

    float frag_depth = texture2D(fbo_depth, f_texcoord).x;
    vec3 back_normal = normalize(backface_texture.rgb).xyz;

    float backface_depth_value = 1.0 - (2.0 * nearPlane) / (farPlane + nearPlane - frag_depth * (farPlane - nearPlane));
    float frontface_depth_value = 1.0 - (2.0 * nearPlane) / (farPlane + nearPlane - ((vView.z+1.0)/2.0) * (farPlane - nearPlane));
    float thickness = clamp(0.0, 1.0, (frontface_depth_value-backface_depth_value)*farPlane);

    gl_FragColor = lighting(back_normal, thickness);
}
