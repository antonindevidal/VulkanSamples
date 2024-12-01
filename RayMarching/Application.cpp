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
    ubo.directionnalLight = glm::vec4(glm::normalize(glm::vec3{ cos(totalTime),sin(totalTime),-1}), 1.0);
    ubo.cameraFront = glm::vec4(cam.front, 1.0);
    ubo.cameraPos = glm::vec4(cam.position, 1.0);
    ubo.windowData = glm::vec4(width, height, CAMERA_FOV, 0.0);
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

    if (window->isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
    {
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

        window->setCursorPosition(screenSize.x / 2, screenSize.y / 2);
        window->setCursorVisible(false);
    }
    else
    {
        window->setCursorVisible(true);
    }
    camera.view = glm::lookAt(camera.position, camera.position + camera.front, camera.up);
}

int main() {
    std::chrono::steady_clock::time_point lastFrameTime;
    Camera camera;

    std::shared_ptr<Window> window = std::make_shared<Window>();
    window->Create(windowName);
    
    Renderer renderer{};
    renderer.createRenderer(window);

    Mesh meshSSRect = renderer.createMesh(verticesSSRect, indicesSSRect);
    Mesh meshSkybox = renderer.createMesh(verticesSkybox, indicesSkybox);

    Texture textureSkybox = renderer.createCubeMap({ "Textures/skybox/nx.png","Textures/skybox/px.png","Textures/skybox/py.png","Textures/skybox/ny.png","Textures/skybox/nz.png","Textures/skybox/pz.png" });

    UniformBuffer uniforms = renderer.createUniformBuffer<UniformBufferObject>();

    DescriptorPool pool = renderer.createDescriptorPool({ {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,1},{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,1} }, 3);

    DescriptorSetLayout layoutUb = renderer.createDescriptorSetlayout(uniforms);
    DescriptorSetLayout layoutSkybox = renderer.createDescriptorSetLayoutSkybox();

    DescriptorSet descriptorSetUb = renderer.createDescriptorSet(layoutUb, pool, uniforms);
    DescriptorSet descriptorSetSkybox = renderer.createDescriptorSetSkybox(layoutSkybox, pool, textureSkybox);

    GraphicsPipeline pipelineRM = renderer.createRaymarchingGraphicsPipeline("Shaders/raymarchingVert.spv", "Shaders/raymarchingFrag.spv", { layoutUb });
    GraphicsPipeline pipelineSkybox = renderer.createGraphicsPipeline("Shaders/skyboxvert.spv", "Shaders/skyboxfrag.spv", { layoutUb, layoutSkybox });

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

        // Skybox
        renderer.bindGraphicsPipeline(pipelineSkybox);
        renderer.bindDescriptorSet(descriptorSetUb);
        renderer.bindDescriptorSet(descriptorSetSkybox, 1);
        renderer.drawMesh(meshSkybox);

        // Ray marching
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