#!/bin/bash

mkdir -p build
cd build

cmake ..
make

sudo mkdir -p /usr/local/share/ps/templates/
sudo cp -r ../templates/* /usr/local/share/ps/templates/
sudo chmod -R 755 /usr/local/share/ps/templates/

sudo mv ps /usr/local/bin/
sudo chmod +x /usr/local/bin/ps

echo "Build and installation completed successfully!"
