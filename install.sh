#!/bin/bash

mkdir -p build
cd build

cmake ..
make

sudo mkdir -p /usr/local/share/pset/templates/
sudo cp -r ../templates/* /usr/local/share/pset/templates/
sudo chmod -R 755 /usr/local/share/pset/templates/

sudo mv pset /usr/local/bin/
sudo chmod +x /usr/local/bin/pset

echo "Build and installation completed successfully!"
