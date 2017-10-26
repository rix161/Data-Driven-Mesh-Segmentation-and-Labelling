#ifndef __CGRENDERER__
#define __CGRENDERER__
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include "CGAL_geodesic.h"

class CGRenderer {

protected:
	static void setupGlutCallBacks();
	void initRenderer(int *argc, char** argv, int sizeX, int sizeY, const char* title);
	static bool mSetupDone;
	static GLuint mVBO;
public:
	static void renderScene();
	CGRenderer() {}
	CGRenderer(int *argc, char** argv, int sizeX, int sizeY, const char* title){
		initRenderer(argc, argv, sizeX, sizeY, title);
	}
	
	void setupRendererParameters(std::vector<Kernel::Point_3>, 
								 std::vector<std::vector<int>>, 
		                         std::vector<std::deque<int>>,
		                         std::vector<std::vector<int>>, 
								 int,int,int);
};

#endif
