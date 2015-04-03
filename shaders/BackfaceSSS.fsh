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

uniform vec3 exctinction_coefficient;
uniform float rim_multiplier;

float thickness = 0;

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
    float Ispec = pow( NdotH, gl_FrontMaterial.shininess );

    // Backlight coefficient multiplied by thickness of object.
    float Iback = (1.0 - thickness)*(1.0 - thickness) * HLINdotL;

    // Hightlight rims to simulate light reflecting under surface.
    float Iedge = 1.0 - max(0.0,dot(normal,eye));
    Iedge *= Iedge * NdotL;

    return
            gl_FrontMaterial.emission * emission_on +
            (gl_FrontMaterial.ambient * light.ambient * ambient_on) +
            (gl_FrontMaterial.diffuse * light.diffuse * Idiff * diffuse_on) +
            (gl_FrontMaterial.diffuse * Iback * light.diffuse * attenuation * vec4(exctinction_coefficient, 1.0)) +
            (gl_FrontMaterial.diffuse * Iedge * rim_multiplier * light.specular * vec4(exctinction_coefficient, 1.0)) +
            (gl_FrontMaterial.specular * light.specular * Ispec * specular_on);
}

vec4 lighting( void )
{
    // normal might be damaged by linear interpolation.
    vec3 normal = normalize( vNormal );
    vec3 back_normal = normalize( texture2D(backface_texture, vec2((vView.x+1.0)/2.0, (vView.y+1.0)/2.0)).xyz );

    vec4 lighting = vec4(0.0, 0.0, 0.0, 0.0);

    for(int i = 0; i<lCount; i++)
    {
        lighting += lightSource( normal, back_normal, vPosition, gl_LightSource[i] );
    }

    return lighting;
}

void main()
{
    float backface_depth = 1.0 - (2.0 * nearPlane) / (farPlane + nearPlane - texture2D(backface_depth, vec2((vView.x+1.0)/2.0, (vView.y+1.0)/2.0)).x * (farPlane - nearPlane));
    float frontface_depth = 1.0 - (2.0 * nearPlane) / (farPlane + nearPlane - ((vView.z+1.0)/2.0) * (farPlane - nearPlane));
    thickness = clamp(0.0, 1.0, (frontface_depth-backface_depth)*25);

    //gl_FragData[0] = vec4(depth_difference, depth_difference, depth_difference, 1.0);//texture2D(backface_texture, vec2((vView.x+1.0)/2.0, (vView.y+1.0)/2.0));
    gl_FragData[0] = lighting();
    gl_FragData[1] = (gl_FrontMaterial.emission + gl_FrontMaterial.diffuse) * emission_on;
}
