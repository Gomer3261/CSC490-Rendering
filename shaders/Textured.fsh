varying vec3 vNormal;
varying vec3 vPosition;

uniform int lCount;
uniform int ambient_on;
uniform int diffuse_on;
uniform int specular_on;
uniform int emission_on;

uniform sampler2D diffuse_texture;
uniform sampler2D specular_texture;
uniform sampler2D normal_texture;

vec4 lightSource( vec3 normal, vec3 position, gl_LightSourceParameters light )
{
        vec3 direction = normalize(light.position.xyz - position);
        vec3 eye = normalize(-position);
        vec3 reflection = normalize(reflect(-direction, normal));

        float NdotL = max(0.0, dot(normal, direction));
        float NdotH = max(0.0, dot(reflection, eye));

        float Idiff = NdotL;
        float Ispec = pow( NdotH, (1-gl_FrontMaterial.shininess)*1000 );

        // 'real' shading
        return
                gl_FrontMaterial.emission * emission_on +
                (gl_FrontMaterial.ambient * light.ambient * ambient_on) +
                (vec4(texture2D(diffuse_texture, gl_TexCoord[0].xy).rgb, 1.0)  * light.diffuse * Idiff * diffuse_on) +
                (vec4(texture2D(specular_texture, gl_TexCoord[0].xy).rgb, 1.0) * light.specular * Ispec * specular_on);
}

vec4 lighting( void )
{
    vec4 lighting = vec4(0.0, 0.0, 0.0, 0.0);

    for(int i = 0; i<lCount; i++)
    {
        lighting += lightSource( normalize(vNormal + (texture2D(normal_texture, gl_TexCoord[0].xy).rgb*1.5 - vec3(0.75, 0.75, 0.75))), vPosition, gl_LightSource[i] );
    }

    return lighting;
}

void main()
{
    gl_FragData[0] = lighting();
    gl_FragData[1] = vec4(((gl_FrontMaterial.emission + gl_FrontMaterial.diffuse) * emission_on), gl_FrontMaterial.diffuse.a);
}
