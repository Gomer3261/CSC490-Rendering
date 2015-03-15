varying vec3 vNormal;
varying vec3 vPosition;
uniform int lCount;

void main()
{
    vNormal = normalize( gl_NormalMatrix * gl_Normal );
    vPosition = vec3( gl_ModelViewMatrix * gl_Vertex );
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
