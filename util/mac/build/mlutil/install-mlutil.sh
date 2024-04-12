#/bin/sh

if [ -d "/usr/local/lib" ] 
then
    echo "Directory /usr/local/lib exists." 
else
    #echo "Warning: Directory /usr/local/lib does not exist. Creating it."
    #sudo mkdir /usr/local/lib

    echo "Error: Directory /usr/local/lib does not exist."
    exit 1
fi

echo "Installing libmlutil.dylib to /usr/local/lib."
cp ./mlutil-DerivedData/mlutil/Build/Products/Debug/libmlutil.dylib /usr/local/lib

if [ -d "/usr/local/include" ] 
then
    echo "Directory /usr/local/include exists." 
else
    #echo "Warning: Directory /usr/local/include does not exist. Creating it."
    #sudo mkdir /usr/local/include
        
    echo "Error: Directory /usr/local/include does not existt."
    exit 1
fi

echo "Installing libmlutil header files to /usr/local/include."
cp -R ./mlutil-DerivedData/mlutil/Build/Products/Debug/usr/local/include/* /usr/local/include
