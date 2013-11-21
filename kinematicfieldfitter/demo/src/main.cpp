// demonstrates usage of the kinematic field fitting library
// pass a mesh to fit on the command line (kff_demo "meshname")
// and this will fit all kinematic field types, then render the results
// press tab or left/right arrows to cycle through fitting results

#include "view.h"

#include "kinematicfieldfit.h"

#include <fstream>
#include <map>

using namespace std;
using namespace kinematicfieldfit;


void printVector(vector<double> &vec)
{
    for (size_t i = 0; i < vec.size(); i++) {
        cout << vec[i] << " ";
    }
}

void drawMesh(kinematicfieldfit::TriangleMesh &mesh) {
	glBegin(GL_TRIANGLES);
	for (size_t i = 0; i < mesh.triangles.size(); i++) {
		glNormal3dv(&mesh.triangleNormals[i][0]);
		for (int ii = 0; ii < 3; ii++) {
			glVertex3dv(&mesh.vertices[ mesh.triangles[i].ind[ii] ][0]);
		}
	}
	glEnd();
}

template<typename Field>
void buildStreamline(double *params, vec3 start, double stepSize, int stepsPerPt, int steps, vector<vec3> &streamlineOut)
{
    streamlineOut.clear(); streamlineOut.push_back(start);
    vec3 p = start;
    for (int i = 0; i < steps; i++) {
        vec3 v = Field::vel(params, p);
        v.normalize();
        v *= stepSize;
        p += v;
        if (i % stepsPerPt == 0)
            streamlineOut.push_back(p);
    }
}

void drawLines(vector<vec3> &line) {
    glBegin(GL_LINE_STRIP);
    for (size_t i = 0; i < line.size(); i++) {
        glVertex3dv(&line[i][0]);
    }
    glEnd();
}

void drawStreamline(vector<vec3> &line) {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    // colors array
    //set color and render mesh repeatedly
    glClearStencil(0x1);
    glEnable(GL_STENCIL_TEST);
    glClear(GL_STENCIL_BUFFER_BIT);
    
    glDisable(GL_LIGHTING);
    glColor3d(.8,.3,.2);
    
    // draw the small line behind
    glLineWidth(1);
    glPointSize(1);
    glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);
    glStencilFunc (GL_EQUAL, 0x1, 0x1);
    //glStencilFunc (GL_ALWAYS, 0x1, 0x1);
    glDisable(GL_DEPTH_TEST);
    drawLines(line);
    
    
    // draw the big line upfront
    glLineWidth(3);
    glPointSize(3);
    glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);
    glStencilFunc (GL_ALWAYS, 0x1, 0x1);
    glEnable(GL_DEPTH_TEST);
    drawLines(line);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);

    glDisable(GL_CULL_FACE);
    
    glEnable(GL_LIGHTING);
}

void display(Viewport &viewport, kinematicfieldfit::TriangleMesh &reference, vector<vector<vec3> > &streamlines) {

    // setup gl state
    glClearColor(.5f,.7f,1,1);
    glEnable(GL_NORMALIZE);
    glDisable(GL_CULL_FACE);

    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    // setup the camera
    viewport.loadView();

	glColor3d(1,1,1);
	drawMesh(reference);

    for (size_t i = 0; i < streamlines.size(); i++) {
        drawStreamline(streamlines[i]);
    }

    
}



void reshape(int w, int h) {
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70.0, ((double)w / MAX(h, 1)), .0001, 5.0);

    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}




int main( int argc, char **argv )
{
	Viewport viewport;
	kinematicfieldfit::TriangleMesh inputMesh;

    const char *defaultInput = "../example_data/k_one_noise_at_base.obj"; // an interesting default example; HEIV tends to improve the solution noticeably in this case
    const char *meshfile = argc>1 ? argv[1] : NULL;
	if (!meshfile) {
		cerr << "No mesh file specified?  Loading default mesh: " << defaultInput << endl;
		meshfile = defaultInput;
	}
	if (!inputMesh.loadObj(meshfile)) {
        cerr << "Couldn't load file " << meshfile << endl;
        return 1;
    }

	
	// Always recenter and scale your data before fitting!
	inputMesh.centerAndScale(1);
    
    
    
    // all the field types
	LinParamKinematicFieldFitter<TransKinFieldEval> translationField;
	LinParamKinematicFieldFitter<ScaleKinFieldEval> scalingField;
	LinParamKinematicFieldFitter<RotTransKinFieldEval> helicalField;
	LinParamKinematicFieldFitter<RotTransScaleKinFieldEval> spiralField;
    const char *kinematicFitTypeNames[] = { "translation", "translation [HEIV]", "scaling", "scaling [HEIV]", "helical", "helical [HEIV]", "spiral", "spiral [HEIV]" };
    int numKinematicFitTypes = 8;
    
    // get fitting results
    vector<double> translationResult, scalingResult, helicalResult, spiralResult;
    translationField.fitKinematicField(inputMesh, translationResult);
    scalingField.fitKinematicField(inputMesh, scalingResult);
    helicalField.fitKinematicField(inputMesh, helicalResult);
    spiralField.fitKinematicField(inputMesh, spiralResult);

    vector<double> translationResultHEIV, scalingResultHEIV, helicalResultHEIV, spiralResultHEIV;
    translationField.fitKinematicField_HEIV(inputMesh, translationResultHEIV);
    scalingField.fitKinematicField_HEIV(inputMesh, scalingResultHEIV);
    helicalField.fitKinematicField_HEIV(inputMesh, helicalResultHEIV);
    spiralField.fitKinematicField_HEIV(inputMesh, spiralResultHEIV);
    
    cout << "translation result: ";
    printVector(translationResult);
    cout << "translation result [HEIV]: ";
    printVector(translationResultHEIV);
    cout << endl << "scaling result: ";
    printVector(scalingResult);
    cout << endl << "scaling result [HEIV]: ";
    printVector(scalingResultHEIV);
    cout << endl << "helical result: ";
    printVector(helicalResult);
    cout << endl << "helical result [HEIV]: ";
    printVector(helicalResultHEIV);
    cout << endl << "spiral result: ";
    printVector(spiralResult);
    cout << endl << "spiral result [HEIV]: ";
    printVector(spiralResultHEIV);
    cout << endl;
    
    vector<vec3> startingSeeds;
    if (!inputMesh.vertices.empty()) {
        startingSeeds.push_back(inputMesh.vertices[rand() % inputMesh.vertices.size()]);
        startingSeeds.push_back(inputMesh.vertices[rand() % inputMesh.vertices.size()]);
        startingSeeds.push_back(inputMesh.vertices[rand() % inputMesh.vertices.size()]);
        startingSeeds.push_back(inputMesh.vertices[rand() % inputMesh.vertices.size()]);
        startingSeeds.push_back(inputMesh.vertices[rand() % inputMesh.vertices.size()]);
    }
    if (startingSeeds.empty()) {
        startingSeeds.push_back(vec3(0,0,0));
    }
    
    vector<vector<vector<vec3> > > streamlineSets(numKinematicFitTypes, vector<vector<vec3> >(2*startingSeeds.size()));
    
    
    for (size_t i = 0; i < startingSeeds.size(); i++) {
        for (int dir = 0; dir <= 1; dir++) {
            double dirFac = (dir*2)-1;
            buildStreamline<TransKinFieldEval>(&translationResult[0], startingSeeds[i], dirFac*.0001, 200, 30000, streamlineSets[0][i*2+dir]);
            buildStreamline<TransKinFieldEval>(&translationResultHEIV[0], startingSeeds[i], dirFac*.0001, 200, 30000, streamlineSets[1][i*2+dir]);
            buildStreamline<ScaleKinFieldEval>(&scalingResult[0], startingSeeds[i], dirFac*.0001, 200, 30000, streamlineSets[2][i*2+dir]);
            buildStreamline<ScaleKinFieldEval>(&scalingResultHEIV[0], startingSeeds[i], dirFac*.0001, 200, 30000, streamlineSets[3][i*2+dir]);
            buildStreamline<RotTransKinFieldEval>(&helicalResult[0], startingSeeds[i], dirFac*.0001, 200, 30000, streamlineSets[4][i*2+dir]);
            buildStreamline<RotTransKinFieldEval>(&helicalResultHEIV[0], startingSeeds[i], dirFac*.0001, 200, 30000, streamlineSets[5][i*2+dir]);
            buildStreamline<RotTransScaleKinFieldEval>(&spiralResult[0], startingSeeds[i], dirFac*.0001, 200, 30000, streamlineSets[6][i*2+dir]);
            buildStreamline<RotTransScaleKinFieldEval>(&spiralResultHEIV[0], startingSeeds[i], dirFac*.0001, 200, 30000, streamlineSets[7][i*2+dir]);
        }
    }
    

	int showKinematicFit = 0;

    glfwInit();

    // default window size:
    int W = 800, H = 500;
    // Open window
    int ok = glfwOpenWindow(W, H, 8, 8, 8, 8, 24, 8, GLFW_WINDOW);
    if( !ok ) { glfwTerminate(); return 0; }
    // setup gl window/perspective based on window height
    reshape(W,H);

    // Set window title
    glfwSetWindowTitle( "Kinematic Fitting Demo" );

    // Enable sticky keys
    glfwEnable( GLFW_STICKY_KEYS );
    
    glEnable(GL_LINE_SMOOTH);
    glEnable( GL_POINT_SMOOTH );
    glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);

	



    // set some lights
    {
       float ambient[3] = { .1f, .1f, .1f };
       float diffuse[3] = { .4f, .3f, .3f };
	   float pos[4] = { 0, 2, 0, 0 };
       
       glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
       glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
       glLightfv(GL_LIGHT1, GL_POSITION, pos);
       glEnable(GL_LIGHT1);
    }
    {
       float ambient[3] = { .1f, .1f, .1f };
       float diffuse[3] = { .1f, .2f, .2f};
       float pos[4] = { 0, 0, -2, 0 };
       glLightfv(GL_LIGHT2, GL_AMBIENT, ambient);
       glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse);
       glLightfv(GL_LIGHT2, GL_POSITION, pos);
       glEnable(GL_LIGHT2);
    }
    {
       float ambient[3] = { .1f, .1f, .1f };
       float diffuse[3] = { .1f, .2f, .1f};
       float pos[4] = { -1, 0, 0, 0 };
       glLightfv(GL_LIGHT3, GL_AMBIENT, ambient);
       glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuse);
       glLightfv(GL_LIGHT3, GL_POSITION, pos);
       glEnable(GL_LIGHT3);
    }
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);

	srand(95);



    viewport.resetCam();



    int mx, my;
    glfwGetMousePos(&mx, &my);

	vec3 mousePos;
    double lastTime = 0, timePerFrame = 1.0/30.0;

    vec4 lightPos(-5,5,5,0);

    bool mouseWasUp = false;

	cout << endl << "press TAB to change kinematic field type" << endl;
	cout << "now showing streamlines generated by fitting kinematic field type: " << kinematicFitTypeNames[showKinematicFit] << endl;
    string windowTitle = "Kinematic Fitting Demo: ";
    windowTitle += kinematicFitTypeNames[showKinematicFit];
    glfwSetWindowTitle(windowTitle.c_str());

    // Main rendering loop
    while (true) {

        bool changedLast = false;

		int logics = 0;
		while (glfwGetTime() - lastTime > timePerFrame) {
			{
				// UI/interaction code:
				if (keyHit(GLFW_KEY_TAB) || keyHit(GLFW_KEY_SPACE) || keyHit(GLFW_KEY_RIGHT)) {
					showKinematicFit++;
					showKinematicFit = showKinematicFit % numKinematicFitTypes;
					cout << endl << "now showing streamlines generated by fitting kinematic field type: " << kinematicFitTypeNames[showKinematicFit] << endl;
                    windowTitle = "Kinematic Fitting Demo: ";
                    windowTitle += kinematicFitTypeNames[showKinematicFit];
                    glfwSetWindowTitle(windowTitle.c_str());
				}
				if (keyHit(GLFW_KEY_LEFT)) {
					showKinematicFit--;
					if (showKinematicFit < 0) showKinematicFit = numKinematicFitTypes - 1;
					cout << endl << "now showing streamlines generated by fitting kinematic field type: " << kinematicFitTypeNames[showKinematicFit] << endl;
                    windowTitle = "Kinematic Fitting Demo: ";
                    windowTitle += kinematicFitTypeNames[showKinematicFit];
                    glfwSetWindowTitle(windowTitle.c_str());
				}
                
				int nmx, nmy;
				glfwGetMousePos(&nmx, &nmy);

                if (!glfwGetMouseButton(GLFW_MOUSE_BUTTON_1) && !glfwGetMouseButton(GLFW_MOUSE_BUTTON_2)) {
                    mouseWasUp = true;
                }

                if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_1)) { // mouse movements update the view
                    // Rotate viewport orientation proportional to mouse motion
                    viewport.mousePos = vec2((double)mx / (double)W,(double)my / (double)H);
                    vec2 newMouse = vec2((double)nmx / (double)W,(double)nmy / (double)H);
                    vec2 diff = (newMouse - viewport.mousePos);
                    double len = diff.length();
					if (!glfwGetKey('Z') && !glfwGetKey(GLFW_KEY_LALT) && len > .001) {
                        vec3 axis = vec3(diff[1]/len, diff[0]/len, 0);
                        viewport.orientation = rotation3D(axis, 180 * len) * viewport.orientation;
                    }
                    if ( (glfwGetKey('Z') || glfwGetKey(GLFW_KEY_LALT)) && fabs(diff[1]) > .001) {
    	                viewport.zoom += diff[1];
    	                if (viewport.zoom < .001) viewport.zoom = .001;
                    }

                    //Record the mouse location for drawing crosshairs
                    viewport.mousePos = newMouse;
                }
                
                mx = nmx; my = nmy;
			}

			lastTime += timePerFrame;
			logics++;

			if (changedLast || logics > 10) // slow down if you really can't keep up
				break;
		}

		if (logics > 0) {
            

            display(viewport, inputMesh, streamlineSets[showKinematicFit]);
            
            glfwSwapBuffers();
		}
		 else {
            glfwSleep( .001 ); // else release control to OS for 5 ms
        }

        // Check if the escape key was pressed, or if the window was closed
        if (glfwGetKey( GLFW_KEY_ESC ) || !glfwGetWindowParam( GLFW_OPENED )) {
            break;
        }
    }

    // cleanup and exit
    glfwTerminate();
    return 0;
}
