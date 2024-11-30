#include "Application.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "Window.hpp"
#include "Renderer.hpp"
#include "Buffer.hpp"
#include "Mesh.hpp"

UniformBufferObject createUniformBuffer(int width, int height, float totalTime, Camera& cam)
{
    UniformBufferObject ubo{};
    ubo.model = glm::rotate(glm::mat4(1.0f),glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = cam.view;
    ubo.proj = glm::perspective(glm::radians(45.0f), width / (float)height, 0.1f, 1000.0f);
    ubo.proj[1][1] *= -1;
    ubo.directionnalLight = glm::vec4(glm::normalize(glm::vec3{-1,0.0,-1 }), 1.0);
    ubo.cameraFront = glm::vec4(cam.front,1.0);
    ubo.cameraPosition = glm::vec4(cam.position, 1.0);
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
    float xoffset = mousePos.x - (screenSize.x/2);
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

    Mesh meshGround = renderer.createMesh(verticesGround, indicesGround);
    Mesh meshSkybox = renderer.createMesh(verticesSkybox, indicesSkybox);
    Mesh meshGrass = renderer.createMesh("Models/grass2.obj");

    Texture texture = renderer.createTexture("Textures/grass.png");
    Texture textureSkybox = renderer.createCubeMap({ "Textures/skybox/nx.png","Textures/skybox/px.png","Textures/skybox/py.png","Textures/skybox/ny.png","Textures/skybox/nz.png","Textures/skybox/pz.png" });

    UniformBuffer uniforms = renderer.createUniformBuffer<UniformBufferObject>();
    ShaderStorageBufferObject ssboGrass = renderer.createShaderStorageBuffer(sizeof(GrassBladeData));
    std::cout << sizeof(glm::vec4) << ' ' << sizeof(GrassBladeData) << std::endl;

    std::array<glm::vec4, 2> grassData;
    grassData[0] = { GRASS_FIELD_POS_X, GRASS_FIELD_POS_Y, GRASS_FIELD_SIZE, NB_BLADES };
    grassData[1] = { WIND_DIRECTION, WIND_POWER, WIND_FREQU, WIND_AMPL };

    renderer.updateSSBO<std::array<glm::vec4, 2>>(ssboGrass, grassData,0);


    DescriptorPool pool = renderer.createDescriptorPool({ {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,1},{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,5}, {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 2 } }, 20);

    DescriptorSetLayout layoutUb = renderer.createDescriptorSetLayoutBuffer();
    DescriptorSetLayout layoutText = renderer.createDescriptorSetlayout(texture);
    DescriptorSetLayout layoutGrassSSBO = renderer.createDescriptorSetLayoutGrass();
    DescriptorSetLayout layoutSkybox = renderer.createDescriptorSetLayoutSkybox();
    DescriptorSetLayout layoutGrassSSBOCompute = renderer.createDescriptorSetLayoutGrassCompute();

    DescriptorSet descriptorSetUb = renderer.createDescriptorSet(layoutUb, pool, uniforms);
    DescriptorSet descriptorSetText = renderer.createDescriptorSet(layoutText, pool, texture);
    DescriptorSet descriptorSetGrass = renderer.createDescriptorSetGrass(layoutGrassSSBO, pool, ssboGrass, texture);
    DescriptorSet descriptorSetSkybox= renderer.createDescriptorSetSkybox(layoutSkybox, pool, textureSkybox);
    DescriptorSet descriptorSetGrassCompute = renderer.createDescriptorSetGrassCompute(layoutGrassSSBOCompute, pool, ssboGrass);

    GraphicsPipeline pipeline = renderer.createGraphicsPipeline("Shaders/vert.spv","Shaders/colorfrag.spv", { layoutUb,layoutText });
    GraphicsPipeline pipelineGrass = renderer.createGraphicsPipeline("Shaders/grassvert.spv","Shaders/grassfrag.spv", { layoutUb, layoutGrassSSBO });
    GraphicsPipeline pipelineSkybox = renderer.createGraphicsPipeline("Shaders/skyboxvert.spv","Shaders/skyboxfrag.spv", { layoutUb, layoutSkybox }, false);

    ComputePipeline pipelineGrassGen = renderer.createComputePipeline("Shaders/grassGenComp.spv", { layoutGrassSSBOCompute });

    
    renderer.startComputeRecoring();
    renderer.bindComputePipeline(pipelineGrassGen);
    renderer.bindDescriptorSet(descriptorSetGrassCompute, 0, VK_PIPELINE_BIND_POINT_COMPUTE);
    renderer.endComputeRecoring(NB_BLADES, 1, 1);
    renderer.waitDeviceIdle(); //Not a problem there but need to be replace with fence

    while (!window->ShouldClose()) {
        window->PollEvents();
        
        // Update

        static auto startTime = std::chrono::high_resolution_clock::now();
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - lastFrameTime).count();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
        lastFrameTime = currentTime;

        moveCamera(camera,window,deltaTime);
        auto matrices = createUniformBuffer(renderer.getSwapchainWidth(), renderer.getSwapchainHeight(), time, camera);
        renderer.updateUniformBuffer<UniformBufferObject>(uniforms, matrices);

        // Draw
        renderer.startFrame();

        renderer.bindGraphicsPipeline(pipelineSkybox);
        renderer.bindDescriptorSet(descriptorSetUb);
        renderer.bindDescriptorSet(descriptorSetSkybox, 1);
        renderer.drawMesh(meshSkybox);
        
        renderer.bindGraphicsPipeline(pipeline);
        renderer.bindDescriptorSet(descriptorSetUb);
        renderer.bindDescriptorSet(descriptorSetText,1);
        renderer.drawMesh(meshGround);
        
        renderer.bindGraphicsPipeline(pipelineGrass);
        renderer.bindDescriptorSet(descriptorSetUb);
        renderer.bindDescriptorSet(descriptorSetGrass, 1);
        renderer.drawMesh(meshGrass, NB_BLADES);
        

        renderer.endFrame();
    }
    renderer.waitDeviceIdle();

    renderer.destroyDescriptorPool(pool);
    renderer.destroyDescriptorSetLayout(layoutText);
    renderer.destroyDescriptorSetLayout(layoutUb);
    renderer.destroyDescriptorSetLayout(layoutGrassSSBO);
    renderer.destroyDescriptorSetLayout(layoutSkybox);
    renderer.destroyDescriptorSetLayout(layoutGrassSSBOCompute);

    renderer.destroyComputePipeline(pipelineGrassGen);
    renderer.destroyGraphicsPipeline(pipeline);
    renderer.destroyGraphicsPipeline(pipelineGrass);
    renderer.destroyGraphicsPipeline(pipelineSkybox);

    renderer.destroyUniformBuffer(uniforms);
    renderer.destroyShaderStorageBufferObject(ssboGrass);
    renderer.destroyTexture(texture);
    renderer.destroyTexture(textureSkybox);
    renderer.destroyMesh(meshGround);
    renderer.destroyMesh(meshSkybox);
    renderer.destroyMesh(meshGrass);

    renderer.destroy();
    window->Destroy();
    return 0;
}