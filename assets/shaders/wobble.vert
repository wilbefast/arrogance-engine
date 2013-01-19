uniform float time; 

void main(void)
{
  gl_Vertex.xyz += gl_Normal.xyz*abs(cos((time*72.*3.1415927)/180.))*0.2;
  gl_Position = ftransform();

  // shine light away from camera
  vec4 N = gl_ModelViewMatrix*vec4(gl_Normal, 0.);
  gl_FrontColor = vec4(vec3(abs(N.z)), 1.);
}