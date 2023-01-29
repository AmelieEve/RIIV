# RIIV

## Project inputs

The database is constituted of two separated directories : 
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

## Feature extraction and outputs

The feature extraction phase shall use OpenCV to extract characteristics from all images. These characteristics can be of various types, being as simple as a density of black pixels (after a binarization) or as complex as SIFT. 

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

## Classification

These extracted features will later be used to build a classifier. The first tests will be done using Weka and its built-in classifiers. This will let us know if the characteristics we chose to extract were relevant or not.

## Project architecture

The project CMakeLists and main are in the root directory, an `assets` directory contains the annotated database from the first phase.