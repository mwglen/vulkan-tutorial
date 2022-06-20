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
   VkInstance  instance;

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
      createInstance();
   }
   
   // Create an instance (connects application to vulkan library)
   void createInstance() {

      // Create a struct with information about our application
      VkApplicationInfo appInfo{};
      appInfo.pApplicationName = "Hello Triangle";
      appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
      appInfo.pEngineName = "No Engine";
      appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
      appInfo.apiVersion = VK_API_VERSION_1_0;

      // Create instance info struct from application info struct
      VkInstanceCreateInfo createInfo{};
      createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
      createInfo.pApplicationInfo = &appInfo;

      /// Add glfw extension information to instance info struct
      // Let GLFW tell us which extensions it needs
      uint32_t glfwExtensionCount = 0;
      const char** glfwExtensions;
      glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

      // Insert the response into the instance info struct
      createInfo.enabledExtensionCount = glfwExtensionCount;
      createInfo.ppEnabledExtensionNames = glfwExtensions;


      /// Tell which global validation layers to enable
      createInfo.enabledLayerCount = 0;

      // Create Instance (second parameter is for custom allocator callbacks)
      VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

      // Verify that the instance was created successfully
      if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
         throw std::runtime_error("failed to create vulkan instance!");
      }
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
