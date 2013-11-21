This is a library demonstrating the kinematic field fitting methods in the paper:

Generalized, Basis-Independent Kinematic Surface Fitting in Computer-Aided Design, Volume 45, Issue 3, March 2013
by James Andrews and Carlo Sequin.

It computes direct fits for four types of kinematic fields; using the LinParamKinematicFieldFitter<FieldType>::fitKinematicField() function
(where FieldType is one of the provided classes for different kinematic field types; see kinematicfieldfit.h)

Kinematic field fitting is useful for fitting helices, surfaces of revolution, cylinders and more.
For common cases such as fitting surfaces of revolution, some additional functions are provided
 to help you extract the results in a format you expect; see the usage instructions below.

BUILD INSTRUCTIONS:

Visual studio / Windows:
It should compile with the provided files "out of the box" on visual studio 2008 or higher

Mac:
Just type "make".



Linux:

The makefiles were written and tested for mac, but should hopefully mostly work on linux too.  (The makefile for the demo will likely require minor adjustments -- you will need to remove the "-framework" bits and replace them with the appropriate libraries for OpenGL.)

USAGE INSTRUCTIONS:

1. include kinematicfieldfit.h

2. CENTER AND RESCALE YOUR DATA! (I don't do this for you!)

3. Create a field fitter of the type you want, e.g. for a spiral field, write:
LinParamKinematicFieldFitter<RotTransScaleKinFieldEval> spiralField;

4. pass the data you want to fit to one of the fitKinematicField member functions; e.g.:
spiralField.fitKinematicField(inputMesh, fittedParameters);
(where fittedParameters is a vector<double>)

If you want a better fit at higher cost, you can also try the _HEIV variant of this function:
spiralField.fitKinematicField_HEIV(inputMesh, fittedParameters, numIters);
(where numIters is the number of iterations of refinement you want, after the initial fit)
This should take time roughly equal to time_of_non_HEIV_fit*(numIters+1).

5. optionally use the helper functions to interpret the field parameters --

e.g. use:
RotTransScaleKinFieldEval::vel(&fittedParameters[0], p);
to find the velocity of a fitted spiral field at point p.

Or use:
RotTransScaleKinFieldEval::getAxisOfRotation(&fittedParams[0], axisDir, axisPt)
to get the axis about which a fitted spiral field rotates (where axisDir is the direction of that axis, and axisPt is a point on that axis)

To use these helper functions to fit a surface of revolution:
1. Fit a helical field with "LinParamKinematicFieldFitter<RotTransKinFieldEval> helicalField; helicalField.fitKinematicField(inputMesh, fittedParameters);"
2. Remove the helical motion with "RotTransKinFieldEval::removePitch(&fittedParameters[0]);"
3. Find the parameters of the surface of revolution using "vec3 dir, ptOnAxis; RotTransKinFieldEval::getAxisOfRotation(&fittedParameters[0], dir, ptOnAxis);"



EXAMPLES:

The demo folder shows usage of the library with opengl to render the results.  Depends on opengl and glfw.  Press tab or arrows to change the displayed fit.

The example folder shows usage of the library without dependencies; it simply prints the parameters found by the fitting methods.

Windows binaries of the demo and example are provided in the demo and example folders.



SUPPORT:

I'm happy to hear from anyone who is interested in using the library!
Send bug reports, feature requests, suggestions, etc, to:
zaphos@gmail.com



CAVEAT EMPTOR:

Note that this code hasn't been extensively tested, or even compiled by anyone other than me as of my writing this readme.  I hope it works well, but please cut me some slack if there are issues!  I will try to help if you email and ask nicely.



VERSION NOTES:
.02 (9/19/2013): added iterative HEIV-based fitting, as described in Sec. 4.2 of the paper.
.01: initial release



ROADMAP:

I don't have plans to actively add to this, but if you feel there is a missing feature you need, feel free to let me know and I might try to add it.