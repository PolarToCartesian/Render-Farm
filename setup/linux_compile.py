# Bad code because I hate python

import os

# https://thispointer.com/python-how-to-get-list-of-files-in-directory-and-sub-directories/
def getListOfFiles(dirName):
    # create a list of file and sub directories 
    # names in the given directory 
    listOfFile = os.listdir(dirName)
    allFiles = list()
    # Iterate over all the entries
    for entry in listOfFile:
        # Create full path
        fullPath = os.path.join(dirName, entry)
        # If entry is a directory then get the list of files in this directory 
        if os.path.isdir(fullPath):
            allFiles = allFiles + getListOfFiles(fullPath)
        else:
            allFiles.append(fullPath)
                
    return allFiles

filenameslist = getListOfFiles("./../Engine/")
filenameslist.append("./../Source.cpp")

filenames = ""

for i in range(len(filenameslist)):
    filenames += filenameslist[i] + " "

command = "g++-8 " + filenames + " -std=c++17 -lstdc++fs -o ./../outputExec -w -lpthread"

os.system(command)