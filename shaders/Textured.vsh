varying vec3 vNormal;
varying vec3 vPosition;
varying vec3 vView;
varying vec4 tanget_vector;
uniform int lCount;

void main()
{
    vNormal = normalize( gl_NormalMatrix * gl_Normal );
    vPosition = vec3( gl_ModelViewMatrix * gl_Vertex );
    vView = ((gl_ModelViewProjectionMatrix * gl_Vertex).xyz)/(gl_ModelViewProjectionMatrix * gl_Vertex).w;
    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
