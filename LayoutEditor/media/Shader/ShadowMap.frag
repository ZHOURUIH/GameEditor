#version 100
precision mediump float;

uniform sampler2D uSampler;

varying vec2 oTexCoord;
varying float depth;

void main()
{
    vec4 aaa = texture2D(uSampler, oTexCoord);
//    if( aaa.a < 0.5 )
//       discard;
//    gl_FragColor = aaa;
    gl_FragColor = vec4(depth,depth,depth,1);
//  gl_FragColor = vec4(0.5,0.5,0.5,1);
}

