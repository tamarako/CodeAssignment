#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>

using namespace std;

class PgmImage
{
public:
    string m_magicNumber;
    int m_width;
    int m_height;
    int m_maxValue;
    vector<int> m_pixels;
};

bool readPgmImageFromFile(filesystem::path pathToBeRead, PgmImage& imageToBeWritten)
{
    bool successfull =false;
    fstream my_file;
    my_file.open(pathToBeRead.string(),ios::in);
        
    if (!my_file)
    {
        cout <<"No file with the corresponding name found! \n";
    }
    else
    {
        string inputString;
        char refChar='#';
        int counter =0;
        bool isValidPgmFile = true;
        while(isValidPgmFile)
        {
            my_file >> inputString;
            if (string::npos != inputString.find(refChar))
            {
                break;
            }
            else if (my_file.eof())
            {
                break;
            }
            if (0==counter)
            {
                imageToBeWritten.m_magicNumber=inputString;
                if("P2"!=imageToBeWritten.m_magicNumber)
                {
                    cout << pathToBeRead.filename().string();
                    cout << " seems to be no regular pgm file. File is ignored."<<endl;
                    isValidPgmFile=false;
                    break;
                }
                
            }
            else if (1==counter)
            {
                try
                {
                    imageToBeWritten.m_width=stoi(inputString);  
                }
                catch(const std::invalid_argument& e)
                {
                    cout << pathToBeRead.filename().string();
                    cout << " seems to be no regular pgm file. File is ignored."<<endl;
                    isValidPgmFile=false;
                    break;
                }
            }
            else if (2 == counter)
            {
                try
                {
                    imageToBeWritten.m_height=stoi(inputString);  
                }
                catch(const std::invalid_argument& e)
                {
                    cout << pathToBeRead.filename().string();
                    cout << " seems to be no regular pgm file. File is ignored."<<endl;
                    isValidPgmFile=false;
                    break;
                }
            }
            else if (3 == counter)
            {
                try
                {
                    imageToBeWritten.m_maxValue=stoi(inputString);  
                }
                catch(const std::invalid_argument& e)
                {
                    cout << pathToBeRead.filename().string();
                    cout << " seems to be no regular pgm file. File is ignored."<<endl;
                    isValidPgmFile=false;
                    break;
                }
            }
            else if(3 < counter)
            {
                try
                {
                    const int pixelValue =stoi(inputString);
                    imageToBeWritten.m_pixels.push_back(pixelValue);
                }
                catch(const std::invalid_argument& e)
                {
                    cout << pathToBeRead.filename().string();
                    cout << " seems to be no regular pgm file. File is ignored."<<endl;
                    isValidPgmFile=false;
                    break;
                }
                if ((imageToBeWritten.m_width*imageToBeWritten.m_height)==imageToBeWritten.m_pixels.size())
                {
                    break;
                }
            }
            
            counter += 1;
        }
        if (isValidPgmFile)
        {
            successfull=true;
        }
    }
    my_file.close();
       
    return successfull;
}
bool createAverageImage(const vector<PgmImage> inputImages, PgmImage& averageImage)
{
    const int numberOfInputImages = inputImages.size();
    bool wasSuccessfull=false;
    if (0!=numberOfInputImages)
    {
        averageImage.m_width=inputImages[0].m_width;
        averageImage.m_height=inputImages[0].m_height;
        averageImage.m_maxValue=inputImages[0].m_maxValue;
        averageImage.m_magicNumber=inputImages[0].m_magicNumber;
        for (int i =0; i<inputImages[0].m_pixels.size();i++)
        {
            int sumOfPixels =0;
            for (auto& pgmImage: inputImages)
            {
                sumOfPixels += pgmImage.m_pixels[i];
            }
            averageImage.m_pixels.push_back(sumOfPixels/numberOfInputImages);
        }
        if(averageImage.m_pixels.size()!= (averageImage.m_width*averageImage.m_height))
        {
            cout << "Something went wrong!"<<endl;
        }
        else
        {
            cout << "Calculation done!"<<endl;
            wasSuccessfull = true;
        }
    }
    else
    {
        cout << "No input image."<<endl;
    }   
    return wasSuccessfull;
}
bool writePgmImageToFile(PgmImage imageToWrite,string nameOfFile)
{
    bool successfull = false;
    string writeToFile =imageToWrite.m_magicNumber+"\n"+to_string(imageToWrite.m_width)+" " + to_string(imageToWrite.m_height) + "\n" + to_string(imageToWrite.m_maxValue) +"\n";
    
    for (int i =0; i<imageToWrite.m_pixels.size();i++)
    {
        if ((i+1)%imageToWrite.m_width ==0)
        {
            writeToFile += to_string(imageToWrite.m_pixels[i])+"\n";
        }
        else
        {
            writeToFile += to_string(imageToWrite.m_pixels[i])+" ";
        }
    }
    fstream my_newFile;
    my_newFile.open(nameOfFile,ios::out);
    if (!my_newFile)
    {
        cout <<"File not created! \n"<<endl;
    }
    else
    {
        cout <<"File successfully created! \n"<<endl;
        my_newFile << writeToFile;
        successfull=true;
    }
    my_newFile.close();
    return successfull;
}

int main()
{
    vector<PgmImage> allImages;
    filesystem::path pathToFolder("/usr/src/averageimages/Images");

    auto dirIterator = filesystem::directory_iterator(pathToFolder);
    cout << "Reading input files..." <<endl;
    for (auto& ordner: dirIterator)
    {
        if(filesystem::is_regular_file(ordner.path()) && (ordner.path().filename().extension()==".pgm"))
        {
            PgmImage imageToBeWritten;
            bool wasWritingToImageClassSuccessfull=readPgmImageFromFile(ordner.path(), imageToBeWritten);
            if(wasWritingToImageClassSuccessfull)
            {
                allImages.push_back(imageToBeWritten);
            }
        }
    }
    cout << "Input successfully read!"<<endl;
    cout << "Calculating pixelwise average..."<<endl;  
    PgmImage imageNew;
    const bool successfull=createAverageImage(allImages,imageNew);

    if(successfull)
    {
        cout << "Creating 'average.pgm' file..."<<endl;
        const string fileToBeWritten="Images/average.pgm";
        const bool writeSuccessfull =writePgmImageToFile(imageNew,fileToBeWritten);
    }

    return 0;
}