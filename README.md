# RIIV

## Project inputs

The learning database is constituted of two separated directories : 
- PNG images containing small hand-drawn symbols. One symbol per image;
- txt files with an identical name, a label describing to which model the image is associated. 

Here is an example of an image and its associated txt file : 

![](accident_000_04_7_2.png)
```
# Projet TIV  Grp:3
label accident
form 00004
scripter 000
page 04
row 7
column 2
size small
```

The test database is a small set of similar images.

## Feature extraction and outputs

The feature extraction phase shall use OpenCV to extract characteristics from all images. These characteristics can be of various types, being as simple as a density of black pixels (after a binarization) or as complex as corners detection. 

These features will be presented in a ARFF file format representing the whole processed image base. An ARFF file looks like that : 

```
@RELATION name

@ATTRIBUTE filename STRING
@ATTRIBUTE pixel_density NUMERIC
@ATTRIBUTE gravity_center_X NUMERIC
@ATTRIBUTE gravity_center_Y NUMERIC
...
@ATTRIBUTE class {accident_small, accident_medium, accident_large, ...}


@DATA
accident_000_04_7_2,0.68,123,45,...,accident_small
...
```

```
@RELATION results
@ATTRIBUTE symbol {accident,bomb,car,casualty,electricity,fire,firebrigade,flood,gas,injury,paramedics,person,police,roadblock}
@ATTRIBUTE height NUMERIC
@ATTRIBUTE width NUMERIC
@ATTRIBUTE diagonalLength NUMERIC
@ATTRIBUTE contourArea NUMERIC
@ATTRIBUTE contourPerimeter NUMERIC
@ATTRIBUTE equivalent_radius NUMERIC
@ATTRIBUTE mean_gray NUMERIC
@ATTRIBUTE center_of_mass_X NUMERIC
@ATTRIBUTE center_of_mass_Y NUMERIC
@ATTRIBUTE mean_gray_zone_0_0 NUMERIC
@ATTRIBUTE center_of_mass_X_zone_0_0 NUMERIC
@ATTRIBUTE center_of_mass_Y_zone_0_0 NUMERIC
...
@ATTRIBUTE mean_gray_zone_2_2 NUMERIC
@ATTRIBUTE center_of_mass_X_zone_2_2 NUMERIC
@ATTRIBUTE center_of_mass_Y_zone_2_2 NUMERIC

@DATA
police,214,214,302.641702, ... ,249.826212,27.504070,27.696206
...
```

## Classification

These extracted features will later be used to build a classifier. The first tests will be done using Weka and its built-in classifiers. This will let us know if the characteristics we chose to extract were relevant or not.

## Project architecture

The project CMakeLists and main are in the root directory, an `images` directory contains the annotated database from the first phase as well as the test database. The main program is in `OpenCV1.cpp` and the correct images directory has to be specified at the beginning of it.