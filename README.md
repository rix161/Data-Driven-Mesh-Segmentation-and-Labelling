<p align="center">
#Data Driven Mesh Segmentation and Labelling 
##Course project for CSE 528: Computer Graphics
</p>

###Introduction

The objective of the project is to explicitly label the mesh into its components. 
The problem of mesh labelling can be viewed a classification problem as such this project trains a random forest classifier trained on mesh data generated 
using a ensemble of mesh descriptors which effectively and concisely capture the surface,volume and orientation of the mesh.

####Mesh Features
The features implemented are
1. Average Geodesic Distance (Local surface property) 
   Approximate average geodesic distance using fast marching algorithm.
<p align="center">
![AGDIMAGE](https://github.com/rix161/SBU_ComputerGraphics/blob/master/TestCGAL/Test/SceenShots/AGDResult.png)
</p>

2. Shape diameter function(Local Volume property)
   Approximation method calculate the distance from surface to the medial axis. Uses Moller-Trumbore algorithm for triangle ray intersection.
<p align="center">	
![SDFImage](https://github.com/rix161/SBU_ComputerGraphics/blob/master/TestCGAL/Test/SceenShots/SDFResult.png)
</p>

3. Curvature (Gaussian,Mean and principle curvatures)(1-Ring neighbourhood surface property)
   First order differential attributes on a piecewise linear triangular mesh.
<p align="center">	   
![CurImage](https://github.com/rix161/SBU_ComputerGraphics/blob/master/TestCGAL/Test/SceenShots/curvatureResult.png)
</p>

4. Volumetric shape images(Local Volume property)
   Better approximation of distance from surface to the medial axis. A Two pass algorithm which internally uses SDF.
<p align="center">	   
![VSIImage](https://github.com/rix161/SBU_ComputerGraphics/blob/master/TestCGAL/Test/SceenShots/VisResult.png)
</p>

5. Shape Context(Surface and mesh face orientation property)
	Combines geodesic distance with the orientation by computing the angle between the surface normal and line connecting the face centres.
<p align="center">	
![SCImage](https://github.com/rix161/SBU_ComputerGraphics/blob/master/TestCGAL/Test/SceenShots/SCResult.png)
</p>

####Mesh Labelling and Segmentation.
A Random Forest Classifier was able to label the mesh with an accuracy of 92% and an SVM Classifier with accuracy of 93%. 
Random Forest classifier was finally chosen as it is faster to train.

#####Results 
<p align="center">	
![ClassImage](https://github.com/rix161/SBU_ComputerGraphics/blob/master/TestCGAL/Test/SceenShots/ClassResult.png)
</p>

Time to generate each features
 <p align="center">	
![TimeFeatureImage](https://github.com/rix161/SBU_ComputerGraphics/blob/master/TestCGAL/Test/SceenShots/TimeFeature.png)
</p> 

Feature wise and Part wise precision
 <p align="center">	
![FeaturePImage](https://github.com/rix161/SBU_ComputerGraphics/blob/master/TestCGAL/Test/SceenShots/PartPrecision.png)
</p> 
 <p align="center">	
![PartPImage](https://github.com/rix161/SBU_ComputerGraphics/blob/master/TestCGAL/Test/SceenShots/FeaturePrecision.png)
</p>

Please Checkout Presentation Folder for detailed report.
 