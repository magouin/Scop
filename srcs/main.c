#include <scop.h>
#include <cglm/cglm.h>   /* for inline */

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

int8_t  init_data(t_d *data)
{
	ft_bzero(data, sizeof(t_d));
	return (1);
}

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s - errno = %d\n", description, error);
}

void resize_callback(GLFWwindow* window, int width, int height)
{
	printf("Resized : %d - %d\n", width, height);
	glViewport(0, 0, width, height);
	// TODO : Reset matrix
}

int8_t	init_glfw(t_d *data)
{
	if (!glfwInit())
		return (0);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 3);
	glfwSetErrorCallback(error_callback);

	data->window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
	if (!data->window)
	{
		glfwTerminate();
		return (0);
	}
	glfwSetFramebufferSizeCallback(data->window, resize_callback);
	glfwMakeContextCurrent(data->window);
	glfwGetWindowSize(data->window, &data->width, &data->height);
	printf("Window size : %d - %d\n", data->width, data->height);
	glViewport(0, 0, data->width, data->height);
	return (1);
}

void  add_vertex(t_d *data, float vertex[], size_t nbr, size_t index)
{
	data->sizeof_vertices[index] = nbr * sizeof(float);
	data->vertices[index] = malloc(nbr * sizeof(float));
	ft_memcpy(data->vertices[index], vertex, nbr * sizeof(float));
}

void  add_color(t_d *data, float color[], size_t nbr, size_t index)
{
	data->sizeof_colors[index] = nbr * sizeof(float);
	data->colors[index] = malloc(nbr * sizeof(float));
	ft_memcpy(data->colors[index], color, nbr * sizeof(float));
}

int8_t	init_all(t_d *data)
{
	if (!init_data(data))
		return (0);
	if (!init_glfw(data))
		return (0);

	glEnable(GL_MULTISAMPLE); 
	glEnable(GL_DEPTH_TEST); 
	return (1);
}

int8_t    init_shaders(t_d *data, const char *frag, const char *vert)
{
	if (!(data->vertex_sh = create_and_compile_shader(vert, GL_VERTEX_SHADER)))
	{
		dprintf(2, "Error while trying to create vertex shader.\n");
		return (0);
	}
 	if (!(data->fragment_sh = create_and_compile_shader(frag, GL_FRAGMENT_SHADER)))
	{
		dprintf(2, "Error while trying to create fragment shader.\n");
		return (0);
	}
	return (1);
}

void    init_vbo_triangle(t_d *data, int vbo_nbr)
{
	glGenBuffers(1, &(data->buffer[vbo_nbr]));

	glBindBuffer(GL_ARRAY_BUFFER, data->buffer[vbo_nbr]);
		glBufferData(GL_ARRAY_BUFFER, data->sizeof_vertices[vbo_nbr] + data->sizeof_colors[vbo_nbr], 0, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, data->sizeof_vertices[vbo_nbr], data->vertices[vbo_nbr]);
		glBufferSubData(GL_ARRAY_BUFFER, data->sizeof_vertices[vbo_nbr], data->sizeof_colors[vbo_nbr], data->colors[vbo_nbr]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void	init_matrices(t_mat4x4 view, t_mat4x4 model, t_d *data)
{
	ft_mat4x4_set_projection(data->projection, (float[]){60, (float)data->width / data->height, 0.1 ,100.0});
	// ft_mat4x4_set_identity(data->projection);
	ft_mat4x4_to_float_array(data->float_projection, data->projection);

	ft_mat4x4_set_look_at_from_float_array(view, (float[]){3, 3, 3}, (float[]){0, 0, 0}, (float[]){0, 1, 0});
	// ft_mat4x4_set_identity(view);

	ft_mat4x4_set_identity(model);
	// ft_mat4x4_rotate(model, 90, (float[]){0, 0, 1.});
	// ft_mat4x4_rotate(model, 60, (float[]){1., 0, 0});
}

int main(void)
{
	t_d		data;


	init_all(&data);
	printf("OPengl version : %s\n", glGetString(GL_VERSION));
	if (!init_shaders(&data, "./srcs/shaders/couleur2D.frag", "./srcs/shaders/couleur2D.vert"))
		return (1);
	if (!(data.program = create_and_link_program(data.vertex_sh, data.fragment_sh)))
		return (2);


	// t_mat4x4 view;
	// t_mat4x4 model;

	// init_matrices(view, model, &data);


	// printf("fovy : %f\n", (M_PI / 3) / ((float)data.width / data.height));
	// printf("ratio : %f\n", (float) data.width / data.height);
	// printf("near : %f\n", .1);
	// printf("far : %f\n", 100.);


	// t_mat4x4 modelview;
	// ft_mat4x4_mult(modelview, view, model);

	// GLfloat modelview_f[16];
	// ft_mat4x4_to_float_array(modelview_f, modelview);

	// GLfloat projection_f[16];
	// ft_mat4x4_to_float_array(projection_f, data.projection);

	// GLfloat view_f[16];
	// ft_mat4x4_to_float_array(view_f, view);

	// printf("modelview : \n");
	// ft_mat4x4_print(modelview);
	// printf("projection : \n");
	// ft_mat4x4_print(data.projection);

	// GLuint modelviewID = glGetUniformLocation(data.program, "modelview");
	// GLuint projID = glGetUniformLocation(data.program, "projection");

	// add_vertex(&data, (float[]){0.0, 0.0,  1, 0.0,  0.0, 1}, 6, 0);
	// add_color(&data, (float[]){1.0, 0.0, 0.0,   0.0, 1.0, 0.0,   0.0, 0.0, 1.0}, 9, 0);


	create_image_from_png(&data, 0, "../Doom_nukem/resources/stone_blocks.png");
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// int width, height, nrChannels;
	// unsigned char *imgdata = stbi_load("textures/container.jpg", &width, &height, &nrChannels, 0); 
	// printf("%d - %d\n", width, height);



		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, data.img[0].w, data.img[0].h, 0, GL_BGRA, GL_UNSIGNED_BYTE, data.img[0].data);
		glGenerateMipmap(GL_TEXTURE_2D);

	add_vertex(&data, (float[]){
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	}, 36 * 5, 0);


	GLuint vao = 0;
	glGenVertexArrays(1, &vao);

	glGenBuffers(1, &data.buffer[0]);

	glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, data.buffer[0]);
			glBufferData(GL_ARRAY_BUFFER, data.sizeof_vertices[0], 0, GL_STATIC_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, 0, data.sizeof_vertices[0], data.vertices[0]);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), NULL);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	// glBindBuffer(GL_ARRAY_BUFFER, 0);

	// glm_mat4_mult(c_model, c_view, c_viewmodel);


	unsigned int transformLoc = glGetUniformLocation(data.program, "obj");

	t_mat4x4	model;
	GLfloat		model_f[16];

	ft_mat4x4_set_rotation(model, -55.0f, (t_vec3){1, 0, 0});
	ft_mat4x4_to_float_array(model_f, model);

	t_mat4x4	view;
	GLfloat	view_f[16];

	ft_mat4x4_set_translation(view, (t_vec3){0, 0, -3});
	ft_mat4x4_to_float_array(view_f, view);

	t_mat4x4	proj;
	GLfloat		proj_f[16];

	ft_mat4x4_set_projection(proj, (float[]){45, (float)data.width / data.height, .1f, 100.0f});
	ft_mat4x4_to_float_array(proj_f, proj);

	int modelLoc = glGetUniformLocation(data.program, "model");
	int viewLoc = glGetUniformLocation(data.program, "view");
	int projLoc = glGetUniformLocation(data.program, "projection");


	glClearColor(.2, .3, .3, 1);
	while (!glfwWindowShouldClose(data.window))
	{
		if (glfwGetKey(data.window, GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(data.window, true);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		glUseProgram(data.program);

		// glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(vao);
			// glUniformMatrix4fv(modelviewID, 1, GL_FALSE, modelview_f);
			// glUniformMatrix4fv(projID, 1, GL_FALSE, projection_f);
			// glDrawArrays(GL_TRIANGLES, 0, 36);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model_f);
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view_f);
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, proj_f);

			glDrawArrays(GL_TRIANGLES, 0, 36);
			// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwPollEvents();
		glfwSwapBuffers(data.window);
	}
	printf("Closing window.\n");

	glfwTerminate();
	return 0;
}
