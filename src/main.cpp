#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <map>
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
   VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

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
      //setupDebugMessenger();
      pickPhysicalDevice();
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
      // Will fail if requested extensions are not available
      if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
         throw std::runtime_error("failed to create vulkan instance!");
   }

   void pickPhysicalDevice() {

      // Count physical devices
      uint32_t deviceCount = 0;
      vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

      // Error if no devices
      if (deviceCount == 0)
         throw std::runtime_error("failed to find GPUs with Vulkan support!");

      // Get physical devices
      std::vector<VkPhysicalDevice> devices(deviceCount);
      vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

      /// Find most suitable device
      // Use an orddered map to automatically sort candidates by increasing score
      std::multimap<int, VkPhysicalDevice> candidates;

      // Calculate score for each device
      for (const auto& device : devices) {
         int score = rateDeviceSuitability(device);
         candidates.insert(std::make_pair(score, device));
      }

      // Pick out the most suitable device
      if (candidates.rbegin()->first > 0) physicalDevice = candidates.rbegin()->second;
      else throw std::runtime_error("failed to find a suitable GPU!");
      
   }
   
   int rateDeviceSuitability(VkPhysicalDevice device) {

      // Get device properties
      VkPhysicalDeviceProperties deviceProperties;
      vkGetPhysicalDeviceProperties(device, &deviceProperties);

      // Get device features
      VkPhysicalDeviceFeatures deviceFeatures;
      vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

      // Start with a score of zero
      int score = 0;

      // Always Prefer Discrete GPUs
      if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
         score += 1000; 
      }

      // Maximum possible size of textures affects graphics quality
      score += deviceProperties.limits.maxImageDimension2D;

      // Return the score
      return score;

   }

   void mainLoop() {
      while (!glfwWindowShouldClose(window)) {
         glfwPollEvents();
      }
   }

   void cleanup() {
      vkDestroyInstance(instance, nullptr);
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
