#version 460 core
out vec4 FragColor;

//uniform sampler2D texture1;
//uniform sampler2D texture2;
uniform vec3 color;

void main(){
    FragColor = vec4(color, 1.0);
    //mix(texture(texture1, TexCoord), texture(texture2, vec2(TexCoord.x, TexCoord.y)), 0.5) *
}