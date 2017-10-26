#ifndef Shader_h
#define Shader_h

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

class Shader {
public:
    GLuint m_Program;

    //シェーダーを生成するコンストラクタ
    Shader(const GLchar *vertexPath, const GLchar *fragmentPath) {
        //1: ファイルのパス名からvertex or fragment のコードを検索する
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        //例外を飛ばせるよう、ifstreamオブジェクトを確保する
        vShaderFile.exceptions(std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::badbit);
        try {
            //ファイルオープン
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            //ファイルのbufferの中身をstreamに読み込む
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            //ファイルをクローズする
            vShaderFile.close();
            fShaderFile.close();
            //streamをstringに変換する
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        } catch (std::ifstream::failure e) {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }

        const GLchar *vShaderCode = vertexCode.c_str();
        const GLchar *fShaderCode = fragmentCode.c_str();
        //2: シェーダーの編集
        GLuint vertex, fragment;
        GLint success;
        GLchar infoLog[512];

        //Vertex Shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, nullptr);
        glCompileShader(vertex);
        //コンパイルエラーの出力
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
            std::cout << "ERROE::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        //Fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, nullptr);
        glCompileShader(fragment);
        //コンパイルエラーの出力
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        //Shader Program
        this->m_Program = glCreateProgram();
        glAttachShader(this->m_Program, vertex);
        glAttachShader(this->m_Program, fragment);
        glLinkProgram(this->m_Program);
        //リンクエラーの出力
        glGetProgramiv(this->m_Program, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(this->m_Program, 512, nullptr, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_RAILED\n" << infoLog << std::endl;
        }

        //プログラム内にリンクされたので、不要となったシェーダーを削除する
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    //設定したシェーダーの使用
    void Use() {
        glUseProgram(this->m_Program);
    }
};


#endif /* Shader_h */
