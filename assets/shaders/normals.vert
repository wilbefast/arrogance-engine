void main(void)
{
  gl_Position = ftransform();
  vec3 N = (gl_Normal+vec3(1.))*0.5;
  gl_FrontColor = vec4(N, 1.);
}