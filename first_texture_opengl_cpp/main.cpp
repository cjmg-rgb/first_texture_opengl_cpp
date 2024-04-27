#include "libs.h";

Vertex vertices[] =
{
	glm::vec3(-.5f, .5f, 0.f), glm::vec3(1.f, 1.f, 0.f), glm::vec2(0.f, 1.f),
	glm::vec3(-.5f, -.5f, 0.f), glm::vec3(0.f, 1.f, 1.f), glm::vec2(0.f, 0.f),
	glm::vec3(.5f, -.5f, 0.f), glm::vec3(1.f, 0.f, 1.f), glm::vec2(1.f, 0.f),
	glm::vec3(.5f, .5f, 0.f), glm::vec3(0.f, 1.f, 1.f), glm::vec2(1.f, 1.f)
};
unsigned int sizeOfVertices = sizeof(vertices) / sizeof(Vertex);

GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3
};
unsigned int sizeOfIndices = sizeof(indices) / sizeof(GLuint);

void getFrameBufferSizeCallback(GLFWwindow* window, int fbw, int fbh)
{
	glViewport(0, 0, fbw, fbh);
}

int main()
{
	// Init GLFW
	glfwInit();

	// Create Window
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	int frame_buffer_width = 0;
	int frame_buffer_height = 0;

	GLFWwindow* window = glfwCreateWindow(500, 500, "Texture", NULL, NULL);
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, getFrameBufferSizeCallback);
	
	// Init GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	// Create Shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
	glCompileShader(vertexShader);
	
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
	glCompileShader(fragmentShader);

	GLuint core_program = glCreateProgram();
	glAttachShader(core_program, vertexShader);
	glAttachShader(core_program, fragmentShader);
	glLinkProgram(core_program);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glUseProgram(0);

	// Vertex Data
	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, vertex_position));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, vertex_color));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, vertex_texcoord));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	
	// Texture
	int img_width, img_height;
	unsigned char* image = SOIL_load_image("images/luffy.png", &img_width, &img_height, NULL, SOIL_LOAD_RGBA);

	if (!image)
	{
		std::cout << "Failed To Load Image\n";
		glfwTerminate();
	}

	GLuint texture0;
	glGenTextures(1, &texture0);
	glBindTexture(GL_TEXTURE_2D, texture0);	
	
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	
	SOIL_free_image_data(image);
	glActiveTexture(0);
	
	int img_width1, img_height1;
	unsigned char* image1 = SOIL_load_image("images/meat.png", &img_width1, &img_height1, NULL, SOIL_LOAD_RGBA);

	if (!image1)
	{
		std::cout << "Failed To Load Image\n";
		glfwTerminate();
	}

	GLuint texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);	
	
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width1, img_height1, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1);
	glGenerateMipmap(GL_TEXTURE_2D);

	
	SOIL_free_image_data(image1);
	glActiveTexture(0);

	// LOOP
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Use a program
		glUseProgram(core_program);

		// Update Uniform
		glUniform1i(glGetUniformLocation(core_program, "texture1"), 1);

		// Textures
		glBindTextureUnit(0, texture0);
		glBindTextureUnit(1, texture1);

		// Bind Vertex Array Object
		glBindVertexArray(VAO);

		glDrawElements(GL_TRIANGLES, sizeOfIndices, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glFlush();

		glUseProgram(0);
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glBindVertexArray(0);
	glDeleteProgram(core_program);
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &VBO);
	glDeleteVertexArrays(1, &EBO);

	glfwTerminate();

	return 0;
}