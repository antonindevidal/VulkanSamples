#include "Application.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "Window.hpp"
#include "Renderer.hpp"



UniformBufferObject createUniformBuffer(int width, int height, float totalTime, Camera& cam)
{
    UniformBufferObject ubo{};
    ubo.model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = cam.view;
    ubo.proj = glm::perspective(glm::radians(45.0f), width / (float)height, 0.1f, 1000.0f);
    ubo.proj[1][1] *= -1;
    ubo.directionnalLight = glm::vec4(glm::normalize(glm::vec3{ cos(45.0),0.0,-sin(45.0) }), 1.0);
    ubo.cameraFront = glm::vec4(cam.front, 1.0);
    ubo.cameraPos = glm::vec4(cam.position, 1.0);
    ubo.time = totalTime;
    return ubo;
}

void moveCamera(Camera& camera, std::shared_ptr<Window> window, float deltaTime)
{

    float speed = CAMERA_SPEED * deltaTime;
    if (window->isKeyPressed(GLFW_KEY_W))
        camera.position += speed * camera.front;
    if (window->isKeyPressed(GLFW_KEY_S))
        camera.position -= speed * camera.front;
    if (window->isKeyPressed(GLFW_KEY_A))
        camera.position -= glm::normalize(glm::cross(camera.front, camera.up)) * speed;
    if (window->isKeyPressed(GLFW_KEY_D))
        camera.position += glm::normalize(glm::cross(camera.front, camera.up)) * speed;
    if (window->isKeyPressed(GLFW_KEY_LEFT_SHIFT))
        camera.position.z -= speed;
    if (window->isKeyPressed(GLFW_KEY_SPACE))
        camera.position.z += speed;

    auto mousePos = window->getMousePosition();
    auto screenSize = window->getFrameBufferSize();
    float xoffset = mousePos.x - (screenSize.x / 2);
    float yoffset = (screenSize.y / 2) - mousePos.y; // reversed since y-coordinates range from bottom to top

    xoffset *= CAMERA_SENSITIVITY;
    yoffset *= CAMERA_SENSITIVITY;


    camera.yaw += xoffset;
    camera.pitch += yoffset;
    if (camera.pitch > 89.0f)
        camera.pitch = 89.0f;
    if (camera.pitch < -89.0f)
        camera.pitch = -89.0f;

    glm::vec3 direction;
    direction.x = -cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    direction.y = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    direction.z = sin(glm::radians(camera.pitch));
    camera.front = glm::normalize(direction);

    camera.view = glm::lookAt(camera.position, camera.position + camera.front, camera.up);
    window->setCursorPosition(screenSize.x / 2, screenSize.y / 2);
}

int main() {
    std::chrono::steady_clock::time_point lastFrameTime;
    Camera camera;

    std::shared_ptr<Window> window = std::make_shared<Window>();
    window->Create(windowName);
    
    Renderer renderer{};
    renderer.createRenderer(window);

    Mesh meshSSRect = renderer.createMesh(verticesSSRect, indicesSSRect);

    UniformBuffer uniforms = renderer.createUniformBuffer<UniformBufferObject>();

    DescriptorPool pool = renderer.createDescriptorPool({ {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,1},{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,1} }, 3);
    DescriptorSetLayout layoutUb = renderer.createDescriptorSetlayout(uniforms);
    DescriptorSet descriptorSetUb = renderer.createDescriptorSet(layoutUb, pool, uniforms);


    GraphicsPipeline pipelineRM = renderer.createGraphicsPipeline("Shaders/raymarchingVert.spv", "Shaders/raymarchingFrag.spv", { layoutUb });


    while (!window->ShouldClose()) {
        window->PollEvents();
        
        // Update
        static auto startTime = std::chrono::high_resolution_clock::now();
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - lastFrameTime).count();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
        lastFrameTime = currentTime;

        moveCamera(camera, window, deltaTime);
        auto matrices = createUniformBuffer(renderer.getSwapchainWidth(), renderer.getSwapchainHeight(), time, camera);
        renderer.updateUniformBuffer<UniformBufferObject>(uniforms, matrices);

        // Draw
        renderer.startFrame();

        renderer.bindGraphicsPipeline(pipelineRM);
        renderer.bindDescriptorSet(descriptorSetUb);
        renderer.drawMesh(meshSSRect);

        renderer.endFrame();
    }
    renderer.waitDeviceIdle();

    renderer.destroyDescriptorPool(pool);
    renderer.destroyDescriptorSetLayout(layoutUb);
    renderer.destroyUniformBuffer(uniforms);
    renderer.destroyGraphicsPipeline(pipelineRM);

    renderer.destroyMesh(meshSSRect);

    renderer.destroy();
    window->Destroy();
    return 0;
}