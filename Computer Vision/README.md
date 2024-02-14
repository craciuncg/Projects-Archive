# Computer Vision

## Overview

This folder contains 2 Computer Vision related projects that I have tried to accomplish.

## Video Interpolation

In that project, I tried to come up with a system that can interpolate 2 frames using classical computer vision techniques, without the aid of machine learning.

The idea I had for this can be summarized in the following:

1. Convert the images from **RGB to HSV** format, HSV is easier to work with when performing segmentation by color

2. Use a criterion for **pixel similarity**; I used segmentation of colors along with their nuances, tones and so on (i.e. red, green, yellow, purple etc.) as the criterion, though I am sure more sophisticated approaches can be used

3. For each color, **retrieve the areas** from the image that are similar with respect to the chosen color

4. As a result, this performs some form of **image segmentation**

5. Use another **similarity criterion for the areas** (i.e. similarity score of 2 areas), and create pairs of areas which are the most similar from both images according to the criterion

6. For each pair, you create several frames, according to how many interpolation points you want, and **interpolate the areas**. For this, I thought of using the contours of the areas, interpolate the contours and then check for the interior of the contour to infer what color to draw based on the original 2 frames.

The implementation is still in progress because I was always coming accross better ways of doing certain parts and new ideas for perfectioning came in.

The implementation is written in C++ and uses **OpenCV**.

I wish I could perfection it, perhaps using more techniques and replacing the straightforward techniques with more elaborated ones and have a final working example.

## Watermark Removal

This was a university project but I added it here because, for me, it was intersting enough.

This is a toy example of removing watermark from images.

It uses the Sobel convolutional filter to detect the edges in the image and to detect the position of the watermarks in the image.

#### Implementation

The implementation is written in C++ using an **OpenGL** framework setup given by my university.

The original requirement was to do the processing on the CPU and thus resulting some serious performance issues. There were some restrictions regarding the time limit, which to me they appeared to be quite permissive, because in practice it seems too much to wait a whole minute for this processing.

In this sense, I decided to write the implementation to run on the GPU, using programs called **shaders**, that run at different stages of the graphics pipeline.

As a result, the performance quite exceeded my expectations, obatining an "insant" processing time (way lower than 1 second).