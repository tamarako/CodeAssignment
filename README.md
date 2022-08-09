# CodeAssignment

## What the code does

Reads all pgm images from a given folder and creates an average image from the input images where each pixel is the average of the pixels at the same coordinates.

Average image is written to the input folder. 

## How to execute
Open a terminal inside the CodeAssignment folder and execute the following commands with inputPath being the absolute path to the folder with the input images you want to have processed

sudo docker build . -t average-image-creation

sudo docker run -v inputPath:/usr/src/averageimages/Images --rm -it average-image-creation

