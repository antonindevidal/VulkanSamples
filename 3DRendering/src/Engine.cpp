#include "Engine.hpp"
UniformBufferObject createMatrices(int width, int height)
{
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    UniformBufferObject ubo{};
    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f), width / (float)height, 0.1f, 10.0f);
    ubo.proj[1][1] *= -1;

    return ubo;
}
Engine::Engine()
{
	//s_Instance = std::make_shared<Engine>(this);

	_window = std::make_shared<Window>();
	_renderer = std::make_shared<Renderer>();


	std::string name = "My engine";
	_window->Create(name);
	_renderer->createRenderer(_window);
}

Engine::~Engine()
{
	_renderer->destroy();
	_window->Destroy();
}

void Engine::run()
{
    Mesh mesh = _renderer->createMesh(vertices, indices);
    Mesh mesh2 = _renderer->createMesh(vertices2, indices2);
    Mesh mesh3 = _renderer->createMesh(vertices3, indices3);

    Texture texture = _renderer->createTexture("Textures/cat.jpg");
    Texture texture2 = _renderer->createTexture("Textures/cat2.jpg");

    UniformBuffer uniforms = _renderer->createUniformBuffer<UniformBufferObject>();

    DescriptorPool pool = _renderer->createDescriptorPool({ {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,1},{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,2} }, 3);
    DescriptorSetLayout layoutUb = _renderer->createDescriptorSetlayout(uniforms);
    DescriptorSetLayout layoutText = _renderer->createDescriptorSetlayout(texture);

    DescriptorSet descriptorSetUb = _renderer->createDescriptorSet(layoutUb, pool, uniforms);
    DescriptorSet descriptorSetText = _renderer->createDescriptorSet(layoutText, pool, texture);
    DescriptorSet descriptorSetText2 = _renderer->createDescriptorSet(layoutText, pool, texture2);

    GraphicsPipeline pipeline = _renderer->createGraphicsPipeline("Shaders/vert.spv", "Shaders/frag.spv", { layoutUb,layoutText });
    GraphicsPipeline pipeline2 = _renderer->createGraphicsPipeline("Shaders/vert.spv", "Shaders/colorfrag.spv", { layoutUb });


    while (!_window->ShouldClose()) {
        _window->PollEvents();

        // Update
        auto matrices = createMatrices(_renderer->getSwapchainWidth(), _renderer->getSwapchainHeight());
        _renderer->updateUniformBuffer<UniformBufferObject>(uniforms, matrices);

        // Draw
        _renderer->startFrame();

        _renderer->bindGraphicsPipeline(pipeline);
        _renderer->bindDescriptorSet(descriptorSetUb);
        _renderer->bindDescriptorSet(descriptorSetText, 1);
        _renderer->drawMesh(mesh);

        _renderer->bindDescriptorSet(descriptorSetText2, 1);
        _renderer->drawMesh(mesh2);

        _renderer->bindGraphicsPipeline(pipeline2);
        _renderer->bindDescriptorSet(descriptorSetUb);
        _renderer->drawMesh(mesh3);

        _renderer->endFrame();
    }
    _renderer->waitDeviceIdle();

    _renderer->destroyDescriptorPool(pool);
    _renderer->destroyDescriptorSetLayout(layoutText);
    _renderer->destroyDescriptorSetLayout(layoutUb);

    _renderer->destroyGraphicsPipeline(pipeline);
    _renderer->destroyGraphicsPipeline(pipeline2);

    _renderer->destroyUniformBuffer(uniforms);
    _renderer->destroyTexture(texture);
    _renderer->destroyTexture(texture2);
    _renderer->destroyMesh(mesh);
    _renderer->destroyMesh(mesh2);
    _renderer->destroyMesh(mesh3);
}
