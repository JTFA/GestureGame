precision mediump float;

///////////////////////////////////////////////////////////
// Uniforms
uniform sampler2D u_diffuseTexture;

///////////////////////////////////////////////////////////
// Varyings
varying vec2 v_texCoord;

void main()
{
    gl_FragColor = texture2D(u_diffuseTexture, v_texCoord);
}
