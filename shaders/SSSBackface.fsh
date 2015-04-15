varying vec3 vNormal;
varying vec3 vPosition;

uniform int lCount;
uniform int ambient_on;
uniform int diffuse_on;
uniform int specular_on;
uniform int emission_on;

void main()
{
    gl_FragColor = vec4((vNormal.x+1)/2, (vNormal.y+1)/2, (vNormal.z+1)/2, 1.0);
}
