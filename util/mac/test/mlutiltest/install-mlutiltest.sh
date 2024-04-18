#/bin/sh

if [ -d "/opt/MagicLantern/lib" ] 
then
    echo "Directory /opt/MagicLantern/lib exists." 
else
    #echo "Warning: Directory /opt/MagicLantern/lib does not exist. Creating it."
    #sudo mkdir -p /opt/MagicLantern/lib

    echo "Error: Directory /opt/MagicLantern/lib does not exist."
    exit 1
fi

echo "Installing libmlutil.dylib to /opt/MagicLantern/lib."
cp ./mlutiltest-DerivedData/mlutiltest/Build/Products/Debug/libmlutiltest.dylib /opt/MagicLantern/lib

if [ -d "/opt/MagicLantern/include/mle" ] 
then
    echo "Directory /opt/MagicLantern/include/mle exists." 
else
    #echo "Warning: Directory /opt/MagicLantern/include/mle does not exist. Creating it."
    #sudo mkdir -p /opt/MagicLantern/include/mle
        
    echo "Error: Directory /opt/MagicLantern/include/mle does not existt."
    exit 1
fi

echo "Installing libmlutil header files to /opt/MagicLantern/include/mle."
cp -R ./mlutiltest-DerivedData/mlutiltest/Build/Products/Debug/usr/local/include/* /opt/MagicLantern/include/mle
