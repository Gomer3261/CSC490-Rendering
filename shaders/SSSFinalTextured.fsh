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

uniform float rim_multiplier;

uniform sampler2D diffuse_texture;
uniform sampler2D specular_texture;
uniform sampler2D normal_texture;

float radius = 150;

float nearPlane = 0.1f;
float farPlane = 25.0f;

vec4 lightSource( vec3 normal, vec3 position, gl_LightSourceParameters light )
{
    float attenuation = 1.0 - distance(light.position.xyz, position)/farPlane;
    vec3 direction = normalize(light.position.xyz - position);
    vec3 eye = normalize(-position);
    vec3 reflection = normalize(-reflect(direction, normal));

    float NdotL = max(0.0, dot(normal, direction));
    float NdotH = max(0.0, dot(reflection, eye));

    float Idiff = NdotL;
    float Ispec = pow( NdotH, (1-texture2D(specular_texture, gl_TexCoord[0].xy).x*gl_FrontMaterial.shininess)*1000 );

    // Hightlight rims to simulate light reflecting under surface.
    float Iedge = 1.0 - max(0.0,dot(normal,eye));
    Iedge *= Iedge * NdotL;

    return
            (gl_FrontMaterial.ambient * light.ambient * ambient_on) +
            (vec4(texture2D(diffuse_texture, gl_TexCoord[0].xy).rgb, 1.0) * light.diffuse * Idiff * diffuse_on) +
            (vec4(texture2D(diffuse_texture, gl_TexCoord[0].xy).rgb, 1.0) * Iedge * rim_multiplier * light.specular * gl_FrontMaterial.emission) +
            (gl_FrontMaterial.specular * light.specular * Ispec * specular_on);
}

vec4 lighting( void )
{
    // normal might be damaged by linear interpolation.
    vec3 normal = normalize(vNormal + (texture2D(normal_texture, gl_TexCoord[0].xy).rgb*1.5 - vec3(0.75, 0.75, 0.75)));

    vec4 lighting = vec4(0.0, 0.0, 0.0, 0.0);

    for(int i = 0; i<lCount; i++)
    {
        lighting += lightSource( normal, vPosition, gl_LightSource[i] );
    }

    return lighting;
}

void main() {
    vec2 f_texcoord = vec2((vView.x+1.0)/2.0, (vView.y+1.0)/2.0);

    //this will be our RGBA sum
    vec4 colour = vec4(0.0);
    float blur = (radius/fbo_width)*(1-texture2D(fbo_depth, f_texcoord).x);

    //apply blurring, using a 9 sample filter with predefined gaussian weights
    colour += texture2D(fbo_texture, vec2(f_texcoord.x, f_texcoord.y - 4.0*blur)) * 0.0162162162;
    colour += texture2D(fbo_texture, vec2(f_texcoord.x, f_texcoord.y - 3.0*blur)) * 0.0540540541;
    colour += texture2D(fbo_texture, vec2(f_texcoord.x, f_texcoord.y - 2.0*blur)) * 0.1216216216;
    colour += texture2D(fbo_texture, vec2(f_texcoord.x, f_texcoord.y - 1.0*blur)) * 0.1945945946;

    colour += texture2D(fbo_texture, f_texcoord) * 0.2270270270;

    colour += texture2D(fbo_texture, vec2(f_texcoord.x, f_texcoord.y + 1.0*blur)) * 0.1945945946;
    colour += texture2D(fbo_texture, vec2(f_texcoord.x, f_texcoord.y + 2.0*blur)) * 0.1216216216;
    colour += texture2D(fbo_texture, vec2(f_texcoord.x, f_texcoord.y + 3.0*blur)) * 0.0540540541;
    colour += texture2D(fbo_texture, vec2(f_texcoord.x, f_texcoord.y + 4.0*blur)) * 0.0162162162;

    gl_FragData[0] = lighting() + colour;
    gl_FragData[1] = vec4(0.0, 0.0, 0.0, 1.0);
}
