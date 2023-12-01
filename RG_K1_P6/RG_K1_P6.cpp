//Autor: Nemanja Simsic
//Opis: Primjer upotrebe tekstura

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned int compileShader(GLenum type, const char* source);
unsigned int createShader(const char* vsSource, const char* fsSource);
static unsigned loadImageToTexture(const char* filePath); //Ucitavanje teksture, izdvojeno u funkciju

int main(void)
{

    if (!glfwInit())
    {
        std::cout << "GLFW Biblioteka se nije ucitala! :(\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;
    unsigned int wWidth = 800;
    unsigned int wHeight = 800;
    const char wTitle[] = "[Generic Title]";
    window = glfwCreateWindow(wWidth, wHeight, wTitle, NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Prozor nije napravljen! :(\n";
        glfwTerminate();
        return 2;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW nije mogao da se ucita! :'(\n";
        return 3;
    }

    unsigned int unifiedShader = createShader("basic.vert", "basic.frag");

    float vertices[] =
    {   //X    Y      S    T 
        1.0, 1.0,    20.0, 20.0,
        -1.0, 1.0,   0.0, 20.0,
        1.0, -1.0,   20.0, 0.0,
        -1.0, -1.0,  0.0, 0.0
    };

    float vertices_signature[] =
    {   //X    Y      S    T 
        1.0, -0.8,   1.0, 1.0,
        0.5, -0.8,   0.0, 1.0,
        1.0, -1.0,  1.0, 0.0,
        0.5, -1.0,  0.0, 0.0,
    };

    float vertices_work1[] =
    {   //X    Y      S    T 
        -0.4, 0.5,   1.0, 1.0,
        -0.8, 0.5,   0.0, 1.0, 
        -0.4, -0.5,  1.0, 0.0,
        -0.8, -0.5,  0.0, 0.0,
    };

    float frameWidth = 0.05;

    // Additional vertices for the frame
    float frameVertices1[] = {
        // Top frame
        -0.4 + frameWidth, 0.5 + frameWidth, 1.0, 1.0,

        // Left frame
        -0.8 - frameWidth, 0.5 + frameWidth, 0.0, 1.0,

        // Right frame
        -0.4 + frameWidth, -0.5 - frameWidth, 1.0, 0.0,

        // Bottom frame
        -0.8 - frameWidth, -0.5 - frameWidth, 0.0, 0.0,
    };

    float vertices_work2[] =
    {   //X    Y      S    T 
        0.2, 0.5,    1.0, 1.0,
        -0.2, 0.5,   0.0, 1.0,
        0.2, -0.5,   1.0, 0.0,
        -0.2, -0.5,  0.0, 0.0,
    };

    // Additional vertices for the frame
    float frameVertices2[] = {
        // Top frame
        0.2 + frameWidth, 0.5 + frameWidth, 1.0, 1.0,

        // Left frame
        -0.2 - frameWidth, 0.5 + frameWidth, 0.0, 1.0,

        // Right frame
        0.2 + frameWidth, -0.5 - frameWidth, 1.0, 0.0,

        // Bottom frame
        -0.2 - frameWidth, -0.5 - frameWidth, 0.0, 0.0,
    };

    float vertices_work3[] =
    {   //X    Y      S    T 
        0.8, 0.5,    1.0, 1.0,
        0.4, 0.5,    0.0, 1.0,
        0.8, -0.5,   1.0, 0.0,
        0.4, -0.5,   0.0, 0.0,
    };

    float frameVertices3[] = {
        // Top frame
        0.8 + frameWidth, 0.5 + frameWidth, 1.0, 1.0,

        // Left frame
        0.4 - frameWidth, 0.5 + frameWidth, 0.0, 1.0,

        // Right frame
        0.8 + frameWidth, -0.5 - frameWidth, 1.0, 0.0,

        // Bottom frame
        0.4 - frameWidth, -0.5 - frameWidth, 0.0, 0.0,
    };

    float vertices_circle1[] = {
        -0.6, 0.0
    };

    // notacija koordinata za teksture je STPQ u GLSL-u (ali se cesto koristi UV za 2D teksture i STR za 3D)
    //ST koordinate u nizu tjemena su koordinate za teksturu i krecu se od 0 do 1, gdje je 0, 0 donji lijevi ugao teksture
    //Npr. drugi red u nizu tjemena ce da mapira boje donjeg lijevog ugla teksture na drugo tjeme
    unsigned int stride = (2 + 2) * sizeof(float);
    unsigned int stride_circle = 2 * sizeof(float);

    unsigned int VAO[9];
    glGenVertexArrays(9, VAO);
    unsigned int VBO[9];
    glGenBuffers(9, VBO);

    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_work1), vertices_work1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_work2), vertices_work2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(VAO[3]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_work3), vertices_work3, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(VAO[4]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(frameVertices1), frameVertices1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(VAO[5]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(frameVertices2), frameVertices2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(VAO[6]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[6]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(frameVertices3), frameVertices3, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(VAO[7]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[7]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_circle1), vertices_circle1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride_circle, (void*)0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(VAO[8]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[8]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_signature), vertices_signature, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    float x = 0;
    float y = 0;
    bool isLine = false;
    bool isPoint = false;
    bool showCircle = false;
    bool mirrorImage = false;
    bool flipImage = false;
    unsigned int circleShader = createShader("circle.vert", "circle.frag");
    unsigned int mirrorEnabledLoc = glGetUniformLocation(unifiedShader, "mirrorEnabled");
    unsigned int flipEnabledLoc = glGetUniformLocation(unifiedShader, "flipEnabled");
    unsigned int uA = glGetUniformLocation(circleShader, "uA");
    unsigned int uPosLoc = glGetUniformLocation(circleShader, "uPos");

    //Tekstura
    unsigned goldTexture = loadImageToTexture("D:\\Projekti\\Racunarska_Grafika\\RG_K1\\RG_K1_P6\\res\\gold.png"); //Ucitavamo teksturu
    unsigned image1 = loadImageToTexture("D:\\Projekti\\Racunarska_Grafika\\RG_K1\\RG_K1_P6\\res\\image01.png"); //Ucitavamo teksturu
    unsigned image2 = loadImageToTexture("D:\\Projekti\\Racunarska_Grafika\\RG_K1\\RG_K1_P6\\res\\image02.png"); //Ucitavamo teksturu
    unsigned image3 = loadImageToTexture("D:\\Projekti\\Racunarska_Grafika\\RG_K1\\RG_K1_P6\\res\\image03.png"); //Ucitavamo teksturu
    unsigned signature = loadImageToTexture("D:\\Projekti\\Racunarska_Grafika\\RG_K1\\RG_K1_P6\\res\\signature.png"); //Ucitavamo teksturu

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, goldTexture); //Podesavamo teksturu
    glGenerateMipmap(GL_TEXTURE_2D); //Generisemo mipmape 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//S = U = X    GL_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);// T = V = Y
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);   //GL_NEAREST, GL_LINEAR
    glBindTexture(GL_TEXTURE_2D, 0);
    unsigned uTexLoc = glGetUniformLocation(unifiedShader, "uTex");
    glUniform1i(uTexLoc, 0); // Indeks teksturne jedinice (sa koje teksture ce se citati boje)

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, image1); //Podesavamo teksturu
    glGenerateMipmap(GL_TEXTURE_2D); //Generisemo mipmape 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//S = U = X    GL_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);// T = V = Y
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);   //GL_NEAREST, GL_LINEAR
    glBindTexture(GL_TEXTURE_2D, 0);
    glUniform1i(uTexLoc, 1); // Indeks teksturne jedinice (sa koje teksture ce se citati boje)

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, image2); //Podesavamo teksturu
    glGenerateMipmap(GL_TEXTURE_2D); //Generisemo mipmape 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//S = U = X    GL_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);// T = V = Y
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);   //GL_NEAREST, GL_LINEAR
    glBindTexture(GL_TEXTURE_2D, 0);
    glUniform1i(uTexLoc, 2); // Indeks teksturne jedinice (sa koje teksture ce se citati boje)

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, image3); //Podesavamo teksturu
    glGenerateMipmap(GL_TEXTURE_2D); //Generisemo mipmape 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//S = U = X    GL_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);// T = V = Y
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);   //GL_NEAREST, GL_LINEAR
    glBindTexture(GL_TEXTURE_2D, 0);
    glUniform1i(uTexLoc, 3); // Indeks teksturne jedinice (sa koje teksture ce se citati boje)

    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, signature); //Podesavamo teksturu
    glGenerateMipmap(GL_TEXTURE_2D); //Generisemo mipmape 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//S = U = X    GL_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);// T = V = Y
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);   //GL_NEAREST, GL_LINEAR
    glBindTexture(GL_TEXTURE_2D, 0);
    glUniform1i(uTexLoc, 4); // Indeks teksturne jedinice (sa koje teksture ce se citati boje)
    //Odnosi se na glActiveTexture(GL_TEXTURE0) u render petlji
    //Moguce je sabirati indekse, tj GL_TEXTURE5 se moze dobiti sa GL_TEXTURE0 + 5 , sto je korisno za iteriranje kroz petlje

    while (!glfwWindowShouldClose(window))
    {

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            y += 0.01;
            if (y > 0.5)
                y = 0.5;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            y -= 0.01;
            if (y < -0.5)
                y = -0.5;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            x += 0.01;
            if (x > 0.2)
                x = 0.2;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            x -= 0.01;
            if (x < -0.2)
                x = -0.2;
        }

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        {
            isPoint = true;
            isLine = false;
            mirrorImage = true;
            flipImage = false;
        }

        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        {
            isPoint = false;
            isLine = true;
            mirrorImage = false;
            flipImage = true;
        }

        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        {
            isPoint = false;
            isLine = false;
            mirrorImage = false;
            flipImage = false;
        }

        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        {
            showCircle = true;
        }

        if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
        {
            showCircle = false;
        }

        glClearColor(0.5, 0.5, 0.5, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(unifiedShader);

        glBindVertexArray(VAO[0]);
        glActiveTexture(GL_TEXTURE0); //tekstura koja se bind-uje nakon ovoga ce se koristiti sa SAMPLER2D uniformom u sejderu koja odgovara njenom indeksu
        glBindTexture(GL_TEXTURE_2D, goldTexture);
        glUniform1i(uTexLoc, 0);
        glUniform1i(mirrorEnabledLoc, false ? 1 : 0);
        glUniform1i(flipEnabledLoc, false ? 1 : 0);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindTexture(GL_TEXTURE_2D, 0);

        glBindVertexArray(VAO[8]);
        glActiveTexture(GL_TEXTURE4); //tekstura koja se bind-uje nakon ovoga ce se koristiti sa SAMPLER2D uniformom u sejderu koja odgovara njenom indeksu
        glBindTexture(GL_TEXTURE_2D, signature);
        glUniform1i(uTexLoc, 4);
        glUniform1i(mirrorEnabledLoc, false ? 1 : 0);
        glUniform1i(flipEnabledLoc, false ? 1 : 0);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindTexture(GL_TEXTURE_2D, 0);

        if (isLine) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else if (isPoint) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            glEnable(GL_PROGRAM_POINT_SIZE);
            glPointSize(4);
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(VAO[4]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glBindVertexArray(VAO[5]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glBindVertexArray(VAO[6]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glBindVertexArray(VAO[1]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, image1);
        glUniform1i(uTexLoc, 1);
        glUniform1i(mirrorEnabledLoc, mirrorImage ? 1 : 0);
        glUniform1i(flipEnabledLoc, flipImage ? 1 : 0);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindTexture(GL_TEXTURE_2D, 0);

        glBindVertexArray(VAO[2]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, image2);
        glUniform1i(uTexLoc, 2);
        glUniform1i(mirrorEnabledLoc, mirrorImage ? 1 : 0);
        glUniform1i(flipEnabledLoc, flipImage ? 1 : 0);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindTexture(GL_TEXTURE_2D, 0);

        glBindVertexArray(VAO[3]);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, image3);
        glUniform1i(uTexLoc, 3);
        glUniform1i(mirrorEnabledLoc, mirrorImage ? 1 : 0);
        glUniform1i(flipEnabledLoc, flipImage ? 1 : 0);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindTexture(GL_TEXTURE_2D, 0);

        if (showCircle) {
            glUseProgram(circleShader);
            glEnable(GL_PROGRAM_POINT_SIZE);
            glPointSize(15);
            glUniform1f(uA, abs(sin(10 * (0.01 + abs(x) + abs(y)) * glfwGetTime())));
            glBindVertexArray(VAO[7]);
            glUniform2f(uPosLoc, x-0.6, y);
            glDrawArrays(GL_POINTS, 0, sizeof(vertices_circle1) / stride_circle);

            glUniform2f(uPosLoc, x, y);
            glDrawArrays(GL_POINTS, 0, sizeof(vertices_circle1) / stride_circle);

            glUniform2f(uPosLoc, x+0.6, y);
            glDrawArrays(GL_POINTS, 0, sizeof(vertices_circle1) / stride_circle);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glDeleteTextures(1, &goldTexture);
    glDeleteBuffers(1, VBO);
    glDeleteVertexArrays(1, VAO);
    glDeleteProgram(unifiedShader);

    glfwTerminate();
    return 0;
}

unsigned int compileShader(GLenum type, const char* source)
{
    std::string content = "";
    std::ifstream file(source);
    std::stringstream ss;
    if (file.is_open())
    {
        ss << file.rdbuf();
        file.close();
        std::cout << "Uspjesno procitao fajl sa putanje \"" << source << "\"!" << std::endl;
    }
    else {
        ss << "";
        std::cout << "Greska pri citanju fajla sa putanje \"" << source << "\"!" << std::endl;
    }
    std::string temp = ss.str();
    const char* sourceCode = temp.c_str();

    int shader = glCreateShader(type);

    int success;
    char infoLog[512];
    glShaderSource(shader, 1, &sourceCode, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        if (type == GL_VERTEX_SHADER)
            printf("VERTEX");
        else if (type == GL_FRAGMENT_SHADER)
            printf("FRAGMENT");
        printf(" sejder ima gresku! Greska: \n");
        printf(infoLog);
    }
    return shader;
}
unsigned int createShader(const char* vsSource, const char* fsSource)
{

    unsigned int program;
    unsigned int vertexShader;
    unsigned int fragmentShader;

    program = glCreateProgram();

    vertexShader = compileShader(GL_VERTEX_SHADER, vsSource);
    fragmentShader = compileShader(GL_FRAGMENT_SHADER, fsSource);


    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);
    glValidateProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(program, 512, NULL, infoLog);
        std::cout << "Objedinjeni sejder ima gresku! Greska: \n";
        std::cout << infoLog << std::endl;
    }

    glDetachShader(program, vertexShader);
    glDeleteShader(vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(fragmentShader);

    return program;
}
static unsigned loadImageToTexture(const char* filePath) {
    int TextureWidth;
    int TextureHeight;
    int TextureChannels;
    unsigned char* ImageData = stbi_load(filePath, &TextureWidth, &TextureHeight, &TextureChannels, 0);
    if (ImageData != NULL)
    {
        //Slike se osnovno ucitavaju naopako pa se moraju ispraviti da budu uspravne
        stbi__vertical_flip(ImageData, TextureWidth, TextureHeight, TextureChannels);

        // Provjerava koji je format boja ucitane slike
        GLint InternalFormat = -1;
        switch (TextureChannels) {
        case 1: InternalFormat = GL_RED; break;
        case 3: InternalFormat = GL_RGB; break;
        case 4: InternalFormat = GL_RGBA; break;
        default: InternalFormat = GL_RGB; break;
        }

        unsigned int Texture;
        glGenTextures(1, &Texture);
        glBindTexture(GL_TEXTURE_2D, Texture);
        glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, TextureWidth, TextureHeight, 0, InternalFormat, GL_UNSIGNED_BYTE, ImageData);
        glBindTexture(GL_TEXTURE_2D, 0);
        // oslobadjanje memorije zauzete sa stbi_load posto vise nije potrebna
        stbi_image_free(ImageData);
        return Texture;
    }
    else
    {
        std::cout << "Textura nije ucitana! Putanja texture: " << filePath << std::endl;
        std::cout << stbi_failure_reason();
        stbi_image_free(ImageData);
        return 0;
    }
}