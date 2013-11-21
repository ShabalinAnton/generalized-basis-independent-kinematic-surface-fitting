// demonstrates usage of the kinematic field fitting library
// pass a mesh to fit on the command line (kff_example -m "meshname")
// and this will fit all kinematic field types to the *whole* input mesh (no segmentation is performed)
// and then output the results to the console

// note that the order of kinematic field parameters is given in the comments of kinematicfieldfit.h
// also note that there are helper functions in the *KinFieldEval classes in kinematicfieldfit.h to help you interpret those parameters


#include "kinematicfieldfit.h"

using namespace kinematicfieldfit;

void printVector(vector<double> &vec)
{
    for (size_t i = 0; i < vec.size(); i++) {
        cout << vec[i] << " ";
    }
}

int main(int argc, char* argv[])
{
	kinematicfieldfit::TriangleMesh inputMesh;

    char *defaultInput = "../example_data/k_one_noise_at_base.obj";
    char *meshfile = argc>1 ? argv[1] : NULL;
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
    

    vector<double> translationResult, scalingResult, helicalResult, spiralResult, spiralResultHEIV;
    translationField.fitKinematicField(inputMesh, translationResult);
    scalingField.fitKinematicField(inputMesh, scalingResult);
    helicalField.fitKinematicField(inputMesh, helicalResult);
    spiralField.fitKinematicField(inputMesh, spiralResult);
    spiralField.fitKinematicField_HEIV(inputMesh, spiralResultHEIV);
    
    cout << "Translation field parameters: "  << endl;
    printVector(translationResult);
    cout << endl;
    
    cout << "Scaling field parameters: "  << endl;
    printVector(scalingResult);
    cout << endl;
    
    cout << "Helical field parameters: "  << endl;
    printVector(helicalResult);
    cout << endl;
    
    cout << "Spiral field parameters: "  << endl;
    printVector(spiralResult);
    cout << endl;

    cout << "Spiral field parameters w/ HEIV: "  << endl;
    printVector(spiralResultHEIV);
    cout << endl;
    

	return 0;
}

