#version 150
out vec4 fragColor;

uniform sampler2D src_tex_unit0;
uniform float usingTexture;
uniform float usingColors;
uniform vec4 globalColor;

in float depth;
in vec4 colorVarying;
in vec2 texCoordVarying;


void main(){
    fragColor = texture(src_tex_unit0, gl_PointCoord) * colorVarying;
}