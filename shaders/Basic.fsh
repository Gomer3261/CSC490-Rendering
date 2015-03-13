varying vec4 vColor;
varying vec3 vNormal;
varying vec3 vPosition;

uniform int lCount;
uniform int ambient_on;
uniform int diffuse_on;
uniform int specular_on;
uniform int emission_on;

vec4 lightSource( vec3 normal, vec3 position, gl_LightSourceParameters light )
{
        vec3 direction = normalize(light.position.xyz - position);
        vec3 eye = normalize(-position);
        vec3 reflect = normalize(-reflect(direction, normal));

        float NdotL = max(0.0, dot(normal, direction));
        float NdotH = max(0.0, dot(reflect, eye));

        float Idiff = NdotL;
        float Ispec = pow( NdotH, gl_FrontMaterial.shininess );

        // 'real' shading
        return
                gl_FrontMaterial.emission * emission_on +
                (gl_FrontMaterial.ambient * light.ambient * ambient_on) +
                (gl_FrontMaterial.diffuse  * light.diffuse * Idiff * diffuse_on) +
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
    gl_FragColor = lighting();
}
