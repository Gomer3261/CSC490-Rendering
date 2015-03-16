varying vec3 vNormal;
varying vec3 vPosition;

uniform int diffuse_on;
uniform int emission_on;

void main()
{
    gl_FragColor = (gl_FrontMaterial.emission + gl_FrontMaterial.diffuse) * emission_on; // + (gl_FrontMaterial.diffuse * diffuse_on * emission_on);
}
