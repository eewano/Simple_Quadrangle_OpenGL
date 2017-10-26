//OpenGL3のシンタックスを使用
#version 330 core

//main.cppで3要素を使用する場合は、GLSLでも3要素を使用しないといけない
layout (location = 0) in vec3 position; //vec3: ３つの要素を持つベクトル layout(... :
layout (location = 1) in vec3 color;
//vec3: ３つの要素を持つベクトル
//layout(location = 0): position属性を与える為のバッファ(colorなら色のバッファ)

//ourColorとしてのアウトプットデータ
out vec3 ourColor;

void main() {
    //position: xyzを表す   1.0f: w(大きさの比率)
    gl_Position = vec4(position, 1.0f);
    //color: RGB これをourColorに代入
    ourColor = color;
}
