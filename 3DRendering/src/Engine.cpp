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
	_context = std::make_shared<Context>();
	_renderer = std::make_shared<Renderer>();


	std::string name = "My engine";
	_window->Create(name);
    _context->create(_window);
	_renderer->create(_window,_context);
}

Engine::~Engine()
{
	_renderer->destroy();
    _context->destroy();
    _window->Destroy();
}

void Engine::run()
{
    Mesh mesh = _renderer->createMesh(vertices, indices);
    Mesh mesh2 = _renderer->createMesh(vertices2, indices2);
    Mesh mesh3 = _renderer->createMesh(vertices3, indices3);

    UniformBuffer uniforms;
    uniforms.create(_context, sizeof(UniformBufferObject));


    DescriptorPool pool;
    pool.create(_context, { {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,1},{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,2} }, 3);
    DescriptorSetLayout layoutUb;
    layoutUb.createUniformBufferLayout(_context, 0);
    DescriptorSet descriptorSetUb;
    descriptorSetUb.createDescriptorSetUniformBuffer(_context, layoutUb, pool, uniforms);

    Material material = _renderer->createMaterial("Shaders/vert.spv", "Shaders/frag.spv", "Textures/cat.jpg");
    Material material2 = _renderer->createMaterial("Shaders/vert.spv", "Shaders/frag.spv", "Textures/cat2.jpg");
    Material material3 = _renderer->createMaterial("Shaders/vert.spv", "Shaders/colorfrag.spv");

    while (!_window->ShouldClose()) {
        _window->PollEvents();

        // Update
        auto size = _window->getFrameBufferSize();
        auto matrices = createMatrices(size.x, size.y);
        uniforms.update(_context, matrices, _renderer->getCurrentFrame());

        // Draw
        _renderer->startFrame();

        _renderer->drawMesh(mesh, material, {descriptorSetUb});
        _renderer->drawMesh(mesh2, material2, {descriptorSetUb});
        _renderer->drawMesh(mesh3, material3, {descriptorSetUb});

        _renderer->endFrame();
    }
    _context->waitDeviceIdle();

    
    pool.destroy(_context);
    layoutUb.destroy(_context);
    uniforms.destroy(_context);

    _renderer->destroyMaterial(material);
    _renderer->destroyMaterial(material2);
    _renderer->destroyMaterial(material3);
    _renderer->destroyMesh(mesh);
    _renderer->destroyMesh(mesh2);
    _renderer->destroyMesh(mesh3);
}
