#include "renderer.h"

GLuint load_cube_map_tx() {
    //texture sides

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    std::vector<std::string> texture_faces {
        "res/textures/grass_side.png",    // right
        "res/textures/grass_side.png",    // left
        "res/textures/grass_top.png",     // top
        "res/textures/dirt.png",  // bottom
        "res/textures/grass_side.png",    // back
        "res/textures/grass_side.png"     // front
    };
    int width, height, nr_channels;
    unsigned char *data;


    for (unsigned int i = 0; i<texture_faces.size(); i++) {

        data = stbi_load(texture_faces[i].c_str(), &width, &height, &nr_channels, 0);
        if (!data) {
            std::cout << "Unable to load texture_faces[" << i << "] = " << texture_faces[i] << std::endl;  
            return -1;
        }
     
        std::cout << "Succesfully loaded texture_faces[ +" << i << "] = " << texture_faces[i] << std::endl;
        std::cout << "Width = " << width << "\nHeight = " << height << "\nnr_channels = " << nr_channels << std::endl;
        
        //GLenum fmt = (nr_channels == 4) ? GL_RGBA : GL_RGB;
        //GLenum ifmt = (nr_channels == 4) ? GL_RGBA8 : GL_RGB8;
        
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data
        );
        
        
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); 

    return texture;
}


GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path){

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}
	
	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

void pipeline_renderer::init() {
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    programID = LoadShaders("res/shaders/vertex.vert", "res/shaders/shader.frag");
    glUseProgram(programID);

    glUseProgram(programID);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    texture = load_cube_map_tx();
    glActiveTexture(GL_TEXTURE0);

    glfwSetInputMode(state.window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    glEnable(GL_CULL_FACE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void pipeline_renderer::render(camera_perspective *cam) {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
               // Create projection and view matrices
            std::cout << state.width << std::endl;
            glm::mat4 projection = glm::perspective(glm::radians(cam->fov), (float)state.width / state.height, 0.1f, 100.0f);
            
            glm::mat4 view = glm::lookAt(
                cam->pos,
                cam->pos + cam->dir.forwards,
                cam->dir.up);
        
            // Base model matrix (no translation for the first cube)
            glm::mat4 model1 = glm::mat4(1.0f); 

            // Translate the second cube to the left
            glm::mat4 model2 = glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, 0.0f, 0.0f)); // Shift by -2.0f along x-axis
            
            // MVP matrices for each cube
            glm::mat4 MVP1 = projection * view * model1;
            glm::mat4 MVP2 = projection * view * model2;

            GLuint MatrixID = glGetUniformLocation(programID, "MVP");
            
            // Draw the first cube
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP1[0][0]);
            glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
            glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // Draw the second cube (shifted left)
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP2[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
}

pipeline_renderer::~pipeline_renderer() {
        glDeleteBuffers(1, &vertexBuffer);
    glDeleteVertexArrays(1, &VertexArrayID);
    glfwSetInputMode(state.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glDeleteProgram(programID);
}