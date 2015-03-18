varying vec3 vNormal;
varying vec3 vPosition;

uniform int lCount;
uniform int ambient_on;
uniform int diffuse_on;
uniform int specular_on;
uniform int emission_on;

uniform float thickness;
uniform vec3 exctinction_coefficient;
uniform float rim_multiplier;

float scene_depth = 25;

vec4 lightSource( vec3 normal, vec3 position, gl_LightSourceParameters light )
{
        float attenuation = 1.0 - distance(light.position.xyz, position)/scene_depth;
        vec3 direction = normalize(light.position.xyz - position);
        vec3 eye = normalize(-position);
        vec3 reflection = normalize(-reflect(direction, normal));

        float NdotL = max(0.0, dot(normal, direction));
        float HLINdotL = 0.5 + dot(-normal,direction)*0.5;
        float NdotH = max(0.0, dot(reflection, eye));

        float Idiff = NdotL;
        float Ispec = pow( NdotH, gl_FrontMaterial.shininess );

        // Backlight coefficient multiplied by thickness of object.
        float Iback = (1.0 - thickness) * HLINdotL;

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

    vec4 lighting = vec4(0.0, 0.0, 0.0, 0.0);

    for(int i = 0; i<lCount; i++)
    {
        lighting += lightSource( normal, vPosition, gl_LightSource[i] );
    }

    return lighting;
}

void main()
{
    gl_FragData[0] = lighting();
    gl_FragData[1] = (gl_FrontMaterial.emission + gl_FrontMaterial.diffuse) * emission_on;
}
