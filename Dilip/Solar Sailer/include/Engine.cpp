/**
*Greeting!!!
*You are always allowed to edit the stuffs here to make things work for you as long as you don't blame me for any kind of misuse
*
*Please note:
* a)GL/glut.h file required for creating the window
* b)Copy "stb_image" folder also from near somewhere as it uses this for image handling
* c)GL-Math library is used for calculations so you need to have "glm" library at your IDE include folder
*   or if you have somewhere else, you can change that by editing the include line below
* d)GLEW is required to have in your IDE include as GL/glew.h
* e)Why are you still reading? Start making stuffs you always imagined!! Good Luck!!!
*
*Author:De'Legend
*Credit:Don't mention it!!! Its all yours!!
*/
#define GLEW_STATIC

#include <GL/glew.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <bits/stdc++.h>
#include "stb_image/stb_image.h"
#include <string.h>

#define ASSERT(x) if(!x)exit(0)

#define GLCall(x)\
        GLErrorDetect::GLClearErrors();\
        x;\
        ASSERT(GLErrorDetect::GLLogError(#x, __FILE__, __LINE__))

/**
Name space used just for handling OpenGL errors
*/
namespace GLErrorDetect{
    static void GLClearErrors(){
        while(glGetError() != GL_NO_ERROR);
    }

    static bool GLLogError(const char* function, const char *file, int line){
        while(GLenum error = glGetError()){
            std::cout << "GL Error Code : " << error << std::endl;
            std::cout << "Error in function :" << function <<  std::endl;
            std::cout << "At location :" << file << std::endl;
            std::cout << "At line number :" << line <<std::endl;
            return false;
        }
        return true;
    }

}

/**
Name space containing functions to be used generally while creating windows and etc
*/
namespace Window{

    /**
    *Pass the arguments exactly from main
    *address of must be passed
    */
    static void createGlutWindow(int &argc, char *argv[],char* Title,glm::vec2 windowPosition,glm::vec2 windowSize){
        glutInit(&argc, argv);
        glutInitWindowSize(windowSize.x,windowSize.y);
        glutInitWindowPosition(windowPosition.x,windowPosition.y);
        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE);
        glutCreateWindow(Title);
    }

    /**
    *Does not draw non-visible faces
    *Call it after window is created
    */
    static void enableBackfaceCulling(){
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }

    /**
    *Disable culling enabled
    *Call it after window is created
    */
    static void disableBackfaceCulling(){
        glDisable(GL_CULL_FACE);
    }

    /**
    *Enables the clipping of non-visible surfaces
    *i.e occlusion culling
    *Call it after window is created
    */
    static void enableDepthTest(){
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
    }

    /**
    *Initialize and Checks if GLEW functions are supported
    *Prints error in console if error exists
    *Prints the GLEW version currently using
    */
    static void initializeGLEW(bool displayStatus = true){
        GLenum err = glewInit();
        if (GLEW_OK != err){
            fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        }
        if(displayStatus){
            fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
        }
    }

    /**
    *Enables alpha blending
    */
    static void enableAlphaBlending(){
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    /**
    *Enables additive blending
    */
    static void enableAdditiveBlending(){
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    }

    /**
    *Disables any blending
    */
    static void disableBlending(){
        glDisable(GL_BLEND);
    }

    /**
    *Enables CLIP_DISTANCE0
    */
    static void enableClipDistance0(){
        glEnable(GL_CLIP_DISTANCE0);
    }
};

/**
Object orienting the OpenGL data types
*/
namespace OpenGL{
    /**
    *Structure of both shaders to be loaded into same program
    */
    struct shaderProgramSource{
        std::string vertexShader;
        std::string fragmentShader;
    };

    /**
    *Used for making pseudo hash map storing the name and location
    */
    struct uniformLocation{
        std::string name;
        int location;
    };

    /**
    *Object oriented version of OpenGL shader type
    */
    class Shader{
    private:
    private:
        unsigned int shader;
        std::string m_filePath;
        std::vector<uniformLocation> locations;

        /**
        *Reads shader from file
        */
        static shaderProgramSource parseShader(const std::string& filePath){
            enum {
                NONE = -1, VERTEX_SHADER = 0, FRAGMENT_SHADER = 1
            };

            std::string line;
            std::stringstream ss[2];
            std::ifstream stream(filePath.c_str());
            int type = NONE;

            if(stream == NULL){
                std::cout << "Cannot Read File" << std::endl;
            }

            while(getline(stream, line) != NULL){
                //If shader definition is found
                if(line.find("#shader") != std::string::npos){
                    //If that is vertex shader
                    if(line.find("vertex") != std::string::npos)
                        type = VERTEX_SHADER;
                    //If that is fragment shader
                    else if(line.find("fragment") != std::string::npos)
                        type = FRAGMENT_SHADER;
                }
                else{
                    ss[type] << line << "\n";
                }
            }
            return {ss[0].str(), ss[1].str()};
        }

        /**
        *Creates the shader of specified type and compiles
        */
        static unsigned int compileShader(GLenum type, std::string& source){
            unsigned int id = glCreateShader(type);
            const char* src = source.c_str();
            glShaderSource(id, 1, &src, NULL);
            glCompileShader(id);

            //Checking the compilation and error handling
            int result;
            glGetShaderiv(id, GL_COMPILE_STATUS, &result);
            if(result == GL_FALSE){
                int msgLength;
                glGetShaderiv(id, GL_INFO_LOG_LENGTH, &msgLength);
                char* message = new char[msgLength];//(char*)alloca(msgLength*sizeof(char));

                glGetShaderInfoLog(id, msgLength, &msgLength, message);
                std::cout << "Failed to compile " <<
                    ((type==GL_VERTEX_SHADER)?"vertex":"fragment") << " shader" << std::endl;
                std::cout << message;
                glDeleteShader(id);
                return 0;
            }
            return id;
        }

        /**
        *Creates shader program and returns its id
        */
        static unsigned int createShader(std::string& vertexShader, std::string& fragmentShader){
            //Create program and shader
            unsigned int program = glCreateProgram();
            unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
            unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

            //Attach created and compiled shader to program
            glAttachShader(program, vs);
            glAttachShader(program, fs);

            //Link and validate the program
            glLinkProgram(program);
            glValidateProgram(program);

            //Delete the shader after we have attached
            glDeleteShader(vs);
            glDeleteShader(fs);

            return program;
        }

        /**
        *Finds the location of uniform in loaded shader file
        */
        int findUniformLocation(std::string uniformName){

            for(unsigned int i=0; i<locations.size(); i++){
                if(locations[i].name == uniformName)
                    return locations[i].location;
            }

            int location = glGetUniformLocation(shader, uniformName.c_str());
            locations.push_back({uniformName, location});

            if(location == -1)
                std::cout << "Warning : " << "uniform '" << uniformName <<  "' doesn't exist" <<std::endl;

            return location;
        }

    public:
        /**
        *Loads the shader from specified file
        */
        void loadShader(std::string shaderFile){
            m_filePath = shaderFile;
            shaderProgramSource shaderProgram = parseShader(shaderFile);
            shader = createShader(shaderProgram.vertexShader, shaderProgram.fragmentShader);
        }

        /**
        *Uses the loaded created shader
        */
        void bind() const{
            GLCall(glUseProgram(shader));
        }

        /**
        *Unbinds shader
        */
        void unbind() const{
            GLCall(glUseProgram(0));
        }

        /**
        *Deletes the used and created shader
        */
        void deleteShader(){
            locations.clear();
            GLCall(glDeleteShader(shader));
        }

        /**
        *Adds the uniform data to the shader used
        *Use this after shader is used
        */
        void addUniform4f(std::string name, float data[]){
            GLCall(glUniform4f(findUniformLocation(name), data[0], data[1], data[2], data[3]));
        }

        /**
        *Adds the uniform data to the shader used
        *Use this after shader is used
        */
        void addUniform4f(std::string name, glm::vec4 data){
            GLCall(glUniform4f(findUniformLocation(name), data.x, data.y, data.z, data.w));
        }

        /**
        *Adds the uniform data to the shader used
        *Use this after shader is used
        */
        void addUniform3f(std::string name, float data[]){
            GLCall(glUniform3f(findUniformLocation(name), data[0], data[1], data[2]));
        }

        /**
        *Adds the uniform data to the shader used
        *Use this after shader is used
        */
        void addUniform2f(std::string name, float data[]){
            GLCall(glUniform2f(findUniformLocation(name), data[0], data[1]));
        }

        /**
        *Adds the uniform data to the shader used
        *Use this after shader is used
        */
        void addUniform1iv(std::string name, int data[]){
            GLCall(glUniform1iv(findUniformLocation(name), 2, data));
        }

        /**
        *Adds the uniform data to the shader used
        *Use this after shader is used
        */
        void addUniform1f(std::string name, float data){
            GLCall(glUniform1f(findUniformLocation(name), data));
        }

        /**
        *Adds the uniform data to the shader used
        *Use this after shader is used
        */
        void addUniform1fv(std::string name, float* data, unsigned int count){
            GLCall(glUniform1fv(findUniformLocation(name), count, data));
        }

        /**
        *Adds the uniform data to the shader used
        *Use this after shader is used
        */
        void addUniform1i(std::string name, int data){
            GLCall(glUniform1i(findUniformLocation(name), data));
        }

        /**
        *Adds the uniform data to the shader used
        *Use this after shader is used
        */
        void addUniformMat4f(std::string name, glm::mat4& data){
            GLCall(glUniformMatrix4fv(findUniformLocation(name), 1, GL_FALSE, &data[0][0]));
        }

        /**
        *Adds the uniform data to the shader used
        *Use this after shader is used
        *@param name = name in shader
        *@param data = pointer to first element of first matrix
        *@param elementCount = no. of matrices to be loaded
        */
        void addUniformMat4fv(std::string name, float* data, unsigned int elementCount){
            GLCall(glUniformMatrix4fv(findUniformLocation(name), elementCount, GL_FALSE, data));
        }

        /**
        *@return location of attribute in shader
        */
        unsigned int getAttriblocation(const std::string& name){
            int location = glGetAttribLocation(shader, name.c_str());
            if(location == -1){
                std::cout << "Cannot find attribute" << std::endl;
                return 0;
            }
            return (unsigned int)location;
        }
    };

    /**
    *Object oriented version of OpenGL texture type
    */
    class Texture{
    private:
        unsigned int m_rendererID;
        std::string m_filePath;
        unsigned char* m_localBuffer;
        int m_height, m_width, m_BPP;

    public:
        /**
        *To load texture from file path specified
        */
        void loadTexture(std::string filePath, bool useMipmap = true, float levelOfDetails=-1.5f, float an = 4.0){
            m_rendererID = 0;
            m_filePath = filePath;
            m_localBuffer = NULL;
            m_height = 0;
            m_width = 0;
            m_BPP = 0;

            //Storing the pixel data in local buffer
            stbi_set_flip_vertically_on_load(1);
            m_localBuffer = stbi_load(filePath.c_str(), &m_width, &m_height, &m_BPP, 4);

            //Generate and bind texture
            GLCall(glGenTextures(1, &m_rendererID));
            GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererID));

            //Specifying the texture properties
            if(useMipmap){
                float aniso;
                GLCall(glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &aniso));
                an = an>aniso?aniso:an;
                GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
                GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, levelOfDetails));
                GLCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, an));
            }
            else{
                GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            }

            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

            //Feeding the data to texture
            if(useMipmap){
                GLCall(gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA8, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer));
            }
            else{
                GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer));
            }

            //Unbinding the texture after data is feeded
            GLCall(glBindTexture(GL_TEXTURE_2D, 0));

            //Freeing the memory from local buffer
            if(m_localBuffer){
                stbi_image_free(m_localBuffer);
            }
        }

        /**
        *@param id of created texture
        *Used if the texture is already created in another location
        */
        void loadCreatedTexture(unsigned int id){
            m_rendererID = id;
        }

        /**
        *@return id of the texture object
        */
        unsigned int getID(){
            return m_rendererID;
        }

        /**
        *Binding the texture at @param slot (default=0 slot)
        */
        void bind(unsigned int slot = 0) const{
            GLCall(glActiveTexture(GL_TEXTURE0 + slot));
            GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererID));
        }

        /**
        *Unbind the texture from currently bounded slot
        */
        void unbind() const{
            GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererID));
        }

        /**
        *Getter for texture width
        */
        inline int getWidth(){
            return m_width;
        }

        /**
        *Getter for texture height
        */
        inline int getHeight(){
            return m_height;
        }

        /**
        *Delete the texture from memory
        */
        void deleteTexture(){
            GLCall(glDeleteTextures(1, &m_rendererID));
        }
    };

    /**
    *Object orienting the vertex buffer where data are stored
    */
    class VertexBuffer{
    private:
        unsigned int m_renderID;

    public:
        /**
        *Null constructor
        */
        VertexBuffer(){
        }

        /**
        *Creates the vertex buffer and binds the data
        */
        void loadData(const void* data, unsigned int dataSize){
            //Generate a array buffer
            GLCall(glGenBuffers(1, &m_renderID));

            //Storing the data into the buffer
            GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_renderID));
            GLCall(glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW));
        }

        /**
        *Creates the vertex buffer but only allocates memory without filling it
        *Also binds automatically
        */
        void allocateDynamically(unsigned int dataSize){
            //Generate a array buffer
            GLCall(glGenBuffers(1, &m_renderID));

            //Storing the data into the buffer
            GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_renderID));
            GLCall(glBufferData(GL_ARRAY_BUFFER, dataSize, NULL, GL_DYNAMIC_DRAW));
        }

        /**
        *Adds data to allocated memory
        */
        void feedDatatoAllocated(const void* data, unsigned int dataSize){
            bind();
            GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, dataSize, data));
        }

        /**
        *Creates the vertex buffer and binds the data
        */
        VertexBuffer(const void* data, unsigned int dataSize){
            loadData(data,dataSize);
        }

        /**
        *Binds the vertex buffer
        */
        void bind() const{
            GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_renderID));
        }

        /**
        *Unbinds the vertex buffer
        */
        void unbind() const{
            GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        }

        /**
        *Deleting the buffer contents and freeing the memory
        */
        void deleteVertexBuffer(){
            GLCall(glDeleteBuffers(1, &m_renderID));
        }
    };

    /**
    *Object orienting the index buffer where indices of vertex buffer is stored
    */
    class IndexBuffer{
    private:
        unsigned int m_renderID;
        unsigned int m_count;

    public:
        /**
        *Null constructor
        */
        IndexBuffer(){
        }

        /**
        *Creates the index buffer and binds the data
        */
        void loadData(const unsigned int* data, unsigned int count){
            m_count = count;
            //Generate a index buffer
            GLCall(glGenBuffers(1, &m_renderID));

            //Storing the data into the buffer
            GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderID));
            GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned int), data, GL_STATIC_DRAW));
        }

        /**
        *Creates the index buffer and binds the data
        */
        IndexBuffer(const unsigned int* data, unsigned int count):m_count(count){
            loadData(data, count);
        }

        /**
        *Binds the index buffer
        */
        void bind() const{
            GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderID));
        }

        /**
        *Unbinds the index buffer
        */
        void unbind() const{
            GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
        }

        /**
        *@return count of indices
        */
        unsigned int getCount(){
            return m_count;
        }

        /**
        *Deleting the index buffer contents and freeing the memory
        */
        void deleteIndexBuffer(){
            GLCall(glDeleteBuffers(1, &m_renderID));
        }
    };

    /**
    *Object orienting the frame buffer data type
    */
    class FrameBuffer{
    private:
        unsigned int m_rendererID;

    public:
        /**
        *Must be called initially when frame buffer is to be initialized
        */
        void generateFrameBuffer(bool colorAttachMent = true){
            GLCall(glGenFramebuffers(1, &m_rendererID));
            GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID));
            if(colorAttachMent) {GLCall(glDrawBuffer(GL_COLOR_ATTACHMENT0));}
            else                {GLCall(glDrawBuffer(GL_NONE));}
            GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        }

        /**
        *@return the texture id of attached color texture
        *Must be linked with another texture to use
        *frame buffer must be generated first to use
        */
        unsigned int AttachColorTexture(int width, int height){
            unsigned int m_rendererID;
            GLCall(glGenTextures(1, &m_rendererID));
            GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererID));
            GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_rendererID, 0));
            return m_rendererID;
        }

        /**
        *@return the texture id of attached depth texture
        *Must be linked with another texture to use
        *frame buffer must be generated first to use
        */
        unsigned int AttachDepthTexture(int width, int height){
            unsigned int m_rendererID;
            GLCall(glGenTextures(1, &m_rendererID));
            GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererID));

            GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_rendererID, 0));
            return m_rendererID;
        }

        /**
        *Binds the frame buffer object
        *But first we need to bind the texture attached or to be used
        */
        void bind(int width, int height){
            GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID));
            GLCall(glViewport(0, 0, width, height));
        }

        /**
        *Unbinds the current frame buffer and binds default frame buffer with specified view-port
        */
        void unbind(int screenWidth, int screenHeight){
            GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
            GLCall(glViewport(0, 0, screenWidth, screenHeight));
        }

        /**
        *Deleting the frame buffer and freeing the memory
        */
        void deleteBuffer(){
            glDeleteFramebuffers(1, &m_rendererID);
        }
    };

    /**
    *Used to make vertex buffer layout for storing the information of each vertex buffer
    */
    struct VertexBufferElement{
        unsigned int type;
        unsigned int count;
        unsigned char normalized;

        /**
        *@return size of type in byte of @param
        */
        static unsigned int getSizeOfType(unsigned int type){
            switch(type){
                case GL_FLOAT:
                    return sizeof(float);
                case GL_UNSIGNED_INT:
                    return sizeof(unsigned int);
                case GL_UNSIGNED_BYTE:
                    return sizeof(unsigned char);
            }
            return 0;
        }
    };

    /**
    *Used to make vertex array for storing the information of vertex buffers
    *Used to specify the layout of vertex buffer in vertex array
    */
    class VertexBufferLayout{
    private:
        int m_stride;
        std::vector<VertexBufferElement> m_elements;
    public:
        /**
        *Initializer/constructor
        */
        VertexBufferLayout():m_stride(0){
        }

        /**
        *Adds the layout for float with count parameter
        */
        void pushFloat(unsigned int count){
            m_elements.push_back({GL_FLOAT, count, GL_FALSE});
            m_stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
        }

        /**
        *Adds the layout for unsigned int with count parameter
        */
        void pushUnsignedInt(unsigned int count){
            m_elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
            m_stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
        }

        /**
        *Adds the layout for unsigned byte with count parameter
        */
        void pushUnsignedChar(unsigned int count){
            m_elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
            m_stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
        }

        /**
        *@return the VertexBufferElement stored
        */
        std::vector<VertexBufferElement> getBufferElement(){
            return m_elements;
        }

        /**
        *@return the stride calculated
        */
        int getStride(){
            return m_stride;
        }
    };

    /**
    *Making object oriented version of vertex array
    */
    class VertexArray{
    private:
        unsigned int m_rendererID;
    public:
        /**
        *Generates the vertexArray
        */
        void generateVertexArray(){
            GLCall(glGenVertexArrays(1, &m_rendererID));
        }

        /**
        *Binds the generated vertex array
        */
        void bind() const{
            GLCall(glBindVertexArray(m_rendererID));
        }

        /**
        *Unbinds the generated vertex array
        */
        void unbind() const{
            GLCall(glBindVertexArray(0));
        }

        /**
        *Stores vertex buffer and layout to the vertex array
        *Vertex array must be generated first
        */
        void addBuffer(VertexBuffer& vb,VertexBufferLayout& layout){
            bind();
            vb.bind();
            std::vector<VertexBufferElement> elements = layout.getBufferElement();
            unsigned int offset = 0;

            for(unsigned int i=0; i < elements.size(); i++){
                VertexBufferElement element = elements[i];
                //Specifying the layout
                GLCall(glEnableVertexAttribArray(i));
                if((element.type==GL_INT)||
                   (element.type==GL_UNSIGNED_INT)||
                   (element.type==GL_BYTE)||
                   (element.type==GL_UNSIGNED_BYTE)||
                   (element.type==GL_SHORT)||
                   (element.type==GL_UNSIGNED_SHORT)){
                    GLCall(glVertexAttribIPointer(i, element.count, element.type,
                                                  layout.getStride(), (const void*)offset));
                }
                else{
                    GLCall(glVertexAttribPointer(i, element.count, element.type,
                                                 element.normalized, layout.getStride(), (const void*)offset));
                }
                offset += element.count * VertexBufferElement::getSizeOfType(element.type);
            }
        }

        /**
        *Deleting the vertex array and freeing the memory
        */
        void deleteVertexArray(){
            GLCall(glDeleteVertexArrays(1, &m_rendererID));
        }
    };

    /**
    *Object version of cube map texture
    */
    class CubeMap{
    private:
        unsigned int m_rendererID;

    public:
        /**
        *@param file path vector contains the list of 6 textures of 6 faces of cube map
        *Order follows as positive x, negative x, positive y, negative y, positive z, negative z
        */
        void loadTexture(std::vector<std::string> filePath){
            int m_height, m_width, m_BPP;
            unsigned char* m_localBuffer;

            //Generate and bind texture
            GLCall(glGenTextures(1, &m_rendererID));
            GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_rendererID));

            //Storing the pixel data in local buffer
            stbi_set_flip_vertically_on_load(0);
            for(int i=0; i<6; i++){
                m_localBuffer = stbi_load(filePath[i].c_str(), &m_width, &m_height, &m_BPP, 4);
                if(!m_localBuffer){
                    std::cout << "Cannot load '" << filePath[i] << "'" << std::endl;
                    return;
                }
                GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer));
                stbi_image_free(m_localBuffer);
            }

            //Specifying the texture properties
            GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
            GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
            GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
        }

        /**
        *Binds the cube map texture into cube-map slot
        */
        void bind(unsigned int slot=0) const{
            GLCall(glActiveTexture(GL_TEXTURE0 + slot));
            GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_rendererID));
        }

        /**
        *Unbinds the current cube map texture
        */
        void unbind() const{
            GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
        }

        /**
        *Deletes the cube map texture
        */
        void deleteTexture(){
            GLCall(glDeleteTextures(1, &m_rendererID));
        }
    };
}

//----------------------
/**
*Structure of simple vertex data containing position,normal and uv
*Data to be passed in simple vertex/fragment shader
*/
struct SimpleVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

/**
*Data of a pixel of any rgba image format
*/
struct PixelData{
    //unsigned char is used to make it allocate one byte only
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};

/**
*Blueprint of object where image data is loaded and manipulated
*/
class Image{
private:
    unsigned char* m_localBuffer;
    int m_height;
    int m_width;
    int m_BPP;

public:
    /**
    *Loads the image into the object's local-buffer
    */
    void loadImage(std::string filePath){
        stbi_set_flip_vertically_on_load(1);
        m_localBuffer = stbi_load(filePath.c_str(), &m_width, &m_height, &m_BPP, 4);
    }

    /**
    *Getter for pixel data at any pixel of image
    */
    PixelData getPixelAt(int i,int j){
        const size_t RGBA = 4;
        unsigned int index = (i+m_width*j)*RGBA;
        unsigned char r = m_localBuffer[index+0];
        unsigned char g = m_localBuffer[index+1];
        unsigned char b = m_localBuffer[index+2];
        unsigned char a = m_localBuffer[index+3];
        return {r,g,b,a};
    }

    /**
    *Getter for width of image
    */
    inline int getWidth(){
        return m_width;
    }

    /**
    *Getter for height of image
    */
    inline int getHeight(){
        return m_height;
    }

    /**
    *Deletes the content of image
    */
    void deleteImage(){
        stbi_image_free(m_localBuffer);
    }
};

/**
*Blueprint of objects whose data are to be loaded from obj file type
*No need to make any other data for rendering
*Shader is not predefined inside, shader passed will be used while rendering
*Simple vertex data type of position,normal and uv should be used in shader
*/
class ObjMesh{
private:
    OpenGL::VertexArray va;
    OpenGL::VertexBuffer vb;
    unsigned int vertex_count;

    /**
    *Takes the string and returns the vector containing the strings
    */
    static std::vector<std::string> splitString(std::string str, char delim = ' '){
        std::vector<std::string> strData;
        std::string tempString = "";
        for(int i=0; str[i]!='\0'; i++){
            if(str[i] == delim){
                tempString += '\n';
                strData.push_back(tempString);
                tempString = "";
                continue;
            }
            tempString += str[i];
        }
        tempString += '\n';
        strData.push_back(tempString);
        return strData;
    }

    /**
    *Reads the data from location and stores in buffers like vertex buffer and index buffer
    */
    void readBuffers(std::string location, float scale){
        std::string line;
        std::ifstream stream(location.c_str());

        std::vector<SimpleVertex> vertices;
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> uvs;

        std::vector<unsigned int> indices;

        if(stream == NULL){
            std::cout << "Cannot Read File" << std::endl;
        }

        while(getline(stream, line) != NULL){
            std::string type;
            std::stringstream ss_datas(line);
            ss_datas >> type;

            if(type == "v"){
                float points[3];
                ss_datas >> points[0] >> points[1] >> points[2];
                positions.push_back(glm::vec3(points[0]*scale, points[1]*scale, points[2]*scale));
            }
            else if (type == "vn"){
                float points[3];
                ss_datas >> points[0] >> points[1] >> points[2];
                normals.push_back(glm::vec3(points[0], points[1], points[2]));
            }
            else if (type == "vt"){
                float points[2];
                ss_datas >> points[0] >> points[1];
                uvs.push_back(glm::vec2(points[0], points[1]));
            }
            else if(type == "f"){
                std::vector<std::string> v_datas = splitString(line);
                for(unsigned int i=1; i<=3; i++){
                    std::vector<std::string> vertex = splitString(v_datas[i], '/');
                    std::stringstream ss_v(vertex[0]);
                    std::stringstream ss_u(vertex[1]);
                    std::stringstream ss_n(vertex[2]);
                    float v_index = 0;
                    float u_index = 0;
                    float n_index = 0;

                    ss_v >> v_index;
                    ss_u >> u_index;
                    ss_n >> n_index;

                    vertices.push_back({positions[v_index-1], normals[n_index-1], uvs[u_index-1]});
                }
            }
        }
        vb.loadData(&vertices[0], vertices.size()*sizeof(SimpleVertex));
        vertex_count = vertices.size();

        vertices.clear();
        positions.clear();
        normals.clear();
        uvs.clear();
    }

public:
    /**
    *Reads obj from @param file location and loads into object
    *Scale defines the scaling of object about local space origin
    */
    void loadFile(std::string fileLocation, float scale = 1){
        readBuffers(fileLocation, scale);

        OpenGL::VertexBufferLayout vbl;
        vbl.pushFloat(3);
        vbl.pushFloat(3);
        vbl.pushFloat(2);

        va.generateVertexArray();
        va.addBuffer(vb, vbl);
    }

    /**
    *binds vertex array and index buffer ready to be rendered
    */
    void bindArray(){
        va.bind();
    }

    /**
    *@return no of indices in obj file
    */
    unsigned int getVertexCount(){
        return vertex_count;
    }

    /**
    *Cleans up data it allocated for rendering and other manipulations
    */
    void cleanUP(){
        va.deleteVertexArray();
        vb.deleteVertexBuffer();
    }
};

/**
*Data format for normal mapped object
*Contains extra data glm::vec3 tangent
*/
struct VertexWithTangentSpace{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCords;
    glm::vec3 tangent;
};

/**
*Blueprint of objects whose data are to be loaded from obj file type and normal map is gonna be used
*No need to make any other data for rendering
*Shader is not predefined inside, shader passed will be used while rendering
*Simple vertex data type of position,normal and uv should be used in shader
*/
class ObjMeshNormalMapped{
private:
private:
    OpenGL::VertexArray va;
    OpenGL::VertexBuffer vb;
    unsigned int vertex_count;

    /**
    *Takes the string and returns the vector containing the strings
    */
    static std::vector<std::string> splitString(std::string str, char delim = ' '){
        std::vector<std::string> strData;
        std::string tempString = "";
        for(unsigned int i=0; str[i]!='\0'; i++){
            if(str[i] == delim){
                tempString += '\n';
                strData.push_back(tempString);
                tempString = "";
                continue;
            }
            tempString += str[i];
        }
        tempString += '\n';
        strData.push_back(tempString);
        return strData;
    }

    /**
    *Reads the data from location and stores in buffers like vertex buffer and index buffer
    */
    void readBuffers(std::string location, float scale){
        std::string line;
        std::ifstream stream(location.c_str());

        std::vector<VertexWithTangentSpace> vertices;
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec3> tangents;
        std::vector<glm::vec2> uvs;

        std::vector<unsigned int> indices;

        if(stream == NULL){
            std::cout << "Cannot Read File" << std::endl;
        }

        while(getline(stream, line) != NULL){
            std::string type;
            std::stringstream ss_datas(line);
            ss_datas >> type;

            if(type == "v"){
                float points[3];
                ss_datas >> points[0] >> points[1] >> points[2];
                positions.push_back(glm::vec3(points[0]*scale, points[1]*scale, points[2]*scale));
            }
            else if (type == "vn"){
                float points[3];
                ss_datas >> points[0] >> points[1] >> points[2];
                normals.push_back(glm::vec3(points[0], points[1], points[2]));
            }
            else if (type == "vt"){
                float points[2];
                ss_datas >> points[0] >> points[1];
                uvs.push_back(glm::vec2(points[0], points[1]));
            }
            else if(type == "f"){
                std::vector<std::string> v_datas = splitString(line);
                glm::vec3 temp_pos[3];
                glm::vec3 temp_normal[3];
                glm::vec2 temp_uv[3];
                for(unsigned int i=1; i<=3; i++){
                    std::vector<std::string> vertex = splitString(v_datas[i], '/');
                    std::stringstream ss_v(vertex[0]);
                    std::stringstream ss_u(vertex[1]);
                    std::stringstream ss_n(vertex[2]);
                    float v_index = 0;
                    float u_index = 0;
                    float n_index = 0;

                    ss_v >> v_index;
                    ss_u >> u_index;
                    ss_n >> n_index;

                    temp_pos[i-1] = positions[v_index-1];
                    temp_normal[i-1] = normals[n_index-1];
                    temp_uv[i-1] = uvs[u_index-1];
                }

                glm::vec3 del_pos1 = temp_pos[1]-temp_pos[0];
                glm::vec3 del_pos2 = temp_pos[2]-temp_pos[0];
                glm::vec2 del_uv1 = temp_uv[1]-temp_uv[0];
                glm::vec2 del_uv2 = temp_uv[2]-temp_uv[0];
                float r = 1/(del_uv1.x*del_uv2.y - del_uv1.y*del_uv2.x);
                glm::vec3 temp_tangent = (del_pos1*del_uv2.y)-(del_pos2*del_uv1.y);
                temp_tangent *= r;

                for(int i=0;i<3;i++){
                    vertices.push_back({temp_pos[i], temp_normal[i], temp_uv[i], temp_tangent});
                }
            }
        }
        vb.loadData(&vertices[0], vertices.size()*sizeof(VertexWithTangentSpace));
        vertex_count = vertices.size();

        vertices.clear();
        positions.clear();
        normals.clear();
        uvs.clear();
    }

public:
    /**
    *Reads obj from @param file location and loads into object
    *Scale defines the scaling of the object about local space origin
    */
    void loadFile(std::string fileLocation, float scale = 1){
        readBuffers(fileLocation, scale);

        OpenGL::VertexBufferLayout vbl;
        vbl.pushFloat(3);
        vbl.pushFloat(3);
        vbl.pushFloat(2);
        vbl.pushFloat(3);

        va.generateVertexArray();
        va.addBuffer(vb, vbl);
    }

    /**
    *binds vertex array and index buffer ready to be rendered
    */
    void bindArray(){
        va.bind();
    }

    /**
    *@return no of indices in obj file
    */
    unsigned int getVertexCount(){
        return vertex_count;
    }

    /**
    *Cleans up the memory it allocated
    */
    void cleanUP(){
        va.deleteVertexArray();
        vb.deleteVertexBuffer();
    }
};
//-------------------------------
/**
*Blueprint for sky box object
*Individual objects will have its own shader considering there will be one or few sky box only
*No need to initialize any other data types to use it
*Users can increase or set the rotation value of sky-box to give 3D-effect
*/
class SkyBox{
private:
    OpenGL::VertexArray va;
    OpenGL::VertexBuffer vb;
    OpenGL::CubeMap cm;
    OpenGL::Shader shader;
    static const float DEFAULT_ROTATION_SPEED = 0.0075;
    float rotation = 0.0;
public:

    /**
    *Load the sky-box object with sky-boxes textures and shader
    *@param skyBoxes: file location of 6 sky-box images
    *@param shaderFile: file location of shader to be used in sky-box
    *@param cameraFar: far point of camera for ensuring that sky-box never clips off the camera
    *@param fogColor: array of fogColor in RGB
    */
    void loadSkyBox(std::vector<std::string> skyBoxes, std::string shaderFile, float cameraFar, float* fogColor){
        cm.loadTexture(skyBoxes);
        shader.loadShader(shaderFile);
        shader.bind();
        shader.addUniform1i("cubeMap", 0);//Slot of cube-map texture
        shader.addUniform3f("fogColor", fogColor);//Color of fog to be used in sky-box

        const float SIZE = cameraFar/1.8;//Ensuring that diagonal of the cube is also covered by vamera
        float positions[]={
            -SIZE,  SIZE, -SIZE,
            -SIZE, -SIZE, -SIZE,
             SIZE, -SIZE, -SIZE,
             SIZE, -SIZE, -SIZE,
             SIZE,  SIZE, -SIZE,
            -SIZE,  SIZE, -SIZE,

            -SIZE, -SIZE,  SIZE,
            -SIZE, -SIZE, -SIZE,
            -SIZE,  SIZE, -SIZE,
            -SIZE,  SIZE, -SIZE,
            -SIZE,  SIZE,  SIZE,
            -SIZE, -SIZE,  SIZE,

             SIZE, -SIZE, -SIZE,
             SIZE, -SIZE,  SIZE,
             SIZE,  SIZE,  SIZE,
             SIZE,  SIZE,  SIZE,
             SIZE,  SIZE, -SIZE,
             SIZE, -SIZE, -SIZE,

            -SIZE, -SIZE,  SIZE,
            -SIZE,  SIZE,  SIZE,
             SIZE,  SIZE,  SIZE,
             SIZE,  SIZE,  SIZE,
             SIZE, -SIZE,  SIZE,
            -SIZE, -SIZE,  SIZE,

            -SIZE,  SIZE, -SIZE,
             SIZE,  SIZE, -SIZE,
             SIZE,  SIZE,  SIZE,
             SIZE,  SIZE,  SIZE,
            -SIZE,  SIZE,  SIZE,
            -SIZE,  SIZE, -SIZE,

            -SIZE, -SIZE, -SIZE,
            -SIZE, -SIZE,  SIZE,
             SIZE, -SIZE, -SIZE,
             SIZE, -SIZE, -SIZE,
            -SIZE, -SIZE,  SIZE,
             SIZE, -SIZE,  SIZE
        };

        va.generateVertexArray();
        vb.loadData(positions, 36*3*sizeof(float));
        OpenGL::VertexBufferLayout vbl;
        vbl.pushFloat(3);
        va.addBuffer(vb, vbl);
    }

    /**
    *Users are allowed to rotate sky-box to give 3D feeling of sky-box
    */
    void setRotation(float rotation_value){
        rotation = rotation_value;
        if(rotation >=360)
            rotation = 0.0;
    }

    /**
    *Increase the rotation value of the sky-box
    */
    void increaseRotation(float delta = DEFAULT_ROTATION_SPEED){
        rotation += delta;
        if(rotation >=360)
            rotation = 0.0;
    }

    /**
    *Renders the sky-box object into the screen
    */
    void render(glm::mat4 view, glm::mat4 projection){
        shader.bind();
        //Removing translation for sky box
        //i.e camera always stays at center for sky-box
        view[3].x = 0;
        view[3].y = 0;
        view[3].z = 0;
        view[3].w = 1;
        view = glm::rotate(view, glm::radians(rotation), glm::vec3(0.0, 1.0, 0.0));
        glm::mat4 mvp = projection*view;
        shader.addUniformMat4f("u_mvp", mvp);
        va.bind();
        cm.bind();
        GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
    }

    /**
    *Cleans up the data allocated by this object
    */
    void cleanUP(){
        va.deleteVertexArray();
        vb.deleteVertexBuffer();
        shader.deleteShader();
        cm.deleteTexture();
    }
};

/**
*Vertex structure to be used for terrain
*Contain the extra component glm::vec2 mapUV to map texture using RGB values
*/
struct TerrainVertex{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
    glm::vec2 mapUV;
};

/**
*Blueprint for Terrain
*Represents actual terrain of the system
*No extra initialization or shader is requires
*Each object will have its own shader considering there will be one or few terrain only
*/
class Terrain{
    private:
        static const float maxHeight = 20.0f;
        float x_scaling;//Scaling of texture with actual terrain data
        float z_scaling;//Scaling of texture with actual terrain data
        float gridSize;//Contains grid of size grid-size
        OpenGL::VertexArray va;
        OpenGL::VertexBuffer vb;
        OpenGL::Texture texture[5];//Yeah!!! array or textures
        OpenGL::Texture depthTexture;//Again depth texture to
        Image height;//And one more image for height mapping ;)
        OpenGL::Shader shader;
        unsigned int num_vertices;
        std::vector<std::vector<float> > height_values;//What? did you think I didn't try >> instead of > >
        //Stores the height values of grid for collision detection

        /**
        *Returns the height of terrain at certain point from height map
        */
        float getHeightFromMap(float x,float z){
            if(x<0 || x>height.getWidth() || z<0 || z>height.getHeight()){
                return 0;
            }
            PixelData pixel = height.getPixelAt(x, z);
            float result = (pixel.r + pixel.g + pixel.b)/3;
            result = result*2 - 255;
            result = result*maxHeight/255.0;
            return result;
        }

        /**
        *Calculate the position of grid and stores in both cpu and gpu buffers
        */
        void calculateVertices(float tileSize, float length, float width){
            glm::vec3 pos1, pos2, pos3;
            glm::vec2 uv1, uv2, uv3;
            glm::vec2 mapuv1, mapuv2, mapuv3;
            glm::vec3 normal = glm::vec3(0, 1, 0);
            std::vector<TerrainVertex> vertices;

            float i_loop = length/tileSize;
            float j_loop = width/tileSize;
            x_scaling = height.getWidth()/length;
            z_scaling = height.getHeight()/width;
            this->gridSize = tileSize;

            for(int i = 0 ; i < (i_loop); i++){
                std::vector<float> zAxisHeights;
                for(int j=0; j < (j_loop); j++){
                    pos1 = glm::vec3(i* tileSize, getHeightFromMap(i*height.getWidth()/i_loop, j*height.getHeight()/j_loop), -(j*tileSize));
                    uv1 = glm::vec2(0, 0);
                    mapuv1 = glm::vec2(i/i_loop, j/j_loop);

                    pos2 = glm::vec3((i+1)* tileSize, getHeightFromMap((i+1)*height.getWidth()/i_loop, j*height.getHeight()/j_loop), -(j*tileSize));
                    uv2 = glm::vec2(1, 0);
                    mapuv2 = glm::vec2((i+1)/i_loop, j/j_loop);

                    pos3 = glm::vec3((i+1)* tileSize, getHeightFromMap((i+1)*height.getWidth()/i_loop, (j+1)*height.getHeight()/j_loop), -((j+1)*tileSize));
                    uv3 = glm::vec2(1, 1);
                    mapuv3 = glm::vec2((i+1)/i_loop, (j+1)/j_loop);

                    vertices.push_back({pos1, normal, uv1, mapuv1});
                    vertices.push_back({pos2, normal, uv2, mapuv2});
                    vertices.push_back({pos3, normal, uv3, mapuv3});
                    zAxisHeights.push_back(pos1.y);

                    pos1 = glm::vec3(i* tileSize, getHeightFromMap(i*height.getWidth()/i_loop, j*height.getHeight()/j_loop), -(j*tileSize));
                    uv1 = glm::vec2(0, 0);
                    mapuv1 = glm::vec2(i/i_loop, j/j_loop);

                    pos2 = glm::vec3((i+1)* tileSize, getHeightFromMap((i+1)*height.getWidth()/i_loop, (j+1)*height.getHeight()/j_loop), -((j+1)*tileSize));
                    uv2 = glm::vec2(1, 1);
                    mapuv2 = glm::vec2((i+1)/i_loop, (j+1)/j_loop);

                    pos3 = glm::vec3(i* tileSize, getHeightFromMap(i*height.getWidth()/i_loop, (j+1)*height.getHeight()/j_loop), -((j+1)*tileSize));
                    uv3 = glm::vec2(0, 1);
                    mapuv3 = glm::vec2((i)/i_loop, (j+1)/j_loop);

                    vertices.push_back({pos1, normal, uv1, mapuv1});
                    vertices.push_back({pos2, normal, uv2, mapuv2});
                    vertices.push_back({pos3, normal, uv3, mapuv3});
                }
                height_values.push_back(zAxisHeights);
                zAxisHeights.clear();
            }
            num_vertices = vertices.size();
            vb.loadData(&vertices[0], sizeof(TerrainVertex)*vertices.size());
            vertices.clear();
        }

    public:
        /**
        *Loads terrain from passed data
        *@param shaderLocation: file location of terrain shader
        *@param textureLocations: file locations of different texture to be applied on terrain
        *@param heightMap: file location of height map of terrain
        *@param tileSize: size of each tile of terrain
        *@param length and width: dimensions of terrain
        */
        void loadTerrain(std::string shaderLocation, std::vector<std::string> textureLoactions, std::string heightMap,float tileSize, float length,float width){
            shader.loadShader(shaderLocation);
            shader.bind();
            {
                float direction[] = {0.0f, -1.0f, 0.5f};
                float a_color[] = {1.0f, 1.0f, 1.0f};
                float d_color[] = {1.0f, 1.0f, 1.0f};
                float s_color[] = {1.0f, 1.0f, 1.0f};

                shader.addUniform3f("dlight.direction", direction);
                shader.addUniform3f("dlight.ambient", a_color);
                shader.addUniform3f("dlight.diffuse", d_color);
                shader.addUniform3f("dlight.specular", s_color);
            }

            {
                float shininess = 32.0f;
                float a_color[] = {0.2f, 0.2f, 0.2f};
                float d_color[] = {1.0f, 1.0f, 1.0f};
                float s_color[] = {1.0f, 1.0f, 1.0f};

                shader.addUniform1f("material.shininess", shininess);
                shader.addUniform3f("material.ambient", a_color);
                shader.addUniform3f("material.diffuse", d_color);
                shader.addUniform3f("material.specular", s_color);
            }

            {
                float sky[]={0.40, 0.50, 0.60};
                shader.addUniform3f("u_skyColor",sky);
                shader.addUniform1i("backSlot", 0);
                shader.addUniform1i("rSlot", 1);
                shader.addUniform1i("gSlot", 2);
                shader.addUniform1i("bSlot", 3);
                shader.addUniform1i("mapSlot", 4);
                shader.addUniform1i("depthSlot", 5);
            }

            //texture.loadTexture(terrainTexture);
            for(int i=0; i<5; i++)
                texture[i].loadTexture(textureLoactions[i]);
            height.loadImage(heightMap);

            calculateVertices(tileSize, length, width);

            OpenGL::VertexBufferLayout vbl;
            vbl.pushFloat(3);
            vbl.pushFloat(3);
            vbl.pushFloat(2);
            vbl.pushFloat(2);

            va.generateVertexArray();
            va.addBuffer(vb, vbl);
        }

        /**
        *Draws the terrain using its own shader
        *Shadow loader should be binded in shadow depth slot before drawing to get to get shadow in terrain
        */
        void draw(glm::mat4 view, glm::mat4 projection, glm::mat4 lightSpace = glm::mat4(1.0),
                    glm::vec4 clippingPlane= glm::vec4(0.0, -1.0, 0.0, 20.0)){
            for(unsigned int i=0; i<5; i++)
                texture[i].bind(i);

            shader.bind();
            shader.addUniformMat4f("u_view", view);
            shader.addUniformMat4f("u_projection", projection);
            shader.addUniform4f("u_clipPlane", clippingPlane);
            shader.addUniformMat4f("u_lightSpace", lightSpace);
            va.bind();
            GLCall(glDrawArrays(GL_TRIANGLES, 0, num_vertices));
        }

        /**
        *Draws the terrain using passed shader targeted for getting depth values only
        *All the uniform values must have been supplied to the shader and vertex format must be TerrainVertex
        */
        //TODO::Make Draw Colorless and without depending upon extra shader
        void draw(OpenGL::Shader shader){
            shader.bind();
            va.bind();
            GLCall(glDrawArrays(GL_TRIANGLES, 0, num_vertices));
        }

        /**
        *Expected value negative in Z & positive in X
        *@return value of height at any point on terrain
        */
        float getHeightAt(float x, float z){
            float index_x = x/gridSize;
            float index_z = -z/gridSize;
            //Return 0 if not within stored range
            if(index_x >= height_values.size() || index_z >= height_values[0].size()){
                return 0;
            }

            //Index value of button left point
            int x0 = (int)index_x;
            int z0 = (int)index_z;

            //Index value of top right point
            int x1 = x0+1;
            int z1 = z0+1;

            //Button right corner by default
            int x2 = x1;
            int z2 = z0;
            //Calculating where on side of triangle point exists
            glm::vec2 buttomLeft = glm::vec2(x0*gridSize, z0*gridSize);
            glm::vec2 pointFromButtomLeft = glm::vec2(x,-z) - buttomLeft;
            glm::vec3 normal;
            glm::vec3 pos[3];

            //If point is in topside of triangle
            if(pointFromButtomLeft.y > pointFromButtomLeft.x){
                x2 = x0;
                z2 = z1;
            }
            float heightFloat[] = {
                height_values[x0][z0],
                height_values[x1][z1],
                height_values[x2][z2]
            };
            pos[0] = glm::vec3(x0*gridSize, heightFloat[0], z0*gridSize);
            pos[1] = glm::vec3(x1*gridSize, heightFloat[1], z1*gridSize);
            pos[2] = glm::vec3(x2*gridSize, heightFloat[2], z2*gridSize);

            glm::vec3 vecA = pos[1] - pos[0];
            glm::vec3 vecB = pos[2] - pos[0];
            normal = glm::cross(vecA, vecB);

            float A,B,C,D;
            A = normal.x;
            B = normal.y;
            C = normal.z;
            D = A*pos[0].x + B*pos[0].y + C*pos[0].z;

            return (D - A*x + C*z)/B;
        }

        /**
        *Returns where depth slot from shadow loader to be binded
        */
        unsigned int shadowDepthSlot(){
            return 5;
        }

        /**
        *Cleans up the memory it allocated in both CPU and GPU
        */
        void cleanUP(){
            shader.deleteShader();
            height.deleteImage();
            for(int i=0; i<5; i++){
                texture[i].deleteTexture();
            }
            height_values.clear();
            va.deleteVertexArray();
            vb.deleteVertexBuffer();
        }
};

class NormalMappedObject{
private:
    ObjMeshNormalMapped object;
    OpenGL::Texture colorTexture;
    OpenGL::Texture normalMap;
    static OpenGL::Shader shader;
public:
    void loadObject(std::string objLocation, std::string textureLocation, std::string normalMapLocation, float scaling=1){
        object.loadFile(objLocation, scaling);
        colorTexture.loadTexture(textureLocation);
        normalMap.loadTexture(normalMapLocation);
    }

    static void specifyShader(std::string shaderLocation, glm::vec3 lightColor, glm::vec3 lightDirection){
        shader.loadShader(shaderLocation);
        shader.bind();
        shader.addUniform1i("u_colorSampler",0);
        shader.addUniform1i("u_normalSampler",1);

        lightDirection = glm::normalize(lightDirection);
        float color[]={lightColor.x, lightColor.y, lightColor.z};
        float direction[]={lightDirection.x, lightDirection.y, lightDirection.z};
        shader.addUniform3f("u_lightColor", color);
        shader.addUniform3f("u_lightDirection", direction);
    }

    void draw(glm::mat4 model, glm::mat4 view, glm::mat4 projection, glm::vec3 camPosition){
        float position[]={camPosition.x, camPosition.y, camPosition.z};
        colorTexture.bind();
        normalMap.bind(1);
        shader.bind();
        shader.addUniformMat4f("u_model", model);
        shader.addUniformMat4f("u_view", view);
        shader.addUniformMat4f("u_projection", projection);
        shader.addUniform3f("u_viewPosition", position);
        object.bindArray();
        GLCall(glDrawArrays(GL_TRIANGLES, 0, object.getVertexCount()));
    }
};

class Planet{
private:
    ObjMesh mesh;
    OpenGL::Texture texture;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 velocity;

public:
    void loadPlanet(std::string objLocation, std::string textureLocation, float scaling = 1){
        mesh.loadFile(objLocation, scaling);
        texture.loadTexture(textureLocation);
    }

    void drawPlanet(OpenGL::Shader& shader){
        shader.bind();
        texture.bind();
        mesh.bindArray();
        GLCall(glDrawArrays(GL_TRIANGLES, 0, mesh.getVertexCount()));
    }
};

class Spaceship{
private:
    glm::vec3 position;
    glm::vec3 direction;
    const float velocity = 0.20f;
    ObjMesh body;
    OpenGL::Texture texture;

public:
    void setPosition(glm::vec3 pos){
        position = pos;
    }

    void setDirection(glm::vec3 dir){
        direction = dir;
    }

    glm::vec3 getPosition(){
        return position;
    }

    glm::vec3 getDirection(){
        return direction;
    }

    void loadFiles(std::string objPath, std::string texturePath, float scaling = 1){
        body.loadFile(objPath, scaling);
        texture.loadTexture(texturePath);
    }

    void moveForward(){
        position += velocity*direction;
    }

    void moveBackward(){
        position -= velocity*direction;
    }

    void moveRight(glm::vec3 upVec = glm::vec3(0.0, 1.0, 0.0)){
        glm::vec3 right = glm::normalize(glm::cross(direction, upVec));
        position += velocity*right;
    }

    void moveLeft(glm::vec3 upVec = glm::vec3(0.0, 1.0, 0.0)){
        glm::vec3 right = glm::normalize(glm::cross(direction, upVec));
        position -= velocity*right;
    }

    void draw(OpenGL::Shader shader){
        shader.bind();
        texture.bind();
        body.bindArray();
        GLCall(glDrawArrays(GL_TRIANGLES, 0, body.getVertexCount()));
    }

    void cleanUP(){
        body.cleanUP();
        texture.deleteTexture();
    }
};

class GUI{
private:
    static OpenGL::Shader shader;
    OpenGL::VertexArray va;
    OpenGL::VertexBuffer vb;
    OpenGL::IndexBuffer ib;
    OpenGL::Texture texture;


public:
    void load(glm::vec2 position,glm::vec2 size, std::string textureLocation){
        float positions[] = {
            position.x - size.x/2, position.y - size.y/2, 0, 0,
            position.x + size.x/2, position.y - size.y/2, 1, 0,
            position.x + size.x/2, position.y + size.y/2, 1, 1,
            position.x - size.x/2, position.y + size.y/2, 0, 1
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        vb.loadData(positions, 4*4*sizeof(float));
        OpenGL::VertexBufferLayout layout;
        layout.pushFloat(2);
        layout.pushFloat(2);
        va.generateVertexArray();
        va.addBuffer(vb, layout);
        ib.loadData(indices, 6);

        texture.loadTexture(textureLocation, false, 0);
    }

    static void specifyShader(std::string shaderLocation){
        GUI::shader.loadShader(shaderLocation);
    }

    static void deleteShader(){
        GUI::shader.deleteShader();
    }

    void draw(){
        texture.bind();
        GUI::shader.bind();
        GUI::shader.addUniform1i("u_slot", 0);
        va.bind();
        ib.bind();
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));
    }

    void cleanUP(){
        va.deleteVertexArray();
        ib.deleteIndexBuffer();
        vb.deleteVertexBuffer();
        texture.deleteTexture();
    }
};

struct PosUV{
    glm::vec3 position;
    glm::vec2 uv;
};

class Billboard{
private:
    OpenGL::VertexArray va;
    OpenGL::VertexBuffer vb;
    OpenGL::IndexBuffer ib;
    glm::vec3 position;
    glm::vec2 size;
    static OpenGL::Shader shader;

    /**
    *Calculate the positions of quads to make sure they are always facing directly to camera
    */
    void calculateVertices(glm::mat4 view){
        glm::vec3 cameraRight = glm::vec3(view[0][0], view[1][0], view[2][0]);
        glm::vec3 cameraUp = glm::vec3(view[0][1], view[1][1], view[2][1]);
        std::vector<PosUV> vertices;
        const float squareVertices[] = {
             -0.5f, -0.5f,
              0.5f, -0.5f,
              0.5f,  0.5f,
             -0.5f,  0.5f
         };
         vertices.push_back({position+cameraRight*squareVertices[0]*size.x +cameraUp*squareVertices[1]*size.y, glm::vec2(0, 0)});
         vertices.push_back({position+cameraRight*squareVertices[2]*size.x +cameraUp*squareVertices[3]*size.y, glm::vec2(1, 0)});
         vertices.push_back({position+cameraRight*squareVertices[4]*size.x +cameraUp*squareVertices[5]*size.y, glm::vec2(1, 1)});
         vertices.push_back({position+cameraRight*squareVertices[6]*size.x +cameraUp*squareVertices[7]*size.y, glm::vec2(0, 1)});
         vb.feedDatatoAllocated(&vertices[0], 4*sizeof(PosUV));
         vertices.clear();
    }

public:
    void loadData(glm::vec3 position, glm::vec2 size){
        this->position = position;
        this->size = size;
        vb.allocateDynamically(4*sizeof(PosUV));
        va.generateVertexArray();
        OpenGL::VertexBufferLayout layout;
        layout.pushFloat(3);
        layout.pushFloat(2);
        va.addBuffer(vb, layout);
        unsigned int indices[] ={0, 1, 2, 2, 3, 0};
        ib.loadData(indices, 6);
    }

    void setPosition(glm::vec3 position){
        this->position = position;
    }

    void setSize(glm::vec2 size){
        this->size = size;
    }

    static void specifyShader(std::string shaderLocation){
        shader.loadShader(shaderLocation);
    }

    void draw(glm::mat4 view, glm::mat4 projection){
        calculateVertices(view);
        va.bind();
        ib.bind();
        shader.bind();
        shader.addUniformMat4f("u_view", view);
        shader.addUniformMat4f("u_projection", projection);
        GLCall(glDrawElements(GL_TRIANGLES, ib.getCount() , GL_UNSIGNED_INT, NULL));
    }
};

class Renderer{

public:

    /**
    *Clears the screen with buffer color
    */
    void clear() const{
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    /**
    *@param shader to be applied
    *@param vertex array where data is saved
    *@param index buffer where index is saved
    */
    void draw(OpenGL::VertexArray& va, OpenGL::IndexBuffer& ib, OpenGL::Shader& shader) const{
        shader.bind();
        va.bind();
        ib.bind();
        GLCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, NULL))    ;
    }

    /**
    *@param obj is object file to be rendered
    *@param shader to be applied
    */
    void draw(ObjMesh& obj, OpenGL::Shader& shader){
        shader.bind();
        obj.bindArray();
        GLCall(glDrawArrays(GL_TRIANGLES, 0, obj.getVertexCount()));
    }
};

/**
*Useful tools but not gonna be rendered or anything
*/
namespace Tools{
    class Camera{
    private:
        glm::vec3 position;
        glm::vec3 camDirection;
        float yaw = 0.0;
        float pitch = 15.0;
        float roll = 0.0;
        const float sensitivity = 0.125f;
        const float MAX_DISTANCE = 4.0f;
        //const float MAX_DISTANCE = 20.0f;
        const float MIN_DISTANCE = 0.0f;
        float distance = MAX_DISTANCE;

        void limitAngleToPositive(float& angle){
            if(angle>360)
                angle -= 360;
            if(angle<0)
                angle +=360;
        }

        void recalculateDistance(){
            if(pitch >=0)
                distance = MAX_DISTANCE;
            else{
                float slope = (MAX_DISTANCE - MIN_DISTANCE)/89.0;
                distance = slope * pitch + MAX_DISTANCE;
            }
        }

    public:
        void setYaw(float yaw){
            this->yaw = yaw;
        }

        void setPitch(float pitch){
            this->pitch = pitch;
        }

        void increaseYaw(float delta = 1){
            yaw += sensitivity*delta;
            limitAngleToPositive(yaw);
        }

        void increasePitch(float delta = 1){
            pitch += sensitivity*delta;
            if(pitch > 75)
                pitch = 75;
            else if(pitch < -80)
                pitch = -80;

            recalculateDistance();
        }


        void setPosition(glm::vec3 pos){
            position = pos;
        }

        glm::vec3 getPosition(){
            return position;
        }

        glm::vec3 getDirection(){
            return camDirection;
        }

        void follow(Spaceship ship){
            float horizontal = cos(glm::radians(pitch));
            float vertical = sin(glm::radians(pitch));
            float x_offset = horizontal*sin(glm::radians(yaw));
            float z_offset = horizontal*cos(glm::radians(yaw));

            position = ship.getPosition();
            camDirection = glm::vec3(x_offset, vertical, z_offset);
            camDirection = glm::normalize(camDirection);
        }

        glm::mat4 getViewMatrix(glm::vec3 upVector = glm::vec3(0.0, 1.0, 0.0)){
            glm::vec3 camRight = glm::normalize(glm::cross(upVector, camDirection));
            glm::vec3 camUp = glm::cross(camDirection, camRight);
            glm::mat4 first = glm::transpose(glm::mat4( glm::vec4(camRight, 0.0),
                                                        glm::vec4(camUp, 0.0),
                                                        glm::vec4(camDirection, 0.0),
                                                        glm::vec4(glm::vec3(0.0), 1.0)));
            glm::mat4 second = glm::mat4(1.0);
            second[3] = glm::vec4(-position, 1.0);

            /*
            view  = |  Rx Ry  Rz  0.0 |   | 1.0 0.0 0.0 -Px |
                    |  Ux Uy  Uz  0.0 | * | 0.0 1.0 0.0 -Py |
                    |  Dx Dy  Dz  0.0 |   | 0.0 0.0 1.0 -Pz |
                    |  0  0   0   1.0 |   | 0.0 0.0 0.0 1.0 |
            */
            return first*second;
        }

        void invertPitch(){
            pitch *= -1;
        }

    };

    struct CameraData{
        glm::mat4 projection;
        glm::mat4 view;
    };

    /**
    *Logic:
    *       -Render the scene from sun's perspective
    *       -Store that value in depth texture
    *       -Use that depth value in comparing depth with that fragment
    *       -Use poisson disk distribution to adjust visibility with dark
    */
    class ShadowLoader{
    private:
        OpenGL::FrameBuffer fb;
        OpenGL::Texture depthTexture;
        int width, height;

        glm::vec3 lightDirection;
        glm::vec3 lightPosition;
        glm::mat4 projection;
        glm::mat4 view;

        /**
        *Calculates 8 positions of perspective position
        */
        std::vector<glm::vec3> calculateVertices(float cam_near, float cam_far, float cam_fov, float ar,
                                                 glm::vec3 camPosition, glm::vec3 direction){
            std::vector<glm::vec3> vertices;
            float nearHeight = cam_near*tan(glm::radians(cam_fov/2));
            float nearWidth = nearHeight*ar;
            float farHeight = cam_far*tan(glm::radians(cam_fov/2));
            float farWidth = farHeight*ar;

            direction = glm::normalize(-direction);
            glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0, 1, 0), direction));
            glm::vec3 up = glm::normalize(glm::cross(direction, right));
            glm::vec3 nearCentre = camPosition + cam_near*direction;
            glm::vec3 farCentre = camPosition + cam_far*direction;

            vertices.push_back(nearCentre + nearHeight*up + nearWidth*right);//near top right
            vertices.push_back(nearCentre + nearHeight*up - nearWidth*right);//near top left
            vertices.push_back(nearCentre - nearHeight*up + nearWidth*right);//near button right
            vertices.push_back(nearCentre - nearHeight*up - nearWidth*right);//near button left
            vertices.push_back(farCentre + farHeight*up + farWidth*right);   //far top right
            vertices.push_back(farCentre + farHeight*up - farWidth*right);   //far top left
            vertices.push_back(farCentre - farHeight*up + farWidth*right);   //far bottom right
            vertices.push_back(farCentre - farHeight*up - farWidth*right);   //far button left
            return vertices;
        }

    public:
        /**
        *@param width and height is resolution of depth texture in pixels
        *@param light direction is direction of light from where shadow to be mapped
        */
        void loadData(int width, int height, int screenWidth, int screenHeight, glm::vec3 lightDirection){
            this->width = width;
            this->height = height;
            this->lightDirection = glm::normalize(lightDirection);
            fb.generateFrameBuffer(false);
            fb.bind(width, height);
            fb.AttachColorTexture(width, height);
            depthTexture.loadCreatedTexture(fb.AttachDepthTexture(width, height));
            fb.unbind(screenWidth, screenHeight);
        }

        /**
        *All drawing after sampling will have shadow
        */
        void startSampling(){
            fb.bind(width, height);
        }

        /**
        *Stops shadow sampling and draws in the screen
        */
        void stopSampling(int screenWidth, int screenHeight){
            fb.unbind(screenWidth, screenHeight);
        }

        /**
        *@param camera and other data from where screen data is rendered
        *@return the camera data from where objects to be rendered for shadow depth sampling
        *These data should be used while rendering while sampling the depth value
        */
        CameraData getCameraData(Camera camera, float cam_fov, float cam_near, float cam_far, float ar, float lightDistance,float proj_offset = 1.0){
            std::vector<glm::vec3> vertices;
            vertices = calculateVertices(cam_near, cam_far, cam_fov, ar, camera.getPosition(), camera.getDirection());
            glm::vec3 diagonal = vertices[0]-vertices[7];
            diagonal.y = 0;
            float distance = proj_offset+glm::length(diagonal);
            glm::vec3 sumPoint = glm::vec3(0.0);
            for(unsigned int i=0; i<vertices.size(); i++)
                sumPoint += vertices[i];
            sumPoint /= vertices.size();
            lightPosition = sumPoint - lightDirection;
            glm::mat4 projection = glm::ortho(-distance/2,distance/2,-distance/2,distance/2,-lightDistance/2,lightDistance/2);
            glm::mat4 view = glm::lookAt(lightPosition, sumPoint, glm::vec3(0, 1, 0));
            return {projection, view};
        }

        /**
        *Binds the depth texture at provided index
        */
        void bindDepthTexture(unsigned int i){
            depthTexture.bind(i);
        }

        void cleanUP(){
            fb.deleteBuffer();
            depthTexture.deleteTexture();
        }
    };

    class MousePicker{
    private:
        glm::vec3 currentRay;
        glm::mat4 view;
        glm::mat4 projection;

        glm::vec2 getNormalizedSpace(float x, float y, float display_width, float display_height){
            float norm_x = 2*x/display_width - 1.0;
            float norm_y = 2*y/display_height - 1.0;
            norm_y *= -1; //flipping y co-ordinate because co-ordinate starts from top left corner
            return glm::vec2(norm_x, norm_y);
        }

        glm::vec4 clipToEyeSpace(glm::vec4 clipSpace){
            glm::mat4 projection_inverse = glm::inverse(projection);
            glm::vec4 eyeCords = projection_inverse * clipSpace;
            return glm::vec4(eyeCords.x, eyeCords.y, -1.0f, 0.0f);
        }

        glm::vec3 eyeToWorld(glm::vec4 eyeSpace){
            glm::mat4 view_inverse = glm::inverse(view);
            glm::vec4 worldCords = view_inverse * eyeSpace;
            glm::vec3 result = glm::vec3(worldCords.x, worldCords.y, worldCords.z);
            result = glm::normalize(result);
            return result;
        }
    public:
        void setMatrices(glm::mat4 view, glm::mat4 projection){
            this->view = view;
            this->projection = projection;
        }

        glm::vec3 getRay(){
            return currentRay;
        }

        /**
        *Using normalized space
        */
        glm::vec3 calculateRay(float x,float y){
            glm::vec4 clipCords = glm::vec4( x, y, -1.0f, 1.0f);
            glm::vec4 eyeSpace = clipToEyeSpace(clipCords);
            glm::vec3 worldSpace = eyeToWorld(eyeSpace);
            return worldSpace;
        }

        /**
        *Using screen co-ordinate space
        */
        glm::vec3 calculateRay(float x,float y, float width, float height)
        {
            glm::vec2 normalized = getNormalizedSpace(x, y, width, height);
            return calculateRay(normalized.x, normalized.y);
        }

    };

    class FPSManager{
    private:
        int targetFPS;
        int countedFPS;
        double initial_time;
        double final_time;
    public:
        FPSManager(int targetFPS){
            this->targetFPS = targetFPS;
            this->countedFPS = targetFPS;
        }

        /**
        *Sets the FPS to be limited
        */
        void setTargetFPS(int targetFPS){
            this->targetFPS = targetFPS;
            this->countedFPS = targetFPS;
        }

        /**
        *Updates the counting of fps and can display too
        */
        void updateCounter(bool displayFPS=false){
            static int frameCount = 0;
            static double lastCountTime = glutGet(GLUT_ELAPSED_TIME);
            float timefromLastCount = glutGet(GLUT_ELAPSED_TIME)-lastCountTime;
            //If more than one second is elapsed
            if( timefromLastCount >= 1000.0){
                countedFPS = frameCount;
                //Resets frame counter and sets time reference for time count
                frameCount = 0;
                lastCountTime = glutGet(GLUT_ELAPSED_TIME);

                if(displayFPS){
                    std::cout<<"Counted FPS = "<<countedFPS<<std::endl;
                }
            }
            frameCount++;
        }

        /**
        *Initiate at the starting of display
        *Necessary to limit FPS
        */
        void initiateTimer(){
            initial_time = glutGet(GLUT_ELAPSED_TIME);
        }

        /**
        *Timer should have been initiated for this to work
        */
        void limitFPS(bool useCountedFPS=true, bool showWarning=true){
            //Limiting FPS
    //        float targetTime = 1000.0/float(targetFPS);
    //        int frame_lag_in_second = targetFPS - countedFPS;
    //        float time_per_frame = 2000.0/float(targetFPS+countedFPS);
    //        float frame_lag_per_frame = frame_lag_in_second*time_per_frame/1000.0;
    //        float time_lag_per_frame = useCountedFPS?(frame_lag_per_frame*time_per_frame):0;

            float offset = 0.95;
            float targetTime = 1000.0/float(targetFPS);
            int frame_lag_in_second = targetFPS - countedFPS;
            float time_lag_per_frame = useCountedFPS?(offset+2.0*float(frame_lag_in_second)/float(targetFPS+countedFPS)):0;

            final_time = glutGet(GLUT_ELAPSED_TIME);
            double delay_time =  targetTime - (final_time-initial_time) - time_lag_per_frame;
            if(delay_time > 0)   Sleep(delay_time);
            else if(showWarning) std::cout<<"Warning: FPS lagging this frame!!!"<<std::endl;
        }

        /**
        *Returns the counted FPS
        */
        int getCountedFPS(){
            return countedFPS;
        }
    };
};
