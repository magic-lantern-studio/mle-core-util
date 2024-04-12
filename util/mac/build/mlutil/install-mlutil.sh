#/bin/sh

if [ -d "/usr/local/lib" ] 
then
    echo "Directory /usr/local/lib exists." 
else
    
    echo "Warning: Directory /usr/local/lib does not exist. Creating it."
    sudo mkdir /usr/local/lib
fi

echo "Installing libmlutil.dylib to /usr/local/lib."
sudo cp ./mlutil-DerivedData/mlutil/Build/Products/Debug/libmlutil.dylib /usr/local/lib

if [ -d "/usr/local/include" ] 
then
    echo "Directory /usr/local/include exists." 
else
    
    echo "Warning: Directory /usr/local/include does not exist. Creating it."
    sudo mkdir /usr/local/include
fi

echo "Installing libmlutil header files to /usr/local/include."
sudo cp -R ./mlutil-DerivedData/mlutil/Build/Products/Debug/usr/local/include/* /usr/local/include
