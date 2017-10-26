//OpenGL3のシンタックスを使用
#version 330 core

//ourColorとしてのインプットデータ
in vec3 ourColor;

//colorとしてのアウトプットデータ
out vec3 color;

void main() {
    //インプットデータをアウトプットデータに代入
    color = ourColor;
}
