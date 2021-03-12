#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <stdio.h>
#include <unistd.h>
#include <string.h>

const char *vertexShaderSource ="#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 ourColor;\n"
    "out vec2 TexCoord; \n"
    "uniform mat4 model; \n"
    "uniform mat4 view; \n"
    "uniform mat4 projection; \n"
    "void main()\n"
    "{\n"
    "   gl_Position = projection * view * model * vec4(aPos, 1.0f);\n"
    "   ourColor = aColor;\n"
    "}\0";

const char *vectorShaderTexture = "#version 330 core\n"
   "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec2 aTexCoord; \n"
    "out vec2 TexCoord; \n"
    "uniform mat4 model; \n"
    "uniform mat4 view; \n"
    "uniform mat4 projection; \n"
    "void main()\n"
    "{\n"
    "   gl_Position = projection * view * model * vec4(aPos, 1.0f);\n"
    "   TexCoord = aTexCoord; \n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "       FragColor = vec4(ourColor.x, ourColor.y, ourColor.z, 1.0f);\n"
    "}\n\0";

const char *fragmentShaderTexture = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec2 TexCoord; \n"
    "uniform sampler2D texture1; \n"
    "void main()\n"
    "{\n"
    "      FragColor = texture(texture1, TexCoord);\n"
    "}\n\0";


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

bool spinObj = 0, spinCam=0;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 objPosition = glm::vec3(0.0f, 0.0f, 0.0f);



void processInput(GLFWwindow *window)
{
   const float cameraSpeed = 0.05f;
   const float objectSpeed = 0.05f;
   if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      cameraPos.y += cameraSpeed;
   if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      cameraPos.y -= cameraSpeed;
   if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      cameraPos.x -= cameraSpeed;
   if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      cameraPos.x += cameraSpeed;
   if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
      cameraPos.z += cameraSpeed;
   if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
      cameraPos.z -= cameraSpeed;

   if(glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
      objPosition.y += objectSpeed;
   if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
      objPosition.y -= objectSpeed;
   if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
      objPosition.x -= objectSpeed;
   if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
      objPosition.x += objectSpeed;
   if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
      objPosition.z += objectSpeed;
   if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
      objPosition.z -= objectSpeed;


   if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
   {
      glfwSetWindowShouldClose(window, true);
   }

   if(glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
   {
      spinObj = 1;
   }
   if(glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
   {
      spinObj = 0;
   }

   if(glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
   {
      spinCam = 1;
   }
   if(glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
   {
      spinCam = 0;
   }

   if(glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
   {
      cameraPos = glm::vec3(3.0f, 3.0f, 3.0f);
      cameraFront = -glm::vec3(3.0f, 3.0f, 3.0f) + objPosition ;
   }

   if(glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
   {
      cameraPos = glm::vec3(3.0f, 0.0f, 0.0f);
      cameraFront = -glm::vec3(3.0f, 0.0f, 0.0f) + objPosition ;
   }

   if(glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
   {
      cameraPos = glm::vec3(2.0f, 6.0f, -3.0f);
      cameraFront = -glm::vec3(2.0f, 6.0f, -3.0f) + objPosition ;
   }
   
   
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

namespace {
   void errorCallback(int error, const char* description) {
      fprintf(stderr, "GLFW error %d: %s\n", error, description);
   }

   GLFWwindow* initialize() {
      int glfwInitRes = glfwInit();
      if (!glfwInitRes) {
         fprintf(stderr, "Unable to initialize GLFW\n");
         return nullptr;
      }

      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
      // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

      GLFWwindow* window = glfwCreateWindow(800, 600, "InitGL", nullptr, nullptr);
      if (!window) {
         fprintf(stderr, "Unable to create GLFW window\n");
         glfwTerminate();
         return nullptr;
      }

      glfwMakeContextCurrent(window);
      glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

      int gladInitRes = gladLoadGL();
      if (!gladInitRes) {
         fprintf(stderr, "Unable to initialize glad\n");
         glfwDestroyWindow(window);
         glfwTerminate();
         return nullptr;
      }

      return window;
   }
}

int main(int argc, char* argv[]) {

   int object_type;
   fprintf(stdout, "Select the solid to be printed\n 1. Hexagonal Dipyramid \n 2. Elongated square dipyramid \n 3. Decagonal Prism \n ");
   scanf("%d", &object_type);

   if(object_type != 1 && object_type!= 2 && object_type != 3)
   {
      fprintf(stdout, "Invalid option\n");
      return 0;
   }
   

   glfwSetErrorCallback(errorCallback);

   GLFWwindow* window = initialize();
   if (!window) {
      return 0;
   }

   unsigned int vertexShader;
   vertexShader = glCreateShader(GL_VERTEX_SHADER);
   if(object_type != 3)
   {
      glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
   }
   else
   {
      glShaderSource(vertexShader, 1, &vectorShaderTexture, NULL);
      
   }
   glCompileShader(vertexShader);
   int success;
   char infoLog[512];
   glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
   if(!success)
   {
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      fprintf(stderr, "Unable to Compile Vertex Shader\n");
      return 0;
   }

   unsigned int fragmentShader;
   fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
   if(object_type != 3)
   {
      glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
   }
   else
   {
      glShaderSource(fragmentShader, 1, &fragmentShaderTexture, NULL);
   }
   glCompileShader(fragmentShader);
   glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
   if(!success)
   {
      glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
      fprintf(stderr, "Unable to Compile Fragment Shader\n");
      return 0;
   }

   unsigned int shaderProgram;
   shaderProgram = glCreateProgram();

   glAttachShader(shaderProgram, vertexShader);
   glAttachShader(shaderProgram, fragmentShader);
   glLinkProgram(shaderProgram);

   glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
   if(!success)
   {
      glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
      fprintf(stderr, "Unable to Link Shaders\n");
      return 0;
   }

   glDeleteShader(vertexShader);
   glDeleteShader(fragmentShader);


   float vertices1[] = {
         1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.86f, 0.0f,  1.0f, 1.0f, 0.0f,
         0.0f,  0.0f,  1.0f,  1.0f, 1.0f, 1.0f,
         
         1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.86f, 0.0f,  1.0f, 1.0f, 0.0f,
         0.0f,  0.0f,  -1.0f, 1.0f, 1.0f, 1.0f,

        -0.5f,  0.86f, 0.0f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.86f, 0.0f,  1.0f, 1.0f, 0.0f,
         0.0f,  0.0f,  1.0f,  1.0f, 1.0f, 1.0f,

         -0.5f,  0.86f, 0.0f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.86f, 0.0f,  1.0f, 1.0f, 0.0f,
         0.0f,  0.0f,  -1.0f, 1.0f, 1.0f, 1.0f,

        -0.5f,  0.86f, 0.0f,  0.0f, 1.0f, 0.0f,
        -1.0f,  0.0f,   0.0f,  0.0f, 1.0f, 1.0f,
        0.0f,  0.0f,  1.0f,  1.0f, 1.0f, 1.0f,

       -0.5f,  0.86f, 0.0f,  0.0f, 1.0f, 0.0f,
        -1.0f,  0.0f,   0.0f,  0.0f, 1.0f, 1.0f,
         0.0f,  0.0f,  -1.0f, 1.0f, 1.0f, 1.0f,

        -1.0f,  0.0f,   0.0f,  0.0f, 1.0f, 1.0f,
        -0.5f, -0.86f, 0.0f,  0.0f, 0.0f, 1.0f,
         0.0f,  0.0f,  1.0f,  1.0f, 1.0f, 1.0f,

         -1.0f,  0.0f,   0.0f,  0.0f, 1.0f, 1.0f,
        -0.5f, -0.86f, 0.0f,  0.0f, 0.0f, 1.0f,
         0.0f,  0.0f,  -1.0f, 1.0f, 1.0f, 1.0f,

        -0.5f, -0.86f, 0.0f,  0.0f, 0.0f, 1.0f,
         0.5f, -0.86f, 0.0f,  1.0f, 0.0f, 1.0f,
         0.0f,  0.0f,  1.0f,  1.0f, 1.0f, 1.0f,

         -0.5f, -0.86f, 0.0f,  0.0f, 0.0f, 1.0f,
         0.5f, -0.86f, 0.0f,  1.0f, 0.0f, 1.0f,
        0.0f,  0.0f,  -1.0f, 1.0f, 1.0f, 1.0f,

         0.5f, -0.86f, 0.0f,  1.0f, 0.0f, 1.0f,
         1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
         0.0f,  0.0f,  1.0f,  1.0f, 1.0f, 1.0f,

         0.5f, -0.86f, 0.0f,  1.0f, 0.0f, 1.0f,
         1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
         0.0f,  0.0f,  -1.0f, 1.0f, 1.0f, 1.0f,
    };

    float vertices2[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,
   0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.3f,
   -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,

    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.3f,
   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.3f,

    0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,
   
   // plane 2
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
    0.0f, -1.0f, 0.0f, 0.3f, 0.4f, 0.5f,

    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
    0.0f, -1.0f, 0.0f, 0.3f, 0.4f, 0.5f,    

    0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
   -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
    0.0f, -1.0f, 0.0f, 0.3f, 0.4f, 0.5f,

     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
     0.0f, -1.0f, 0.0f, 0.3f, 0.4f, 0.5f,


   // Plane1
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
    0.0f,  1.0f, 0.0f, 0.5f, 0.4f, 0.3f,

    0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.3f,
    0.0f,  1.0f, 0.0f, 0.5f, 0.4f, 0.3f,

    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.3f,
     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
     0.0f,  1.0f, 0.0f, 0.5f, 0.4f, 0.3f,

     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
     0.0f,  1.0f, 0.0f, 0.5f, 0.4f, 0.3f,


};


float vertices3[] = {

   0.0f, 0.25f, 0.0f, 0.0f, 0.0f,
   1.0f, 0.25f, 0.0f, 0.5f, 1.0f,
   0.809f, 0.25f, 0.58f, 1.0f, 0.0f, 
   
   0.0f, 0.25f, 0.0f, 0.0f, 0.0f,
   0.809f, 0.25f, 0.58f, 0.5f, 1.0f,
   0.309f, 0.25f, 0.95f, 1.0f, 0.0f,

   0.0f, 0.25f, 0.0f, 0.0f, 0.0f,
   0.309f, 0.25f, 0.95f, 0.5f, 1.0f,
   -0.309f, 0.25f, 0.95f, 1.0f, 0.0f,

   0.0f, 0.25f, 0.0f, 0.0f, 0.0f,
   -0.309f, 0.25f, 0.95f, 0.5f, 1.0f,
   -0.809f, 0.25f, 0.58f, 1.0f, 0.0f,

   0.0f, 0.25f, 0.0f, 0.0f, 0.0f,
   -0.809f, 0.25f, 0.58f, 0.5f, 1.0f,
   -1.0f, 0.25f, 0.0f, 1.0f, 0.0f,
   
   0.0f, 0.25f, 0.0f, 0.0f, 0.0f,
   -1.0f, 0.25f, 0.0f, 0.5f, 1.0f,
   -0.809f, 0.25f, -0.58f, 1.0f, 0.0f,

   0.0f, 0.25f, 0.0f, 0.0f, 0.0f,
   -0.809f, 0.25f, -0.58f,  0.5f, 1.0f,
   -0.309f, 0.25f, -0.95f, 1.0f, 0.0f,

   0.0f, 0.25f, 0.0f, 0.0f, 0.0f,
   -0.309f, 0.25f, -0.95f,0.5f, 1.0f,
   0.309f, 0.25f, -0.95f, 1.0f, 0.0f,

   0.0f, 0.25f, 0.0f, 0.0f, 0.0f,
   0.309f, 0.25f, -0.95f,0.5f, 1.0f,
   0.809f, 0.25f, -0.58f, 1.0f, 0.0f,

   0.0f, 0.25f, 0.0f, 0.0f, 0.0f,
   0.809f, 0.25f, -0.58f, 0.5f, 1.0f,
   1.0f, 0.25f, 0.0f, 1.0f, 0.0f,

   // negative plane

   0.0f, -0.25f, 0.0f, 0.0f, 0.0f,
   1.0f, -0.25f, 0.0f, 0.5f, 1.0f,
   0.809f, -0.25f, 0.58f, 1.0f, 0.0f, 
   
   0.0f, -0.25f, 0.0f, 0.0f, 0.0f,
   0.809f, -0.25f, 0.58f, 0.5f, 1.0f,
   0.309f, -0.25f, 0.95f, 1.0f, 0.0f,

   0.0f, -0.25f, 0.0f, 0.0f, 0.0f,
   0.309f, -0.25f, 0.95f, 0.5f, 1.0f,
   -0.309f, -0.25f, 0.95f, 1.0f, 0.0f,

   0.0f, -0.25f, 0.0f, 0.0f, 0.0f,
   -0.309f, -0.25f, 0.95f, 0.5f, 1.0f,
   -0.809f, -0.25f, 0.58f, 1.0f, 0.0f,

   0.0f, -0.25f, 0.0f, 0.0f, 0.0f,
   -0.809f, -0.25f, 0.58f, 0.5f, 1.0f,
   -1.0f, -0.25f, 0.0f, 1.0f, 0.0f,
   
   0.0f, -0.25f, 0.0f, 0.0f, 0.0f,
   -1.0f, -0.25f, 0.0f, 0.5f, 1.0f,
   -0.809f, -0.25f, -0.58f, 1.0f, 0.0f,

   0.0f, -0.25f, 0.0f, 0.0f, 0.0f,
   -0.809f, -0.25f, -0.58f,  0.5f, 1.0f,
   -0.309f, -0.25f, -0.95f, 1.0f, 0.0f,

   0.0f, -0.25f, 0.0f, 0.0f, 0.0f,
   -0.309f, -0.25f, -0.95f,0.5f, 1.0f,
   0.309f, -0.25f, -0.95f, 1.0f, 0.0f,

   0.0f, -0.25f, 0.0f, 0.0f, 0.0f,
   0.309f, -0.25f, -0.95f,0.5f, 1.0f,
   0.809f, -0.25f, -0.58f, 1.0f, 0.0f,

   0.0f, -0.25f, 0.0f, 0.0f, 0.0f,
   0.809f, -0.25f, -0.58f, 0.5f, 1.0f,
   1.0f, -0.25f, 0.0f, 1.0f, 0.0f,

   // squares

   1.0f, -0.25f, 0.0f, 0.0f, 0.0f,
   1.0f, +0.25f, 0.0f, 1.0f, 0.0f,
   0.809f, -0.25f, 0.58f, 0.0f, 1.0f,
   0.809f, -0.25f, 0.58f, 0.0f, 1.0f,
   0.809f, +0.25f, 0.58f, 1.0f, 1.0f,
   1.0f, +0.25f, 0.0f, 1.0f, 0.0f,

   0.809f, -0.25f, 0.58f, 0.0f, 0.0f,
   0.809f, +0.25f, 0.58f, 1.0f, 0.0f,
   0.309f, -0.25f, 0.95f, 0.0f, 1.0f,
   0.309f, -0.25f, 0.95f, 0.0f, 1.0f,
   0.309f, +0.25f, 0.95f, 1.0f, 1.0f,
   0.809f, +0.25f, 0.58f, 1.0f, 0.0f,

   0.309f, -0.25f, 0.95f, 0.0f, 0.0f,
   0.309f, +0.25f, 0.95f, 1.0f, 0.0f,
   -0.309f, -0.25f, 0.95f, 0.0f, 1.0f,
   -0.309f, -0.25f, 0.95f, 0.0f, 1.0f,
   -0.309f, +0.25f, 0.95f, 1.0f, 1.0f,
   0.309f, +0.25f, 0.95f, 1.0f, 0.0f,

   -0.309f, +0.25f, 0.95f, 1.0f, 0.0f,
   -0.309f, -0.25f, 0.95f, 0.0f, 0.0f,
   -0.809f, -0.25f, 0.58f, 0.0f, 1.0f,
   -0.809f, -0.25f, 0.58f, 0.0f, 1.0f,
   -0.809f, +0.25f, 0.58f, 1.0f, 1.0f,
   -0.309f, +0.25f, 0.95f, 1.0f, 0.0f,

    -0.809f, +0.25f, 0.58f, 1.0f, 0.0f,
     -0.809f, -0.25f, 0.58f, 0.0f, 0.0f,
     -1.0f, -0.25f, 0.0f, 0.0f, 1.0f,
     -1.0f, -0.25f, 0.0f, 0.0f, 1.0f,
     -1.0f, +0.25f, 0.0f, 1.0f, 1.0f,
   -0.809f, +0.25f, 0.58f, 1.0f, 0.0f,

   -1.0f, +0.25f, 0.0f, 1.0f, 0.0f,
   -1.0f, -0.25f, 0.0f, 0.0f, 0.0f,
   -0.809f, -0.25f, -0.58f, 0.0f, 1.0f,
   -0.809f, -0.25f, -0.58f, 0.0f, 1.0f,
   -0.809f, +0.25f, -0.58f, 1.0f, 1.0f, 
   -1.0f, +0.25f, 0.0f, 1.0f, 0.0f,

   -0.809f, +0.25f, -0.58f, 1.0f, 0.0f,
   -0.809f, -0.25f, -0.58f, 0.0f, 0.0f,
   -0.309f, -0.25f, -0.95f, 0.0f, 1.0f,
   -0.309f, -0.25f, -0.95f, 0.0f, 1.0f,
   -0.309f, +0.25f, -0.95f, 1.0f, 1.0f,
   -0.809f, +0.25f, -0.58f, 1.0f, 0.0f,

   -0.309f, +0.25f, -0.95f, 1.0f, 0.0f,
   -0.309f, -0.25f, -0.95f, 0.0f, 0.0f,
   0.309f, -0.25f, -0.95f, 0.0f, 1.0f,
   0.309f, -0.25f, -0.95f, 0.0f, 1.0f,
   0.309f, +0.25f, -0.95f, 1.0f, 1.0f,
   -0.309f, 0.25f, -0.95f, 1.0f, 0.0f,

   0.309f, +0.25f, -0.95f, 1.0f, 0.0f,
   0.309f, -0.25f, -0.95f, 0.0f, 0.0f,
   0.809f, -0.25f, -0.58f, 0.0f, 1.0f,
   0.809f, -0.25f, -0.58f, 0.0f, 1.0f,
   0.809f, +0.25f, -0.58f, 1.0f, 1.0f,
   0.309f, +0.25f, -0.95f, 1.0f, 0.0f,

   0.809f, +0.25f, -0.58f, 1.0f, 0.0f,
   0.809f, -0.25f, -0.58f, 0.0f, 0.0f,
   1.0f, -0.25f, 0.0f, 0.0f, 1.0f,
   1.0f, -0.25f, 0.0f, 0.0f, 1.0f,
   1.0f, +0.25f, 0.0f, 1.0f, 1.0f,
    0.809f, +0.25f, -0.58f, 1.0f, 0.0f

};


   unsigned int VBO, VAO, EBO;
   glGenVertexArrays(1, &VAO);
   glGenBuffers(1, &VBO);
   glGenBuffers(1, &EBO);

   glBindVertexArray(VAO);

   int vertex_num;
   glBindBuffer(GL_ARRAY_BUFFER, VBO);

   if(object_type == 1)
   {
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1,GL_STATIC_DRAW);
      vertex_num = 36;
   }

   else if(object_type == 2)
   {
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2,GL_STATIC_DRAW);
      vertex_num = 60;
   }

   else
   {
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3,GL_STATIC_DRAW);
      vertex_num = 120;
   }
   
   if(object_type != 3)
   {
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
      glEnableVertexAttribArray(0);

      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3 * sizeof(float)));
      glEnableVertexAttribArray(1);
   }
   else
   {
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
      glEnableVertexAttribArray(0);

      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3 * sizeof(float)));
      glEnableVertexAttribArray(1);
   }

   unsigned int texture1;

   if(object_type == 3)
   {
      glGenTextures(1, &texture1);
      glBindTexture(GL_TEXTURE_2D, texture1);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      char buffer[PATH_MAX];

      getcwd(buffer, sizeof(buffer));
      strcat(buffer, "/wall.jpg");
      int width, height, nrChannels;
      unsigned char *data = stbi_load(buffer, &width, &height, &nrChannels, 0);
      if(data!=NULL)
      {
         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,GL_UNSIGNED_BYTE, data);
         glGenerateMipmap(GL_TEXTURE_2D);
         glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
      }
      else
      {
         fprintf(stderr, "Unable to load texture \n");
         return 0;
      }
      stbi_image_free(data);
   }
   
   glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

   glUseProgram(shaderProgram);
   

   glm::mat4 projection;
   projection = glm::perspective(glm::radians(45.0f), 800.0f/600.0f, 0.1f, 100.0f);

   glEnable(GL_DEPTH_TEST);
  
   while (!glfwWindowShouldClose(window)) {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      processInput(window);


      glm::mat4 view;
      view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
      if(spinCam)
      {
         view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
         float radius = 4.0f;
         float camX   = sin(glfwGetTime()) * radius;
         float camZ   = cos(glfwGetTime()) * radius;
         view = glm::lookAt(objPosition + glm::vec3(camX, 0.0f, camZ),objPosition, glm::vec3(0.0f, 1.0f, 0.0f));
      }
      

      int viewLoc = glGetUniformLocation(shaderProgram, "view");
      glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

      int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
      glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

      int modelLoc = glGetUniformLocation(shaderProgram, "model");

      if(object_type == 3)
      {
         glActiveTexture(GL_TEXTURE0);
         glBindTexture(GL_TEXTURE_2D, texture1);
      }
      glBindVertexArray(VAO);
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, objPosition);
      if(spinObj)
      {
         model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(1.0f, 0.0f, 0.0f));
      }


      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
      glDrawArrays(GL_TRIANGLES, 0, vertex_num);
      
       glfwSwapBuffers(window);
      glfwPollEvents();
   }

   glDeleteVertexArrays(1, &VAO);
   glDeleteBuffers(1, &VBO);
   glDeleteProgram(shaderProgram);

   glfwDestroyWindow(window);
   glfwTerminate();

   return 0;
}

