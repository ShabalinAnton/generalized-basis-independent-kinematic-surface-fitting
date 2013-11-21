This is a demo of the kinematic field fitting library, to help users visualize the fitting results it provides.

This will fit a kinematic field to the vertices and vertex normals of an input mesh, which must be in the wavefront obj file format.  Note that this demo *only* performs fitting, not segmentation, so it will always fit one field to all input data.

Pass the desired obj file on the command line, for example:
kff_demo example_data/default.obj


Use the tab key to cycle through fitting results for different types of fields, including translational, conical, helical, and spiral fields.