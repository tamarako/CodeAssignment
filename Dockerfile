FROM gcc:latest

Copy . /usr/src/averageimages

WORKDIR usr/src/averageimages

RUN g++ -o AverageImageCreation generateAverageImage.cpp 

CMD ["./AverageImageCreation"]
