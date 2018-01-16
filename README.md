
# Data Driven Mesh Segmentation and Labelling 
## Course project for CSE 528: Computer Graphics


### Introduction

The objective of the project is to explicitly label the mesh into its components. 
The problem of mesh labelling can be viewed a classification problem as such this project trains a random forest classifier trained on mesh data generated 
using a ensemble of mesh descriptors which effectively and concisely capture the surface,volume and orientation of the mesh.

#### Mesh Features
##### The features implemented are

1. Average Geodesic Distance (Local surface property) 
>Approximate average geodesic distance using fast marching algorithm.
>>![AGDIMAGE](https://github.com/rix161/SBU_ComputerGraphics/blob/master/TestCGAL/Test/SceenShots/AGDResult.png)

2. Shape diameter function(Local Volume property)
>Approximation method calculate the distance from surface to the medial axis. Uses Moller-Trumbore algorithm for triangle ray intersection.
>>![SDFImage](https://github.com/rix161/SBU_ComputerGraphics/blob/master/TestCGAL/Test/SceenShots/SDFResult.png)

3. Curvature (Gaussian,Mean and principle curvatures)(1-Ring neighbourhood surface property)
> First order differential attributes on a piecewise linear triangular mesh.   
>>![CurImage](https://github.com/rix161/SBU_ComputerGraphics/blob/master/TestCGAL/Test/SceenShots/curvatureResult.png)

4. Volumetric shape images(Local Volume property)
> Better approximation of distance from surface to the medial axis. A Two pass algorithm which internally uses SDF.   
>>![VSIImage](https://github.com/rix161/SBU_ComputerGraphics/blob/master/TestCGAL/Test/SceenShots/VisResult.png)

5. Shape Context(Surface and mesh face orientation property)
>Combines geodesic distance with the orientation by computing the angle between the surface normal and line connecting the face centres.
>>![SCImage](https://github.com/rix161/SBU_ComputerGraphics/blob/master/TestCGAL/Test/SceenShots/SCResult.png)


#### Mesh Labelling and Segmentation.
##### A Random Forest Classifier was able to label the mesh with an accuracy of 92% and an SVM Classifier with accuracy of 93%. 
##### Random Forest classifier was finally chosen as it is faster to train.

##### Results 
	
>![ClassImage](https://github.com/rix161/SBU_ComputerGraphics/blob/master/TestCGAL/Test/SceenShots/ResultClass.png)
>>Time to generate each features
>>>![TimeFeatureImage](https://github.com/rix161/SBU_ComputerGraphics/blob/master/TestCGAL/Test/SceenShots/TimeFeature.png)
>>Feature wise and Part wise precision
>>![FeaturePImage](https://github.com/rix161/SBU_ComputerGraphics/blob/master/TestCGAL/Test/SceenShots/PartPrecision.png)
>>![PartPImage](https://github.com/rix161/SBU_ComputerGraphics/blob/master/TestCGAL/Test/SceenShots/FeaturePrecision.png)


Please Checkout [Presentation](https://github.com/rix161/SBU_ComputerGraphics/blob/master/Presentation/Project%20Report/Data%20Driven%20Mesh%20segmentation%20and%20labeling.pdf)Folder for detailed report.
 