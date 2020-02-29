precision mediump float;

///////////////////////////////////////////////////////////
// Uniforms
uniform sampler2D SamplerY;
uniform sampler2D SamplerUV;

uniform sampler2D u_diffuseTexture;

///////////////////////////////////////////////////////////
// Varyings
varying vec2 v_texCoord;

void main()
{
    mediump vec3 yuv;
    lowp vec3 rgb;
    yuv.x = texture2D(SamplerY, v_texCoord).r;
    yuv.yz = texture2D(SamplerUV, v_texCoord).ar - vec2(0.5, 0.5);
    rgb = mat3(      1,       1,      1,
                     0, -.18732, 1.8556,
               1.57481, -.46813,      0) * yuv;
    gl_FragColor = vec4(rgb, 1);
}
