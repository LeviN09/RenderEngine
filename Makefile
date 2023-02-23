OPENGLFLAGS := -lGL -lGLU -lglut -lm -lglfw -lGLEW -Lvendor -lstb
OBJDIR := objects
CPPDIR := sources
HPPDIR := headers
OBJS := VAO.o VBO.o EBO.o shaderClass.o renderer.o renderObject.o texture.o light.o camera.o plane.o cube.o sphere.o
OBJPATH := $(addprefix $(OBJDIR)/, $(OBJS))

out: main.cpp $(OBJPATH)
	g++ main.cpp $(OBJPATH) -o out $(OPENGLFLAGS)

$(OBJDIR)/%.o: $(CPPDIR)/%.cpp
	g++ $^ -c -o $@

$(CPPDIR)/%.cpp: $(HPPDIR)/%.hpp