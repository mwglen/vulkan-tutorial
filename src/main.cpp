#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>     
#include <stdexcept>
#include <cstdlib>    // Provides EXIT_SUCCESS and EXIT_FAILURE

const uint32_t WIDTH  = 800;
const uint32_t HEIGHT = 600;

class HelloTriangleApplication {
public:
   void run() {
      initWindow();
      initVulkan();
      mainLoop();
      cleanup();
   }

private:
   GLFWwindow* window;

   void initWindow() {
      // Initialize GLFW library
      glfwInit(); 

      // Dont create an OpenGL context
      glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

      // Disable Window Resize
      glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

      // Create Window
      window = glfwCreateWindow(800, 600, "Vulkan", nullptr, nullptr);
   }

   void initVulkan() {

   }

   void mainLoop() {
      while (!glfwWindowShouldClose(window)) {
         glfwPollEvents();
      }
   }

   void cleanup() {
      glfwDestroyWindow(window);
      glfwTerminate();
   }
};

int main() {
   HelloTriangleApplication app;

   try {
      app.run();
   } catch (const std::exception& e) {
      std::cerr << e.what() << std::endl;
      return EXIT_FAILURE;
   }

   return EXIT_SUCCESS;
}
