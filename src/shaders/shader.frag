// Shader
// Equirectangular Projection to Little Planets

uniform sampler2D textureUniform;
varying vec2 textureCoord;
const float PI = 3.141592653589793;
const float scale = 8.0;
const float aspect = -0.6;

void main(){
  vec2 rads = vec2(PI * 2.0, PI);
  vec2 pnt = (textureCoord - 0.5) * vec2(scale, scale * aspect);
  float x2y2 = pnt.x * pnt.x + pnt.y * pnt.y;
  vec3 sphere_pnt = vec3(2.0 * pnt, x2y2 - 1.0) / (x2y2 + 1.0);
  float r = length(sphere_pnt);
  float lon = atan(sphere_pnt.y, sphere_pnt.x);
  float lat = acos(sphere_pnt.z / r);
  gl_FragColor = texture2D(textureUniform, vec2(lon, lat) / rads);
}

