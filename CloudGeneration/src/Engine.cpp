#include "Engine.hpp"

Engine* Engine::s_Instance;

UniformBufferObject createUniformBuffer(int width, int height, float totalTime, Camera& cam)
{

    UniformBufferObject ubo{};
    ubo.model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = cam.view;
    ubo.proj = glm::perspective(glm::radians(45.0f), width / (float)height, 0.1f, 1000.0f);
    ubo.proj[1][1] *= -1;
    ubo.directionnalLight = glm::vec4(glm::normalize(glm::vec3{ cos(totalTime),sin(totalTime),-1 }), 1.0);
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

Engine::Engine()
{
	s_Instance = this;

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
    std::chrono::steady_clock::time_point lastFrameTime;
    Camera camera;

    Mesh mesh = _renderer->createMesh(vertices, indices);
    Mesh mesh2 = _renderer->createMesh(vertices2, indices2);
    Mesh mesh3 = _renderer->createMesh(vertices3, indices3);
    Mesh meshRM = _renderer->createMesh(verticesSSRect, indicesSSRect);

    UniformBuffer uniforms;
    uniforms.create(_context, sizeof(UniformBufferObject));


    DescriptorPool pool;
    pool.create(_context, { {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,1},{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,2} }, 3);
    DescriptorSetLayout layoutUb;
    layoutUb.createUniformBufferLayout(_context, 0);
    DescriptorSet descriptorSetUb;
    descriptorSetUb.createDescriptorSetUniformBuffer(_context, layoutUb, pool, uniforms);

    Material material = _renderer->createMaterial("Shaders/vert.spv", "Shaders/frag.spv", "resources/Textures/cat.jpg");
    Material material2 = _renderer->createMaterial("Shaders/vert.spv", "Shaders/frag.spv", "resources/Textures/cat2.jpg");
    Material material3 = _renderer->createMaterial("Shaders/vert.spv", "Shaders/colorfrag.spv");
    Material materialRM = _renderer->createMaterialRayMarching("Shaders/raymarchingVert.spv", "Shaders/raymarchingFrag.spv");


    while (!_window->ShouldClose()) {
        _window->PollEvents();

        // Update
        auto size = _window->getFrameBufferSize();

        static auto startTime = std::chrono::high_resolution_clock::now();
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - lastFrameTime).count();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
        lastFrameTime = currentTime;

        moveCamera(camera, _window, deltaTime);
        auto matrices = createUniformBuffer(_renderer->getSwapchainWidth(), _renderer->getSwapchainHeight(), time, camera);
        uniforms.update(_context, matrices, _renderer->getCurrentFrame());

        // Draw
        _renderer->startFrame();

        _renderer->drawMesh(mesh, material, {descriptorSetUb});
        _renderer->drawMesh(mesh2, material2, {descriptorSetUb});
        _renderer->drawMesh(mesh3, material3, {descriptorSetUb});
        _renderer->drawMesh(meshRM, materialRM, {descriptorSetUb});

        _renderer->endFrame();
    }
    _context->waitDeviceIdle();

    
    pool.destroy(_context);
    layoutUb.destroy(_context);
    uniforms.destroy(_context);

    _renderer->destroyMaterial(material);
    _renderer->destroyMaterial(material2);
    _renderer->destroyMaterial(material3);
    _renderer->destroyMaterial(materialRM);
    _renderer->destroyMesh(mesh);
    _renderer->destroyMesh(mesh2);
    _renderer->destroyMesh(mesh3);
    _renderer->destroyMesh(meshRM);
    
}
