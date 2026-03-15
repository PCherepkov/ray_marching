#pragma once

#include <fstream>

#include "../../def.h"

/* common header file for shaders */

class shader {
public:
    enum uniform_types {
        VEC3, IVEC2, MAT4, FLT
    };

    string path;
    uint prg;
    shader() {}
    ~shader() {
    }

    bool operator==(shader& other) {
        return this->prg == other.prg;
    }

    void SetUniform(const string& name, uniform_types t, void* data) {
        glUseProgram(prg);
        uint loc = glGetUniformLocation(prg, name.c_str());
        switch (t)
        {
        case VEC3:
            glUniform3fv(loc, 1, value_ptr(*(vec3*)data));
            break;
        case IVEC2:
            glUniform2iv(loc, 1, value_ptr(*(ivec2*)data));
            break;
        case MAT4:
            glUniformMatrix4fv(loc, 1, false, value_ptr(*(mat4*)data));
            break;
        case FLT:
            glUniform1f(loc, *(flt*)data);
            break;
        default:
            break;
        }
        glUseProgram(0);
    }

    void ApplyVP(const mat4& view, const mat4& proj) {
        glUseProgram(prg);
        glUniformMatrix4fv(glGetUniformLocation(prg, "projection"), 1, GL_FALSE, value_ptr(proj));
        glUniformMatrix4fv(glGetUniformLocation(prg, "view"), 1, GL_FALSE, value_ptr(view));
        glUseProgram(0);
    }

    shader(const string& name) {
        path = name;

        /* loading data for shaders */
        ifstream
            f(name + "frag.glsl", ios::in),
            v(name + "vert.glsl", ios::in);
        // f.open(/*name + "../rnd/shd/DEFAULT/frag.glsl"*/ "tmp.txt");
        // v.open(/*name + "../rnd/shd/DEFAULT/vert.glsl"*/ "tmp.txt");

        if (f.fail() || v.fail()) {
            std::cout << "Error while shader source loading\n";
            std::cout << name;
            f.close();
            v.close();
            return;
        }

        uint vert_id, frag_id;

        string src;
        char buf[256];
        while (!f.eof()) {
            f.getline(buf, 256);
            src += buf;
            src += '\n';
        }

        char* fsrc = new char[src.size()];
        memcpy(fsrc, src.data(), src.size());
        fsrc[src.size() - 1] = 0;
        src.clear();

        while (!v.eof()) {
            v.getline(buf, 256);
            src += buf;
            src += '\n';
        }

        const char* vsrc;
        vsrc = src.data();

        f.close();
        v.close();

        /* compiling vertex shader */
        vert_id = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vert_id, 1, &vsrc, NULL);
        glCompileShader(vert_id);

        char log[512];

        /* vertex shader logs */
        int status;
        glGetShaderiv(vert_id, GL_COMPILE_STATUS, &status);
        if (!status) {
            glGetShaderInfoLog(vert_id, 512, NULL, log);
            cout << "vertex shader compilation error\n" << log << endl;
            return;
        }

        /* compiling fragment shader */
        frag_id = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(frag_id, 1, &fsrc, NULL);
        glCompileShader(frag_id);

        /* fragment shader logs */
        glGetShaderiv(frag_id, GL_COMPILE_STATUS, &status);
        if (!status) {
            glGetShaderInfoLog(frag_id, 512, NULL, log);
            cout << "fragment shader compilation error\n" << log << endl;
            return;
        }

        /* Shader program creation */
        prg = glCreateProgram();
        glAttachShader(prg, vert_id);
        glAttachShader(prg, frag_id);
        glLinkProgram(prg);

        /* linking logs */
        glGetProgramiv(prg, GL_LINK_STATUS, &status);
        if (!status) {
            glGetProgramInfoLog(prg, 512, NULL, log);
            cout << "linking error\n" << log << endl;
            return;
        }

        /* shader objects deleting */
        glDeleteShader(vert_id);
        glDeleteShader(frag_id);
        delete fsrc;
    }
};

/* END OF 'shaders.h' FILE */
