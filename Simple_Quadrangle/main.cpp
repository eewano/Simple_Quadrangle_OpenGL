#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Utility.h"
#include "Shader.h"

struct Vec2 {
    GLint x, y;
};

const Vec2 WINDOW_SIZE = {800, 600};

GLFWwindow *window;

int main(int argc, const char *argv[]) {

    //当プロジェクトの作業ディレクトリを表示
    std::cout << "Current directory is " << GetCurrentWorkingDir().c_str() << ".\n";

    //GLFWの初期化(失敗したら終了)
    if (!glfwInit()) {
        return EXIT_FAILURE;
    }

    //GLFWにて要求されるオプションのセット
    //(window生成時に一緒に作成されるコンテキストやスワップチェイン、深度バッファの情報等の指定)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Macの際には必要
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    //GLFWの機能を用いて、GLFWwindowオブジェクトを生成
    window = glfwCreateWindow(WINDOW_SIZE.x, WINDOW_SIZE.y, "Simple_Quadrangle", nullptr, nullptr);

    Vec2 screen;
    //glViewportで必要とされるフレームバッファのサイズを取得
    glfwGetFramebufferSize(window, &screen.x, &screen.y);

    //window生成失敗時のプログラムの終了処理
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window.\n";
        glfwTerminate();
        return EXIT_FAILURE;
    }

    //エラー時に呼ばれるコールバックを設定
    glfwSetErrorCallback(ErrorCallback);

    //windowのコンテキストをカレントにする
    glfwMakeContextCurrent(window);

    //最新の方法でポインターや拡張機能を検索し、glewを有効にする(コアプロファイルで必要)
    glewExperimental = GL_TRUE;

    //GLEWの初期化
    if (GLEW_OK != glewInit()) {
        std::cout << "Failed to initialize GLEW.\n";
        return -1;
    }

    //ビューポートの定義(ウィンドウの左下隅が原点で、そこからx,yの数値分が左下の原点座標となる)
    glViewport(0, 0, screen.x, screen.y);

    //各シェーダーglslファイルの読み込み
    Shader shader("resources/VertexShader.glsl", "resources/FragmentShader.glsl");

    GLuint VAO; //VAO: Vertex Array Object -> 描画に用いるバッファやバッファ設定の集まり
    GLuint VBO; //VBO: Vertex Buffer Objext -> 描画に用いるバッファ(データ)

    //VAO(バッファデータや設定)の生成
    glGenVertexArrays(1, &VAO);
    //VBO(バッファ)の生成
    glGenBuffers(1, &VBO);
    //現在のVAOに設定
    glBindVertexArray(VAO);
    //現在のVBOに設定(GL_ARRAY_BUFFER: 頂点データやTexture座標のバッファ)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //Position属性
    glVertexAttribPointer(
            0, //属性No. 0である必要はないが、VertexShaderファイル内のlayout location(position)と一致していないと駄目
            3, //VertexShaderファイルのpositionのデータ型がvec3(3要素)なので3
            GL_FLOAT, //タイプ
            GL_FALSE, //データ型が整数型であった時にそれを[0, 1]又は[-1, 1]の範囲に正規化するか否(GL_FALSE)かを指定
            6 * sizeof(GLfloat), //頂点情報とその間隔を指定
            (GLvoid *) 0 //頂点情報を格納している領域の先頭の位置を指定。ここでは頂点バッファオブジェクトの先頭から格納されているので0を指定
    );
    //attribute変数(属性No.0なので0)に頂点情報を対応付ける
    glEnableVertexAttribArray(0);

    //Color属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);



    //ここで、escキーが押されたらウィンドウを閉じる処理を入れておく
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0) {
        //キーが押されたりマウスを操作した等、該当する応答機能が何かしら作動したかどうかをチェックする
        glfwPollEvents();

        //glViewportで指定した範囲をこれで指定した色で塗り潰す
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //指定したバッファを特定の色で消去する
        glClear(GL_COLOR_BUFFER_BIT);

        for (GLint i = 0; i < 9; i++) {
            //多用する i * 0.1f をcalcでまとめる
            GLfloat calc = i * 0.1f;

            if (i % 2 == 0) {
                GLfloat vertices[] = {
                        //x, y, z, R, G, B
                        0.9f - calc, 0.9f - calc, 0.0f, 0.6f, 0.6f, 0.6f, //四角形の右上の座標
                        0.9f - calc, -0.9f + calc, 0.0f, 0.0f, 0.0f, 0.0f, //右下
                        -0.9f + calc, -0.9f + calc, 0.0f, 0.0f, 0.0f, 0.7f, //左下
                        -0.9f + calc, 0.9f - calc, 0.0f, 1.0f, 1.0f, 0.0f, //左上
                };

                //現在のバッファにデータを送信
                glBufferData(
                        GL_ARRAY_BUFFER,
                        sizeof(vertices), //アップロードするデータのバイト数
                        vertices, //アップロードするデータのポイントの設定
                        GL_STATIC_DRAW //データの利用方法(GL_STATIC_DRAW: 一度だけアップロードする)
                );

                //設定したシェーダーの使用
                shader.Use();
                //VAOの有効化(VAOに割り当てた設定とバッファが復元される)
                glBindVertexArray(VAO);
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
                //VAOの無効化
                glBindVertexArray(0);

            } else {
                //背景色と同じ四角形の集まり
                GLfloat vertices[] = {
                        0.9f - calc, 0.9f - calc, 0.0f, 0.2f, 0.3f, 0.3f, //Top Right
                        0.9f - calc, -0.9f + calc, 0.0f, 0.2f, 0.3f, 0.3f, //Bottom Right
                        -0.9f + calc, -0.9f + calc, 0.0f, 0.2f, 0.3f, 0.3f, //Bottom Left
                        -0.9f + calc, 0.9f - calc, 0.0f, 0.2f, 0.3f, 0.3f, //Top Left
                };

                glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

                shader.Use();
                glBindVertexArray(VAO);
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
                glBindVertexArray(0);

            }
        }

        //スクリーンバッファの交換(ダブルバッファのスワップ)
        glfwSwapBuffers(window);
    }

    //arraysで指定された頂点配列オブジェクト(VAO)を削除
    glDeleteVertexArrays(1, &VAO);
    //使用済みのバッファオブジェクトの削除
    glDeleteBuffers(1, &VBO);

    //GLFW使用の際はプログラムの最後にこれを呼び出し、全てのウィンドウの破棄及びリソースの解放を行なう。
    //これを呼び出した後に再度GLFW機能を利用する場合は改めてglfwInitを呼び出す必要がある。
    glfwTerminate();

    return 0;
}
