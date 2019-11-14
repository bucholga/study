#version 330 core

uniform sampler2D furMap;

uniform float tl;
uniform vec3  light;
uniform float time;

in vec2 tex;
in vec3 lt;
in vec3 ht;
in vec3 furDir;

out vec4 color;

void main () 
{
    vec4 c = texture ( furMap, 0.25*tex * vec2 ( 6.0, 12.0 ) );
    if ( c.a < 0.02 ) discard;
    vec3 t    = normalize ( furDir );
    float tl   = dot( normalize ( lt ), t );
    float th   = dot( normalize ( ht ), t );
    float spec = pow( 1.0 - th*th, 70.0 );
    color = vec4 ( c.rgb*sqrt(1.0-tl*tl) + c.rgb*vec3(spec),  c.a*0.1 ); 
}